#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途: Direct Write 本地文字枚举器

class LocalFontFileEnumerator : public IDWriteFontFileEnumerator {
public:
	// 创建 ->_<-
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) {
		if (ppvObject == NULL)
			return E_INVALIDARG;

		if (IsEqualIID(riid, __uuidof(IUnknown))) {
			*ppvObject = static_cast<IUnknown*>(this);
			AddRef();
			return S_OK;
		}
		else if (IsEqualIID(riid, __uuidof(IDWriteFontFileEnumerator))) {
			*ppvObject = static_cast<IDWriteFontFileEnumerator*>(this);
			AddRef();
			return S_OK;
		}

		*ppvObject = NULL;
		return E_NOINTERFACE;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef() {
		return static_cast<ULONG>(InterlockedIncrement(reinterpret_cast<LONG*>(&m_cRefCount)));
	}

	virtual ULONG STDMETHODCALLTYPE Release() {
		ULONG newCount = static_cast<ULONG>(InterlockedDecrement(reinterpret_cast<LONG*>(&m_cRefCount)));

		if (newCount == 0)
			delete this;

		return newCount;
	}

	virtual HRESULT STDMETHODCALLTYPE GetCurrentFontFile(IDWriteFontFile **ppFontFile) {
		if (!ppFontFile)
			return E_INVALIDARG;

		if (!m_pFilePath || !m_pFactory)
			return E_FAIL;

		*ppFontFile = SafeAcquire(m_pCurFontFie);

		return m_pCurFontFie ? S_OK : E_FAIL;
	}

	virtual HRESULT STDMETHODCALLTYPE MoveNext(BOOL *pHasCurrentFile) {

		if (pHasCurrentFile == NULL)
			return E_INVALIDARG;

		if (!m_pFilePath || !m_pFactory)
			return E_FAIL;

		HRESULT hr = S_OK;
		if (*pHasCurrentFile = m_iFileIndex < m_cFileCount){
			hr = m_pFactory->CreateFontFileReference(m_pFilePath[m_iFileIndex], nullptr, &m_pCurFontFie);
			if (*pHasCurrentFile = SUCCEEDED(hr)){
				++m_iFileIndex;
			}
		}
		return hr;
	}

public:
	LocalFontFileEnumerator(IDWriteFactory *pFontFile) :
		m_cRefCount(1),
		m_pFactory(SafeAcquire(pFontFile)),
		m_cFileCount(0),
		m_iFileIndex(0),
		m_pFilePath(nullptr),
		m_pCurFontFie(nullptr)
	{
	}
	// 初始化
	HRESULT Initialize(WCHAR ** pFilePath, const UINT cFileCount){
		if (!pFilePath || !(*pFilePath))
			return E_INVALIDARG;
		m_cFileCount = cFileCount;
		m_iFileIndex = 0;
		m_pFilePath = pFilePath;
		return S_OK;
	}
private:
	~LocalFontFileEnumerator() {
		SafeRelease(m_pFactory);
	}

private:
	// 引用计数
	ULONG                       m_cRefCount;
	// 文件数量
	UINT                        m_cFileCount;
	// 当前文件索引编号
	UINT						m_iFileIndex;
	// 当前Direct Write Font File
	IDWriteFontFile*			m_pCurFontFie;
	// DWrite 工厂
	IDWriteFactory*             m_pFactory;
	// 文件路径 数组
	WCHAR**						m_pFilePath;
private:
	LocalFontFileEnumerator(const LocalFontFileEnumerator&) = delete;
	LocalFontFileEnumerator& operator=(const LocalFontFileEnumerator&) = delete;
};