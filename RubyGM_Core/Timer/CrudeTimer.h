
// Author  : DustPG
// License : MIT: see more in "License.txt"

// ��;: 
// CrudeTimer�� �;��ȼ�ʱ�� ���ڳ�ʱ���ʱ ����: 16ms
//		�������ǰ����ʱ��
// !!ʹ��ʱ�����а������ļ����ڿɿ��ط���ʼ����̬��Ա����!!

#pragma once

#define LTimer CrudeTimer::Instance()

// �;��ȼ�ʱ��
class CrudeTimer
{
public:
	// ��ȡʵ��
	static CrudeTimer*			Instance(){ return &s_instance; }
	// ��ȡ��ǰ����ʱ��
	DWORD						NowTime(){ return GetTickCount() - m_dwStartTime - m_dwPausedTime; }
	// ��ͣ
	void						Pause(){ if (m_dwPauseTime == 0) m_dwPauseTime = GetTickCount(); }
	// �ָ�
	void						Recover(){
		if (m_dwPauseTime){
			m_dwPausedTime += GetTickCount() - m_dwPauseTime;
			m_dwPauseTime = 0;
		}
	}
	// ����
	void						Reset(){m_dwStartTime = GetTickCount(); m_dwPausedTime = m_dwPauseTime = 0;	}
private:
	// ��ʼ��ʱʱ��
	DWORD						m_dwStartTime;
	// ��ͣ��ʼʱ��
	DWORD						m_dwPauseTime;
	// �ۼ���ͣʱ��
	DWORD						m_dwPausedTime;
private:
	// ǿ��Ψһʵ��
	CrudeTimer() :m_dwStartTime(GetTickCount()), m_dwPauseTime(0), m_dwPausedTime(0)
	{
	}
	// Ψһʵ��
	static	CrudeTimer			s_instance;
};