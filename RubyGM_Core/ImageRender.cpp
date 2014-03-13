#include "stdafx.h"
#include "include.h"

// ImageRender类构造函数
ImageRender::ImageRender()
{
	m_parameters.DirtyRectsCount = 0;
	m_parameters.pDirtyRects = nullptr;
	m_parameters.pScrollRect = nullptr;
	m_parameters.pScrollOffset = nullptr;

	// 为缓存赋值
	BitmapCache::s_ppRenderTarget = &m_pD2DDeviceContext;
	BitmapCache::s_ppWICFactory = &m_pWICFactory;
	GeometryCahce::s_ppD2DFactory = &m_pD2DFactory;
}


// 创建设备无关资源
HRESULT ImageRender::CreateResources(){
	// 声明待使用指针

	// D3D 设备
	ID3D11Device*						pD3DDevice = nullptr;
	// D3D 设备上下文
	ID3D11DeviceContext*				pD3DDeviceContext = nullptr;
	// DXGI 适配器
	IDXGIAdapter*						pDxgiAdapter = nullptr;
	// DXGI 工厂
	IDXGIFactory2*						pDxgiFactory = nullptr;
	// 2D 纹理 后台缓冲
	ID3D11Texture2D*					pBackBuffer = nullptr;
	// DXGI Surface 后台缓冲
	IDXGISurface*						pDxgiBackBuffer = nullptr;
	// DXGI 设备
	IDXGIDevice1*						pDxgiDevice = nullptr;


	HRESULT hr = S_OK;



	// 创建DShow相关对象

	// 注册 
	m_hD2DVRLibrary = LoadLibraryW(L"System/D2DVideoRenderer2.dll");
	if (!m_hD2DVRLibrary){
		MessageBoxW(nullptr, L"Load Library Failed\n", L"Error", MB_OK);
		return E_FAIL;
	}

	typedef int (WINAPI *PROC_DllGetClassObject)(REFCLSID rclsid, REFIID riid, LPVOID FAR* ppv);
	PROC_DllGetClassObject proc_DllGetClassObject = nullptr;
	proc_DllGetClassObject = (PROC_DllGetClassObject)GetProcAddress(m_hD2DVRLibrary, "DllGetClassObject");
	if (!proc_DllGetClassObject){
		MessageBoxW(nullptr, L"Load Library Failed - 2\n", L"Error", MB_OK);
		return E_FAIL;
	}
	// {2A63FDFF-ADE3-40CF-AF6C-9D7C242C0273}
	/*static const GUID CLSID_DustDirect2DVideoRenderer =
	{ 0x2a63fdff, 0xade3, 0x40cf, { 0xaf, 0x6c, 0x9d, 0x7c, 0x24, 0x2c, 0x2, 0x73 } };

	// {9E705921-A075-48A0-8A68-CA8534C6C499}
	static const GUID IID_IDustDirect2DVideoRenderer =*/
	IClassFactory *pClassFactory = nullptr;
	proc_DllGetClassObject(CLSID_DustDirect2DVideoRenderer, IID_IClassFactory, (void**)&pClassFactory);
	if (!pClassFactory){
		MessageBoxW(nullptr, L"Create IClassFactory Failed\n失败!", L"Error", MB_OK);
		return E_FAIL;
	}

	if (SUCCEEDED(hr)){
		hr = pClassFactory->CreateInstance(0, IID_IBaseFilter, reinterpret_cast<void**> (&m_pRenderFilter));
		SafeRelease(pClassFactory);
	}

	if (SUCCEEDED(hr)){
		hr = CoCreateInstance(CLSID_FilterGraph, nullptr,
			CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGraph);
	}

	if (SUCCEEDED(hr)){
		hr = m_pGraph->QueryInterface(IID_IMediaControl, (void **)&m_pControl);
	}

	if (SUCCEEDED(hr)){
		hr = m_pGraph->QueryInterface(IID_IFilterGraph2, (void**)&m_pFilterGraph2);
	}

	/*if (SUCCEEDED(hr)){
		hr = ::CoCreateInstance(CLSID_DustDirect2DVideoRenderer, nullptr, CLSCTX_ALL, __uuidof(IBaseFilter), (void**)&m_pRenderFilter);
	}*/

	if (SUCCEEDED(hr)){
		hr = m_pRenderFilter->QueryInterface(IID_IDustDirect2DVideoRenderer, (void**)&m_pD2DVideoRenderer);
	}

	if (SUCCEEDED(hr)){
		hr = m_pRenderFilter->QueryInterface(IID_IQualProp, (void**)&m_pQuality);
	}


	if (SUCCEEDED(hr)){//FILTER_NAME
		hr = m_pFilterGraph2->AddFilter(m_pRenderFilter, L"Enhanced Video Renderer");
		//SafeRelease(m_pRenderFilter);
	}
	// 创建D3D相关对象



	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	// Debug状态 有D3D DebugLayer就取消注释
	//creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// This array defines the set of DirectX hardware feature levels this app  supports.
	// The ordering is important and you should  preserve it.
	// Don't forget to declare your app's minimum required feature level in its
	// description.  All apps are assumed to support 9.1 unless otherwise stated.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	// 创建 m_pD3DDevice & Context 
	if (SUCCEEDED(hr)){
		hr = D3D11CreateDevice(
			nullptr,                    // specify null to use the default adapter
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			creationFlags,              // optionally set debug and Direct2D compatibility flags
			featureLevels,              // list of feature levels this app can support
			ARRAYSIZE(featureLevels),   // number of possible feature levels
			D3D11_SDK_VERSION,
			&pD3DDevice,              // returns the Direct3D device created
			&m_featureLevel,            // returns feature level of device created
			&pD3DDeviceContext        // returns the device immediate context
			);
	}
	if (SUCCEEDED(hr))
	{
		// 创建 IDXGIDevice
		hr = pD3DDevice->QueryInterface(__uuidof(IDXGIDevice1), (void **)&pDxgiDevice);
	}


	if (SUCCEEDED(hr))
	{
		// 创建 D2D 工厂
		hr = D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory1),
			reinterpret_cast<void**>(&m_pD2DFactory)
			);

	}
	if (SUCCEEDED(hr))
	{
		// 顺便获取dpi
		m_pD2DFactory->GetDesktopDpi(&m_dpi, &m_dpi);
		// 创建 WIC 工厂.
		hr = CoCreateInstance(
			CLSID_WICImagingFactory2,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory2,
			reinterpret_cast<void **>(&m_pWICFactory)
			);
	}

	if (SUCCEEDED(hr))
	{
		// 创建 DirectWrite 工厂.
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(m_pDWriteFactory),
			reinterpret_cast<IUnknown **>(&m_pDWriteFactory)
			);
	}


	if (SUCCEEDED(hr))
	{
		// 这时创建D2D设备
		hr = m_pD2DFactory->CreateDevice(pDxgiDevice, &m_pD2DDevice);
	}


	if (SUCCEEDED(hr))
	{
		// 这时创建D2D设备
		hr = m_pD2DDevice->CreateDeviceContext(
			D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			&m_pD2DDeviceContext
			);
	}

	RECT clientRect;
	::GetClientRect(m_hwnd, &clientRect);
	// Allocate a descriptor.
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
	swapChainDesc.Width = clientRect.right;                           // use automatic sizing
	swapChainDesc.Height = clientRect.bottom;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // this is the most common swapchain format
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;                // don't use multi-sampling
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;                     // use double buffering to enable flip
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	// WIN8
	//swapChainDesc.Scaling = DXGI_SCALING_NONE;
	//swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // all apps must use this SwapEffect
	// WIN7
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// Identify the physical adapter (GPU or card) this device is runs on.
	if (SUCCEEDED(hr))
	{
		hr = pDxgiDevice->GetAdapter(&pDxgiAdapter);
	}

	// Get the factory object that created the DXGI device.
	if (SUCCEEDED(hr))
	{
		hr = pDxgiAdapter->GetParent(IID_PPV_ARGS(&pDxgiFactory));
	}
	// Get the final swap chain for this window from the DXGI factory

	if (SUCCEEDED(hr))
	{
		// 设置全屏参数
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc;
		// 设置刷新率60FPS
		fullscreenDesc.RefreshRate.Denominator = 1;
		fullscreenDesc.RefreshRate.Numerator = 1;
		// 上层优先
		fullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST;
		// 建议值 否则会进行强行全屏拉伸
		fullscreenDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
		// 全屏显示
		fullscreenDesc.Windowed = FALSE;

		//IDXGIOutput* pRestrictToOutput = nullptr;
		hr = pDxgiFactory->CreateSwapChainForHwnd(
			pD3DDevice,
			m_hwnd,
			&swapChainDesc,
			false ? &fullscreenDesc : nullptr,
			nullptr,
			&m_pSwapChain
			);
	}
	// 确保DXGI队列里边不会超过一帧T
	if (SUCCEEDED(hr))
	{
		hr = pDxgiDevice->SetMaximumFrameLatency(1);
	}

	// Get the backbuffer for this window which is be the final 3D render target.
	if (SUCCEEDED(hr))
	{
		hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	}
	// Now we set up the Direct2D render target bitmap linked to the swapchain. 
	// Whenever we render to this bitmap, it is directly rendered to the 
	// swap chain associated with the window.
	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
		m_dpi,
		m_dpi
		);

	// Direct2D needs the dxgi version of the backbuffer surface pointer.
	if (SUCCEEDED(hr))
	{
		hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pDxgiBackBuffer));
	}
	// Get a D2D surface from the DXGI back buffer to use as the D2D render target.
	if (SUCCEEDED(hr))
	{
		hr = m_pD2DDeviceContext->CreateBitmapFromDxgiSurface(
			pDxgiBackBuffer,
			&bitmapProperties,
			&m_pD2DTargetBimtap
			);
		// 现在 就能设置 Direct2D 渲染目标 了.
		m_pD2DDeviceContext->SetTarget(m_pD2DTargetBimtap);
	}

	if (SUCCEEDED(hr)){
		hr = m_pD2DDeviceContext->CreateCompatibleRenderTarget(&m_pVideoTarget);
	}


	if (SUCCEEDED(hr)){
		hr = m_pVideoTarget->GetBitmap(&m_pVideoBitmap);
	}
	// 1.8 1.c 0.5
	if (SUCCEEDED(hr)){
		hr = m_pDWriteFactory->CreateCustomRenderingParams(1.8F, 0.5F, 1.F, DWRITE_PIXEL_GEOMETRY_RGB,
			DWRITE_RENDERING_MODE_OUTLINE,	&m_pDWParams);
	}
	if (SUCCEEDED(hr)){
		hr = TextFormatCache::Init(&m_pDWriteFactory);
	}
	if (SUCCEEDED(hr)){
		m_pD2DDeviceContext->SetTextRenderingParams(m_pDWParams);
	}
	/*if (SUCCEEDED(hr)){
		m_hVideoRenderedEventToken = (HANDLE)m_pD2DVideoRenderer->SetRenderTarget((ID2D1RenderTarget**)(&m_pVideoTarget));
		m_pFilterGraph2->RenderFile(L"H:\\D\\live wallpaper\\Eden_OP_without_sub_title.wmv", nullptr);
		//m_pFilterGraph2->RenderFile(L"H:\\Wildlife.wmv", nullptr);
		m_pControl->Run();
	}*/

	SafeRelease(pD3DDevice);
	SafeRelease(pD3DDeviceContext);
	SafeRelease(pDxgiDevice);
	SafeRelease(pDxgiAdapter);
	SafeRelease(pDxgiFactory);
	SafeRelease(pBackBuffer);
	SafeRelease(pDxgiBackBuffer);

	return hr;
}

// ImageRender析构函数
ImageRender::~ImageRender(){
	DiscardDeviceResources();

	SafeRelease(m_pVideoBitmap);
	SafeRelease(m_pVideoTarget);

	SafeRelease(m_pControl);
	SafeRelease(m_pGraph);
	SafeRelease(m_pRenderFilter);
	SafeRelease(m_pFilterGraph2);
	SafeRelease(m_pD2DVideoRenderer);
	SafeRelease(m_pQuality);

	SafeRelease(m_pSwapChain);
	SafeRelease(m_pD2DFactory);
	SafeRelease(m_pWICFactory);
	SafeRelease(m_pDWParams);
	SafeRelease(m_pDWriteFactory);
	if (m_hD2DVRLibrary){
		FreeLibrary(m_hD2DVRLibrary);
	}

}

// 丢弃设备相关资源
void ImageRender::DiscardDeviceResources(){
	SafeRelease(m_pD2DDevice);
	SafeRelease(m_pD2DDeviceContext);
	SafeRelease(m_pD2DTargetBimtap);
}

// 渲染图形图像
HRESULT ImageRender::RenderThread(ImageRender* pThis){
	HRESULT hr = S_OK;
	// FPS
	static const UINT refresh_count = 30;
//#define USING_HTIMER
#ifdef USING_HTIMER
	HTimer->MovStartEnd();
#else
	MTimer->Start();
#endif
	//
	UINT frame_counter(0);

	WCHAR window_title_old[MAX_PATH];
	GetWindowTextW(pThis->m_hwnd, window_title_old, MAX_PATH);
	WCHAR window_title_buffer[MAX_PATH];
	/*static const WCHAR msc_fontName[] = L"Verdana";
	static const FLOAT msc_fontSize = 50;
	IDWriteTextFormat* pTextFormat = nullptr;
	ID2D1SolidColorBrush* pBlackBrush = nullptr;
	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite text format object.
		hr = pThis->m_pDWriteFactory->CreateTextFormat(
			msc_fontName,
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			msc_fontSize,
			L"", //locale
			&pTextFormat
			);
	}
	if (SUCCEEDED(hr))
	{
		// Center the text horizontally and vertically.
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

		pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);


	}
	if (SUCCEEDED(hr))
	{
		hr = pThis->m_pD2DDeviceContext->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black),
			&pBlackBrush
			);
	}*/
	while (true){
		if (pThis->m_pD2DDeviceContext){
			// 开始
			pThis->m_pD2DDeviceContext->BeginDraw();
			// 重置转换
			pThis->m_pD2DDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
			// 刻画 66CCFF
			static const D2D1_COLOR_F clearColor = D2D1::ColorF(D2D1::ColorF::ColorF(0x66CCFF, 1.f));
			pThis->m_pD2DDeviceContext->Clear(clearColor);


			/*static FLOAT blur = 0.F;
			blur += 0.1f;
			if (blur >= 1000.F)
			blur = 0.f;
			m_pEffect->SetValue(D2D1_DISPLACEMENTMAP_PROP_SCALE, blur);*/

			//m_pD2DDeviceContext->DrawImage(GetBitmap(BitmapCache::Key(L"sampleImage.jpg")));
			/*POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(pThis->m_hwnd, &pt);
			D2D1_VECTOR_3F position = { (FLOAT)pt.x, (FLOAT)pt.y, 50.f };


			pThis->m_pEffect->SetValue(D2D1_POINTDIFFUSE_PROP_LIGHT_POSITION, position);

			m_pD2DDeviceContext->DrawImage(m_pEffect);/*,
			D2D1_INTERPOLATION_MODE_LINEAR,
			D2D1_COMPOSITE_MODE_SOURCE_COPY
			);*/
			//const SpriteList &list = Sprite::GetList();
			BitmapCache::Bitmap* bitmap;
			for (const auto& list : Sprite::GetArray()){
				for (auto itr : list){
					itr->check();
					if (itr->visible && (bitmap = itr->d2dbitmap())){
						itr->SetPosition();
						pThis->m_pD2DDeviceContext->DrawBitmap(bitmap,
							itr->des_rect(),
							itr->opacity,
							itr->interpolation_mode,
							itr->bitmap->src_rect
							);
					}
				}
			}
			/*static const WCHAR sc_helloWorld[] = L"Hello, World!";
			pThis->m_pD2DDeviceContext->DrawText(
				sc_helloWorld,
				ARRAYSIZE(sc_helloWorld) - 1,
				pTextFormat,
				D2D1::RectF(0, 0, 320, 120),
				pBlackBrush
				);*/
			// 正在播放视频时
			/*if (pThis->m_hVideoRenderedEventToken){
				WaitForSingleObject(pThis->m_hVideoRenderedEventToken, INFINITE);
				ResetEvent(pThis->m_hVideoRenderedEventToken);
				pThis->m_pD2DDeviceContext->DrawBitmap(pThis->m_pVideoBitmap);
				}*/

			// 结束刻画
			hr = pThis->m_pD2DDeviceContext->EndDraw();
			// 收到重建消息时(ps:D2DDeviceContext 正常情况应该是收不到了 显卡驱动崩溃时会收到)
			if (hr == D2DERR_RECREATE_TARGET)
			{
#ifdef _DEBUG
				_cwprintf(L"收到D2D 重建消息 : D2DERR_RECREATE_TARGET\n");
#else
				MessageBox(nullptr, L"收到Direct2D 重建消息,可能是因为显示环境的改变或者显卡驱动出现问题",
					L"╮（￣▽￣）╭ 错误 ╮（￣▽￣）╭", MB_OK);
#endif

				hr = S_FALSE;
			}
			// 手动交换前后缓存区
			if (SUCCEEDED(hr)){
				// 强制刷新 因为有强行刷新的机制，刷新时正在暂停调试时会触发...
				Graphics::ReadyToRender(100);
				if (pThis->m_boolSet.value(BooleanSet::AExitFlag))
					goto end;
				
				//
				/*while (!Graphics::ReadyToRender(20)){
					if (pThis->m_boolSet.value(BooleanSet::AExitFlag)){
						goto end;
					}
				}*/
				//Graphics::ReadyToRender(1000);
				// pThis->m_hVideoRenderedEventToken
				hr = pThis->m_pSwapChain->Present1(1, 0, &pThis->m_parameters);
				Graphics::RenderFinished();

				// 计算FPS
				if (frame_counter > refresh_count){
#ifdef USING_HTIMER
					FLOAT fps = (FLOAT)frame_counter / HTimer->DeltaF_s();
					HTimer->MovStartEnd();
					HTimer->RefreshFrequency();
#else
					FLOAT fps = (FLOAT)(frame_counter * 1000) / (FLOAT)MTimer->Delta();
					MTimer->Start();
#endif
					wsprintf(window_title_buffer, L"%s - %d.%02d FPS", window_title_old, (UINT)fps,
						((UINT)(fps*100.F)) % 100);
					SetWindowTextW(pThis->m_hwnd, window_title_buffer);
					frame_counter = 0;
				}
				++frame_counter;
			}
		}
		else {
#ifdef _DEBUG
			_cwprintf(L"D2D Device Context -> 0\n");
#endif
			goto end;
		}
		if (pThis->m_boolSet.value(BooleanSet::AExitFlag)){
			goto end;
		}
	}
end:
	//SafeRelease(pTextFormat);
	//SafeRelease(pBlackBrush);
	return hr;
}


// 从文件读取位图
HRESULT ImageRender::LoadBitmapFromFile(
	ID2D1DeviceContext *pRenderTarget,
	IWICImagingFactory2 *pIWICFactory,
	PCWSTR uri,
	UINT destinationWidth,
	UINT destinationHeight,
	ID2D1Bitmap1 **ppBitmap
	)
{
	IWICBitmapDecoder *pDecoder = nullptr;
	IWICBitmapFrameDecode *pSource = nullptr;
	IWICStream *pStream = nullptr;
	IWICFormatConverter *pConverter = nullptr;
	IWICBitmapScaler *pScaler = nullptr;

	HRESULT hr = pIWICFactory->CreateDecoderFromFilename(
		uri,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
		);

	if (SUCCEEDED(hr))
	{
		hr = pDecoder->GetFrame(0, &pSource);
	}
	if (SUCCEEDED(hr))
	{
		hr = pIWICFactory->CreateFormatConverter(&pConverter);
	}


	if (SUCCEEDED(hr))
	{
		if (destinationWidth != 0 || destinationHeight != 0)
		{
			UINT originalWidth, originalHeight;
			hr = pSource->GetSize(&originalWidth, &originalHeight);
			if (SUCCEEDED(hr))
			{
				if (destinationWidth == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
					destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
				}
				else if (destinationHeight == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
					destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
				}

				hr = pIWICFactory->CreateBitmapScaler(&pScaler);
				if (SUCCEEDED(hr))
				{
					hr = pScaler->Initialize(
						pSource,
						destinationWidth,
						destinationHeight,
						WICBitmapInterpolationModeCubic
						);
				}
				if (SUCCEEDED(hr))
				{
					hr = pConverter->Initialize(
						pScaler,
						GUID_WICPixelFormat32bppPBGRA,
						WICBitmapDitherTypeNone,
						nullptr,
						0.f,
						WICBitmapPaletteTypeMedianCut
						);
				}
			}
		}
		else
		{
			hr = pConverter->Initialize(
				pSource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				nullptr,
				0.f,
				WICBitmapPaletteTypeMedianCut
				);
		}
	}
	if (SUCCEEDED(hr))
	{
		hr = pRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			nullptr,
			ppBitmap
			);
	}

	SafeRelease(pDecoder);
	SafeRelease(pSource);
	SafeRelease(pStream);
	SafeRelease(pConverter);
	SafeRelease(pScaler);

	return hr;
}
