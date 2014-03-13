#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// ��;: ��֡�ĳ��� ���������Ⱦ


// ���ɷ�ʽ
enum TransitionMode{
	AlphaFadeOut = 0, // Alpha ���뽥�� ͨ���޸�ǰ��͸���ȴﵽ����Ŀ��
};



class Graphics{
public:
	// ͼ��ˢ�� ���ȴ�ˢ�½���
	static VALUE		Update(VALUE self);
	// ��ʼ����ģ��
	static HRESULT		Init(const HWND);
	// �رձ�ģ��
	static void			Close();
	// ����ͼ�� �Ǽ�ͼ����ɵĳ�ʼ����
	static void			Freeze();
	// ����ͼ�� �Ǽ�ͼ����ɵ���β����
	static void			Transition(UINT time = 30, TransitionMode mode = AlphaFadeOut, UINT parameter = 0);
	// ��ȡ��������֡��
	static const UINT	GetFrameCount(){ return s_numFrameCouter; }
	// ��ȡ��������֡��
	static const UINT	GetFrameRate(){ return s_DwmTimingInfo.rateCompose.uiNumerator; }
	// �Ƿ�������Ⱦ
	// time		[in]	:����ȴ�ʱ�� ����Ĭ��ֵ:10ms
	static bool			ReadyToRender(DWORD time = 10){return WAIT_OBJECT_0 == WaitForSingleObject(s_hReadyToRender, time); }
	// ������Ⱦ
	static void			RenderFinished(){
		// ��ʾ�������
		SetEvent(s_hRenderFinished);
		// ��������
		ResetEvent(s_hReadyToRender);
	}
private:
	// ��ʾ������¾��
	static HANDLE			s_hReadyToRender;
	// ��ʾ���������Ͼ��
	static HANDLE			s_hRenderFinished;
	// ��Ϸ����֡��
	static UINT				s_numFrameCouter;
	// ʱ����Ϣ
	static DWM_TIMING_INFO	s_DwmTimingInfo;
};

