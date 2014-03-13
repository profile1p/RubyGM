#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// ��;: �Ա�����ĳ��� �ӹܶԲ���ϵͳ��ͨѶ

class ThisApp{
public:
	// ��������
	~ThisApp(){
		if (m_pRenderThread){
			// ��ʽ����
			m_pRenderThread->~thread();
			MPool.Free(m_pRenderThread);
			m_pRenderThread = nullptr;
		}
		if (m_pGameLogicThread){
			// ��ʽ����
			m_pGameLogicThread->~thread();
			MPool.Free(m_pGameLogicThread);
			m_pGameLogicThread = nullptr;
		}
	}
	// ��ʼ��
	HRESULT	Initialize();
	// ��Ϣѭ��
	void		RunMessageLoop();
	// ����ִ��Ruby�ű�
	VALUE			TryEval(const char* str);
	// ��ȡ���ھ��
	HWND				GetWindowHanlde(){ return m_hwnd; }
	// ��ȡ���ڿ��
	UINT					GetWindowWidth(){
		RECT clientRect; ::GetClientRect(m_hwnd, &clientRect);
		return clientRect.right;
	}
	// ��ȡ���ڸ߶�
	UINT					GetWindowHeight(){
		RECT clientRect; ::GetClientRect(m_hwnd, &clientRect);
		return clientRect.bottom;
	}
	// ��ȡ��ȾĿ����������
	UINT					GetRTWidth(){ return (UINT)m_imageRender.GetWidth(); }
	// ��ȡ��ȾĿ��������߶�
	UINT					GetRTHeight(){ return (UINT)m_imageRender.GetWidth(); }
private:
	// ���ڹ��̺���
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	// ͼ��ͼ����Ⱦ��
	ImageRender					m_imageRender = ImageRender();
	// ���ھ��
	HWND						m_hwnd = nullptr;
	// ��Ⱦ�߳�
	std::thread*				m_pRenderThread = nullptr;
	// ��Ϸ�߼��߳�
	std::thread*				m_pGameLogicThread = nullptr;
};