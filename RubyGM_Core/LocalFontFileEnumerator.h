#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// ��;: Direct Write ��������ö����

class LocalFontFileEnumerator : public IDWriteFontFileEnumerator {
public:
	// ���� ->_<-
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
	// ��ʼ��
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
	// ���ü���
	ULONG                       m_cRefCount;
	// �ļ�����
	UINT                        m_cFileCount;
	// ��ǰ�ļ��������
	UINT						m_iFileIndex;
	// ��ǰDirect Write Font File
	IDWriteFontFile*			m_pCurFontFie;
	// DWrite ����
	IDWriteFactory*             m_pFactory;
	// �ļ�·�� ����
	WCHAR**						m_pFilePath;
private:
	LocalFontFileEnumerator(const LocalFontFileEnumerator&) = delete;
	LocalFontFileEnumerator& operator=(const LocalFontFileEnumerator&) = delete;
};