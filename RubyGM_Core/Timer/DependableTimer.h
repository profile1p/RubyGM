
// Author  : DustPG
// License : MIT: see more in "License.txt"

// ��;:
// DependableTimer�� �о��ȼ�ʱ�� ����һ��Ҫ�� ����:ms����


#pragma once
#ifndef _INC_MMSYSTEM
#include <mmsystem.h>
#pragma comment ( lib, "winmm.lib" )
#endif

#define MTimer DependableTimer::Instance()

class DependableTimer{
public:
	// ����ʵ��
	static DependableTimer*	Instance(){ return &s_instance; }
	// ��ʼ��ʱ
	void					Start(){ m_dwStartTime = timeGetTime(); }
	// ��ȡʱ����
	DWORD					Delta(){ return timeGetTime() - m_dwStartTime; }
private:
	// ��ʼ��ʱʱ��
	DWORD					m_dwStartTime;
	// ǿ�Ƶ�ʵ��
	DependableTimer() :m_dwStartTime(0){ Start(); }
	// Ψһʵ��
	static	DependableTimer	s_instance;
};
