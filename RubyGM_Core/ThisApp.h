#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途: 对本程序的抽象 接管对操作系统的通讯

class ThisApp{
public:
	// 析构函数
	~ThisApp(){
		if (m_pRenderThread){
			// 显式析构
			m_pRenderThread->~thread();
			MPool.Free(m_pRenderThread);
			m_pRenderThread = nullptr;
		}
		if (m_pGameLogicThread){
			// 显式析构
			m_pGameLogicThread->~thread();
			MPool.Free(m_pGameLogicThread);
			m_pGameLogicThread = nullptr;
		}
	}
	// 初始化
	HRESULT	Initialize();
	// 消息循环
	void		RunMessageLoop();
	// 尝试执行Ruby脚本
	VALUE			TryEval(const char* str);
	// 获取窗口句柄
	HWND				GetWindowHanlde(){ return m_hwnd; }
	// 获取窗口宽度
	UINT					GetWindowWidth(){
		RECT clientRect; ::GetClientRect(m_hwnd, &clientRect);
		return clientRect.right;
	}
	// 获取窗口高度
	UINT					GetWindowHeight(){
		RECT clientRect; ::GetClientRect(m_hwnd, &clientRect);
		return clientRect.bottom;
	}
	// 获取渲染目标呈现器宽度
	UINT					GetRTWidth(){ return (UINT)m_imageRender.GetWidth(); }
	// 获取渲染目标呈现器高度
	UINT					GetRTHeight(){ return (UINT)m_imageRender.GetWidth(); }
private:
	// 窗口过程函数
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	// 图形图像渲染类
	ImageRender					m_imageRender = ImageRender();
	// 窗口句柄
	HWND						m_hwnd = nullptr;
	// 渲染线程
	std::thread*				m_pRenderThread = nullptr;
	// 游戏逻辑线程
	std::thread*				m_pGameLogicThread = nullptr;
};