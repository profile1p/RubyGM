#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途:	使用Direct2D承载DirectShow渲染Filter
//			效率较差，因为每帧都需要从内存复制到显存，不过播放视频足够了
//			需要注册"D2D(→_→)Video(←_←)Renderer" Renderer Filter


// {2A63FDFF-ADE3-40CF-AF6C-9D7C242C0273}
static const GUID CLSID_DustDirect2DVideoRenderer =
{ 0x2a63fdff, 0xade3, 0x40cf, { 0xaf, 0x6c, 0x9d, 0x7c, 0x24, 0x2c, 0x2, 0x73 } };

// {9E705921-A075-48A0-8A68-CA8534C6C499}
static const GUID IID_IDustDirect2DVideoRenderer =
{ 0x9e705921, 0xa075, 0x48a0, { 0x8a, 0x68, 0xca, 0x85, 0x34, 0xc6, 0xc4, 0x99 } };

#define FILTER_NAME L"D2D(→_→)Video(←_←)Renderer"


// 接口声明 该类提供接口
DECLARE_INTERFACE_(ID2DVideoRenderer, IUnknown)
{
	STDMETHOD(SetRenderTarget)(ID2D1RenderTarget** ppRenderTarget) PURE;
};
