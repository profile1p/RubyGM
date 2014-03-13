#include "stdafx.h"
#include "include.h"



// 初始化Graphics
HRESULT Graphics::Init(const HWND hwnd){
	s_hReadyToRender = CreateEvent(NULL, FALSE, FALSE, NULL);
	s_hRenderFinished = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!s_hReadyToRender || !s_hRenderFinished)
#ifdef _DEBUG
		assert(0&&"创建对象失败!\n");
#else
		return E_FAIL;
#endif
	// 获取屏幕刷新率 DWM没有运行的话 再利用DC试试  一般为60hz
	ZeroMemory(&s_DwmTimingInfo, sizeof(s_DwmTimingInfo));
	s_DwmTimingInfo.cbSize = sizeof(s_DwmTimingInfo);
	if (FAILED(DwmGetCompositionTimingInfo(NULL, &s_DwmTimingInfo))){
		HDC hdc = GetDC(hwnd);
		s_DwmTimingInfo.rateCompose.uiDenominator = 1;
		s_DwmTimingInfo.rateCompose.uiNumerator = GetDeviceCaps(hdc, VREFRESH);
		ReleaseDC(hwnd, hdc);
	}
	return S_OK;
}


// 关闭Graphics
void Graphics::Close(){
	if (s_hReadyToRender){
		CloseHandle(s_hReadyToRender);
		s_hReadyToRender = NULL;
	}
	if (s_hRenderFinished){
		CloseHandle(s_hRenderFinished);
		s_hRenderFinished = NULL;
	}
}


// 刷新Graphics
VALUE Graphics::Update(VALUE self){
	++s_numFrameCouter;
	Sleep(1);
	SetEvent(s_hReadyToRender);
	WaitForSingleObject(s_hRenderFinished, INFINITE);
	ResetEvent(s_hRenderFinished);
	return Qnil;
}



// 冻结图像 是简单图像过渡的初始操作
void	Graphics::Freeze(){

}



// 过渡图像 是简单图像过渡的收尾操作
// time				[in]	: 过渡时间 默认30帧 60Hz显示器即为半秒 120Hz显示器即为四分之一秒
// mode				[in]	: 过渡方式 默认Alpha渐入渐出
// parameter		[in&op]	: 每个过渡方法可能会给出单个或多个解决方案
void	Graphics::Transition(UINT time, TransitionMode mode, UINT parameter){

}