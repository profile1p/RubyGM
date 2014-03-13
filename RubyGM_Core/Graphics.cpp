#include "stdafx.h"
#include "include.h"



// ��ʼ��Graphics
HRESULT Graphics::Init(const HWND hwnd){
	s_hReadyToRender = CreateEvent(NULL, FALSE, FALSE, NULL);
	s_hRenderFinished = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!s_hReadyToRender || !s_hRenderFinished)
#ifdef _DEBUG
		assert(0&&"��������ʧ��!\n");
#else
		return E_FAIL;
#endif
	// ��ȡ��Ļˢ���� DWMû�����еĻ� ������DC����  һ��Ϊ60hz
	ZeroMemory(&s_DwmTimingInfo, sizeof(s_DwmTimingInfo));
	s_DwmTimingInfo.cbSize = sizeof(s_DwmTimingInfo);
	if (FAILED(DwmGetCompositionTimingInfo(NULL, &s_DwmTimingInfo))){
		HDC hdc = GetDC(hwnd);
		s_DwmTimingInfo.rateCompose.uiDenominator = 1;
		s_DwmTimingInfo.rateCompose.uiNumerator = GetDeviceCaps(hdc, VREFRESH);
		ReleaseDC(hwnd, hdc);
	}
	return S_OK;
}


// �ر�Graphics
void Graphics::Close(){
	if (s_hReadyToRender){
		CloseHandle(s_hReadyToRender);
		s_hReadyToRender = NULL;
	}
	if (s_hRenderFinished){
		CloseHandle(s_hRenderFinished);
		s_hRenderFinished = NULL;
	}
}


// ˢ��Graphics
VALUE Graphics::Update(VALUE self){
	++s_numFrameCouter;
	Sleep(1);
	SetEvent(s_hReadyToRender);
	WaitForSingleObject(s_hRenderFinished, INFINITE);
	ResetEvent(s_hRenderFinished);
	return Qnil;
}



// ����ͼ�� �Ǽ�ͼ����ɵĳ�ʼ����
void	Graphics::Freeze(){

}



// ����ͼ�� �Ǽ�ͼ����ɵ���β����
// time				[in]	: ����ʱ�� Ĭ��30֡ 60Hz��ʾ����Ϊ���� 120Hz��ʾ����Ϊ�ķ�֮һ��
// mode				[in]	: ���ɷ�ʽ Ĭ��Alpha���뽥��
// parameter		[in&op]	: ÿ�����ɷ������ܻ�������������������
void	Graphics::Transition(UINT time, TransitionMode mode, UINT parameter){

}