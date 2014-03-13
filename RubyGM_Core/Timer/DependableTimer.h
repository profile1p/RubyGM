
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途:
// DependableTimer类 中精度计时器 用于一般要求 精度:ms级别


#pragma once
#ifndef _INC_MMSYSTEM
#include <mmsystem.h>
#pragma comment ( lib, "winmm.lib" )
#endif

#define MTimer DependableTimer::Instance()

class DependableTimer{
public:
	// 返回实例
	static DependableTimer*	Instance(){ return &s_instance; }
	// 开始计时
	void					Start(){ m_dwStartTime = timeGetTime(); }
	// 获取时间间隔
	DWORD					Delta(){ return timeGetTime() - m_dwStartTime; }
private:
	// 开始计时时间
	DWORD					m_dwStartTime;
	// 强制单实例
	DependableTimer() :m_dwStartTime(0){ Start(); }
	// 唯一实例
	static	DependableTimer	s_instance;
};
