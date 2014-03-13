#pragma once

// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途:  对于音频播放器的抽象

class AudioPlayer{
public:
	// 初始化对象
	HRESULT				Init();
	// 摧毁对象
	void				Destroy();
private:
	// IXAudio2 接口
	IXAudio2*							m_pXAudio2Engine = nullptr;
private:
	// 私有化构造函数
	AudioPlayer();
	// 私有化析构函数
	~AudioPlayer(){ Destroy(); }
	// 删除复制构造函数
	AudioPlayer(AudioPlayer&) = delete;
	// 单实例
	static	AudioPlayer				s_instance;
};