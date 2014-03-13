#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途: 对帧的抽象 负责调用渲染


// 过渡方式
enum TransitionMode{
	AlphaFadeOut = 0, // Alpha 渐入渐出 通过修改前后不透明度达到过度目的
};



class Graphics{
public:
	// 图像刷新 并等待刷新结束
	static VALUE		Update(VALUE self);
	// 初始化本模块
	static HRESULT		Init(const HWND);
	// 关闭本模块
	static void			Close();
	// 冻结图像 是简单图像过渡的初始操作
	static void			Freeze();
	// 过渡图像 是简单图像过渡的收尾操作
	static void			Transition(UINT time = 30, TransitionMode mode = AlphaFadeOut, UINT parameter = 0);
	// 获取程序运行帧数
	static const UINT	GetFrameCount(){ return s_numFrameCouter; }
	// 获取程序运行帧率
	static const UINT	GetFrameRate(){ return s_DwmTimingInfo.rateCompose.uiNumerator; }
	// 是否允许渲染
	// time		[in]	:允许等待时间 存在默认值:10ms
	static bool			ReadyToRender(DWORD time = 10){return WAIT_OBJECT_0 == WaitForSingleObject(s_hReadyToRender, time); }
	// 结束渲染
	static void			RenderFinished(){
		// 提示治疗完毕
		SetEvent(s_hRenderFinished);
		// 放弃治疗
		ResetEvent(s_hReadyToRender);
	}
private:
	// 提示画面更新句柄
	static HANDLE			s_hReadyToRender;
	// 提示画面更新完毕句柄
	static HANDLE			s_hRenderFinished;
	// 游戏运行帧数
	static UINT				s_numFrameCouter;
	// 时间信息
	static DWM_TIMING_INFO	s_DwmTimingInfo;
};

