// 对XInput进行再次封装

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

// PS3按键位置
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
	// 是否触发按钮?
	// button		[in] :指定按钮(允许多个)
	static bool Trigger(WORD button){
		return !(s_curXInputState.Gamepad.wButtons & button) && (s_oldXInputState.Gamepad.wButtons & button);
	}
	// 是否按下按钮?
	// button		[in] :指定按钮(允许多个)
	static bool Press(WORD button){
		return(s_curXInputState.Gamepad.wButtons & button) > 0;
	}
	// 获取左摇杆X轴数值
	static short ThumbLX(){
		return s_curXInputState.Gamepad.sThumbLX;
	}
	// 获取左摇杆Y轴数值
	static short ThumbLY(){
		return s_curXInputState.Gamepad.sThumbLY;
	}
	// 获取右摇杆X轴数值
	static short ThumbRX(){
		return s_curXInputState.Gamepad.sThumbRX;
	}
	// 获取右摇杆Y轴数值
	static short ThumbRY(){
		return s_curXInputState.Gamepad.sThumbRY;
	}
	// 获取左扳机轴(L2)数值
	static short LeftTrigger(){
		return s_curXInputState.Gamepad.bLeftTrigger;
	}
	// 获取右扳机轴(R2)数值
	static short RightTrigger(){
		return s_curXInputState.Gamepad.bRightTrigger;
	}
	// 刷新状态
	static void Update(){
		// 更新控制器状态
		s_oldXInputState = s_curXInputState;
		if (ERROR_SUCCESS != XInputGetState(s_dwGamePadIndex, &s_curXInputState))
			Initialize();
		// 检查震动状态
		if (s_dwShockEndTime && s_dwShockEndTime < GetTickCount()){
			s_Vibration.wLeftMotorSpeed = s_Vibration.wRightMotorSpeed = 0;
			XInputSetState(s_dwGamePadIndex, &s_Vibration);
			s_dwShockEndTime = 0;
		}

	}
	// 设置震动状态
	// left			[in] :左马达出力
	// right		[in] :右马达出力
	// shock_time	[in] :震动时间 单位：毫秒 0为一直震动直到下次改变震动状态
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
	// 初始化控制器
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
	// 震动状态
	static XINPUT_VIBRATION		s_Vibration;
	// 当前手柄索引
	static DWORD				s_dwGamePadIndex;
	// 震动到点时间
	static DWORD				s_dwShockEndTime;
	// 上帧手柄状态
	static XINPUT_STATE			s_oldXInputState;
	// 当前手柄状态
	static XINPUT_STATE			s_curXInputState;
};

