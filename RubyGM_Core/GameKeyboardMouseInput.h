// ��ʹ��DirectInput �Լ�����м��

#pragma once


// ��Ϸ���������� ���಻�������ⴴ�����ر��Ƕ�̬
class GameKeyboardMouseInput{
public:
	// ���̻�������С
	static const DWORD					KEYBOARD_BUFFER_SIZE = 256;
public:
	// ��ʼ��
	HRESULT	Init(const HINSTANCE hInst, const HWND hwnd);
public:
	// ˢ���豸
	inline void							Update(){
		// �������״̬ ����Ӧ���豸��ʧ���
		if (m_pDirectInputDeviceMouse){
			if (DIERR_INPUTLOST == m_pDirectInputDeviceMouse->GetDeviceState(
				sizeof(DIMOUSESTATE), m_cMouseBuffer + m_dwMouseBufferOffsetOld))
				m_pDirectInputDeviceMouse->Acquire();
			m_dwMouseBufferOffsetOld = m_dwMouseBufferOffsetOld ? 0 : 1;
			m_dwMouseBufferOffsetNew = m_dwMouseBufferOffsetNew ? 0 : 1;
			m_fPointX += m_mSensitivity * FLOAT(m_cMouseBuffer[m_dwMouseBufferOffsetNew].lX);
			m_fPointY += m_mSensitivity * FLOAT(m_cMouseBuffer[m_dwMouseBufferOffsetNew].lY);
		}
		// ���¼���״̬ ����Ӧ���豸��ʧ���
		if (m_pDirectInputDeviceKeyboard){
			if (DIERR_INPUTLOST == m_pDirectInputDeviceKeyboard->GetDeviceState(
				GameKeyboardMouseInput::KEYBOARD_BUFFER_SIZE, m_cKeyboardBuffer + m_dwKeyboardBufferOffsetOld))
				m_pDirectInputDeviceKeyboard->Acquire();
			m_dwKeyboardBufferOffsetOld = m_dwKeyboardBufferOffsetOld ? 0 : GameKeyboardMouseInput::KEYBOARD_BUFFER_SIZE;
			m_dwKeyboardBufferOffsetNew = m_dwKeyboardBufferOffsetNew ? 0 : GameKeyboardMouseInput::KEYBOARD_BUFFER_SIZE;
		}
	}
	// �Ƿ�ʹ�ü��̰��˼�
	inline BOOL							KPress(DWORD key){ return *(m_cKeyboardBuffer + m_dwKeyboardBufferOffsetNew + key); }
	// �Ƿ�ʹ�ü���Trigger�˼�
	inline BOOL							KTrigger(DWORD key){
		return (*(m_cKeyboardBuffer + m_dwKeyboardBufferOffsetOld + key) && !*(m_cKeyboardBuffer + m_dwKeyboardBufferOffsetNew + key));
	}
	// �Ƿ�ʹ�ü��̰��˼�
	inline BOOL							MPress(DWORD btn){ return ((m_cMouseBuffer + m_dwMouseBufferOffsetNew)->rgbButtons[btn]); }
	// �Ƿ�ʹ�ü���Trigger�˼�
	inline BOOL							MTrigger(DWORD btn){
		return !(m_cMouseBuffer + m_dwMouseBufferOffsetNew)->rgbButtons[btn] && ((m_cMouseBuffer + m_dwMouseBufferOffsetOld)->rgbButtons[btn]);
	}
private:
	// ���캯��
	GameKeyboardMouseInput();
	// ��������
	~GameKeyboardMouseInput();
	// DirectInput����ָ��
	IDirectInput8*						m_pDirectInputObject = nullptr;
	// DirectInput�豸ָ��(����)
	IDirectInputDevice8*				m_pDirectInputDeviceKeyboard = nullptr;
	// DirectInput�豸ָ��(���)
	IDirectInputDevice8*				m_pDirectInputDeviceMouse = nullptr;
	// ���ھ�� ����У�����λ��
	HWND								m_hwnd = nullptr;
	// ����״̬���� 2�� ���ڱȽ��Ƿ񴥷�(Trigger)ĳ��
	BYTE								m_cKeyboardBuffer[KEYBOARD_BUFFER_SIZE * 2];
	// ����״̬����ƫ��ֵ ָ���ֵ
	DWORD								m_dwKeyboardBufferOffsetOld = 0;
	// ����״̬����ƫ��ֵ ָ����ֵ
	DWORD								m_dwKeyboardBufferOffsetNew = GameKeyboardMouseInput::KEYBOARD_BUFFER_SIZE;
	// ���״̬���� 2�� ���ڱȽ��Ƿ񴥷�(Trigger)ĳ��
	DIMOUSESTATE						m_cMouseBuffer[2];
	// ���״̬����ƫ��ֵ ָ���ֵ
	DWORD								m_dwMouseBufferOffsetOld = 0;
	// ���״̬����ƫ��ֵ ָ����ֵ
	DWORD								m_dwMouseBufferOffsetNew = 1;
	// ��굱ǰX����
	FLOAT								m_fPointX = 0.F;
	// ��굱ǰY����
	FLOAT								m_fPointY = 0.F;
	// ���������
	FLOAT								m_mSensitivity = 1.F;
public:
	// GameKeyboardMouseInput һ��ʵ�� ��֧�ֶ��������Ӿ�̬����
	static GameKeyboardMouseInput		s_inputPlayer1;
};

#define KMInput GameKeyboardMouseInput::s_inputPlayer1
