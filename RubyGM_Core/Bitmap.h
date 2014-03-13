#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// ��;: Direct2D Bitmap���ٶȷ�װ

#ifdef USING_CPlusPlus_MODE
#define StopsSetColor(obj, clr, idx) (obj).bitmap->brush.gradient_stops[(idx)].color = (clr)
#define StopsSetPosition(obj, pos, idx) (obj).bitmap->brush.gradient_stops[(idx)].position = (pos)
#endif

class Bitmap{
public:
	// ����ӳ��
	typedef	std::map<VALUE, Bitmap*, std::less<VALUE>, GameAllocator<std::pair<const VALUE, Bitmap*>>> Map;
	// ��������
	typedef Map::value_type vtype;
	// У����
	static const VALUE			CHECK_CODE = 0x87654321;
public:
	// Ruby����ӿ�
	enum EnumRubyObjectDataIndex
	{
		ACheckCode = 0,			// У����
		BBitmapWidth,				// λͼ���
		CBitmapHeight,				// λͼ�߶�
		DFont,						// �������
		EBrush,						// ��ˢ����
		FDisposedFlag,				// �ͷű�ʶ
		RUBY_OBJECT_DATA_SIZE		// ��ö������С
	};
	// Ruby�ӿڶ���: ����������
	// �½�����
	static VALUE NewObject(int argc, VALUE* argv, VALUE self);
	// �ͷŶ���
	static VALUE Dispose(VALUE self);
	// font=
	static VALUE FontIs(VALUE self, VALUE font_id);
	// brush=
	static VALUE BrushIs(VALUE self, VALUE brush_id);
	// dup clone
	static VALUE DupClone(VALUE self);
	// ��png��ʽ����
	static VALUE SaveAsPng(int argc, VALUE* argv, VALUE self);
public:
	// �̻���������
	static VALUE DrawTextBaisc(int argc, VALUE* argv, VALUE self);
	// �̻�����
	static VALUE DrawRect(int argc, VALUE* argv, VALUE self);
	// �̻�Բ�Ǿ���
	static VALUE DrawRoundedRect(int argc, VALUE* argv, VALUE self);
	// �̻�����
	static VALUE DrawLine(int argc, VALUE* argv, VALUE self);
	// �̻���Բ
	static VALUE DrawEllipse(int argc, VALUE* argv, VALUE self);
	// �̻�������
	static VALUE DrawGeometry(int argc, VALUE* argv, VALUE self);
	// ��伸����
	static VALUE FillGeometry(int argc, VALUE* argv, VALUE self);
	// ������
	static VALUE FillRect(int argc, VALUE* argv, VALUE self);
	// ���Բ�Ǿ���
	static VALUE FillRoundedRect(int argc, VALUE* argv, VALUE self);
	// �����Բ
	static VALUE FillEllipse(int argc, VALUE* argv, VALUE self);
	// ������ֲ�
	static VALUE FillOpacityMask(int argc, VALUE* argv, VALUE self);
public:
	// ��ʼ��Bitmap
	static void Init();
	// �����Ruby���ɵ�λͼ
	static void Clear();
public:
	// ���ļ�����λͼ
	Bitmap(const BitmapCache::Key& file_name);
	// ���ɿհ�λͼ
	Bitmap(UINT width, UINT height);
	// 
	Bitmap(Bitmap&) = delete;
	// ��������
	~Bitmap();
	// ��ȡD2Dλͼ
	BitmapCache::Bitmap*		GetD2DBitmap() { return m_pd2dbitmap; }
	// ����: ��ȡD2Dλͼ
	const BitmapCache::Bitmap*	GetD2DBitmap() const { return m_pd2dbitmap; }
	// �ؽ�λͼ������Ŀ��
	void				RecreateBitmapRenderTarget();
#ifdef USING_CPlusPlus_MODE
	// �̻�����
	void				draw_text(const D2D1_RECT_F* rect, const BitmapCache::Key& string);
	// �̻�������
	void				draw_geometry(ID2D1Geometry* , FLOAT = 1.F, ID2D1StrokeStyle* = nullptr);
	// ��伸����
	void				fill_geometry();
	// ���λͼ
	void				fill_rect(const D2D1_RECT_F* pRect);
#endif
	// ��ȡ���
	UINT				width()const{ return m_bmpSize.width; }
	// ��ȡ�߶�
	UINT				height()const{ return m_bmpSize.height; }
	// ��ȡ���
	float				widthf()const{ return m_bmpSizeF.width; }
	// ��ȡ�߶�
	float				heightf()const{ return m_bmpSizeF.height; }
	// ��ȡλͼ����
	const D2D1_RECT_F*	get_rect()const{ return &m_Rect; }
#ifdef USING_CPlusPlus_MODE
	// �ֶ�ˢ�±�ˢ �ֶ����ִ��Ч��(C++ vs Java)
	void				refresh_brush(){
		SafeRelease(m_pNowBrush);
		if (m_pNowBrush = BrushCache::GetBrush(brush))
			m_pNowBrush->AddRef()
	}
	// �ֶ�ˢ������ �Զ���߿���Ч��(Java vs C++)
	void				refresh_font(){
		SafeRelease(m_pNowFont);
		if (m_pNowFont = TextFormatCache::GetTextFormat(font))
			m_pNowFont->AddRef();
	}
#else
	void				refresh_brush(int index){
		SafeRelease(m_pNowBrush);
		if (m_pNowBrush = BrushCache::GetBrush(index))
			m_pNowBrush->AddRef();
	}
	// �ֶ�ˢ������ �Զ���߿���Ч��(Java vs C++)
	void				refresh_font(int index){
		SafeRelease(m_pNowFont);
		if (m_pNowFont = TextFormatCache::GetTextFormat(index))
			m_pNowFont->AddRef();
	}
#endif
public:
	// ����Դ���� Ĭ��Ϊλͼ��С
	D2D1_RECT_F					src_rect;
#ifdef USING_CPlusPlus_MODE
	// ����
	TextFormat					font;
	// ��ˢ
	GameBrush					brush;
#endif
private:
	// λͼ����
	D2D1_RECT_F					m_Rect;
	// λͼ��С
	struct{ UINT width, height; }m_bmpSize;
	// λͼ��С
	D2D_SIZE_F 					m_bmpSizeF;
	// ��ǰת������
	D2D1_MATRIX_3X2_F			m_nowMatrix = D2D1::Matrix3x2F::Identity();
	// ��ǰ��ˢ
	ID2D1Brush*					m_pNowBrush = nullptr;
	// ��ǰ����
	IDWriteTextFormat*			m_pNowFont = nullptr;
	// D2Dλͼ
	BitmapCache::Bitmap*		m_pd2dbitmap = nullptr;
	// λͼ������Ŀ��
	ID2D1BitmapRenderTarget*	m_pBitmapRT = nullptr;
public:
	// Ruby ��������
	RArray*						m_aryData = nullptr;
	// ����ӳ��
	static Map					s_mapRubytoCPlusPlus;
	// new ����
	void*	operator			new(size_t size){ return MPool.Alloc(size); }
		// delete ����
	void	operator			delete(void* p)	{ MPool.Free(p); }
	// new[] ����
	void*	operator			new[](size_t size) = delete;
	// delete[] ����
	void	operator			delete[](void* p) = delete;
	// ruby ��Bitmap��id
	static VALUE				s_rbBitmapID;
};