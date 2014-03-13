#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// ��;:	�Լ�д�ĸ�Ч������

// �����б�
// SArray ��̬���� SArray<Ԫ���࣬ ��С>

// DArray ��̬����

// SStack ��̬ջ

// DStack ��̬ջ

// SQueue ��̬(ѭ��)����

// DList ��̬����




// ��ʽ����:


// Object	�������
// Num		��������
template<typename Object, UINT Num>
class SArray{
public:
	// ������ʼ��ֵ�Ĺ��캯��
	SArray(const Object& init){ for (UINT i = 0; i < Num; ++i)m_array[i] = init; }
	// ����ת��Object*
	operator Object*() {return m_array; }
	// Ĭ�ϵĹ��캯��
	inline SArray(){}
	// ��������
	inline Object&			operator[](const int index);
	// ��������
	inline const Object&	operator[](const int index)const;
	// ��ȡ����
	inline const UINT		size(){ return Num; }
	// ��ʼ
	inline Object*			begin(){ return m_array; }
	// ����
	inline Object*			end(){ return m_array + Num; }
	// ��ʼ
	inline const Object*	begin()const{ return m_array; }
	// ����
	inline const Object*	end()const{ return m_array + Num; }
	// new ����
	void* operator			new(size_t size){ return MPoolEx.Alloc(size); }
		// delete ����
	void operator			delete(void* p)	{ MPoolEx.Free(p); }
	// new[] ����
	void* operator			new[](size_t size) = delete;
	// delete[] ����
	void operator			delete[](void* p) = delete;
private:
	// �����Ա
	Object					m_array[Num];
};

template<typename Object>
class DArray{
public:
	// ����������
	typedef		DArray*			Iterator;
public:
	// ���캯��
	DArray(size_t init_capacity);
	// ��������
	~DArray();
	// ��ֹ���ƹ���
	DArray(DArray&) = delete;
	// ��ʼ���б� ����
	DArray(std::initializer_list);
	// move ����
	DArray(DArray&&);
	// ��ֹ = ��ֵ
	DArray& operator &= (DArray) = delete;
	DArray& operator &= (DArray&) = delete;
	// �������� = move��ֵ
	DArray& operator &= (DArray&&);
	// ����ĩβ PUSH ����
	Iterator			push_back(const Object&);
	// ����
	BOOL				resize(size_t expand_size);
	// ������ Cost ����
	BOOL				try_no_cost_resize(size_t expand_size);
private:
	// ����ָ��
	Object*				data = nullptr;
	// �����С
	size_t				size = 0;
	// ��������
	size_t				capacity = 0;
};

// ��̬ջ
template<typename Object, int Num>
class SStack{
	
};




// ��̬ջ
template<typename Object>
class DStack{
public:
	// ���캯��
	DStack();
	// ��������
	~DStack();
	// ѹ��ջ�� ������λ��ָ��
	void push(Object&e);
	// ����ջ��
	BOOL pop(Object&e);
	// ��ȡ����
	int size(){ return nowStackIndex*EACH_STACK_MAX + pEachStackTop[nowStackIndex]; }
	// �Ƿ�Ϊ��
	BOOL empty(){ return pEachStackTop[nowStackIndex] == 0 && nowStackIndex == 0; }
private:
	// ÿ��ջ��С
	static const int EACH_STACK_MAX = 256;
private:
	// ��ջ
	Object**		ppStack;
	// ÿ��ջ
	int*			pEachStackTop;
	// ��ǰջ����
	int				stackSize;
	// ��ǰջ���
	int				nowStackIndex;
};

















// ��ʽʵ��



// SArrayʵ��
// ��������
template<typename Object, UINT Num>
inline Object&	SArray<Object, Num>::operator[](const int index){
#ifdef _DEBUG
	if (index >= Num)
		throw("index ������Χ!", this);
#endif
	// �����������
#ifdef CONTAINER_ALLOW_REV
	if (index < 0){
		if (Num + index >= 0)
			return m_array[Num + index];
#ifdef _DEBUG
		else
			throw("��������index ������Χ!");
#endif
#elif defined(_DEBUG)
	if (index < 0)
		throw("index С��0!", this);
#endif
	return m_array[index];
}


// ��������
template<typename Object, UINT Num>
inline const Object&	SArray<Object, Num>::operator[](const int index)const{
#ifdef _DEBUG
	if (index >= Num)
		throw("index ������Χ!", this);
#endif
	// �����������
#ifdef CONTAINER_ALLOW_REV
	if (index < 0){
		if (Num + index >= 0)
			return m_array[Num + index];
#ifdef _DEBUG
		else
			throw("��������index ������Χ!");
#endif
#elif defined(_DEBUG)
	if (index < 0)
		throw("index С��0!", this);
#endif
	return m_array[index];
}
