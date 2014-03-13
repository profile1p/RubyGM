#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途: 对图形图像的抽象 负责对图像渲染

class ImageRender{
	// 布尔集合
	enum BooleanSet{
		AExitFlag = 0x00000001,		// 退出标识
	};
public:
	// 构造函数
	ImageRender();
	// 析构函数
	~ImageRender();
	// 设置窗口句柄
	HRESULT SetHwnd(HWND hwnd){	m_hwnd = hwnd;return CreateResources();	}
	// 退出渲染
	void	ExitRender(){ m_boolSet.set_value_true(BooleanSet::AExitFlag); }
	// 获取宽度
	FLOAT	GetWidth(){ return m_pD2DDeviceContext ? 0.F : m_pD2DDeviceContext->GetSize().width; }
	// 获取高度
	FLOAT	GetHeight(){ return m_pD2DDeviceContext ? 0.F : m_pD2DDeviceContext->GetSize().height; }
public:
	// 单独渲染线程
	static HRESULT RenderThread(ImageRender* pThis);
	// 读取文件
	static HRESULT ImageRender::LoadBitmapFromFile(
		ID2D1DeviceContext *pRenderTarget,
		IWICImagingFactory2 *pIWICFactory,
		PCWSTR uri,
		UINT destinationWidth,
		UINT destinationHeight,
		ID2D1Bitmap1 **ppBitmap
		);
private:
	// 创建资源
	HRESULT CreateResources();
	// 丢弃设备有关资源
	void DiscardDeviceResources();
private:
	// 布尔集合
	Bool32								m_boolSet = Bool32();
	// D2D 工厂
	ID2D1Factory1*						m_pD2DFactory = nullptr;
	// D2D 设备
	ID2D1Device*						m_pD2DDevice = nullptr;
	// D2D 设备上下文
	ID2D1DeviceContext*					m_pD2DDeviceContext = nullptr;
	// WIC 工厂
	IWICImagingFactory2*				m_pWICFactory = nullptr;
	// DWrite工厂
	IDWriteFactory1*					m_pDWriteFactory = nullptr;
	// 字体渲染参数
	IDWriteRenderingParams*				m_pDWParams = nullptr;
	// DXGI 交换链
	IDXGISwapChain1*					m_pSwapChain = nullptr;
	// D2D 位图 储存当前显示的位图
	ID2D1Bitmap1*						m_pD2DTargetBimtap = nullptr;
	// Filter Graph 管理器
	IGraphBuilder*						m_pGraph = nullptr;
	// Filter Graph2
	IFilterGraph2*						m_pFilterGraph2 = nullptr;
	// 媒体控制
	IMediaControl*						m_pControl = nullptr;
	// 渲染Filter
	IBaseFilter*						m_pRenderFilter = nullptr;
	// 渲染接口
	ID2DVideoRenderer*					m_pD2DVideoRenderer = nullptr;
	// 
	IQualProp*							m_pQuality = nullptr;
	// 
	ID2D1BitmapRenderTarget*			m_pVideoTarget = nullptr;
	// 
	ID2D1Bitmap*						m_pVideoBitmap = nullptr;
	//  所创设备特性等级
	D3D_FEATURE_LEVEL					m_featureLevel;
	// 手动交换链
	DXGI_PRESENT_PARAMETERS				m_parameters;
	// 渲染完毕事件句柄Token 不用关闭
	HANDLE								m_hVideoRenderedEventToken = nullptr;
	// 窗口句柄
	HWND								m_hwnd = nullptr;
	// D2D Video Renderer Filter 库句柄
	HMODULE								m_hD2DVRLibrary = nullptr;
	// 屏幕DPI
	FLOAT								m_dpi = 0.F;
};