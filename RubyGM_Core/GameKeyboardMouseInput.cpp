#include "stdafx.h"
#include "include.h"




// GameKeyboardMouseInput���캯��
GameKeyboardMouseInput::GameKeyboardMouseInput()
{
	// ��ջ�����
	ZeroMemory(m_cKeyboardBuffer, sizeof(m_cKeyboardBuffer));
	ZeroMemory(m_cMouseBuffer, sizeof(m_cMouseBuffer));
}



// GameKeyboardMouseInput��ʼ�� ����
HRESULT GameKeyboardMouseInput::Init(const HINSTANCE hInst, const HWND hwnd)
{
	m_hwnd = hwnd;
	// �Ѿ����ڵĻ��Ͳ���Ҫ��
	if (m_pDirectInputObject)
		return S_FALSE;
	HRESULT hr = S_OK;
	// ����DInput����
	if SUCCEEDED(hr){
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInputObject, 0);
	}
	// ����DInput����豸
	if SUCCEEDED(hr){
		hr = m_pDirectInputObject->CreateDevice(GUID_SysMouse, &m_pDirectInputDeviceMouse, 0);
	}
	// �������ݸ�ʽ :���
	if SUCCEEDED(hr){
		hr = m_pDirectInputDeviceMouse->SetDataFormat(&c_dfDIMouse);
	}
	// ����Э���ȼ� ����ռ
	if SUCCEEDED(hr){
		hr = m_pDirectInputDeviceMouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	}
	// ����DInput�����豸
	if SUCCEEDED(hr){
		hr = m_pDirectInputObject->CreateDevice(GUID_SysKeyboard, &m_pDirectInputDeviceKeyboard, 0);
	}
	// �������ݸ�ʽ :����
	if SUCCEEDED(hr){
		hr = m_pDirectInputDeviceKeyboard->SetDataFormat(&c_dfDIKeyboard);
	}
	// ����Э���ȼ� ����ռ
	if SUCCEEDED(hr){
		hr = m_pDirectInputDeviceKeyboard->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	}
	// �����������豸 ֪ͨ����ϵͳ�Ѿ�׼�����
	if SUCCEEDED(hr){
		hr = m_pDirectInputDeviceMouse->Acquire();
	}
	// ��ü��������豸 ֪ͨ����ϵͳ�Ѿ�׼�����
	if SUCCEEDED(hr){
		hr = m_pDirectInputDeviceKeyboard->Acquire();
	}
	return hr;
}


// GameKeyboardMouseInput��������
GameKeyboardMouseInput::~GameKeyboardMouseInput(){
	// ��������豸
	if (m_pDirectInputDeviceMouse)
		m_pDirectInputDeviceMouse->Unacquire();
	SafeRelease(m_pDirectInputDeviceMouse);

	// ���������豸
	if (m_pDirectInputDeviceKeyboard)
		m_pDirectInputDeviceKeyboard->Unacquire();
	SafeRelease(m_pDirectInputDeviceKeyboard);

	SafeRelease(m_pDirectInputObject);
}


/*/ ˢ���豸
void	GameKeyboardMouseInput::Update(){
	// �������״̬
	if (m_pDirectInputDeviceMouse){
		m_pDirectInputDeviceMouse->GetDeviceState(
			GameKeyboardMouseInput::MOUSE_BUFFER_SIZE, m_cMouseBuffer + m_dwMouseBufferOffsetOld);
		m_dwMouseBufferOffsetOld = m_dwMouseBufferOffsetOld ? 0 : GameKeyboardMouseInput::MOUSE_BUFFER_SIZE;
		m_dwMouseBufferOffsetNew = m_dwMouseBufferOffsetNew ? 0 : GameKeyboardMouseInput::MOUSE_BUFFER_SIZE;
	}
	// ���¼���״̬
	if (m_pDirectInputDeviceKeyboard){
		m_pDirectInputDeviceKeyboard->GetDeviceState(
			GameKeyboardMouseInput::KEYBOARD_BUFFER_SIZE, m_cKeyboardBuffer + m_dwKeyboardBufferOffsetOld);
		m_dwKeyboardBufferOffsetOld = m_dwKeyboardBufferOffsetOld ? 0 : GameKeyboardMouseInput::KEYBOARD_BUFFER_SIZE;
		m_dwKeyboardBufferOffsetNew = m_dwKeyboardBufferOffsetNew ? 0 : GameKeyboardMouseInput::KEYBOARD_BUFFER_SIZE;
	}
}*/