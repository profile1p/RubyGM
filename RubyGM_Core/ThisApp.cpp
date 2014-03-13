#include "stdafx.h"
#include "include.h"

// 初始化ThisApp
HRESULT ThisApp::Initialize(){
	WNDCLASS wc;

	ZeroMemory(&wc, sizeof(wc));
	wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbWndExtra = DLGWINDOWEXTRA;
	wc.hInstance = HINST_THISCOMPONENT;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIconW(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDI_APP_ICON));
	wc.lpfnWndProc = ThisApp::WndProc;
	wc.lpszClassName = AppInfo::APP_WINDOW_CLASS_NAME;
	wc.cbWndExtra = sizeof(LONG_PTR);
	RegisterClass(&wc);
	// 读取ini文件
	RECT window_rect = { 0, 0, AppInfo::SHOW_WIDTH, AppInfo::SHOW_HEIGHT };
	wchar_t title[32];
	DWORD rc = GetPrivateProfileStringW(L"Game", L"title", L"", title, sizeof title, L".\\Game.ini");
	if (rc){
		char buffer[8];
		if (GetPrivateProfileStringA("Game", "width", "", buffer, sizeof buffer, ".\\Game.ini"))
			window_rect.right = atoi(buffer);
		
		if (GetPrivateProfileStringA("Game", "height", "", buffer, sizeof buffer, ".\\Game.ini"))
			window_rect.bottom = atoi(buffer);
		
	}
	else
		wcscpy_s(title, AppInfo::APP_WINDOW_TITLE_NAME);
	// 调整窗口大小
	DWORD window_style = WS_CAPTION | WS_VISIBLE | WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX;
	AdjustWindowRect(&window_rect, window_style, FALSE);
	window_rect.right -= window_rect.left;
	window_rect.bottom -= window_rect.top;
	window_rect.left = (GetSystemMetrics(SM_CXFULLSCREEN) - window_rect.right) / 2;
	window_rect.top = (GetSystemMetrics(SM_CYFULLSCREEN) - window_rect.right) / 2;
	//window_rect.right += window_rect.left;
	//window_rect.bottom += window_rect.top;
	// 创建窗口
	m_hwnd = CreateWindowW(wc.lpszClassName, title, window_style,
		window_rect.left, window_rect.top, window_rect.right, window_rect.bottom, 0, 0, HINST_THISCOMPONENT, this);
	// 显示窗口
	if (m_hwnd){
		::GetClientRect(m_hwnd, &window_rect);
#ifdef _DEBUG
		_cwprintf(L"Window: %4d, %4d\n", window_rect.right, window_rect.bottom);
#endif
		ClientToScreen(m_hwnd, (LPPOINT)(&window_rect));
		window_rect.right += window_rect.left;
		window_rect.bottom += window_rect.top;
		ShowWindow(m_hwnd, SW_NORMAL);
		UpdateWindow(m_hwnd);
		// 设定并初始化渲染
		HRESULT hr = m_imageRender.SetHwnd(m_hwnd);
		if (SUCCEEDED(hr)){
			// 成功的话，创建渲染线程
			m_pRenderThread = (std::thread*)MPool.Alloc(sizeof std::thread);
			// 检查指针
			hr = m_pRenderThread ? S_OK : E_OUTOFMEMORY;
		}
		if (SUCCEEDED(hr)){
			// 显式析构
			new(m_pRenderThread) std::thread(ImageRender::RenderThread, &m_imageRender);
			// 成功的话，创建游戏逻辑线程
			m_pGameLogicThread = (std::thread*)MPool.Alloc(sizeof std::thread);
			// 检查指针
			hr = m_pGameLogicThread ? S_OK : E_OUTOFMEMORY;
			// 隐藏鼠标
			//::ShowCursor(FALSE);
			//ClipCursor(&window_rect);
		}
		if (SUCCEEDED(hr)){
			// 显式析构
			new(m_pGameLogicThread)std::thread(Game::LogicThread, m_hwnd);
			// 初始化输入接口
			hr = KMInput.Init(HINST_THISCOMPONENT, m_hwnd);
		}
		return hr;
	}
	return E_FAIL;
}


// 尝试执行脚本
VALUE ThisApp::TryEval(const char* str){
	static char buffer[1024];
	VALUE rc = Qnil;
	VALUE err;
	__try{
		rc = rb_eval_string(str);
	}
	__except (EXCEPTION_EXECUTE_HANDLER){}
	if (!NIL_P(err = rb_errinfo())){
		// class
		VALUE kclass = rb_class_path(CLASS_OF(err));
		// message
		VALUE message = rb_obj_as_string(err);
		// backtrace
		VALUE ary = rb_funcall(err, rb_intern("backtrace"), 0);
		VALUE brstr = rb_funcall(ary, rb_intern("to_s"), 0);
		// sprintf
		sprintf_s(buffer, "Error:  %s\n%s\nbacktrace:  %s\n", StringValuePtr(kclass),
			StringValuePtr(message),
			StringValuePtr(brstr));
		MessageBoxA(nullptr, buffer, "Error", MB_OK);
		rb_set_errinfo(Qnil);
	}
	return rc;
}

LRESULT CALLBACK ThisApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){

	// 处理标识
	bool handled = false;
	// return code
	LRESULT rcode = 0;

	ThisApp* pThis;
	if (WM_CREATE == message)
	{
		/*pThis = reinterpret_cast<MainWindow*>(lParam);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));*/
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		pThis = (ThisApp *)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA,
			PtrToUlong(pThis)
			);
	}
	else
	{
		pThis = reinterpret_cast<ThisApp *>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA
			)));
	}
	// 消息处理
	if (pThis){
		switch (message){
		case WM_DESTROY:
			rcode = 1;
			handled = true;
			PostQuitMessage(0);
			break;
		case WM_SETFOCUS:
#ifdef _DEBUG
			_cwprintf(L"WM_SETFOCUS\n");
#endif
			break;
		case WM_KILLFOCUS:
#ifdef _DEBUG
			_cwprintf(L"WM_KILLFOCUS\n");
#endif
			break;
		case WM_CLOSE:
		{
			__try{
				rb_raise(rb_eSystemExit, "System Exit");
			}
			__except (EXCEPTION_EXECUTE_HANDLER){}
		}
			pThis->m_imageRender.ExitRender();
			Game::Exit();
			pThis->m_pGameLogicThread->join();
			pThis->m_pRenderThread->join();
			rcode = 1;
			handled = true;
			DestroyWindow(hwnd);
			break;
		}//s
	}//i
	if (!handled){
		rcode = DefWindowProc(hwnd, message, wParam, lParam);
	}
	return rcode;

};


// 消息循环
void ThisApp::RunMessageLoop()
{
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
