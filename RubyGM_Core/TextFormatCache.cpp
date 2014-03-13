#include "stdafx.h"
#include "include.h"

// 初始化
HRESULT	TextFormatCache::Init(DWriteFactory** ppO) {
	s_ppDWriteFactory = ppO;
#ifdef _DEBUG
	if (!s_ppDWriteFactory || !(*s_ppDWriteFactory)){
		MessageBoxW(nullptr, L"参数错误!\n INVALIDARG!", L"Error", MB_OK);
		return E_INVALIDARG;
	}
#endif
	// 检查自定义字体
	HRESULT hr = S_OK;
	UINT fileCount = 0;
	SArray<WCHAR*, 256> aryFilePath(nullptr);
	WIN32_FIND_DATA fileinfo;
	HANDLE hFile = FindFirstFileW(L"Font/*.*tf", &fileinfo);
	DWORD errorcode = 0;
	while (hFile != INVALID_HANDLE_VALUE&&errorcode != ERROR_NO_MORE_FILES)
	{
		aryFilePath[fileCount] = (WCHAR*)MPool.Alloc(MAX_PATH);
		if (!aryFilePath[fileCount]){
			hr = E_OUTOFMEMORY;
			break;
		}
		//wcscpy_s(aryFilePath[fileCount], MAX_PATH, fileinfo.cFileName);
		wsprintf(aryFilePath[fileCount], L"Font/%s", fileinfo.cFileName);
		++fileCount;
		FindNextFile(hFile, &fileinfo);
		errorcode = GetLastError();
	}
	FindClose(hFile);
	hFile = nullptr;
	// 
	if (!fileCount)
		goto seek_text_format;

	// 载入字体
	s_pLoader = new LocalFontCollectionLoader;
	if (!s_pLoader)
		return E_OUTOFMEMORY;
	hr = (*s_ppDWriteFactory)->RegisterFontCollectionLoader(s_pLoader);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"RegisterFontCollectionLoader 失败-Failed", L"Error", MB_OK);
		return hr;
	}
	//
	hr = (*s_ppDWriteFactory)->CreateCustomFontCollection(s_pLoader, aryFilePath, fileCount * sizeof aryFilePath[0], &s_pCollection);
	if (FAILED(hr)) {
		MessageBoxW(nullptr, L"CreateCustomFontCollection 失败-Failed", L"Error", MB_OK);
		return hr;
	}
	// 载入系统笔刷
seek_text_format:
	std::ifstream ifs("Data/system.tft", std::ios::binary);
	UINT32 data, length;
	TextFormatCache::TextFormatParameter* pParameter(nullptr);
	if (ifs.good()){
		// 读取首数据
		ifs.read((char*)&data, sizeof data);
		for (UINT i = 0; i < data; ++i){
			// 读取长度
			ifs.read((char*)&length, sizeof length);
			if (length){
				if (pParameter = (TextFormatParameter*)MPool.Alloc(length)){
					pParameter->this_length = length;
					ifs.read((char*)&(pParameter->verson), length - sizeof(length));
					// 检查版本
					// if(pParameter->verson)
					SetTextFormat(i, pParameter);
					MPool.Free(pParameter);
				}
			}
		}
		ifs.close();
	}
	// 设定C++笔刷
	return S_OK;
}

#ifdef USING_CPlusPlus_MODE
// 获取字体格式
IDWriteTextFormat*		TextFormatCache::GetTextFormat(const TextFormat& font){
	TextFormatCache::Map::iterator itr = s_mapTextCache.find(font);
	if (itr != s_mapTextCache.end())
		return itr->second;
	return CreateTextFormat(font);
}


// 创建字体格式
IDWriteTextFormat*		TextFormatCache::CreateTextFormat(const TextFormat& font){
#ifdef _DEBUG
	TextFormatCache::Map::iterator itr = s_mapTextCache.find(font);
	if (itr != s_mapTextCache.end()){
		_cwprintf(L"目标字体已存在.\n");
		return itr->second;
	}
	if (!s_ppDWriteFactory){
		_cwprintf(L"s_ppDWriteFactory未初始化.\n");
		return NULL;
	}
	if (!(*s_ppDWriteFactory)){

		_cwprintf(L"DWriteFactory未初始化.\n");
		return NULL;
	}
#endif
	IDWriteTextFormat* pTF;
	
	(*s_ppDWriteFactory)->CreateTextFormat(
		font.font_name.c_str(),
		s_pCollection,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		font.font_size * (96.0f / 72.f),
		L"", //locale
		&pTF
		);
	if (pTF){
		pTF->SetTextAlignment(font.font_alignment);
	}
#ifdef _DEBUG
	else
		_cwprintf(L"IDWriteTextFormat 创建失败.\n");
#endif
	s_mapTextCache.insert(Map::value_type(font, pTF));
	return pTF;
}


// 清除缓存
void		TextFormatCache::Clear(){
	for (TextFormatCache::Map::iterator itr = s_mapTextCache.begin(); itr != s_mapTextCache.end(); ++itr)
		SafeRelease(itr->second);
	s_mapTextCache.clear();
	// COM 组件清理
	SafeRelease(s_pCollection);
	(*s_ppDWriteFactory)->UnregisterFontCollectionLoader(s_pLoader);
	SafeRelease(s_pLoader);
}
#else

// 清除缓存
void		TextFormatCache::Clear(){
	for (auto& i : s_aryTextFormat){
		SafeRelease(i);
	}
}


VALUE TextFormatCache::RB_CreateFont(int argc, VALUE* argv, VALUE self){
	if (argc < 3 || argc > 13) {
		rb_raise(rb_eArgError, "wrong number of arguments\n参数数量出错");
		return Qfalse;
	}
	/*
	参数说明:
	create_font(index, font_name, font_size, [font_weight, font_style, 
	font_stretch, text_alignmen, paragraph_alignment, word_wrapping, 
	incremental_tabstop, line_spacing_method, line_spacing, baseline])
	*/
	wchar_t buffer[MAX_PATH];
	UINT length = ::MultiByteToWideChar(CP_UTF8, NULL, RSTRING_PTR(argv[1]), 
		RSTRING_LEN(argv[1]) + 1, buffer, sizeof buffer);
	// 多用几字节又不会死
	UINT this_length = length + sizeof TextFormatParameter;
	TextFormatParameter* pParameter((TextFormatParameter*)MPool.Alloc(this_length));
	if (pParameter){
		new(pParameter)TextFormatParameter();
		pParameter->this_length = this_length;
		// font_name
		wcscpy_s(pParameter->font_name, length, buffer);
		// font_size
		pParameter->font_size = RB_INT_FLOAT_TO_FLOAT(argv[2]);
		// font_weight
		if (argc >= 4){
			pParameter->font_weight = (DWRITE_FONT_WEIGHT)FIX2INT(argv[3]);
		}
		// font_style
		if (argc >= 5){
			pParameter->font_style = (DWRITE_FONT_STYLE)FIX2INT(argv[4]);
		}
		// font_style
		if (argc >= 6){
			pParameter->font_stretch = (DWRITE_FONT_STRETCH)FIX2INT(argv[5]);
		}
		// text_alignmen
		if (argc >= 7){
			pParameter->text_alignmen = (DWRITE_TEXT_ALIGNMENT)FIX2INT(argv[6]);
		}
		// paragraph_alignment
		if (argc >= 8){
			pParameter->paragraph_alignment = (DWRITE_PARAGRAPH_ALIGNMENT)FIX2INT(argv[7]);
		}
		// word_wrapping
		if (argc >= 9){
			pParameter->word_wrapping = (DWRITE_WORD_WRAPPING)FIX2INT(argv[8]);
		}
		// incremental_tabstop
		if (argc >= 10){
			pParameter->incremental_tabstop = RB_INT_FLOAT_TO_FLOAT(argv[9]);
		}
		// line_spacing_method
		if (argc >= 11){
			pParameter->line_spacing_method = (DWRITE_LINE_SPACING_METHOD)FIX2INT(argv[10]);
		}
		// line_spacing
		if (argc >= 12){
			pParameter->line_spacing = RB_INT_FLOAT_TO_FLOAT(argv[11]);
		}
		// baseline
		if (argc >= 13){
			pParameter->baseline = RB_INT_FLOAT_TO_FLOAT(argv[12]);
		}
		auto rcode = SetTextFormat(FIX2INT(*argv), pParameter);
		MPool.Free(pParameter);
		return rcode ? Qtrue : Qfalse;
	}
	return Qfalse;
}

// 创建TF
IDWriteTextFormat*	TextFormatCache::CreateTextFormat(const TextFormatParameter* pParameter){
	IDWriteTextFormat* pTextFormat(nullptr);
#ifdef _DEBUG
	if (!s_ppDWriteFactory){
		_cwprintf(L"s_ppDWriteFactory未初始化.\n");
		return NULL;
	}
	if (!(*s_ppDWriteFactory)){
		_cwprintf(L"DWriteFactory未初始化.\n");
		return NULL;
	}
#endif
	(*s_ppDWriteFactory)->CreateTextFormat(
		pParameter->font_name,
		s_pCollection,
		pParameter->font_weight,
		pParameter->font_style,
		pParameter->font_stretch,
		pParameter->font_size * (96.0f / 72.f),
		L"", //locale
		&pTextFormat
		);
	if (pTextFormat){
#ifdef _DEBUG
		HRESULT hr(S_OK);
		// 设置行对齐
		if (SUCCEEDED(hr))
			hr = pTextFormat->SetTextAlignment(pParameter->text_alignmen);
		// 设置列对齐
		if (SUCCEEDED(hr))
			hr = pTextFormat->SetParagraphAlignment(pParameter->paragraph_alignment);
		// 设置自动换行
		if (SUCCEEDED(hr))
			hr = pTextFormat->SetWordWrapping(pParameter->word_wrapping);
		// 设置制表符宽度
		if (SUCCEEDED(hr))
			hr = pTextFormat->SetIncrementalTabStop(pParameter->incremental_tabstop);
		// 段落排列方向
		//if (SUCCEEDED(hr))
			//hr = pTextFormat->SetFlowDirection(DWRITE_FLOW_DIRECTION_BOTTOM_TO_TOP);
		// 设置行距
		if (SUCCEEDED(hr))
			hr = pTextFormat->SetLineSpacing(pParameter->line_spacing_method, pParameter->line_spacing,
			pParameter->baseline);
		if (FAILED(hr)){
			MessageBoxW(nullptr, L"<TextFormatCache::CreateTextFormat>: OK to created but failed to set\n"
				L"创建成功但是设置失败", L"Error", MB_OK);
			SafeRelease(pTextFormat);
			return nullptr;
		}
#else
		// 设置行对齐
		pTextFormat->SetTextAlignment(pParameter->text_alignmen);
		// 设置列对齐
		pTextFormat->SetParagraphAlignment(pParameter->paragraph_alignment);
		// 设置自动换行
		pTextFormat->SetWordWrapping(pParameter->word_wrapping);
		// 设置制表符宽度
		pTextFormat->SetIncrementalTabStop(pParameter->incremental_tabstop);
		// 段落排列方向
		//pTextFormat->SetFlowDirection(pParameter->flow_direction);
		// 设置行距
		pTextFormat->SetLineSpacing(pParameter->line_spacing_method, pParameter->line_spacing,
			pParameter->baseline);
#endif
		return pTextFormat;
	}
	return nullptr;
	//s_aryTextFormat[idx]->SetTrimming();
}
#endif

HRESULT TextFormatCache::CreateFontFaceFromFontFile(wchar_t* filename, IDWriteFontFace **ppFontFace)
{
	HRESULT hr = S_OK;

	IDWriteFontFace* pFontFace = NULL;
	IDWriteFontFile* pFontFiles = NULL;

	//Gets a filename from a application directory
	WCHAR fontFolderPath[MAX_PATH];
	UINT curDirLength = GetCurrentDirectory(
		sizeof(fontFolderPath),
		fontFolderPath
		);

	if (curDirLength == 0)
	{
		return E_UNEXPECTED;
	}

	if (SUCCEEDED(hr))
	{
		hr = StringCchCatW(
			fontFolderPath,
			sizeof(fontFolderPath),
			L"\\"
			);
	}

	if (SUCCEEDED(hr))
	{
		hr = StringCchCatW(
			fontFolderPath,
			sizeof(fontFolderPath),
			(STRSAFE_LPCWSTR)filename
			);
	}

	if (SUCCEEDED(hr))
	{
		hr = (*s_ppDWriteFactory)->CreateFontFileReference(
			fontFolderPath,
			NULL,
			&pFontFiles);
	}

	IDWriteFontFile* fontFileArray[] = { pFontFiles };

	if (SUCCEEDED(hr))
	{
		hr = (*s_ppDWriteFactory)->CreateFontFace(
			DWRITE_FONT_FACE_TYPE_TRUETYPE,
			1, // file count
			fontFileArray,
			0,
			DWRITE_FONT_SIMULATIONS_NONE,
			&pFontFace
			);
	}

	//Return the pointer to the caller
	*ppFontFace = pFontFace;

	SafeRelease(pFontFiles);

	return hr;
}
