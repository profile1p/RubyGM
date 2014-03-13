#include "stdafx.h"
#include "include.h"

#define ppCacheRT BitmapCache::s_ppRenderTarget

// 对Ruby接口实现



// 新建Bitmap对象
VALUE Bitmap::NewObject(int argc, VALUE* argv, VALUE self){
	if (argc > 2 || argc == 0) {
		rb_raise(rb_eArgError, "wrong number of arguments\n参数数量出错");
		return self;
	}
	Bitmap* pBitmap = nullptr;
	if (argc == 2){
		pBitmap = new Bitmap(FIX2UINT(argv[0]), FIX2UINT(argv[1]));
		pBitmap->refresh_brush(0);
		pBitmap->refresh_font(0);
	}
	else{
		static wchar_t buffer[MAX_PATH];
		::MultiByteToWideChar(CP_UTF8, NULL, RSTRING_PTR(argv[0]), 
			RSTRING_LEN(argv[0]) + 1, buffer, sizeof buffer);
		pBitmap = new Bitmap(buffer);
	}
	check_pointer(pBitmap, "<Bitmap::NewObject>: new bitmap -> nullptr");
	VALUE rb_m_data = rb_ary_new2(EnumRubyObjectDataIndex::RUBY_OBJECT_DATA_SIZE);
	rb_iv_set(self, "@_edata_", rb_m_data);
	pBitmap->m_aryData = RARRAY(rb_m_data);
	rb_ary_store(rb_m_data, EnumRubyObjectDataIndex::ACheckCode, (VALUE)CHECK_CODE);
	rb_ary_store(rb_m_data, EnumRubyObjectDataIndex::BBitmapWidth, INT2FIX(pBitmap->width()));
	rb_ary_store(rb_m_data, EnumRubyObjectDataIndex::CBitmapHeight, INT2FIX(pBitmap->height()));
	rb_ary_store(rb_m_data, EnumRubyObjectDataIndex::DFont, INT2FIX(0));
	rb_ary_store(rb_m_data, EnumRubyObjectDataIndex::EBrush, INT2FIX(0));

	s_mapRubytoCPlusPlus.insert(vtype(self, pBitmap));
#ifdef _DEBUG
	_cwprintf(L"new bitmap: 0x%08X\n", (UINT)pBitmap);
#endif
	ROBJECT(self)->as.heap.ivptr = (VALUE*)pBitmap;
	return self;
}


// 释放Bitmap对象
VALUE Bitmap::Dispose(VALUE self){
	Bitmap* pThis = (Bitmap*)ROBJECT(self)->as.heap.ivptr;
#ifdef _DEBUG
	_cwprintf(L"delete bitmap: 0x%08X\n", (UINT)pThis);
#endif
#if defined(_DEBUG) || defined(_GAME_DEBUG)
	if (pThis->m_aryData->as.heap.ptr[EnumRubyObjectDataIndex::ACheckCode] != CHECK_CODE){
		MessageBoxW(nullptr, L"self: pointer invain\n指针无效!", L"<Bitmap::Dispose>: Error", MB_OK);
	}
#endif
	delete pThis;
	s_mapRubytoCPlusPlus.erase(self);
	return Qnil;
}

// font=
VALUE Bitmap::FontIs(VALUE self, VALUE font_id){
	Bitmap* pThis = (Bitmap*)ROBJECT(self)->as.heap.ivptr;
#if defined(_DEBUG) || defined(_GAME_DEBUG)
	if (pThis->m_aryData->as.heap.ptr[EnumRubyObjectDataIndex::ACheckCode] != CHECK_CODE){
		MessageBoxW(nullptr, L"self: pointer invain\n指针无效!", L"<Bitmap::FontIs>: Error", MB_OK);
	}
#endif
	rb_ary_store((VALUE)pThis->m_aryData, EnumRubyObjectDataIndex::DFont, font_id);
	//const_cast<VALUE*>(pThis->m_aryData->as.heap.ptr)[EnumRubyObjectDataIndex::DFont] = font_id;
	return font_id;
}

// brush=
VALUE Bitmap::BrushIs(VALUE self, VALUE brush_id){
	Bitmap* pThis = (Bitmap*)ROBJECT(self)->as.heap.ivptr;
#if defined(_DEBUG) || defined(_GAME_DEBUG)
	if (pThis->m_aryData->as.heap.ptr[EnumRubyObjectDataIndex::ACheckCode] != CHECK_CODE){
		MessageBoxW(nullptr, L"self: pointer invain\n指针无效!", L"<Bitmap::FontIs>: Error", MB_OK);
	}
#endif
	pThis->refresh_font(FIX2INT(brush_id));
	//rb_ary_store(pThis->rb_m_data, EnumRubyObjectDataIndex::DFont, font_id);
	const_cast<VALUE*>(pThis->m_aryData->as.heap.ptr)[EnumRubyObjectDataIndex::DFont] = brush_id;
	return brush_id;
}

// dup clone
VALUE Bitmap::DupClone(VALUE self){
#if defined(_DEBUG) || defined(_GAME_DEBUG)
	if (!s_rbBitmapID){
		MessageBoxW(nullptr,L"Bitmap class id is null\nBitmap类ID为0", L"<Bitmap::DupClone>Error", MB_OK);
		return Qnil;
	}
#endif
	Bitmap* pThis = (Bitmap*)ROBJECT(self)->as.heap.ivptr;
#if defined(_DEBUG) || defined(_GAME_DEBUG)
	if (pThis->m_aryData->as.heap.ptr[EnumRubyObjectDataIndex::ACheckCode] != CHECK_CODE){
		MessageBoxW(nullptr, L"self: pointer invain\n指针无效!", L"<Bitmap::FontIs>: Error", MB_OK);
	}
#endif
	VALUE argv[] = { INT2FIX(pThis->width()), INT2FIX(pThis->height()) };
	VALUE new_bmp = rb_class_new_instance(sizeof(argv) / sizeof(argv[0]), argv, s_rbBitmapID);
	Bitmap* pNew = (Bitmap*)ROBJECT(new_bmp)->as.heap.ivptr;
	if (SUCCEEDED(pNew->m_pd2dbitmap->CopyFromBitmap(nullptr, pThis->m_pd2dbitmap, nullptr))){
		return new_bmp;
	}
	rb_funcall(new_bmp, rb_intern("dispose"), 0);
	return Qnil;
}

// 刻画字体
VALUE Bitmap::DrawTextBaisc(int argc, VALUE* argv, VALUE self){
	RObject* obj = ROBJECT(self);
	Bitmap* pThis = (Bitmap*)obj->as.heap.ivptr;

	D2D1_RECT_F rect;
	static wchar_t buffer[MAX_PATH];
	UINT32 string_length = MAX_PATH;
	if (argc == 5){
		string_length = ::MultiByteToWideChar(CP_UTF8, NULL, RSTRING_PTR(argv[4]), 
			RSTRING_LEN(argv[4]) + 1, buffer, sizeof buffer);
		rect.left = RB_INT_FLOAT_TO_FLOAT(argv[0]);
		rect.top = RB_INT_FLOAT_TO_FLOAT(argv[1]);
		rect.right = RB_INT_FLOAT_TO_FLOAT(argv[2]);
		rect.bottom = RB_INT_FLOAT_TO_FLOAT(argv[3]);
		//string_length = pString->as.heap.len;
	}
	else{
		rb_raise(rb_eArgError, "wrong number of arguments\n参数数量出错");
		return self;
	}
	if (pThis->m_pBitmapRT){
		pThis->m_pBitmapRT->BeginDraw();
		pThis->m_pBitmapRT->SetTransform(&(pThis->m_nowMatrix));
		pThis->m_pBitmapRT->DrawText(buffer, string_length, pThis->m_pNowFont,
			&rect, pThis->m_pNowBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		if (D2DERR_RECREATE_TARGET == pThis->m_pBitmapRT->EndDraw()){
			BrushCache::Recreate();
			pThis->Bitmap::RecreateBitmapRenderTarget();
		}
#ifdef _DEBUG
		_cwprintf(L"刻画字体:\"%s\"\n", buffer);
#endif
	}
#if defined(_DEBUG) || defined(_GAME_DEBUG)
	else{
		_cwprintf(L"载入位图文件缓存的位图不可修改.\n");
		MessageBoxW(nullptr, L"Cached bitmap cannot changed, "
			L"please use dump or clone then call this method\n"
			L"禁止修改缓存", L"Error", MB_OK);
	}
#endif
	return self;
}

// 清除
void	Bitmap::Clear(){
	for (auto& i : s_mapRubytoCPlusPlus){
		if (i.second){
			delete i.second;
			i.second = nullptr;
		}
	}
	s_mapRubytoCPlusPlus.clear();

}



// 使用文件构建位图
Bitmap::Bitmap(const BitmapCache::Key& file_name) :
m_pd2dbitmap(BitmapCache::GetBitmap(file_name)), m_pBitmapRT(NULL){
	if (m_pd2dbitmap){
		// 添加计数
		m_pd2dbitmap->AddRef();
		m_bmpSizeF = m_pd2dbitmap->GetSize();
		src_rect = { 0.F, 0.F, m_bmpSizeF.width, m_bmpSizeF.height };
		m_bmpSize = { static_cast<UINT>(m_bmpSizeF.width), static_cast<UINT>(m_bmpSizeF.height) };
	}
	m_Rect = src_rect;
}


// 使用指定大小构建位图
Bitmap::Bitmap(UINT width, UINT height) :m_pd2dbitmap(NULL), m_pBitmapRT(NULL){
	//m_nowMatrix = D2D1::Matrix3x2F::Translation(D2D1::SizeF(10.F, 120.F));
	m_bmpSize = { width, height };
	src_rect = { 0.F, 0.F, static_cast<float>(width), static_cast<float>(height) };
	m_bmpSizeF = { static_cast<float>(width), static_cast<float>(height) };
	m_Rect = src_rect;
	RecreateBitmapRenderTarget();
#ifdef _DEBUG
	if (m_pBitmapRT){
		if (FAILED(m_pBitmapRT->GetBitmap((ID2D1Bitmap**)&m_pd2dbitmap)))
			_cwprintf(L"GetBitmap失败\n");
	}
	else{
		_cwprintf(L"GetBitmapRenderTarget失败\n");
	}
#else
	if (m_pBitmapRT)
		m_pBitmapRT->GetBitmap((ID2D1Bitmap**)&m_pd2dbitmap);
#endif
}


// 析构函数
Bitmap::~Bitmap(){
	SafeRelease(m_pNowBrush);
	SafeRelease(m_pNowFont);
	SafeRelease(m_pd2dbitmap);
	// 释放手动创建的位图 不要把if去掉，不然可能会释放缓存当中的位图
	if (m_pBitmapRT){
		SafeRelease(m_pBitmapRT);
	}
}


#ifdef USING_CPlusPlus_MODE
// 刻画几何体
void Bitmap::draw_geometry(ID2D1Geometry* pGeometry, FLOAT strokeWidth, ID2D1StrokeStyle* pStrokeStyle){
	if (m_pBitmapRT){
		m_pBitmapRT->BeginDraw();
		m_pBitmapRT->SetTransform(&m_nowMatrix);
		m_pBitmapRT->DrawGeometry(pGeometry, m_pNowBrush, strokeWidth, pStrokeStyle);
		if (D2DERR_RECREATE_TARGET == m_pBitmapRT->EndDraw()){
			BrushCache::Recreate();
			Bitmap::RecreateBitmapRenderTarget();
		}
	}
#ifdef _DEBUG
	else
		_cwprintf(L"载入位图文件缓存的位图不可修改.\n");
#endif

}

// 填空矩形区域
void Bitmap::fill_rect(const D2D1_RECT_F* pRect){
	if (m_pBitmapRT){
		m_pBitmapRT->BeginDraw();
		m_pBitmapRT->SetTransform(&m_nowMatrix);
		m_pBitmapRT->FillRectangle(pRect, m_pNowBrush);
		if (D2DERR_RECREATE_TARGET == m_pBitmapRT->EndDraw()){
			BrushCache::Recreate();
			Bitmap::RecreateBitmapRenderTarget();
		}
	}
#ifdef _DEBUG
	else
		_cwprintf(L"载入位图文件缓存的位图不可修改.\n");
#endif
}


// 刻画文字
void	Bitmap::draw_text(const D2D1_RECT_F* rect, const BitmapCache::Key& string){
	if (m_pBitmapRT){
		m_pBitmapRT->BeginDraw();
		m_pBitmapRT->SetTransform(&m_nowMatrix);
		m_pBitmapRT->DrawText(string.c_str(), string.size(), m_pNowFont,
			rect, m_pNowBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		if (D2DERR_RECREATE_TARGET == m_pBitmapRT->EndDraw()){
			BrushCache::Recreate();
			Bitmap::RecreateBitmapRenderTarget();
		}
#ifdef _DEBUG
		_cwprintf(L"刻画字体:\"%s\"\n", string.c_str());
#endif
	}
#ifdef _DEBUG
	else
		_cwprintf(L"载入位图文件缓存的位图不可修改.\n");
#endif
}
#endif;




// 创建位图呈现器目标
void Bitmap::RecreateBitmapRenderTarget(){
	if (m_pBitmapRT){
		SafeRelease(m_pd2dbitmap);
		SafeRelease(m_pBitmapRT);
	}
#ifdef _DEBUG
	if (!ppCacheRT)
		assert(0&&"BitmapCache::s_ppRenderTarget无效!\n");
	if (!(*ppCacheRT))
		assert(0&&L"BitmapCache::s_ppRenderTarget指向无效!\n");
#endif
	(*ppCacheRT)->CreateCompatibleRenderTarget(m_bmpSizeF, &m_pBitmapRT);
	m_pBitmapRT->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
	//m_pBitmapRT->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	//auto a = m_pBitmapRT->GetAntialiasMode();
}


// 刻画矩形
VALUE Bitmap::DrawRect(int argc, VALUE* argv, VALUE self){

	return Qfalse;
}
// 刻画圆角矩形
VALUE Bitmap::DrawRoundedRect(int argc, VALUE* argv, VALUE self){

	return Qfalse;
}
// 刻画线条
VALUE Bitmap::DrawLine(int argc, VALUE* argv, VALUE self){

	return Qfalse;
}
// 刻画椭圆
VALUE Bitmap::DrawEllipse(int argc, VALUE* argv, VALUE self){

	return Qfalse;
}
// 刻画几何体
VALUE Bitmap::DrawGeometry(int argc, VALUE* argv, VALUE self){
	return Qfalse;
}
// 填充几何体
VALUE Bitmap::FillGeometry(int argc, VALUE* argv, VALUE self){

	return Qfalse;
}
// 填充矩形
VALUE Bitmap::FillRect(int argc, VALUE* argv, VALUE self){

	return Qfalse;
}
// 填充圆角矩形
VALUE Bitmap::FillRoundedRect(int argc, VALUE* argv, VALUE self){

	return Qfalse;
}
// 填充椭圆
VALUE Bitmap::FillEllipse(int argc, VALUE* argv, VALUE self){

	return Qfalse;
}
// 填充遮罩层
VALUE Bitmap::FillOpacityMask(int argc, VALUE* argv, VALUE self){

	return Qfalse;
}

#undef ppCacheRT