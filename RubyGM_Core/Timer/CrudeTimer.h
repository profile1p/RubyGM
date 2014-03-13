
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途: 
// CrudeTimer类 低精度计时器 用于长时间计时 精度: 16ms
//		比如程序当前运行时间
// !!使用时请自行包含本文件并在可靠地方初始化静态成员变量!!

#pragma once

#define LTimer CrudeTimer::Instance()

// 低精度计时器
class CrudeTimer
{
public:
	// 获取实例
	static CrudeTimer*			Instance(){ return &s_instance; }
	// 获取当前经过时间
	DWORD						NowTime(){ return GetTickCount() - m_dwStartTime - m_dwPausedTime; }
	// 暂停
	void						Pause(){ if (m_dwPauseTime == 0) m_dwPauseTime = GetTickCount(); }
	// 恢复
	void						Recover(){
		if (m_dwPauseTime){
			m_dwPausedTime += GetTickCount() - m_dwPauseTime;
			m_dwPauseTime = 0;
		}
	}
	// 重置
	void						Reset(){m_dwStartTime = GetTickCount(); m_dwPausedTime = m_dwPauseTime = 0;	}
private:
	// 开始计时时间
	DWORD						m_dwStartTime;
	// 暂停开始时间
	DWORD						m_dwPauseTime;
	// 累计暂停时间
	DWORD						m_dwPausedTime;
private:
	// 强制唯一实例
	CrudeTimer() :m_dwStartTime(GetTickCount()), m_dwPauseTime(0), m_dwPausedTime(0)
	{
	}
	// 唯一实例
	static	CrudeTimer			s_instance;
};