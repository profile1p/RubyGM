#pragma once

// Author  : DustPG
// License : MIT: see more in "License.txt"

// ��;:  ������Ƶ�������ĳ���

class AudioPlayer{
public:
	// ��ʼ������
	HRESULT				Init();
	// �ݻٶ���
	void				Destroy();
private:
	// IXAudio2 �ӿ�
	IXAudio2*							m_pXAudio2Engine = nullptr;
private:
	// ˽�л����캯��
	AudioPlayer();
	// ˽�л���������
	~AudioPlayer(){ Destroy(); }
	// ɾ�����ƹ��캯��
	AudioPlayer(AudioPlayer&) = delete;
	// ��ʵ��
	static	AudioPlayer				s_instance;
};