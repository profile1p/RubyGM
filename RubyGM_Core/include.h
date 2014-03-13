#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// ��;: �������޸�ͷ�ļ���������cpp�ļ���#include��������ֹͷ�ļ�֮���Ұ���
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
����ͼ����ʾ - basic graphics interface
	done: Mar.4th 2014

����ͼ����ʾ Ruby�ӿ�  basic graphics interface (ruby part
	done: Mar.5th 2014

����ͼ����ʾ - advanced graphics interface
	- done : text (font face) path geometry - Mar.9th 2014

����ӿ� - input interface
	- done : basic - Mar.9th 2014

����ͼ����ʾ Ruby�ӿ� advanced graphics interface (ruby part
	- draw text - Mar.11th 2014

��Ƶ�ӿ� - video interface
	- done : direct2d video renderer filter - run time load - Mar.11th 2014

����ӿ� Ruby�ӿ� - input interface (ruby part

��Ƶ�ӿ� - audio interface

��Ƶ�ӿ� Ruby�ӿ� audio interface (ruby part

��Ƶ�ӿ� Ruby�ӿ� video interface (ruby part

�����Ż� - code optimization

ͼ��ͼ��߼����� - graph process interface

ͼ��ͼ��߼����� Ruby�ӿ� graph process interface (ruby part

���� - other

*/