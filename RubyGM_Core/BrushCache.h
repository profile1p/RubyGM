#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// ��;: ����Direct Write �� Brush �Ļ������



#ifdef USING_CPlusPlus_MODE
// Note:

// ����ֻʵ���˾��򽥱䡢���Խ����ˢ(��Ȼ����ģ�ⴿɫ��һ����Ϸ����)��λͼ��ˢ����������ʵ��
// D2D1_BRUSH_PROPERTIES �о���������ɫ���ƣ�����û����Ϊ���Լ��� ��Ϸ��ˢ�ṹ��


// ���ֹͣ�������������а� ������Ϊһ��3������(�Գ����Խ���)
#define BRUSH_MAX_GRADIENT_STOP		3

// map���ֵ ��ˢ����
struct GameBrush
{
	enum BrushType{
		Linear = 0,		// ���Խ���
		Radial,			// ���򽥱�
		//Bitmap,			// λͼ��ˢ
	};
	GameBrush(BrushType _type = BrushType::Linear, UINT count = 1){
		ZeroMemory(this, sizeof(GameBrush));
		type = _type;
		stop_count = count;
	}
	bool operator<(const GameBrush& obj)const{
		return memcmp((char*)this, (char*)&obj, sizeof(GameBrush)) < 0;
	}
	// ��ˢ����
	BrushType			type;
	// ����ֹͣ��
	D2D1_GRADIENT_STOP	gradient_stops[BRUSH_MAX_GRADIENT_STOP];
	// ����ֹͣ������ С��2�Ļ� �����ɴ�ɫ��ˢ
	UINT				stop_count;
	union {
		// ���Խ�������
		D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES	linear_properties;
		// ���򽥱�����
		D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES	radial_properties;
	};
};

// ����
class BrushCache{
public:
	// �������� �����ˢ
	typedef std::map<GameBrush, ID2D1Brush*> Map;
public:
	// �������
	static void					Clear();
	// �ؽ�����
	static void					Recreate();
	// ��ȡ��ˢ
	static ID2D1Brush*			GetBrush(const GameBrush& brush_info);
private:
	// ����ָ����ˢ
	static ID2D1Brush*			CreateBrush(const GameBrush& brush_info);
	// �����ˢ�Ļ���
	static Map					s_mapBrushCache;
public:
	// ͸��ɫ
	static const D2D1_COLOR_F	s_colorTransparent;
	// (��ɫ)��͸��
	static const D2D1_COLOR_F	s_colorTranslucent;
	// ��ɫ
	static const D2D1_COLOR_F	s_colorWhite;
	// ��ɫ
	static const D2D1_COLOR_F	s_colorBlack;
};
#else

// ����
class BrushCache{
public:
	// Ruby �ӿ�
	static VALUE	RB_CreateBrush(int argc, VALUE* argv, VALUE self);
public:
	// ��ˢ����
	enum class BrushType{
		Linear = 0,		// ���Խ���
		Radial,			// ���򽥱�
		Bitmap,			// λͼ��ˢ
	};
	// ���������ˢ�Ĳ���  ��ɫ�Ͱ�stop_count��Ϊ1
	struct GameBrushParameter{
		// ���ṹ���С(��λ�ֽ�, ��������)
		UINT32			this_length = 0;
		// ��ˢ����
		BrushType		brush_type = BrushType::Linear;
		// ��������
		union {
			// ���Խ�������
			D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES	linear_properties;
			// ���򽥱�����
			D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES	radial_properties;
		};
		// ֹͣ������
		UINT32					stop_count = 1;
		// ����ֹͣ��
		D2D1_GRADIENT_STOP		gradient_stops[1];
	};
	// �������� �����ˢ
	typedef SArray<ID2D1Brush*, 1024> Array;
public:
	// ��ʼ��
	static void					Init();
	// �������
	static void					Clear();
	// �ؽ�����
	static void					Recreate();
	// ���ñ�ˢ
	static BOOL					SetBrush(const UINT i, const GameBrushParameter* p){
		if (i < s_aryBrushCache.size()){
			SafeRelease(s_aryBrushCache[i]);
			ID2D1Brush*	pBrush(CreateBrush(p));
			s_aryBrushCache[i] = pBrush;
			// return pBrush;
			return pBrush ? TRUE : FALSE;
		}
#if defined(_DEBUG) || defined(_GAME_DEBUG)
		MessageBoxW(nullptr, L"<BrushCache::SetBrush>: Out of range\n���ñ�ˢ��ų�����Χ."
			, L"Error", MB_OK);
#endif
		return FALSE;
	}
	// ��ȡ��ˢ
	static ID2D1Brush*			GetBrush(const UINT index){ return index < s_aryBrushCache.size() ? s_aryBrushCache[index] : nullptr; }
private:
	// ����ָ����ˢ
	static ID2D1Brush*			CreateBrush(const GameBrushParameter*);
	// �����ˢ�Ļ���
	static Array				s_aryBrushCache;
public:
	// ͸��ɫ
	static const D2D1_COLOR_F	s_colorTransparent;
	// (��ɫ)��͸��
	static const D2D1_COLOR_F	s_colorTranslucent;
	// ��ɫ
	static const D2D1_COLOR_F	s_colorWhite;
	// ��ɫ
	static const D2D1_COLOR_F	s_colorBlack;
};
#endif