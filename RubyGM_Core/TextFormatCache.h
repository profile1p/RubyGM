#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// ��;:	����Direct Write �� Text Format�Ļ������ ʹ�ù������� => C++ ģʽ
//			����Direct Write �� Text Format�Ļ������ ʹ��������� => Ruby ģʽ

#ifdef USING_CPlusPlus_MODE
struct TextFormat{
	// <
	bool operator<(const TextFormat& obj)const{
		return font_name < obj.font_name && font_size < obj.font_size && font_alignment < obj.font_alignment &&
			font_weight < obj.font_weight && font_style < obj.font_style;
	}
	// ��������
	BitmapCache::Key		font_name = AppInfo::DEFAULT_TEXT_FONT_NAME;
	// �����ϸ
	DWRITE_FONT_WEIGHT		font_weight = DWRITE_FONT_WEIGHT_NORMAL;
	// ��ʽ
	DWRITE_FONT_STYLE		font_style = DWRITE_FONT_STYLE_NORMAL;
	// ���䷽ʽ
	DWRITE_TEXT_ALIGNMENT	font_alignment = DWRITE_TEXT_ALIGNMENT_LEADING;
	// ��С ��λ: ����? DIP?
	float					font_size = AppInfo::DEFAULT_TEXT_FONT_SIZE;
};


// TF ����
class TextFormatCache{
public:
	// Direct Write����
	typedef ::IDWriteFactory1 DWriteFactory;
	// ��������
	typedef std::map<TextFormat, IDWriteTextFormat*> Map;
	// װ����
	typedef IDWriteFontCollectionLoader Loader;
	// ���弯 Font Collection
	typedef IDWriteFontCollection Collection;
public:
	// ��ʼ��
	static HRESULT	Init(DWriteFactory** ppO);
	// �������
	static void Clear();
	// ��ȡ����
	static IDWriteTextFormat*		GetTextFormat(const TextFormat& font);
	// ���ļ�����������������(Font Face)
	static HRESULT					CreateFontFaceFromFontFile(wchar_t* filename, IDWriteFontFace **ppFontFace);
private:
	// ��������
	static IDWriteTextFormat*		CreateTextFormat(const TextFormat& font);
private:
	// ͼ�񻺴�
	static Map						s_mapTextCache;
	// DWrite����
	static DWriteFactory**			s_ppDWriteFactory;
	// ��������������
	static Loader*					s_pLoader;
	// ���弯
	static Collection*				s_pCollection;
};
#else
// TF �̵�
class TextFormatCache{
public:
	// �ı���ʽ���ɲ���
	struct TextFormatParameter{
		// ���ṹ���С(��λ�ֽ�, ��������)
		UINT32						this_length = 0;
		// �汾
		UINT32						verson = 1;
		// �����С
		FLOAT						font_size = 22.F;
		// �����ϸ һ��Normal
		DWRITE_FONT_WEIGHT			font_weight = DWRITE_FONT_WEIGHT_NORMAL;
		// ��ʽ һ��Normal
		DWRITE_FONT_STYLE			font_style = DWRITE_FONT_STYLE_NORMAL;
		// ���� һ��Normal
		DWRITE_FONT_STRETCH			font_stretch = DWRITE_FONT_STRETCH_NORMAL;
		// �������з��� Ĭ������
		DWRITE_FLOW_DIRECTION		flow_direction = DWRITE_FLOW_DIRECTION_LEFT_TO_RIGHT;
		// ���������Ʊ�λ֮��Ĺ̶����� ��tab��ȣ� Ĭ���������С*4
		FLOAT						incremental_tabstop = 0.F;
		// �����о�-ȡ�䷽�� ָ�����ȷ���и� Ĭ��: DEFAULT
		DWRITE_LINE_SPACING_METHOD	line_spacing_method = DWRITE_LINE_SPACING_METHOD_DEFAULT;
		// �����о�-�о� �и߻�һ�����ߵ���һ�����ߵľ��� Ĭ��0.0
		FLOAT						line_spacing = 0.F;
		// �����о�-���� �ж��������ߵľ��� ����ֵ0.8F Ĭ��0.0
		FLOAT						baseline = 0.F;
		// �ж��� Ĭ�������(0)
		DWRITE_TEXT_ALIGNMENT		text_alignmen = DWRITE_TEXT_ALIGNMENT_LEADING;
		// ��(��)���� Ĭ���϶���(0)
		DWRITE_PARAGRAPH_ALIGNMENT	paragraph_alignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		// �Զ����� Ĭ�� �Զ�����(0)
		DWRITE_WORD_WRAPPING		word_wrapping = DWRITE_WORD_WRAPPING_WRAP;
		// 
		WCHAR						font_name[2];
	};
	// Rub �� �����ı���ʽ
	static VALUE RB_CreateFont(int argc, VALUE* argv, VALUE self);
public:
	// Direct Write����
	typedef ::IDWriteFactory1 DWriteFactory;
	// ��������
	typedef SArray<IDWriteTextFormat*, 1024> Array;
	// װ����
	typedef IDWriteFontCollectionLoader Loader;
	// ���弯 Font Collection
	typedef IDWriteFontCollection Collection;
public:
	// ��ʼ��
	static HRESULT	Init(DWriteFactory** ppO);// { s_ppDWriteFactory = ppO; }
	// �������
	static void		Clear();
	// ��ȡTF
	static IDWriteTextFormat*		GetTextFormat(const UINT index){ return s_aryTextFormat[index]; }
	// ���ļ�����������������(Font Face)
	static HRESULT					CreateFontFaceFromFontFile(wchar_t* filename, IDWriteFontFace **ppFontFace);
	// ����TF
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
		MessageBoxW(nullptr, L"<TextFormatCache::SetTextFormat>: Out of range\n�����ı���ʽ������Χ."
			, L"Error", MB_OK);
#endif
		return FALSE;
	}
private:
	// ����TF
	static IDWriteTextFormat*		CreateTextFormat(const TextFormatParameter*);
	// TF����
	static Array					s_aryTextFormat;
	// ��������������
	static Loader*					s_pLoader;
	// ���弯
	static Collection*				s_pCollection;
public:
	// DWrite����
	static DWriteFactory**			s_ppDWriteFactory;
};

#endif