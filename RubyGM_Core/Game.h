#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途: 游戏逻辑线程

class Game{
public:
	// 更新输入
	static VALUE	Input_Update(VALUE self){
		KMInput.Update();
		GamePadInput::Update();
		return self;
	}
	// 游戏逻辑线程
	static HRESULT	LogicThread(HWND hwnd);
	// 退出游戏
	static VALUE	Exit(VALUE self = 0, VALUE code = 0){ s_bExitFlag = true; return Qtrue; }
private:
	// 定义对Ruby接口
	static void		define_interfaces();
	// 尝试运行Ruby脚本
	static VALUE	try_eval(const char* str);
	// 再定义print(cp)
	static VALUE	print(VALUE self, VALUE out);
#if defined(_DEBUG) || defined(_GAME_DEBUG)
	static void		Test();
#endif
	static BOOL		s_bExitFlag;
};