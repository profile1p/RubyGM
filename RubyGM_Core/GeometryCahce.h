#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途: 对于Direct2D 集合对象的缓存管理(不一定是缓存了)

class GeometryCahce{
public:
	// 新建Ruby 几何对象
	static VALUE NewGeometry(int argc, VALUE* argv, VALUE self);
	// 释放对象
	static VALUE Dispose(VALUE self);
	// 开始绘制
	static VALUE DeginPaint(VALUE self);
	// 结束绘制
	static VALUE EndPaint(VALUE self);
public:
	// 移动绘制点

public:
	// D2D 工厂
	typedef ::ID2D1Factory1 D2DFactory;
	// 路径几何
	typedef	::ID2D1PathGeometry1 PathGeometry;
	// Geometry Sink
	typedef ::ID2D1GeometrySink GeometrySink;
	// 文本集合对象参数
	struct FontGeometryParameter
	{
		// 字形
		IDWriteFontFace*			pFontFace;
		// 字体逻辑大小
		FLOAT						emSize;
		// 文本名称
		WCHAR*						pString;
		// 文本长度
		UINT						uStringLength;
		// 如果为 TRUE，则字形的升部沿基线运行。如果为 FALSE，则字形的升部垂直于基线运行
		BOOL						isSideways;
		// 字形的视觉顺序。如果此参数为 FALSE，则字形从左向右步进。如果为 TRUE，则步进方向为从右向左
		BOOL						isRightToLeft;
		// 输出
		PathGeometry*				pPathGeometry;
	};
public:
	// 创建文本几何
	static HRESULT CreateFontGeometry(FontGeometryParameter* pParameter);
public:
	// D2D 工厂
	static D2DFactory**				s_ppD2DFactory;
private:
	// 储存对象

};