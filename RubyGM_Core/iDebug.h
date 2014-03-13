// �Զ������
#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// ��;: ���� ����/Ԥ����

#ifndef assert
#include <cassert>
#endif

#define CHECK_POINTER_NULL

#ifdef _DEBUG
#define check_pointer(p, msg) assert(p&&msg&&"�ڴ治��.Out of memory.")
#else
#if defined(_WINDOWS_) && defined(CHECK_POINTER_NULL)
#define check_pointer(p, msg) p||MessageBoxA(NULL, "�ڴ治��.Out of memory.", msg##"Error", MB_OK)
#else
#define check_pointer(p, msg)
#endif
#endif