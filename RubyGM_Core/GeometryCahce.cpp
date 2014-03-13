#include "stdafx.h"
#include "include.h"




// �����ı�����
HRESULT GeometryCahce::CreateFontGeometry(FontGeometryParameter * pParameter){
	// ������
	if (!pParameter)
		return E_INVALIDARG;
	// ����ռ�
	UINT* pCodePoints = static_cast<UINT*>(MPool.Alloc(pParameter->uStringLength * sizeof UINT));
	if (!pCodePoints)
		return E_OUTOFMEMORY;
	UINT16* pGlyphIndices = static_cast<UINT16*>(MPool.Alloc(pParameter->uStringLength * sizeof UINT16));
	if (!pCodePoints){
		MPool.Free(pCodePoints);
		return E_OUTOFMEMORY;
	}
	// UCS2 -> UCS4 ǰ���0���ɣ�������ת��
	for (UINT i = 0; i < pParameter->uStringLength; ++i)
		pCodePoints[i] = pParameter->pString[i];
	// ����·������
	HRESULT hr = (*s_ppD2DFactory)->CreatePathGeometry(&(pParameter->pPathGeometry));
	GeometryCahce::GeometrySink* pGeometrySink(nullptr);
	if (SUCCEEDED(hr)){
		if (SUCCEEDED(hr = (pParameter->pPathGeometry)->Open(&pGeometrySink))){
			// �����������
			hr = pParameter->pFontFace->GetGlyphIndices(pCodePoints, pParameter->uStringLength, pGlyphIndices);
		}
		else{
			SafeRelease(pParameter->pPathGeometry);
		}
	}

	if (SUCCEEDED(hr)){
		// ��������·������
		hr = pParameter->pFontFace->GetGlyphRunOutline(pParameter->emSize, pGlyphIndices, nullptr, nullptr,
			pParameter->uStringLength, pParameter->isSideways, pParameter->isRightToLeft,
			pGeometrySink);
	}
	if (SUCCEEDED(hr)){
		// �رռ��δ���
		hr = pGeometrySink->Close();
	}
	SafeRelease(pGeometrySink);
	// �ͷſռ�
	MPool.Free(pGlyphIndices);
	MPool.Free(pCodePoints);
	return S_OK;
}
