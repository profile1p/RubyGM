#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途:	对于Direct Write 的 Text Format的缓存管理 使用关联容器 => C++ 模式
//			对于Direct Write 的 Text Format的缓存管理 使用数组管理 => Ruby 模式

#ifdef USING_CPlusPlus_MODE
struct TextFormat{
	// <
	bool operator<(const TextFormat& obj)const{
		return font_name < obj.font_name && font_size < obj.font_size && font_alignment < obj.font_alignment &&
			font_weight < obj.font_weight && font_style < obj.font_style;
	}
	// 字体名称
	BitmapCache::Key		font_name = AppInfo::DEFAULT_TEXT_FONT_NAME;
	// 字体粗细
	DWRITE_FONT_WEIGHT		font_weight = DWRITE_FONT_WEIGHT_NORMAL;
	// 样式
	DWRITE_FONT_STYLE		font_style = DWRITE_FONT_STYLE_NORMAL;
	// 对其方式
	DWRITE_TEXT_ALIGNMENT	font_alignment = DWRITE_TEXT_ALIGNMENT_LEADING;
	// 大小 单位: 像素? DIP?
	float					font_size = AppInfo::DEFAULT_TEXT_FONT_SIZE;
};


// TF 缓存
class TextFormatCache{
public:
	// Direct Write工厂
	typedef ::IDWriteFactory1 DWriteFactory;
	// 关联容器
	typedef std::map<TextFormat, IDWriteTextFormat*> Map;
	// 装载器
	typedef IDWriteFontCollectionLoader Loader;
	// 字体集 Font Collection
	typedef IDWriteFontCollection Collection;
public:
	// 初始化
	static HRESULT	Init(DWriteFactory** ppO);
	// 清除缓存
	static void Clear();
	// 获取字体
	static IDWriteTextFormat*		GetTextFormat(const TextFormat& font);
	// 从文件创建绝对字体引用(Font Face)
	static HRESULT					CreateFontFaceFromFontFile(wchar_t* filename, IDWriteFontFace **ppFontFace);
private:
	// 创建字体
	static IDWriteTextFormat*		CreateTextFormat(const TextFormat& font);
private:
	// 图像缓存
	static Map						s_mapTextCache;
	// DWrite工厂
	static DWriteFactory**			s_ppDWriteFactory;
	// 本地字体载入器
	static Loader*					s_pLoader;
	// 字体集
	static Collection*				s_pCollection;
};
#else
// TF 商店
class TextFormatCache{
public:
	// 文本格式生成参数
	struct TextFormatParameter{
		// 本结构体大小(单位字节, 允许柔性)
		UINT32						this_length = 0;
		// 版本
		UINT32						verson = 1;
		// 字体大小
		FLOAT						font_size = 22.F;
		// 字体粗细 一般Normal
		DWRITE_FONT_WEIGHT			font_weight = DWRITE_FONT_WEIGHT_NORMAL;
		// 样式 一般Normal
		DWRITE_FONT_STYLE			font_style = DWRITE_FONT_STYLE_NORMAL;
		// 拉伸 一般Normal
		DWRITE_FONT_STRETCH			font_stretch = DWRITE_FONT_STRETCH_NORMAL;
		// 段落排列方向 默认左到右
		DWRITE_FLOW_DIRECTION		flow_direction = DWRITE_FLOW_DIRECTION_LEFT_TO_RIGHT;
		// 两个相邻制表位之间的固定距离 即tab宽度， 默认是字体大小*4
		FLOAT						incremental_tabstop = 0.F;
		// 设置行距-取间方法 指定如何确定行高 默认: DEFAULT
		DWRITE_LINE_SPACING_METHOD	line_spacing_method = DWRITE_LINE_SPACING_METHOD_DEFAULT;
		// 设置行距-行距 行高或一条基线到另一条基线的距离 默认0.0
		FLOAT						line_spacing = 0.F;
		// 设置行距-基线 行顶部到基线的距离 建议值0.8F 默认0.0
		FLOAT						baseline = 0.F;
		// 行对齐 默认左对齐(0)
		DWRITE_TEXT_ALIGNMENT		text_alignmen = DWRITE_TEXT_ALIGNMENT_LEADING;
		// 段(行)对齐 默认上对齐(0)
		DWRITE_PARAGRAPH_ALIGNMENT	paragraph_alignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		// 自动换行 默认 自动换行(0)
		DWRITE_WORD_WRAPPING		word_wrapping = DWRITE_WORD_WRAPPING_WRAP;
		// 
		WCHAR						font_name[2];
	};
	// Rub 端 创建文本格式
	static VALUE RB_CreateFont(int argc, VALUE* argv, VALUE self);
public:
	// Direct Write工厂
	typedef ::IDWriteFactory1 DWriteFactory;
	// 关联容器
	typedef SArray<IDWriteTextFormat*, 1024> Array;
	// 装载器
	typedef IDWriteFontCollectionLoader Loader;
	// 字体集 Font Collection
	typedef IDWriteFontCollection Collection;
public:
	// 初始化
	static HRESULT	Init(DWriteFactory** ppO);// { s_ppDWriteFactory = ppO; }
	// 清除缓存
	static void		Clear();
	// 获取TF
	static IDWriteTextFormat*		GetTextFormat(const UINT index){ return s_aryTextFormat[index]; }
	// 从文件创建绝对字体引用(Font Face)
	static HRESULT					CreateFontFaceFromFontFile(wchar_t* filename, IDWriteFontFace **ppFontFace);
	// 设置TF
	static BOOL						SetTextFormat(const UINT i, TextFormatParameter* p){
		if (p && i < s_aryTextFormat.size()) {
			if (p->incremental_tabstop <= 0.F)
				p->incremental_tabstop = p->font_size * (4.f * 96.0f / 72.f);
			SafeRelease(s_aryTextFormat[i]);
			IDWriteTextFormat*	pTF(CreateTextFormat(p));
			s_aryTextFormat[i] = pTF;
			// return pBrush;
			return pTF ? TRUE : FALSE;
		}
#if defined(_DEBUG) || defined(_GAME_DEBUG)
		MessageBoxW(nullptr, L"<TextFormatCache::SetTextFormat>: Out of range\n设置文本格式超过范围."
			, L"Error", MB_OK);
#endif
		return FALSE;
	}
private:
	// 创建TF
	static IDWriteTextFormat*		CreateTextFormat(const TextFormatParameter*);
	// TF数组
	static Array					s_aryTextFormat;
	// 本地字体载入器
	static Loader*					s_pLoader;
	// 字体集
	static Collection*				s_pCollection;
public:
	// DWrite工厂
	static DWriteFactory**			s_ppDWriteFactory;
};

#endif