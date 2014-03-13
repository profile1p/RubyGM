#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// ��;: ���32��bool��������������������������Լ�ֽڣ�������Ϊ32��λ����
//		AMD64ƽ̨�뻻��64λ��
class Bool32{
public:
	// λ
	UINT32	bit(UINT32 _unnamed){ return _value&(1 << _unnamed); }
	// ֵ
	UINT32	value(UINT32 _unnamed){ return _value&_unnamed; }
	// λ
	void	set_bit_false(UINT32 _unnamed){ _value &= ~(1 << _unnamed); }
	// ֵ
	void	set_value_false(UINT32 _unnamed){ _value &= _unnamed; }
	// λ
	void	set_bit_true(UINT32 _unnamed){ _value |= (1 << _unnamed); }
	// ֵ
	void	set_value_true(UINT32 _unnamed){ _value |= _unnamed; }
	// λ
	void	switch_bit(UINT32 _unnamed){ _value ^= (1 << _unnamed); }
	// ֵ
	void	switch_value(UINT32 _unnamed){ _value ^= _unnamed; }
private:
	UINT32			_value = 0;
};