#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// ��;: ����Direct2D Bitmap�Ļ������

class BitmapCache{
public:
	// λͼ��ʽ
	typedef ID2D1Bitmap1 Bitmap;
	// ��Ⱦ������
	typedef ID2D1DeviceContext RenderTarget;
	// WIC ����
	typedef IWICImagingFactory2 WICFactory;
	// �������� ��
	typedef GameString	Key;
	// �������� ����
	typedef	std::map<Key, Bitmap*, std::less<Key>, GameAllocator<std::pair<const Key, Bitmap*>>> Map;
public:
	// �������
	static void			Clear();
	// �ؽ�����
	static void			Recreate();
	// ��ȡͼƬ
	// bitmapName	[in] : �ļ���
	// ����: �������Ϊλͼ��ָ�� ʧ�ܻ��ٴ����� ֱ��ok
	static Bitmap* GetBitmap(const WCHAR* bitmapName){
		Key name = bitmapName;
		Bitmap* bitmap = NULL;
		while (!(bitmap = GetBitmap(name))){
			Sleep(20);
		}
		return bitmap;
	}
	// ��ȡͼƬ
	// bitmapName	[in] : �ļ���
	// ����: NULL��ʾʧ�� �������Ϊλͼ��ָ��
	static Bitmap* GetBitmap(const Key& bitmapName);
private:
	// ͼ�񻺴�
	static Map				s_mapBitmapCache;
public:
	// RT
	static RenderTarget**	s_ppRenderTarget;
	// WIC ����
	static WICFactory**		s_ppWICFactory;
};