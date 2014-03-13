#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途: 对游戏图元的高度抽象
class Sprite{
public:
	// 对象映射
	typedef	std::map<VALUE, Sprite*, std::less<VALUE>, GameAllocator<std::pair<const VALUE, Sprite*>>> Map;
	// 对象类型
	typedef Map::value_type vtype;
	// 校验码
	static const VALUE			CHECK_CODE = 0x12345678;
public:
	// Ruby对象接口
	enum EnumRubyObjectDataIndex
	{
		ACheckCode = 0,			// 校验码
		BNeedRefreshFlag,			// Ruby对象刷新标识
		CSpriteX,					// X坐标
		DSpriteY,					// Y坐标
		DSpriteZ,					// Z坐标
		FDisposedFlag,				// 释放标识
		GLayerID,					// 层标识
		HZoomX,						// X轴方向放大率
		IZoomY,						// Y轴方向放大率
		JOpacity,					// 不透明度
		KVisible,					// 可视性
		LSpriteOX,					// OX坐标
		MSpriteOY,					// OY坐标
		NInterpolationMode,		// 插值方法
		RUBY_OBJECT_DATA_SIZE		// 本枚举量大小
	};
	// Ruby接口定义: 不能是内联
	// 新建对象
	static VALUE NewObject(int argc, VALUE* argv, VALUE self);
	// 释放对象
	static VALUE Dispose(VALUE self);
	// 链接Bitmap
	static VALUE LinkBitmap(VALUE self, VALUE rbbitmap);
	// z=
	static VALUE SetZ(VALUE self, VALUE new_z);
public:
	// 雪碧链表
	typedef	std::list<Sprite*, GameAllocator<Sprite*>>	List;
	// 层标识
	typedef SArray<List, 32>	Array;
public:
	// 构造函数
	Sprite(UINT layer);
	// 析构函数
	~Sprite();
	// 清除缓存
	static void Clear();
	// 删除函数
	Sprite() = delete; Sprite(Sprite&) = delete;
public:
	// 检查更新 Ruby -> C++
	void						check();
	// 获取Z坐标
	const int					z(){ return m_z; }
	// 获取位图
	BitmapCache::Bitmap*		d2dbitmap() { if (bitmap) return bitmap->GetD2DBitmap(); return NULL; }
	// 重载: 获取位图
	const BitmapCache::Bitmap*	d2dbitmap()const{ if (bitmap) return bitmap->GetD2DBitmap(); return NULL; }
	// 获取目标矩形
	const D2D1_RECT_F&	des_rect(){ return m_desRect; }
	// 设置位置
	void			SetPosition(){
		if (bitmap){
			m_desRect.left = x;
			m_desRect.right = x + bitmap->widthf();
			m_desRect.top = y;
			m_desRect.bottom = y + bitmap->heightf();
		}
#ifdef _DEBUG
		else
			assert(0&&"bitmap 创建失败");
#endif
	}
	// 新建位图 建议使用本函数创建新位图
	void				new_bitmap(const WCHAR* file_name){
		if (bitmap)
			delete bitmap;
		BitmapCache::Key name = file_name;
		bitmap = new Bitmap(name);
	}
	// 重载 新建位图 建议使用本函数创建新位图
	void				new_bitmap(UINT width, UINT height){
		if (bitmap)
			delete bitmap;
		bitmap = new Bitmap(width, height);
	}
public:
	// 位图
	::Bitmap*			bitmap = nullptr;
	// X坐标
	float				x = 0.F;
	// Y坐标
	float				y = 0.F;
	// 不透明度
	float				opacity = 1.F;
	// X方向放大率
	float				zoom_x = 1.F;
	// Y方向放大率
	float				zoom_y = 1.F;
	// 旋转角度
	float				rotation = 0.F;
	// 是否可视
	BOOL				visible = TRUE;
	// 插值类型
	D2D1_BITMAP_INTERPOLATION_MODE interpolation_mode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
private:
	// 目标矩形
	D2D1_RECT_F			m_desRect;
	// 原点X坐标
	float				m_ox = 0.F;
	// 原点Y坐标
	float				m_oy = 0.F;
	// Z坐标
	int					m_z = 0;
	// Ruby 对象数据
	RArray*				m_aryData = nullptr;
public:
	// 获取数组
	static const Array&	GetArray(){ return s_array; }
private:
	// 对Ruby对象
	static Map			s_mapRubytoCPlusPlus;
	// 加入链表
	static	void		push_in_list(Sprite* obj);
private:
	// 精灵列表
	//static List			s_list;
	// 精灵列表
	static Array		s_array;
	// 设置Z坐标
	void				set_z(int new_z){
		s_array[m_layer].remove(this);
		m_z = new_z;
		push_in_list(this);
	}
public:
	// new 操作
	void*	operator			new(size_t size){ return MPool.Alloc(size); }
		// delete 操作
	void	operator			delete(void* p)	{ MPool.Free(p); }
	// new[] 操作
	void*	operator			new[](size_t size) = delete;
	// delete[] 操作
	void	operator			delete[](void* p) = delete;
private:
	// 获取 层ID
	const UINT				layer_id()const{ return m_layer; };
	// 层ID 禁止修改
	const UINT				m_layer;
};