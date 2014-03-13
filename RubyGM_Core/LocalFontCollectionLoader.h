#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途: Direct Write 本地文字集载入器

// 注: 虚函数不能是内联，所以不用担心代码长度

class LocalFontCollectionLoader : public IDWriteFontCollectionLoader {
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) {
		if (ppvObject == NULL)
			return E_INVALIDARG;

		if (IsEqualIID(riid, __uuidof(IUnknown))) {
			*ppvObject = static_cast<IUnknown*>(this);
			AddRef();
			return S_OK;
		}
		else if (IsEqualIID(riid, __uuidof(IDWriteFontCollectionLoader))) {
			*ppvObject = static_cast<IDWriteFontCollectionLoader*>(this);
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

	virtual HRESULT STDMETHODCALLTYPE CreateEnumeratorFromKey(
		IDWriteFactory *pFactory,
		const void *collectionKey,
		UINT32 collectionKeySize,
		IDWriteFontFileEnumerator **ppFontFileEnumerator
		) {
		WCHAR** pFilePath = (WCHAR**)collectionKey;

		if (pFactory == NULL || ppFontFileEnumerator == NULL)
			return E_INVALIDARG;
		
		LocalFontFileEnumerator *pEnumerator = new LocalFontFileEnumerator(pFactory);

		if (!pEnumerator)
			return E_OUTOFMEMORY;
		HRESULT hr;
		hr = pEnumerator->Initialize(pFilePath, collectionKeySize/sizeof(WCHAR*));
		if (FAILED(hr))
		{
			SafeRelease(pEnumerator);
			return hr;
		}
		*ppFontFileEnumerator = pEnumerator;
		return S_OK;
	}

public:
	LocalFontCollectionLoader()://const WCHAR **szFontfilePath, UINT file_count) :
		//m_pfilepath(szFontfilePath),
		//m_cFileCount(file_count),
		m_cRefCount(1)
	{
	}

private:
	~LocalFontCollectionLoader() {
	}

private:
	ULONG                       m_cRefCount;
	//UINT						m_cFileCount;
	//const WCHAR**               m_pfilepath;

private:
	LocalFontCollectionLoader(const LocalFontCollectionLoader&) = delete;
	LocalFontCollectionLoader& operator=(const LocalFontCollectionLoader&) = delete;
};