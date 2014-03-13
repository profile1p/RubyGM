// 对使用DirectInput 对键鼠进行检测

#pragma once


// 游戏键鼠输入类 本类不允许随意创建，特别是动态
class GameKeyboardMouseInput{
public:
	// 键盘缓冲区大小
	static const DWORD					KEYBOARD_BUFFER_SIZE = 256;
public:
	// 初始化
	HRESULT	Init(const HINSTANCE hInst, const HWND hwnd);
public:
	// 刷新设备
	inline void							Update(){
		// 更新鼠标状态 还有应对设备丢失情况
		if (m_pDirectInputDeviceMouse){
			if (DIERR_INPUTLOST == m_pDirectInputDeviceMouse->GetDeviceState(
				sizeof(DIMOUSESTATE), m_cMouseBuffer + m_dwMouseBufferOffsetOld))
				m_pDirectInputDeviceMouse->Acquire();
			m_dwMouseBufferOffsetOld = m_dwMouseBufferOffsetOld ? 0 : 1;
			m_dwMouseBufferOffsetNew = m_dwMouseBufferOffsetNew ? 0 : 1;
			m_fPointX += m_mSensitivity * FLOAT(m_cMouseBuffer[m_dwMouseBufferOffsetNew].lX);
			m_fPointY += m_mSensitivity * FLOAT(m_cMouseBuffer[m_dwMouseBufferOffsetNew].lY);
		}
		// 更新键盘状态 还有应对设备丢失情况
		if (m_pDirectInputDeviceKeyboard){
			if (DIERR_INPUTLOST == m_pDirectInputDeviceKeyboard->GetDeviceState(
				GameKeyboardMouseInput::KEYBOARD_BUFFER_SIZE, m_cKeyboardBuffer + m_dwKeyboardBufferOffsetOld))
				m_pDirectInputDeviceKeyboard->Acquire();
			m_dwKeyboardBufferOffsetOld = m_dwKeyboardBufferOffsetOld ? 0 : GameKeyboardMouseInput::KEYBOARD_BUFFER_SIZE;
			m_dwKeyboardBufferOffsetNew = m_dwKeyboardBufferOffsetNew ? 0 : GameKeyboardMouseInput::KEYBOARD_BUFFER_SIZE;
		}
	}
	// 是否使用键盘按了键
	inline BOOL							KPress(DWORD key){ return *(m_cKeyboardBuffer + m_dwKeyboardBufferOffsetNew + key); }
	// 是否使用键盘Trigger了键
	inline BOOL							KTrigger(DWORD key){
		return (*(m_cKeyboardBuffer + m_dwKeyboardBufferOffsetOld + key) && !*(m_cKeyboardBuffer + m_dwKeyboardBufferOffsetNew + key));
	}
	// 是否使用键盘按了键
	inline BOOL							MPress(DWORD btn){ return ((m_cMouseBuffer + m_dwMouseBufferOffsetNew)->rgbButtons[btn]); }
	// 是否使用键盘Trigger了键
	inline BOOL							MTrigger(DWORD btn){
		return !(m_cMouseBuffer + m_dwMouseBufferOffsetNew)->rgbButtons[btn] && ((m_cMouseBuffer + m_dwMouseBufferOffsetOld)->rgbButtons[btn]);
	}
private:
	// 构造函数
	GameKeyboardMouseInput();
	// 析构函数
	~GameKeyboardMouseInput();
	// DirectInput对象指针
	IDirectInput8*						m_pDirectInputObject = nullptr;
	// DirectInput设备指针(键盘)
	IDirectInputDevice8*				m_pDirectInputDeviceKeyboard = nullptr;
	// DirectInput设备指针(鼠标)
	IDirectInputDevice8*				m_pDirectInputDeviceMouse = nullptr;
	// 窗口句柄 用于校正鼠标位置
	HWND								m_hwnd = nullptr;
	// 键盘状态缓冲 2倍 用于比较是否触发(Trigger)某键
	BYTE								m_cKeyboardBuffer[KEYBOARD_BUFFER_SIZE * 2];
	// 键盘状态缓冲偏移值 指向旧值
	DWORD								m_dwKeyboardBufferOffsetOld = 0;
	// 键盘状态缓冲偏移值 指向新值
	DWORD								m_dwKeyboardBufferOffsetNew = GameKeyboardMouseInput::KEYBOARD_BUFFER_SIZE;
	// 鼠标状态缓冲 2倍 用于比较是否触发(Trigger)某键
	DIMOUSESTATE						m_cMouseBuffer[2];
	// 鼠标状态缓冲偏移值 指向旧值
	DWORD								m_dwMouseBufferOffsetOld = 0;
	// 鼠标状态缓冲偏移值 指向新值
	DWORD								m_dwMouseBufferOffsetNew = 1;
	// 鼠标当前X坐标
	FLOAT								m_fPointX = 0.F;
	// 鼠标当前Y坐标
	FLOAT								m_fPointY = 0.F;
	// 鼠标灵敏度
	FLOAT								m_mSensitivity = 1.F;
public:
	// GameKeyboardMouseInput 一号实例 欲支持多人请多添加静态变量
	static GameKeyboardMouseInput		s_inputPlayer1;
};

#define KMInput GameKeyboardMouseInput::s_inputPlayer1
