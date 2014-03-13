#include "stdafx.h"
#include "include.h"




// 创建文本几何
HRESULT GeometryCahce::CreateFontGeometry(FontGeometryParameter * pParameter){
	// 检查参数
	if (!pParameter)
		return E_INVALIDARG;
	// 申请空间
	UINT* pCodePoints = static_cast<UINT*>(MPool.Alloc(pParameter->uStringLength * sizeof UINT));
	if (!pCodePoints)
		return E_OUTOFMEMORY;
	UINT16* pGlyphIndices = static_cast<UINT16*>(MPool.Alloc(pParameter->uStringLength * sizeof UINT16));
	if (!pCodePoints){
		MPool.Free(pCodePoints);
		return E_OUTOFMEMORY;
	}
	// UCS2 -> UCS4 前面加0即可，即不做转换
	for (UINT i = 0; i < pParameter->uStringLength; ++i)
		pCodePoints[i] = pParameter->pString[i];
	// 创建路径集合
	HRESULT hr = (*s_ppD2DFactory)->CreatePathGeometry(&(pParameter->pPathGeometry));
	GeometryCahce::GeometrySink* pGeometrySink(nullptr);
	if (SUCCEEDED(hr)){
		if (SUCCEEDED(hr = (pParameter->pPathGeometry)->Open(&pGeometrySink))){
			// 创建索引编号
			hr = pParameter->pFontFace->GetGlyphIndices(pCodePoints, pParameter->uStringLength, pGlyphIndices);
		}
		else{
			SafeRelease(pParameter->pPathGeometry);
		}
	}

	if (SUCCEEDED(hr)){
		// 创建轮廓路径几何
		hr = pParameter->pFontFace->GetGlyphRunOutline(pParameter->emSize, pGlyphIndices, nullptr, nullptr,
			pParameter->uStringLength, pParameter->isSideways, pParameter->isRightToLeft,
			pGeometrySink);
	}
	if (SUCCEEDED(hr)){
		// 关闭几何创建
		hr = pGeometrySink->Close();
	}
	SafeRelease(pGeometrySink);
	// 释放空间
	MPool.Free(pGlyphIndices);
	MPool.Free(pCodePoints);
	return S_OK;
}
