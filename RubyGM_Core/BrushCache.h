#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途: 对于Direct Write 的 Brush 的缓存管理



#ifdef USING_CPlusPlus_MODE
// Note:

// 这里只实现了径向渐变、线性渐变笔刷(当然可以模拟纯色，一般游戏够了)，位图笔刷可自行扩充实现
// D2D1_BRUSH_PROPERTIES 感觉可以用颜色控制，这里没有作为属性加入 游戏笔刷结构体


// 最大停止点数量量力而行吧 个人认为一般3个足矣(对称线性渐变)
#define BRUSH_MAX_GRADIENT_STOP		3

// map左键值 笔刷类型
struct GameBrush
{
	enum BrushType{
		Linear = 0,		// 线性渐变
		Radial,			// 径向渐变
		//Bitmap,			// 位图笔刷
	};
	GameBrush(BrushType _type = BrushType::Linear, UINT count = 1){
		ZeroMemory(this, sizeof(GameBrush));
		type = _type;
		stop_count = count;
	}
	bool operator<(const GameBrush& obj)const{
		return memcmp((char*)this, (char*)&obj, sizeof(GameBrush)) < 0;
	}
	// 笔刷类型
	BrushType			type;
	// 渐变停止点
	D2D1_GRADIENT_STOP	gradient_stops[BRUSH_MAX_GRADIENT_STOP];
	// 渐变停止点数量 小于2的话 会生成纯色笔刷
	UINT				stop_count;
	union {
		// 线性渐变属性
		D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES	linear_properties;
		// 径向渐变属性
		D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES	radial_properties;
	};
};

// 缓存
class BrushCache{
public:
	// 关联容器 缓存笔刷
	typedef std::map<GameBrush, ID2D1Brush*> Map;
public:
	// 清除缓存
	static void					Clear();
	// 重建缓存
	static void					Recreate();
	// 获取笔刷
	static ID2D1Brush*			GetBrush(const GameBrush& brush_info);
private:
	// 创建指定笔刷
	static ID2D1Brush*			CreateBrush(const GameBrush& brush_info);
	// 储存笔刷的缓存
	static Map					s_mapBrushCache;
public:
	// 透明色
	static const D2D1_COLOR_F	s_colorTransparent;
	// (白色)半透明
	static const D2D1_COLOR_F	s_colorTranslucent;
	// 白色
	static const D2D1_COLOR_F	s_colorWhite;
	// 黑色
	static const D2D1_COLOR_F	s_colorBlack;
};
#else

// 缓存
class BrushCache{
public:
	// Ruby 接口
	static VALUE	RB_CreateBrush(int argc, VALUE* argv, VALUE self);
public:
	// 笔刷类型
	enum class BrushType{
		Linear = 0,		// 线性渐变
		Radial,			// 径向渐变
		Bitmap,			// 位图笔刷
	};
	// 创建渐变笔刷的参数  纯色就把stop_count设为1
	struct GameBrushParameter{
		// 本结构体大小(单位字节, 允许柔性)
		UINT32			this_length = 0;
		// 笔刷类型
		BrushType		brush_type = BrushType::Linear;
		// 渐变属性
		union {
			// 线性渐变属性
			D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES	linear_properties;
			// 径向渐变属性
			D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES	radial_properties;
		};
		// 停止点数量
		UINT32					stop_count = 1;
		// 渐变停止点
		D2D1_GRADIENT_STOP		gradient_stops[1];
	};
	// 关联容器 缓存笔刷
	typedef SArray<ID2D1Brush*, 1024> Array;
public:
	// 初始化
	static void					Init();
	// 清除缓存
	static void					Clear();
	// 重建缓存
	static void					Recreate();
	// 设置笔刷
	static BOOL					SetBrush(const UINT i, const GameBrushParameter* p){
		if (i < s_aryBrushCache.size()){
			SafeRelease(s_aryBrushCache[i]);
			ID2D1Brush*	pBrush(CreateBrush(p));
			s_aryBrushCache[i] = pBrush;
			// return pBrush;
			return pBrush ? TRUE : FALSE;
		}
#if defined(_DEBUG) || defined(_GAME_DEBUG)
		MessageBoxW(nullptr, L"<BrushCache::SetBrush>: Out of range\n设置笔刷编号超过范围."
			, L"Error", MB_OK);
#endif
		return FALSE;
	}
	// 获取笔刷
	static ID2D1Brush*			GetBrush(const UINT index){ return index < s_aryBrushCache.size() ? s_aryBrushCache[index] : nullptr; }
private:
	// 创建指定笔刷
	static ID2D1Brush*			CreateBrush(const GameBrushParameter*);
	// 储存笔刷的缓存
	static Array				s_aryBrushCache;
public:
	// 透明色
	static const D2D1_COLOR_F	s_colorTransparent;
	// (白色)半透明
	static const D2D1_COLOR_F	s_colorTranslucent;
	// 白色
	static const D2D1_COLOR_F	s_colorWhite;
	// 黑色
	static const D2D1_COLOR_F	s_colorBlack;
};
#endif