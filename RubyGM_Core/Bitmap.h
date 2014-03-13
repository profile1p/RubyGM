#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途: Direct2D Bitmap的再度封装

#ifdef USING_CPlusPlus_MODE
#define StopsSetColor(obj, clr, idx) (obj).bitmap->brush.gradient_stops[(idx)].color = (clr)
#define StopsSetPosition(obj, pos, idx) (obj).bitmap->brush.gradient_stops[(idx)].position = (pos)
#endif

class Bitmap{
public:
	// 对象映射
	typedef	std::map<VALUE, Bitmap*, std::less<VALUE>, GameAllocator<std::pair<const VALUE, Bitmap*>>> Map;
	// 对象类型
	typedef Map::value_type vtype;
	// 校验码
	static const VALUE			CHECK_CODE = 0x87654321;
public:
	// Ruby对象接口
	enum EnumRubyObjectDataIndex
	{
		ACheckCode = 0,			// 校验码
		BBitmapWidth,				// 位图宽度
		CBitmapHeight,				// 位图高度
		DFont,						// 字体对象
		EBrush,						// 笔刷对象
		FDisposedFlag,				// 释放标识
		RUBY_OBJECT_DATA_SIZE		// 本枚举量大小
	};
	// Ruby接口定义: 不能是内联
	// 新建对象
	static VALUE NewObject(int argc, VALUE* argv, VALUE self);
	// 释放对象
	static VALUE Dispose(VALUE self);
	// font=
	static VALUE FontIs(VALUE self, VALUE font_id);
	// brush=
	static VALUE BrushIs(VALUE self, VALUE brush_id);
	// dup clone
	static VALUE DupClone(VALUE self);
	// 以png格式保存
	static VALUE SaveAsPng(int argc, VALUE* argv, VALUE self);
public:
	// 刻画基本字体
	static VALUE DrawTextBaisc(int argc, VALUE* argv, VALUE self);
	// 刻画矩形
	static VALUE DrawRect(int argc, VALUE* argv, VALUE self);
	// 刻画圆角矩形
	static VALUE DrawRoundedRect(int argc, VALUE* argv, VALUE self);
	// 刻画线条
	static VALUE DrawLine(int argc, VALUE* argv, VALUE self);
	// 刻画椭圆
	static VALUE DrawEllipse(int argc, VALUE* argv, VALUE self);
	// 刻画几何体
	static VALUE DrawGeometry(int argc, VALUE* argv, VALUE self);
	// 填充几何体
	static VALUE FillGeometry(int argc, VALUE* argv, VALUE self);
	// 填充矩形
	static VALUE FillRect(int argc, VALUE* argv, VALUE self);
	// 填充圆角矩形
	static VALUE FillRoundedRect(int argc, VALUE* argv, VALUE self);
	// 填充椭圆
	static VALUE FillEllipse(int argc, VALUE* argv, VALUE self);
	// 填充遮罩层
	static VALUE FillOpacityMask(int argc, VALUE* argv, VALUE self);
public:
	// 初始化Bitmap
	static void Init();
	// 清除由Ruby生成的位图
	static void Clear();
public:
	// 从文件加载位图
	Bitmap(const BitmapCache::Key& file_name);
	// 生成空白位图
	Bitmap(UINT width, UINT height);
	// 
	Bitmap(Bitmap&) = delete;
	// 析构函数
	~Bitmap();
	// 获取D2D位图
	BitmapCache::Bitmap*		GetD2DBitmap() { return m_pd2dbitmap; }
	// 重载: 获取D2D位图
	const BitmapCache::Bitmap*	GetD2DBitmap() const { return m_pd2dbitmap; }
	// 重建位图呈现器目标
	void				RecreateBitmapRenderTarget();
#ifdef USING_CPlusPlus_MODE
	// 刻画文字
	void				draw_text(const D2D1_RECT_F* rect, const BitmapCache::Key& string);
	// 刻画几何体
	void				draw_geometry(ID2D1Geometry* , FLOAT = 1.F, ID2D1StrokeStyle* = nullptr);
	// 填充几何体
	void				fill_geometry();
	// 填充位图
	void				fill_rect(const D2D1_RECT_F* pRect);
#endif
	// 获取宽度
	UINT				width()const{ return m_bmpSize.width; }
	// 获取高度
	UINT				height()const{ return m_bmpSize.height; }
	// 获取宽度
	float				widthf()const{ return m_bmpSizeF.width; }
	// 获取高度
	float				heightf()const{ return m_bmpSizeF.height; }
	// 获取位图矩形
	const D2D1_RECT_F*	get_rect()const{ return &m_Rect; }
#ifdef USING_CPlusPlus_MODE
	// 手动刷新笔刷 手动提高执行效率(C++ vs Java)
	void				refresh_brush(){
		SafeRelease(m_pNowBrush);
		if (m_pNowBrush = BrushCache::GetBrush(brush))
			m_pNowBrush->AddRef()
	}
	// 手动刷新字体 自动提高开发效率(Java vs C++)
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
	// 手动刷新字体 自动提高开发效率(Java vs C++)
	void				refresh_font(int index){
		SafeRelease(m_pNowFont);
		if (m_pNowFont = TextFormatCache::GetTextFormat(index))
			m_pNowFont->AddRef();
	}
#endif
public:
	// 传送源矩形 默认为位图大小
	D2D1_RECT_F					src_rect;
#ifdef USING_CPlusPlus_MODE
	// 字体
	TextFormat					font;
	// 笔刷
	GameBrush					brush;
#endif
private:
	// 位图矩形
	D2D1_RECT_F					m_Rect;
	// 位图大小
	struct{ UINT width, height; }m_bmpSize;
	// 位图大小
	D2D_SIZE_F 					m_bmpSizeF;
	// 当前转换矩阵
	D2D1_MATRIX_3X2_F			m_nowMatrix = D2D1::Matrix3x2F::Identity();
	// 当前笔刷
	ID2D1Brush*					m_pNowBrush = nullptr;
	// 当前字体
	IDWriteTextFormat*			m_pNowFont = nullptr;
	// D2D位图
	BitmapCache::Bitmap*		m_pd2dbitmap = nullptr;
	// 位图呈现器目标
	ID2D1BitmapRenderTarget*	m_pBitmapRT = nullptr;
public:
	// Ruby 对象数据
	RArray*						m_aryData = nullptr;
	// 对象映射
	static Map					s_mapRubytoCPlusPlus;
	// new 操作
	void*	operator			new(size_t size){ return MPool.Alloc(size); }
		// delete 操作
	void	operator			delete(void* p)	{ MPool.Free(p); }
	// new[] 操作
	void*	operator			new[](size_t size) = delete;
	// delete[] 操作
	void	operator			delete[](void* p) = delete;
	// ruby 端Bitmap类id
	static VALUE				s_rbBitmapID;
};