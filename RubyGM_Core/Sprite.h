#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// ��;: ����ϷͼԪ�ĸ߶ȳ���
class Sprite{
public:
	// ����ӳ��
	typedef	std::map<VALUE, Sprite*, std::less<VALUE>, GameAllocator<std::pair<const VALUE, Sprite*>>> Map;
	// ��������
	typedef Map::value_type vtype;
	// У����
	static const VALUE			CHECK_CODE = 0x12345678;
public:
	// Ruby����ӿ�
	enum EnumRubyObjectDataIndex
	{
		ACheckCode = 0,			// У����
		BNeedRefreshFlag,			// Ruby����ˢ�±�ʶ
		CSpriteX,					// X����
		DSpriteY,					// Y����
		DSpriteZ,					// Z����
		FDisposedFlag,				// �ͷű�ʶ
		GLayerID,					// ���ʶ
		HZoomX,						// X�᷽��Ŵ���
		IZoomY,						// Y�᷽��Ŵ���
		JOpacity,					// ��͸����
		KVisible,					// ������
		LSpriteOX,					// OX����
		MSpriteOY,					// OY����
		NInterpolationMode,		// ��ֵ����
		RUBY_OBJECT_DATA_SIZE		// ��ö������С
	};
	// Ruby�ӿڶ���: ����������
	// �½�����
	static VALUE NewObject(int argc, VALUE* argv, VALUE self);
	// �ͷŶ���
	static VALUE Dispose(VALUE self);
	// ����Bitmap
	static VALUE LinkBitmap(VALUE self, VALUE rbbitmap);
	// z=
	static VALUE SetZ(VALUE self, VALUE new_z);
public:
	// ѩ������
	typedef	std::list<Sprite*, GameAllocator<Sprite*>>	List;
	// ���ʶ
	typedef SArray<List, 32>	Array;
public:
	// ���캯��
	Sprite(UINT layer);
	// ��������
	~Sprite();
	// �������
	static void Clear();
	// ɾ������
	Sprite() = delete; Sprite(Sprite&) = delete;
public:
	// ������ Ruby -> C++
	void						check();
	// ��ȡZ����
	const int					z(){ return m_z; }
	// ��ȡλͼ
	BitmapCache::Bitmap*		d2dbitmap() { if (bitmap) return bitmap->GetD2DBitmap(); return NULL; }
	// ����: ��ȡλͼ
	const BitmapCache::Bitmap*	d2dbitmap()const{ if (bitmap) return bitmap->GetD2DBitmap(); return NULL; }
	// ��ȡĿ�����
	const D2D1_RECT_F&	des_rect(){ return m_desRect; }
	// ����λ��
	void			SetPosition(){
		if (bitmap){
			m_desRect.left = x;
			m_desRect.right = x + bitmap->widthf();
			m_desRect.top = y;
			m_desRect.bottom = y + bitmap->heightf();
		}
#ifdef _DEBUG
		else
			assert(0&&"bitmap ����ʧ��");
#endif
	}
	// �½�λͼ ����ʹ�ñ�����������λͼ
	void				new_bitmap(const WCHAR* file_name){
		if (bitmap)
			delete bitmap;
		BitmapCache::Key name = file_name;
		bitmap = new Bitmap(name);
	}
	// ���� �½�λͼ ����ʹ�ñ�����������λͼ
	void				new_bitmap(UINT width, UINT height){
		if (bitmap)
			delete bitmap;
		bitmap = new Bitmap(width, height);
	}
public:
	// λͼ
	::Bitmap*			bitmap = nullptr;
	// X����
	float				x = 0.F;
	// Y����
	float				y = 0.F;
	// ��͸����
	float				opacity = 1.F;
	// X����Ŵ���
	float				zoom_x = 1.F;
	// Y����Ŵ���
	float				zoom_y = 1.F;
	// ��ת�Ƕ�
	float				rotation = 0.F;
	// �Ƿ����
	BOOL				visible = TRUE;
	// ��ֵ����
	D2D1_BITMAP_INTERPOLATION_MODE interpolation_mode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
private:
	// Ŀ�����
	D2D1_RECT_F			m_desRect;
	// ԭ��X����
	float				m_ox = 0.F;
	// ԭ��Y����
	float				m_oy = 0.F;
	// Z����
	int					m_z = 0;
	// Ruby ��������
	RArray*				m_aryData = nullptr;
public:
	// ��ȡ����
	static const Array&	GetArray(){ return s_array; }
private:
	// ��Ruby����
	static Map			s_mapRubytoCPlusPlus;
	// ��������
	static	void		push_in_list(Sprite* obj);
private:
	// �����б�
	//static List			s_list;
	// �����б�
	static Array		s_array;
	// ����Z����
	void				set_z(int new_z){
		s_array[m_layer].remove(this);
		m_z = new_z;
		push_in_list(this);
	}
public:
	// new ����
	void*	operator			new(size_t size){ return MPool.Alloc(size); }
		// delete ����
	void	operator			delete(void* p)	{ MPool.Free(p); }
	// new[] ����
	void*	operator			new[](size_t size) = delete;
	// delete[] ����
	void	operator			delete[](void* p) = delete;
private:
	// ��ȡ ��ID
	const UINT				layer_id()const{ return m_layer; };
	// ��ID ��ֹ�޸�
	const UINT				m_layer;
};