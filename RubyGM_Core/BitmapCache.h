#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途: 对于Direct2D Bitmap的缓存管理

class BitmapCache{
public:
	// 位图形式
	typedef ID2D1Bitmap1 Bitmap;
	// 渲染呈现器
	typedef ID2D1DeviceContext RenderTarget;
	// WIC 工厂
	typedef IWICImagingFactory2 WICFactory;
	// 关联容器 键
	typedef GameString	Key;
	// 关联容器 容器
	typedef	std::map<Key, Bitmap*, std::less<Key>, GameAllocator<std::pair<const Key, Bitmap*>>> Map;
public:
	// 清除缓存
	static void			Clear();
	// 重建缓存
	static void			Recreate();
	// 获取图片
	// bitmapName	[in] : 文件名
	// 返回: 其余的则为位图的指针 失败会再次重试 直到ok
	static Bitmap* GetBitmap(const WCHAR* bitmapName){
		Key name = bitmapName;
		Bitmap* bitmap = NULL;
		while (!(bitmap = GetBitmap(name))){
			Sleep(20);
		}
		return bitmap;
	}
	// 获取图片
	// bitmapName	[in] : 文件名
	// 返回: NULL表示失败 其余的则为位图的指针
	static Bitmap* GetBitmap(const Key& bitmapName);
private:
	// 图像缓存
	static Map				s_mapBitmapCache;
public:
	// RT
	static RenderTarget**	s_ppRenderTarget;
	// WIC 工厂
	static WICFactory**		s_ppWICFactory;
};