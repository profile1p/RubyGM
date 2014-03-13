#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// ��;: ��Ϸ�߼��߳�

class Game{
public:
	// ��������
	static VALUE	Input_Update(VALUE self){
		KMInput.Update();
		GamePadInput::Update();
		return self;
	}
	// ��Ϸ�߼��߳�
	static HRESULT	LogicThread(HWND hwnd);
	// �˳���Ϸ
	static VALUE	Exit(VALUE self = 0, VALUE code = 0){ s_bExitFlag = true; return Qtrue; }
private:
	// �����Ruby�ӿ�
	static void		define_interfaces();
	// ��������Ruby�ű�
	static VALUE	try_eval(const char* str);
	// �ٶ���print(cp)
	static VALUE	print(VALUE self, VALUE out);
#if defined(_DEBUG) || defined(_GAME_DEBUG)
	static void		Test();
#endif
	static BOOL		s_bExitFlag;
};