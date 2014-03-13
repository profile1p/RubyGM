#include "stdafx.h"
#include "include.h"



// 获取图片
// bitmapName	[in] : 文件名
// 返回: NULL表示失败 其余的则为位图的指针
BitmapCache::Bitmap* BitmapCache::GetBitmap(const BitmapCache::Key& bitmapName){
	if (*s_ppRenderTarget == NULL || *s_ppWICFactory == NULL)
		return NULL;
	BitmapCache::Bitmap* pBitmap;
	// 缓存中没有的话，从文件中读取
	BitmapCache::Map::iterator itr = s_mapBitmapCache.find(bitmapName);
	if (itr == s_mapBitmapCache.end()){
		// 读取成功的话
		if (SUCCEEDED(ImageRender::LoadBitmapFromFile(*s_ppRenderTarget, *s_ppWICFactory, bitmapName.c_str(), 0, 0, &pBitmap)))
			s_mapBitmapCache[bitmapName] = pBitmap;
		else
			s_mapBitmapCache[bitmapName] = NULL;
		return s_mapBitmapCache[bitmapName];
	}
	else
		return itr->second;
}


// 清除缓存
void BitmapCache::Clear(){
	for (BitmapCache::Map::iterator itr = s_mapBitmapCache.begin(); itr != s_mapBitmapCache.end(); ++itr){
		SafeRelease(itr->second);
	}
	s_mapBitmapCache.clear();
}


// 重建缓存
void BitmapCache::Recreate(){
#ifdef _DEBUG
	_cwprintf(L"缓存重建！\n");
#endif
	for (BitmapCache::Map::iterator itr = s_mapBitmapCache.begin(); itr != s_mapBitmapCache.end(); ++itr){
		SafeRelease(itr->second);
		itr->second = GetBitmap(itr->first);
	}
	//SceneManager::ReassignBitmap();
}