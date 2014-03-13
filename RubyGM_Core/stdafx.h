#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途:  包含不再修改头文件,生成预编译文件

// 游戏使用纯C++模式(对C++用法习惯优化)
#define USING_CPlusPlus_MODE
// 游戏使用标准C++ 不使用空数组
#define USING_STANDARD_C_PLUS_PLUS

#ifndef WINVER              // Allow use of features specific to Windows 7 or later.
#define WINVER 0x0700       
#endif						// Change this to the appropriate value to target other versions of Windows.

#ifndef _WIN32_WINNT        // Allow use of features specific to Windows 7 or later.
#define _WIN32_WINNT 0x0700
#endif						// Change this to the appropriate value to target other versions of Windows.

#ifndef DIRECTINPUT_VERSION // Dinput 版本
#define DIRECTINPUT_VERSION 0x0800
#endif

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#ifdef _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#include <Shlwapi.h>
#include <mmsystem.h> 
#include <mmreg.h>
#include <dwmapi.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>
// C++ 
// 使用 _fclose_nolock 来替代fclose
#define USING_fclose_nolock

//#include <allocators>
#include <iostream>
#include <sstream>
#include <cassert>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <array>
#include <stack>
#include <list>
#include <map>


#include <Xinput.h>
#include <dinput.h>

// Effect
/*#ifndef INITGUID
#undef DEFINE_GUID
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	EXTERN_C const GUID DECLSPEC_SELECTANY name \
	= { l, w1, w2, { b1, b2, b3, b4, b5, b6, b7, b8 } }
#include <d2d1effects_1.h>
#undef DEFINE_GUID
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	EXTERN_C const GUID FAR name
#else*/
#include <d2d1effects_1.h>
//#endif // INITGUID

// Ruby
#include "ruby.h"
// 使用dll加载
#pragma comment ( lib, "lib/msvcr120-ruby210.lib" )

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#define _USE_MATH_DEFINES

// DX
#include <D3D11.h>
#include <d3d11_1.h>
#include <DXGI1_2.h>
#include <dwrite_1.h>
#include <wincodec.h>
#include <dshow.h>

#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <d2d1effectauthor.h>
#include <d2d1effecthelpers.h>



template<class Interface>
inline void SafeRelease(Interface *&pInterfaceToRelease){
	if (pInterfaceToRelease != nullptr){
		pInterfaceToRelease->Release();
		pInterfaceToRelease = nullptr;
	}
}
template <typename Interface>
inline Interface* SafeAcquire(Interface* newObject)
{
	if (newObject != nullptr)
		newObject->AddRef();

	return newObject;
}

#pragma comment ( lib, "dxguid.lib")
#pragma comment ( lib, "D3D11.lib" )
#pragma comment ( lib, "d2d1.lib" )
#pragma comment ( lib, "windowscodecs.lib" )
#pragma comment ( lib, "dwmapi.lib" )
#pragma comment ( lib, "Msdmo.lib" )
#pragma comment ( lib, "dmoguids.lib" )
#pragma comment ( lib, "amstrmid.lib" )
#pragma comment ( lib, "avrt.lib" )
#pragma comment ( lib, "dwrite.lib" )
#pragma comment ( lib, "XINPUT9_1_0.lib" )
#pragma comment ( lib, "dinput8.lib" )
#pragma comment ( lib, "Shlwapi.lib" )
#pragma comment ( lib, "winmm.lib" )




#include "iDebug.h"
#include "D2DVideoRenderer.h"
#include "Bool32.h"
#include "mpool/EzMemNode.h"
#include "mpool/EzMemPool.h"
#include "mpool/EzMemNodeEx.h"
#include "mpool/EzMemPoolEx.h"


#include "res.h"
#include "AppInfo.h"
#include "Timer.h"
#include "GamePadInput.h"
#include "GameKeyboardMouseInput.h"
#include "GameAllocator.h"
#include "LocalFontFileEnumerator.h"
#include "LocalFontCollectionLoader.h"