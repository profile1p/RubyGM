#include "stdafx.h"
#include "include.h"


// 游戏逻辑
HRESULT Game::LogicThread(HWND hwnd){
	if (SUCCEEDED(CoInitialize(NULL)) && SUCCEEDED(Graphics::Init(hwnd)))
	{
		RUBY_INIT_STACK;
		ruby_init();
		// 初始化完毕后，关闭控制台
#if !defined(_DEBUG) &&  !defined(_GAME_DEBUG)
		_cwprintf(L"loaded..\n");
		FreeConsole();
#endif
#ifndef USING_CPlusPlus_MODE
		BrushCache::Init();
#endif
		define_interfaces();
#if defined(_DEBUG) || defined(_GAME_DEBUG)
		Test();
#endif
		{
#ifdef USING_CPlusPlus_MODE
			BitmapCache::Key text(L"方正楷体!");
			Sprite sprite;
			sprite.new_bitmap(640, 480);
			StopsSetColor(sprite, BrushCache::s_colorWhite, 0);
			StopsSetPosition(sprite, 0.F, 0);
			StopsSetColor(sprite, BrushCache::s_colorBlack, 1);
			StopsSetPosition(sprite, .5F, 1);
			StopsSetColor(sprite, BrushCache::s_colorWhite, 2);
			StopsSetPosition(sprite, 1.F, 2);
			//SetColor(sprite, BrushCache::s_colorWhite, 2);
			sprite.bitmap->brush.stop_count = 1;
			sprite.bitmap->brush.linear_properties.startPoint.x = 0.F;
			sprite.bitmap->brush.linear_properties.startPoint.y = 0.F;
			sprite.bitmap->brush.linear_properties.endPoint.x = 0.F;
			sprite.bitmap->brush.linear_properties.endPoint.y = 240.F;
			sprite.bitmap->refresh_brush();
			sprite.bitmap->brush.type = GameBrush::BrushType::Linear;
			//sprite.bitmap->fill_rect(sprite.bitmap->get_rect());
			sprite.bitmap->brush.stop_count = 3;
			//sprite.bitmap->refresh_brush();
			sprite.bitmap->font.font_size = 55.F;
			sprite.bitmap->font.font_name = L"FZKai-Z03";
			sprite.bitmap->refresh_font();
			//sprite.bitmap->draw_text(sprite.bitmap->get_rect(), text);

			GeometryCahce::FontGeometryParameter parameter = {0};
			parameter.emSize = 100.F;
#define SRTING_TEST L"泥壕Hello,world!"
			parameter.pString = SRTING_TEST;
			parameter.uStringLength = sizeof(SRTING_TEST) / sizeof(WCHAR)-1;
			TextFormatCache::CreateFontFaceFromFontFile(L"Font/FZHTK.ttf", &parameter.pFontFace);
			if (SUCCEEDED(GeometryCahce::CreateFontGeometry(&parameter)))
				sprite.bitmap->draw_geometry(parameter.pPathGeometry, 2.F);
			SafeRelease(parameter.pFontFace);
#else
/*#define FONT_NAME_USING L"微软雅黑"
			UINT this_length = sizeof(FONT_NAME_USING) + sizeof TextFormatCache::TextFormatParameter - 4;
			TextFormatCache::TextFormatParameter* pParameter =
				(TextFormatCache::TextFormatParameter*)MPool.Alloc(this_length);
			new(pParameter)TextFormatCache::TextFormatParameter();
			if (pParameter){
				pParameter->this_length = this_length;
				pParameter->verson = 1;
				pParameter->font_size = 100.F;
				wcscpy_s(pParameter->font_name, sizeof FONT_NAME_USING, FONT_NAME_USING);
				if (TextFormatCache::SetTextFormat(0, pParameter)){
					int a = 0;
				}
				MPool.Free(pParameter);
			}
#undef FONT_NAME_USING*/
#endif

			/*for (int i = 0; i < 9000; ++i)
			{
				//sprite.x += 0.04F;
				//sprite.y += 0.03F;
				Graphics::Update(0);
				if (s_bExitFlag){
					_cwprintf(L"s_bExitFlag\n");
					break;
				}
			}*/
		}
		Bitmap::Clear();
		Sprite::Clear();
		BitmapCache::Clear();
		TextFormatCache::Clear();
		BrushCache::Clear();
		Graphics::Close();
		//ruby_finalize();
		CoUninitialize();
	}
	else{
		MessageBoxW(hwnd, L"初始化COM失败\nFailed to initialize COM.", L"Error", MB_OK);
		PostMessage(hwnd, WM_CLOSE, 0, 0);
		return E_FAIL;
	}
//end_of_func:
	// 以免处理两次WM_CLOSE
	if (!s_bExitFlag)
		PostMessage(hwnd, WM_CLOSE, 0, 0);
	return S_OK;
}

// CP -> 
VALUE Game::print(VALUE self, VALUE out){
	RString* str = RSTRING(out);
	printf("%s\n", str->as.heap.ptr);
	return self;
}


// 定义对Ruby接口
void Game::define_interfaces(){
	// 重定向print输出
	rb_define_method(rb_mKernel, "cp", RUBY_METHOD_FUNC(Game::print), 1);
	// 载入游戏标准库
	std::ifstream ifs("gamestd/gamelib.rb");
	std::istreambuf_iterator<char> beg(ifs), end;
	std::string string_read(beg, end);
	ifs.close();
	try_eval(string_read.c_str());
	// 定义接口 -- Brush_Manager
	VALUE brushmm_id = rb_define_module("Brush_Manager");
	rb_define_module_function(brushmm_id, "create_brush", RUBY_METHOD_FUNC(BrushCache::RB_CreateBrush), -1);

	// 定义接口 -- Brush_Manager
	VALUE fontmm_id = rb_define_module("Font_Manager");
	rb_define_module_function(fontmm_id, "create_font", RUBY_METHOD_FUNC(TextFormatCache::RB_CreateFont), -1);

	// 定义接口 -- Graphics 模块
	VALUE graphics_id = rb_define_module("Graphics");
	rb_define_module_function(graphics_id, "update", RUBY_METHOD_FUNC(Graphics::Update), 0);

	// 定义接口 -- Input 模块
	VALUE input_id = rb_define_module("Input");
	rb_define_module_function(input_id, "update", RUBY_METHOD_FUNC(Game::Input_Update), 0);

	// 定义接口 -- Bitmap 类
	Bitmap::s_rbBitmapID = try_eval("Bitmap");
	rb_define_method(Bitmap::s_rbBitmapID, "initialize", RUBY_METHOD_FUNC(Bitmap::NewObject), -1);
	rb_define_method(Bitmap::s_rbBitmapID, "dispose", RUBY_METHOD_FUNC(Bitmap::Dispose), 0);
	rb_define_method(Bitmap::s_rbBitmapID, "font=", RUBY_METHOD_FUNC(Bitmap::FontIs), 1);
	rb_define_method(Bitmap::s_rbBitmapID, "brush=", RUBY_METHOD_FUNC(Bitmap::BrushIs), 1);
	rb_define_method(Bitmap::s_rbBitmapID, "draw_text", RUBY_METHOD_FUNC(Bitmap::DrawTextBaisc), -1);
	rb_define_method(Bitmap::s_rbBitmapID, "dup", RUBY_METHOD_FUNC(Bitmap::DupClone), 0);
	rb_define_method(Bitmap::s_rbBitmapID, "clone", RUBY_METHOD_FUNC(Bitmap::DupClone), 0);

	// 定义接口 -- Sprite 类
	VALUE sprite_id = try_eval("Sprite");
	rb_define_method(sprite_id, "initialize", RUBY_METHOD_FUNC(Sprite::NewObject), -1);
	rb_define_method(sprite_id, "dispose", RUBY_METHOD_FUNC(Sprite::Dispose), 0);
	rb_define_method(sprite_id, "bitmap=", RUBY_METHOD_FUNC(Sprite::LinkBitmap), 1);
}

// 尝试运行Ruby脚本
VALUE Game::try_eval(const char* str){
	static char buffer[1024];
	VALUE rc = Qnil;
	VALUE err;
	__try{
		rc = rb_eval_string(str);
	}
	__except (EXCEPTION_EXECUTE_HANDLER){
#ifdef _DEBUG
		_cwprintf(L"<Game::try_eval>: 你懂的\n");
#endif
	}
	if (!NIL_P(err = rb_errinfo())){
		// class
		VALUE kclass = rb_class_path(CLASS_OF(err));
		// message
		VALUE message = rb_obj_as_string(err);
		// backtrace
		VALUE ary = rb_funcall(err, rb_intern("backtrace"), 0);
		VALUE brstr = rb_funcall(ary, rb_intern("to_s"), 0);
		// sprintf
		sprintf_s(buffer, "Error:  %s\n%s\nbacktrace:  %s\n", StringValuePtr(kclass),
			StringValuePtr(message),
			StringValuePtr(brstr));
		MessageBoxA(nullptr, buffer, "Error", MB_OK);
		rb_set_errinfo(Qnil);
	}
	return rc;
}

#if defined(_DEBUG) || defined(_GAME_DEBUG)
void Game::Test(){
	std::ifstream ifs("gamestd/test.rb");
	std::istreambuf_iterator<char> beg(ifs), end;
	std::string string_read(beg, end);
	ifs.close();
	try_eval(string_read.c_str());
}
#endif