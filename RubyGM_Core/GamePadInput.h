// ��XInput�����ٴη�װ

#pragma once
/*
#define XINPUT_GAMEPAD_DPAD_UP          0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN        0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT        0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
#define XINPUT_GAMEPAD_START            0x0010
#define XINPUT_GAMEPAD_BACK             0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB       0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB      0x0080
#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
#define XINPUT_GAMEPAD_A                0x1000
#define XINPUT_GAMEPAD_B                0x2000
#define XINPUT_GAMEPAD_X                0x4000
#define XINPUT_GAMEPAD_Y                0x8000
*/

// PS3����λ��
#define XINPUT_PS3_DPAD_UP          0x0001
#define XINPUT_PS3_DPAD_DOWN        0x0002
#define XINPUT_PS3_DPAD_LEFT        0x0004
#define XINPUT_PS3_DPAD_RIGHT       0x0008
#define XINPUT_PS3_START            0x0010
#define XINPUT_PS3_SELECT           0x0020
#define XINPUT_PS3_L3               0x0040
#define XINPUT_PS3_R3               0x0080
#define XINPUT_PS3_L1               0x0100
#define XINPUT_PS3_R1               0x0200
#define XINPUT_PS3_Cross            0x1000
#define XINPUT_PS3_Circle           0x2000
#define XINPUT_PS3_Square           0x4000
#define XINPUT_PS3_Triangle         0x8000

class GamePadInput{
public:
	// �Ƿ񴥷���ť?
	// button		[in] :ָ����ť(������)
	static bool Trigger(WORD button){
		return !(s_curXInputState.Gamepad.wButtons & button) && (s_oldXInputState.Gamepad.wButtons & button);
	}
	// �Ƿ��°�ť?
	// button		[in] :ָ����ť(������)
	static bool Press(WORD button){
		return(s_curXInputState.Gamepad.wButtons & button) > 0;
	}
	// ��ȡ��ҡ��X����ֵ
	static short ThumbLX(){
		return s_curXInputState.Gamepad.sThumbLX;
	}
	// ��ȡ��ҡ��Y����ֵ
	static short ThumbLY(){
		return s_curXInputState.Gamepad.sThumbLY;
	}
	// ��ȡ��ҡ��X����ֵ
	static short ThumbRX(){
		return s_curXInputState.Gamepad.sThumbRX;
	}
	// ��ȡ��ҡ��Y����ֵ
	static short ThumbRY(){
		return s_curXInputState.Gamepad.sThumbRY;
	}
	// ��ȡ������(L2)��ֵ
	static short LeftTrigger(){
		return s_curXInputState.Gamepad.bLeftTrigger;
	}
	// ��ȡ�Ұ����(R2)��ֵ
	static short RightTrigger(){
		return s_curXInputState.Gamepad.bRightTrigger;
	}
	// ˢ��״̬
	static void Update(){
		// ���¿�����״̬
		s_oldXInputState = s_curXInputState;
		if (ERROR_SUCCESS != XInputGetState(s_dwGamePadIndex, &s_curXInputState))
			Initialize();
		// �����״̬
		if (s_dwShockEndTime && s_dwShockEndTime < GetTickCount()){
			s_Vibration.wLeftMotorSpeed = s_Vibration.wRightMotorSpeed = 0;
			XInputSetState(s_dwGamePadIndex, &s_Vibration);
			s_dwShockEndTime = 0;
		}

	}
	// ������״̬
	// left			[in] :��������
	// right		[in] :��������
	// shock_time	[in] :��ʱ�� ��λ������ 0Ϊһֱ��ֱ���´θı���״̬
	static void SetVibration(WORD left, WORD right, DWORD shock_time = 0){
#ifdef _DEBUG
		if (shock_time)
			_cwprintf(L"SetVibration:left:%d, right:%d, shock_time:%d\n", left, right, shock_time);
		else
			_cwprintf(L"SetVibration:left:%d, right:%d, shock_time:INFINITY\n", left, right);
#endif
		s_Vibration.wLeftMotorSpeed = left;
		s_Vibration.wRightMotorSpeed = right;
		if (shock_time)
			s_dwShockEndTime = GetTickCount() + shock_time;
		else
			s_dwShockEndTime = 0;
		XInputSetState(s_dwGamePadIndex, &s_Vibration);
	}
	// ��ʼ��������
	static void Initialize(){
		XINPUT_STATE state;
		for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
		{
			ZeroMemory(&state, sizeof(XINPUT_STATE));
			if (ERROR_SUCCESS == XInputGetState(i, &state)){
				s_dwGamePadIndex = i;
				break;
			}
		}
	}
private:
	// ��״̬
	static XINPUT_VIBRATION		s_Vibration;
	// ��ǰ�ֱ�����
	static DWORD				s_dwGamePadIndex;
	// �𶯵���ʱ��
	static DWORD				s_dwShockEndTime;
	// ��֡�ֱ�״̬
	static XINPUT_STATE			s_oldXInputState;
	// ��ǰ�ֱ�״̬
	static XINPUT_STATE			s_curXInputState;
};

