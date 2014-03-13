#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途: 存放32个bool变量的容器，方便对齐操作，节约字节，即长度为32的位向量
//		AMD64平台请换成64位的
class Bool32{
public:
	// 位
	UINT32	bit(UINT32 _unnamed){ return _value&(1 << _unnamed); }
	// 值
	UINT32	value(UINT32 _unnamed){ return _value&_unnamed; }
	// 位
	void	set_bit_false(UINT32 _unnamed){ _value &= ~(1 << _unnamed); }
	// 值
	void	set_value_false(UINT32 _unnamed){ _value &= _unnamed; }
	// 位
	void	set_bit_true(UINT32 _unnamed){ _value |= (1 << _unnamed); }
	// 值
	void	set_value_true(UINT32 _unnamed){ _value |= _unnamed; }
	// 位
	void	switch_bit(UINT32 _unnamed){ _value ^= (1 << _unnamed); }
	// 值
	void	switch_value(UINT32 _unnamed){ _value ^= _unnamed; }
private:
	UINT32			_value = 0;
};