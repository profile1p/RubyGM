#include "stdafx.h"
#include "include.h"

//#define MAKE_SYSTEM_FILE

// 应用程序入口
int wmain(int argc, wchar_t** agrv){ 
#ifdef MAKE_SYSTEM_FILE
	void make_system_file();
	make_system_file();
	return 0;
#endif
	/*wchar_t* path = (wchar_t*)MPoolEx.Alloc(MAX_PATH*sizeof(wchar_t));
	if(!path) return -1;
	wcscpy_s(path, MAX_PATH, *agrv);
	::PathRemoveFileSpec(path);
	::PathRemoveFileSpec(path);
	MPoolEx.Free(path);*/
	//std::array<WCHAR, MAX_PATH> dstring = { L"我打算大苏打" };
	//system("set path=%path%;F:\\runtime\\");
//int APIENTRY wWinMain(HINSTANCE, HINSTANCE , LPWSTR, int){
#if !defined(_DEBUG) &&  !defined(_GAME_DEBUG)
	_cwprintf(L"loading...please wait...\n");
#endif
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
	srand(GetTickCount());
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		//RUBY_INIT_STACK;
		//ruby_init();
		{
			//
			ThisApp app;
			if (SUCCEEDED(app.Initialize())){
				app.RunMessageLoop();
			}
			else
				MessageBoxW(nullptr, L"初始化程序失败\n failed to initialize application.", L"Error", MB_OK);

		}
		//ruby_finalize();
		CoUninitialize();
	}
	else
		MessageBoxW(nullptr, L"初始化COM失败\nFailed to initialize COM.", L"Error", MB_OK);
	return EXIT_SUCCESS;
}

#ifdef MAKE_SYSTEM_FILE
void make_system_file(){
	// 设定笔刷
	std::ofstream ofs("Data/system.brs", std::ios::binary);
#pragma pack(push) //保存对齐状态
#pragma pack(1)
	struct Temp1
	{
		UINT32			size;
		BrushCache::GameBrushParameter p1;
		BrushCache::GameBrushParameter p2;
		D2D1_GRADIENT_STOP	p2_stop_ex[1];
	};
	Temp1 temp1;
	temp1.size = 2;
	// p1 纯白
	temp1.p1.this_length = sizeof(BrushCache::GameBrushParameter);
	temp1.p1.stop_count = 1;
	temp1.p1.gradient_stops->color = D2D1::ColorF::ColorF(0xFFFFFF, 1.F);
	// p2 线性渐变 6cf->黑色
	temp1.p2.this_length = sizeof BrushCache::GameBrushParameter + sizeof temp1.p2_stop_ex;
	temp1.p2.stop_count = 2;
	temp1.p2.gradient_stops->color = D2D1::ColorF::ColorF(0x66CCFF, 1.F);
	temp1.p2.gradient_stops->position = 0.F;
	temp1.p2_stop_ex->color = D2D1::ColorF::ColorF(0, 1.F);
	temp1.p2_stop_ex->position = 1.F;
	temp1.p2.brush_type = BrushCache::BrushType::Linear;
	temp1.p2.linear_properties.startPoint.x = temp1.p2.linear_properties.startPoint.y = 0.F;
	temp1.p2.linear_properties.endPoint.x = temp1.p2.linear_properties.endPoint.y = 256.F;
	ofs.write((char*)(&temp1), sizeof temp1);
	ofs.close();
	ofs.~basic_ofstream();
	new(&ofs) std::ofstream("Data/system.tft", std::ios::binary);




	struct Temp2
	{
		UINT32			size;
		TextFormatCache::TextFormatParameter p1;
		WCHAR			p1_exdata[6];
		TextFormatCache::TextFormatParameter p2;
		WCHAR			p2_exdata[14];
	};
	Temp2 temp2;
	temp2.size = 2;
	temp2.p1.this_length = sizeof temp2.p1 + sizeof temp2.p1_exdata;
	wcscpy_s(temp2.p1.font_name, 8, L"微软雅黑");
	temp2.p2.this_length = sizeof temp2.p2 + sizeof temp2.p2_exdata;
	wcscpy_s(temp2.p2.font_name, 16, L"Digital");
	ofs.write((char*)(&temp2), sizeof temp2);
#pragma pack(pop) // 恢复



	ofs.close();
}
#endif



// 内存池 (先初始化，不然...)
EzMemPoolEx							EzMemPoolEx::s_instance;
EzMemPool							EzMemPool::s_instance;

// 初始化Graphics静态成员
HANDLE						Graphics::s_hReadyToRender = nullptr;
HANDLE						Graphics::s_hRenderFinished = nullptr;
UINT						Graphics::s_numFrameCouter = 0;
DWM_TIMING_INFO				Graphics::s_DwmTimingInfo;


// 初始化BitmapCache静态成员
BitmapCache::Map				BitmapCache::s_mapBitmapCache;
BitmapCache::RenderTarget**		BitmapCache::s_ppRenderTarget = nullptr;
BitmapCache::WICFactory**		BitmapCache::s_ppWICFactory = nullptr;

// 初始化Brush静态成员
#ifdef USING_CPlusPlus_MODE
BrushCache::Map					BrushCache::s_mapBrushCache;
#else
BrushCache::Array				BrushCache::s_aryBrushCache(nullptr);
#endif
const D2D1_COLOR_F				BrushCache::s_colorTransparent = D2D1::ColorF(D2D1::ColorF::Black, 0.0F);
const D2D1_COLOR_F				BrushCache::s_colorTranslucent = D2D1::ColorF(D2D1::ColorF::White, 0.5F);
const D2D1_COLOR_F				BrushCache::s_colorWhite = D2D1::ColorF(D2D1::ColorF::White);
const D2D1_COLOR_F				BrushCache::s_colorBlack = D2D1::ColorF(D2D1::ColorF::Black);

// 初始化Game静态成员
BOOL							Game::s_bExitFlag = FALSE;

// 初始化Bitmap静态成员
Bitmap::Map						Bitmap::s_mapRubytoCPlusPlus;
VALUE							Bitmap::s_rbBitmapID(0);

// 初始化Sprite静态成员
//Sprite::List					Sprite::s_list;
Sprite::Array					Sprite::s_array;
Sprite::Map						Sprite::s_mapRubytoCPlusPlus;


// 初始化TextFormatCache静态成员
#ifdef USING_CPlusPlus_MODE
TextFormatCache::DWriteFactory**	TextFormatCache::s_ppDWriteFactory = nullptr;
TextFormatCache::Map				TextFormatCache::s_mapTextCache;
TextFormatCache::Loader*			TextFormatCache::s_pLoader = nullptr;
TextFormatCache::Collection*		TextFormatCache::s_pCollection = nullptr;
#else
TextFormatCache::DWriteFactory**	TextFormatCache::s_ppDWriteFactory = nullptr;
TextFormatCache::Array				TextFormatCache::s_aryTextFormat(nullptr);
TextFormatCache::Loader*			TextFormatCache::s_pLoader = nullptr;
TextFormatCache::Collection*		TextFormatCache::s_pCollection = nullptr;
#endif

// 初始化集合缓存
GeometryCahce::D2DFactory**			GeometryCahce::s_ppD2DFactory = nullptr;

// 输入接口
GameKeyboardMouseInput				KMInput;


// 低精度计时器
CrudeTimer					CrudeTimer::s_instance;
// 中精度计时器
DependableTimer				DependableTimer::s_instance;
// 高精度计时器
PrecisionTimer				PrecisionTimer::s_instance;


// 初始化GamePadInput静态成员
XINPUT_VIBRATION			GamePadInput::s_Vibration = { 0, 0 };
DWORD						GamePadInput::s_dwGamePadIndex = 0;
DWORD						GamePadInput::s_dwShockEndTime = 0;
XINPUT_STATE				GamePadInput::s_oldXInputState = { 0 };
XINPUT_STATE				GamePadInput::s_curXInputState = { 0 };