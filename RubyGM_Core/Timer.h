// 包含3个文件
#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途: 包含3种计时器

// !!使用时请自行在可靠地方初始化静态成员变量!!
// 比如在stdafx.cpp里面
/*
// 低精度计时器
CrudeTimer					CrudeTimer::s_instance;
// 中精度计时器
DependableTimer				DependableTimer::s_instance;
// 高精度计时器
PrecisionTimer				PrecisionTimer::s_instance;
*/

#include "Timer/CrudeTimer.h"
#include "Timer/DependableTimer.h"
#include "Timer/PrecisionTimer.h"
