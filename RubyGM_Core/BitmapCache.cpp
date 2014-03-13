#include "stdafx.h"
#include "include.h"



// ��ȡͼƬ
// bitmapName	[in] : �ļ���
// ����: NULL��ʾʧ�� �������Ϊλͼ��ָ��
BitmapCache::Bitmap* BitmapCache::GetBitmap(const BitmapCache::Key& bitmapName){
	if (*s_ppRenderTarget == NULL || *s_ppWICFactory == NULL)
		return NULL;
	BitmapCache::Bitmap* pBitmap;
	// ������û�еĻ������ļ��ж�ȡ
	BitmapCache::Map::iterator itr = s_mapBitmapCache.find(bitmapName);
	if (itr == s_mapBitmapCache.end()){
		// ��ȡ�ɹ��Ļ�
		if (SUCCEEDED(ImageRender::LoadBitmapFromFile(*s_ppRenderTarget, *s_ppWICFactory, bitmapName.c_str(), 0, 0, &pBitmap)))
			s_mapBitmapCache[bitmapName] = pBitmap;
		else
			s_mapBitmapCache[bitmapName] = NULL;
		return s_mapBitmapCache[bitmapName];
	}
	else
		return itr->second;
}


// �������
void BitmapCache::Clear(){
	for (BitmapCache::Map::iterator itr = s_mapBitmapCache.begin(); itr != s_mapBitmapCache.end(); ++itr){
		SafeRelease(itr->second);
	}
	s_mapBitmapCache.clear();
}


// �ؽ�����
void BitmapCache::Recreate(){
#ifdef _DEBUG
	_cwprintf(L"�����ؽ���\n");
#endif
	for (BitmapCache::Map::iterator itr = s_mapBitmapCache.begin(); itr != s_mapBitmapCache.end(); ++itr){
		SafeRelease(itr->second);
		itr->second = GetBitmap(itr->first);
	}
	//SceneManager::ReassignBitmap();
}