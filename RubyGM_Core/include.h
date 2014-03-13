#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途: 包含待修改头文件，减少在cpp文件中#include次数，防止头文件之间乱包含
#undef USING_CPlusPlus_MODE
#include "Container.h"
#include "BitmapCache.h"
#include "GeometryCahce.h"
#include "TextFormatCache.h"
#include "BrushCache.h"
#include "Bitmap.h"
#include "Sprite.h"
#include "Graphics.h"
#include "Game.h"

#include "ImageRender.h"
#include "ThisApp.h"

#define RB_INT_FLOAT_TO_FLOAT(v) FIXNUM_P(v)?(FLOAT)FIX2INT(v):(FLOAT)RFLOAT(v)->float_value
//#pragma comment ( lib, "lib/msvcr120-ruby210-static.lib" )


/*
To Do List:
基本图像显示 - basic graphics interface
	done: Mar.4th 2014

基本图像显示 Ruby接口  basic graphics interface (ruby part
	done: Mar.5th 2014

完整图像显示 - advanced graphics interface
	- done : text (font face) path geometry - Mar.9th 2014

输入接口 - input interface
	- done : basic - Mar.9th 2014

完整图像显示 Ruby接口 advanced graphics interface (ruby part
	- draw text - Mar.11th 2014

视频接口 - video interface
	- done : direct2d video renderer filter - run time load - Mar.11th 2014

输入接口 Ruby接口 - input interface (ruby part

音频接口 - audio interface

音频接口 Ruby接口 audio interface (ruby part

视频接口 Ruby接口 video interface (ruby part

代码优化 - code optimization

图形图像高级方法 - graph process interface

图形图像高级方法 Ruby接口 graph process interface (ruby part

其他 - other

*/