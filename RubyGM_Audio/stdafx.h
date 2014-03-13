#pragma once

// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途:  包含不再修改头文件,生成预编译文件

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
// Windows Header Files:

#include <Windows.h>


#include <wchar.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <list>
#include <map>



template<class Interface>
inline void SafeRelease(Interface *&pInterfaceToRelease){
	if (pInterfaceToRelease != NULL){
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

//
#include "../RubyGM_Core/mpool/EzMemNode.h"
#include "../RubyGM_Core/mpool/EzMemNodeEx.h"
#include "../RubyGM_Core/mpool/EzMemPool.h"
#include "../RubyGM_Core/mpool/EzMemPoolEx.h"
#include "../RubyGM_Core/GameAllocator.h"