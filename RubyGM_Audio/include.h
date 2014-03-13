#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途:  包含非公共库, 用于其他项目


#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>


#include <Xaudio2.h>

#pragma comment (lib, "lib/libogg_static")
#pragma comment (lib, "lib/libvorbis_static")
#pragma comment (lib, "lib/libvorbisfile_static")


template<class Interface>
inline void SafeDestroyVoice(Interface *&pInterfaceToDestroy){
	if (pInterfaceToDestroy != NULL){
		pInterfaceToDestroy->DestroyVoice();
		pInterfaceToDestroy = NULL;
	}
}

#include "AudioPlayer.h"