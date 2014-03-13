#include "stdafx.h"
#include "include.h"


// ��Ruby�ӿ�ʵ��



// �½�����
VALUE Sprite::NewObject(int argc, VALUE* argv, VALUE self){
	UINT layer;
	if (argc > 1) { 
		rb_raise(rb_eArgError, "wrong number of arguments\n������������");
		return self;
	}
	if ((layer = argc ? FIX2INT(argv[0]) : s_array.size() - 1) >= s_array.size()){
		rb_raise(rb_eArgError, "INVALIDARG arguments\n������Ч");
		return self;
	}

	Sprite* pSprite = new Sprite(layer);
#ifdef _DEBUG
	if (pSprite){
#endif
		VALUE rb_m_data = rb_ary_new2(EnumRubyObjectDataIndex::RUBY_OBJECT_DATA_SIZE);
		rb_iv_set(self, "@_edata_", rb_m_data);
		rb_iv_set(self, "@bitmap", Qnil);
		pSprite->m_aryData = RARRAY(rb_m_data);
		rb_ary_store(rb_m_data, EnumRubyObjectDataIndex::ACheckCode, (VALUE)CHECK_CODE);
		rb_ary_store(rb_m_data, EnumRubyObjectDataIndex::BNeedRefreshFlag, Qfalse);
		rb_ary_store(rb_m_data, EnumRubyObjectDataIndex::CSpriteX, rb_float_new(0.));
		rb_ary_store(rb_m_data, EnumRubyObjectDataIndex::DSpriteY, rb_float_new(0.));
		rb_ary_store(rb_m_data, EnumRubyObjectDataIndex::DSpriteZ, INT2FIX(pSprite->m_z));

		s_mapRubytoCPlusPlus.insert(vtype(self, pSprite));
		ROBJECT(self)->as.heap.ivptr = (VALUE*)pSprite;
#ifdef _DEBUG
		_cwprintf(L"new sprite: 0x%08X\n", (UINT)pSprite);
	}
	else
		assert(0 && "<Sprite::NewObject>:: new Sprite -> nullptr");
#endif
	return self;
}

// �ͷŶ���
VALUE Sprite::Dispose(VALUE self){
	Sprite* pSprite = (Sprite*)(ROBJECT(self)->as.heap.ivptr);
#ifdef _DEBUG
	_cwprintf(L"delete sprite: 0x%08X\n", (UINT)pSprite);
#endif
	delete pSprite;
	s_mapRubytoCPlusPlus.erase(self);
	return Qnil;
}


// ����Bitmap
VALUE Sprite::LinkBitmap(VALUE self, VALUE rbbitmap){
	// Sprite
	//VALUE rb_m_data = rb_iv_get(self, "@_edata_");
	//RArray* ary = RARRAY(rb_m_data);
	//Sprite* pSprite = (Sprite*)ary->as.heap.ptr[Sprite::EnumRubyObjectDataIndex::ASpritePointer];

	Sprite* pSprite = (Sprite*)ROBJECT(self)->as.heap.ivptr;
#if defined(_DEBUG) || defined(_GAME_DEBUG)
	//_cwprintf(L"CheckCode: 0x%X\n", RARRAY_PTR(VALUE(pSprite->m_aryData))[EnumRubyObjectDataIndex::ACheckCode]);
	if (RARRAY_PTR(VALUE(pSprite->m_aryData))[EnumRubyObjectDataIndex::ACheckCode] != CHECK_CODE){
		MessageBoxW(nullptr, L"self: pointer invain\nָ����Ч!", L"<Sprite::LinkBitmap>: Error", MB_OK);
	}
#endif
	if (NIL_P(rbbitmap))
		pSprite->bitmap = nullptr;
	else{
		// Bitmap
		//rb_m_data = rb_iv_get(rbbitmap, "@_edata_");
		//ary = RARRAY(rb_m_data);
		//Bitmap* pBitmap = (Bitmap*)ary->as.heap.ptr[Bitmap::EnumRubyObjectDataIndex::ABitmapPointer];
		Bitmap* pBitmap = (Bitmap*)(ROBJECT(rbbitmap)->as.heap.ivptr);
#if defined(_DEBUG) || defined(_GAME_DEBUG)
		if (pBitmap->m_aryData->as.heap.ptr[Bitmap::EnumRubyObjectDataIndex::ACheckCode] != Bitmap::CHECK_CODE){
			MessageBoxW(nullptr, L"rbbitmap: pointer invain\nָ����Ч!", L"<Sprite::LinkBitmap>: Error", MB_OK);
		}
#endif
		// Link
		pSprite->bitmap = pBitmap;
		rb_iv_set(self, "@bitmap", rbbitmap);

	}
	return rbbitmap;
}

VALUE Sprite::SetZ(VALUE self, VALUE new_z){
	Sprite* pSprite = (Sprite*)ROBJECT(self)->as.heap.ivptr;
#if defined(_DEBUG) || defined(_GAME_DEBUG)
	if (pSprite->m_aryData->as.heap.ptr[EnumRubyObjectDataIndex::ACheckCode] != CHECK_CODE){
		MessageBoxW(nullptr, L"self: pointer invain\nָ����Ч!", L"<Sprite::LinkBitmap>: Error", MB_OK);
	}
#endif
	rb_ary_store(VALUE(pSprite->m_aryData), EnumRubyObjectDataIndex::DSpriteZ, new_z);
	pSprite->set_z(FIX2INT(new_z));
	return new_z;
}
// C++ �ӿ�
// Sprite �๹�캯��
Sprite::Sprite(UINT layer) :m_layer(layer)
{
	// ��ʼ������
	m_desRect = { 0.F, 0.F, 0.F, 0.F };
	// ��������
	push_in_list(this);
}


// Sprite ����������
Sprite::~Sprite(){
	s_array[m_layer].remove(this);
	/*if (bitmap){
#ifdef _DEBUG
		_cwprintf(L"Sprite#bitmap �ѱ�ɾ����0x%X # 0x%X\n", this, bitmap);
#endif
		delete bitmap;
		bitmap = nullptr;
	}*/
}

// ��������
void Sprite::push_in_list(Sprite* obj){
	// ����Ϊ�ջ��ߴ��ڵ���β�ͣ���push_back
	if (s_array[obj->m_layer].empty() || obj->m_z >= s_array[obj->m_layer].back()->m_z){
		s_array[obj->m_layer].push_back(obj);
		return;
	}
	// ����嵽��һ��С�ڵ��ڵ�������ָ��ֵǰ��
	for (auto itr = s_array[obj->m_layer].begin(); itr != s_array[obj->m_layer].end(); ++itr){
		if (obj->m_z <= (*itr)->m_z){
			s_array[obj->m_layer].insert(itr, obj);
			return;
		}
	}
#ifdef _DEBUG
	MessageBoxW(nullptr, L"��������ʧ��!\n", L"Error", MB_OK);
#endif
}


// ������ Ruby -> C++
void Sprite::check(){
	if (!m_aryData)
		return;
	RFloat* pFloat;
	// ���ˢ��
	if (m_aryData->as.heap.ptr[EnumRubyObjectDataIndex::BNeedRefreshFlag] == Qtrue){
		// ȡ����ʶ ���Ƽ���ô�� -_-||
		const_cast<VALUE*>(m_aryData->as.heap.ptr)[EnumRubyObjectDataIndex::BNeedRefreshFlag] = Qfalse;
		// X���� �������ͱ���
		if (FIXNUM_P(m_aryData->as.heap.ptr[EnumRubyObjectDataIndex::CSpriteX])){
			this->x = (FLOAT)FIX2INT(m_aryData->as.heap.ptr[EnumRubyObjectDataIndex::CSpriteX]);
		}
		else{
			pFloat = RFLOAT(m_aryData->as.heap.ptr[EnumRubyObjectDataIndex::CSpriteX]);
			this->x = (FLOAT)(pFloat->float_value);
		}
		// Y���� �������ͱ���
		if (FIXNUM_P(m_aryData->as.heap.ptr[EnumRubyObjectDataIndex::DSpriteY])){
			this->y = (FLOAT)FIX2INT(m_aryData->as.heap.ptr[EnumRubyObjectDataIndex::DSpriteY]);
		}
		else{
			pFloat = RFLOAT(m_aryData->as.heap.ptr[EnumRubyObjectDataIndex::DSpriteY]);
			this->y = (FLOAT)(pFloat->float_value);
		}
	}
}


// ����
void Sprite::Clear(){
	for (auto& i : s_mapRubytoCPlusPlus){
		if (i.second){
			delete i.second;
			i.second = nullptr;
		}
	}
	s_mapRubytoCPlusPlus.clear();
}