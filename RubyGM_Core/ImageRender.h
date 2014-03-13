#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// ��;: ��ͼ��ͼ��ĳ��� �����ͼ����Ⱦ

class ImageRender{
	// ��������
	enum BooleanSet{
		AExitFlag = 0x00000001,		// �˳���ʶ
	};
public:
	// ���캯��
	ImageRender();
	// ��������
	~ImageRender();
	// ���ô��ھ��
	HRESULT SetHwnd(HWND hwnd){	m_hwnd = hwnd;return CreateResources();	}
	// �˳���Ⱦ
	void	ExitRender(){ m_boolSet.set_value_true(BooleanSet::AExitFlag); }
	// ��ȡ���
	FLOAT	GetWidth(){ return m_pD2DDeviceContext ? 0.F : m_pD2DDeviceContext->GetSize().width; }
	// ��ȡ�߶�
	FLOAT	GetHeight(){ return m_pD2DDeviceContext ? 0.F : m_pD2DDeviceContext->GetSize().height; }
public:
	// ������Ⱦ�߳�
	static HRESULT RenderThread(ImageRender* pThis);
	// ��ȡ�ļ�
	static HRESULT ImageRender::LoadBitmapFromFile(
		ID2D1DeviceContext *pRenderTarget,
		IWICImagingFactory2 *pIWICFactory,
		PCWSTR uri,
		UINT destinationWidth,
		UINT destinationHeight,
		ID2D1Bitmap1 **ppBitmap
		);
private:
	// ������Դ
	HRESULT CreateResources();
	// �����豸�й���Դ
	void DiscardDeviceResources();
private:
	// ��������
	Bool32								m_boolSet = Bool32();
	// D2D ����
	ID2D1Factory1*						m_pD2DFactory = nullptr;
	// D2D �豸
	ID2D1Device*						m_pD2DDevice = nullptr;
	// D2D �豸������
	ID2D1DeviceContext*					m_pD2DDeviceContext = nullptr;
	// WIC ����
	IWICImagingFactory2*				m_pWICFactory = nullptr;
	// DWrite����
	IDWriteFactory1*					m_pDWriteFactory = nullptr;
	// ������Ⱦ����
	IDWriteRenderingParams*				m_pDWParams = nullptr;
	// DXGI ������
	IDXGISwapChain1*					m_pSwapChain = nullptr;
	// D2D λͼ ���浱ǰ��ʾ��λͼ
	ID2D1Bitmap1*						m_pD2DTargetBimtap = nullptr;
	// Filter Graph ������
	IGraphBuilder*						m_pGraph = nullptr;
	// Filter Graph2
	IFilterGraph2*						m_pFilterGraph2 = nullptr;
	// ý�����
	IMediaControl*						m_pControl = nullptr;
	// ��ȾFilter
	IBaseFilter*						m_pRenderFilter = nullptr;
	// ��Ⱦ�ӿ�
	ID2DVideoRenderer*					m_pD2DVideoRenderer = nullptr;
	// 
	IQualProp*							m_pQuality = nullptr;
	// 
	ID2D1BitmapRenderTarget*			m_pVideoTarget = nullptr;
	// 
	ID2D1Bitmap*						m_pVideoBitmap = nullptr;
	//  �����豸���Եȼ�
	D3D_FEATURE_LEVEL					m_featureLevel;
	// �ֶ�������
	DXGI_PRESENT_PARAMETERS				m_parameters;
	// ��Ⱦ����¼����Token ���ùر�
	HANDLE								m_hVideoRenderedEventToken = nullptr;
	// ���ھ��
	HWND								m_hwnd = nullptr;
	// D2D Video Renderer Filter ����
	HMODULE								m_hD2DVRLibrary = nullptr;
	// ��ĻDPI
	FLOAT								m_dpi = 0.F;
};