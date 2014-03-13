// 自定义调试
#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途: 调试 函数/预处理

#ifndef assert
#include <cassert>
#endif

#define CHECK_POINTER_NULL

#ifdef _DEBUG
#define check_pointer(p, msg) assert(p&&msg&&"内存不足.Out of memory.")
#else
#if defined(_WINDOWS_) && defined(CHECK_POINTER_NULL)
#define check_pointer(p, msg) p||MessageBoxA(NULL, "内存不足.Out of memory.", msg##"Error", MB_OK)
#else
#define check_pointer(p, msg)
#endif
#endif