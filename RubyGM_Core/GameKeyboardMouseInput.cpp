#include "stdafx.h"
#include "include.h"




// GameKeyboardMouseInput构造函数
GameKeyboardMouseInput::GameKeyboardMouseInput()
{
	// 清空缓存区
	ZeroMemory(m_cKeyboardBuffer, sizeof(m_cKeyboardBuffer));
	ZeroMemory(m_cMouseBuffer, sizeof(m_cMouseBuffer));
}



// GameKeyboardMouseInput初始化 函数
HRESULT GameKeyboardMouseInput::Init(const HINSTANCE hInst, const HWND hwnd)
{
	m_hwnd = hwnd;
	// 已经存在的话就不需要了
	if (m_pDirectInputObject)
		return S_FALSE;
	HRESULT hr = S_OK;
	// 创建DInput对象
	if SUCCEEDED(hr){
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInputObject, 0);
	}
	// 创建DInput鼠标设备
	if SUCCEEDED(hr){
		hr = m_pDirectInputObject->CreateDevice(GUID_SysMouse, &m_pDirectInputDeviceMouse, 0);
	}
	// 设置数据格式 :鼠标
	if SUCCEEDED(hr){
		hr = m_pDirectInputDeviceMouse->SetDataFormat(&c_dfDIMouse);
	}
	// 设置协作等级 不独占
	if SUCCEEDED(hr){
		hr = m_pDirectInputDeviceMouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	}
	// 创建DInput键盘设备
	if SUCCEEDED(hr){
		hr = m_pDirectInputObject->CreateDevice(GUID_SysKeyboard, &m_pDirectInputDeviceKeyboard, 0);
	}
	// 设置数据格式 :键盘
	if SUCCEEDED(hr){
		hr = m_pDirectInputDeviceKeyboard->SetDataFormat(&c_dfDIKeyboard);
	}
	// 设置协作等级 不独占
	if SUCCEEDED(hr){
		hr = m_pDirectInputDeviceKeyboard->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	}
	// 获得鼠标输入设备 通知操作系统已经准备完毕
	if SUCCEEDED(hr){
		hr = m_pDirectInputDeviceMouse->Acquire();
	}
	// 获得键盘输入设备 通知操作系统已经准备完毕
	if SUCCEEDED(hr){
		hr = m_pDirectInputDeviceKeyboard->Acquire();
	}
	return hr;
}


// GameKeyboardMouseInput析构函数
GameKeyboardMouseInput::~GameKeyboardMouseInput(){
	// 放弃鼠标设备
	if (m_pDirectInputDeviceMouse)
		m_pDirectInputDeviceMouse->Unacquire();
	SafeRelease(m_pDirectInputDeviceMouse);

	// 放弃键盘设备
	if (m_pDirectInputDeviceKeyboard)
		m_pDirectInputDeviceKeyboard->Unacquire();
	SafeRelease(m_pDirectInputDeviceKeyboard);

	SafeRelease(m_pDirectInputObject);
}


/*/ 刷新设备
void	GameKeyboardMouseInput::Update(){
	// 更新鼠标状态
	if (m_pDirectInputDeviceMouse){
		m_pDirectInputDeviceMouse->GetDeviceState(
			GameKeyboardMouseInput::MOUSE_BUFFER_SIZE, m_cMouseBuffer + m_dwMouseBufferOffsetOld);
		m_dwMouseBufferOffsetOld = m_dwMouseBufferOffsetOld ? 0 : GameKeyboardMouseInput::MOUSE_BUFFER_SIZE;
		m_dwMouseBufferOffsetNew = m_dwMouseBufferOffsetNew ? 0 : GameKeyboardMouseInput::MOUSE_BUFFER_SIZE;
	}
	// 更新键盘状态
	if (m_pDirectInputDeviceKeyboard){
		m_pDirectInputDeviceKeyboard->GetDeviceState(
			GameKeyboardMouseInput::KEYBOARD_BUFFER_SIZE, m_cKeyboardBuffer + m_dwKeyboardBufferOffsetOld);
		m_dwKeyboardBufferOffsetOld = m_dwKeyboardBufferOffsetOld ? 0 : GameKeyboardMouseInput::KEYBOARD_BUFFER_SIZE;
		m_dwKeyboardBufferOffsetNew = m_dwKeyboardBufferOffsetNew ? 0 : GameKeyboardMouseInput::KEYBOARD_BUFFER_SIZE;
	}
}*/