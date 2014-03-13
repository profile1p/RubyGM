#pragma once
// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途:	自己写的高效率容器

// 容器列表
// SArray 静态数组 SArray<元素类， 大小>

// DArray 动态数组

// SStack 静态栈

// DStack 动态栈

// SQueue 静态(循环)队列

// DList 动态链表




// 正式声明:


// Object	储存对象
// Num		储存数量
template<typename Object, UINT Num>
class SArray{
public:
	// 给定初始化值的构造函数
	SArray(const Object& init){ for (UINT i = 0; i < Num; ++i)m_array[i] = init; }
	// 隐形转换Object*
	operator Object*() {return m_array; }
	// 默认的构造函数
	inline SArray(){}
	// 对象引用
	inline Object&			operator[](const int index);
	// 对象常引用
	inline const Object&	operator[](const int index)const;
	// 获取长度
	inline const UINT		size(){ return Num; }
	// 开始
	inline Object*			begin(){ return m_array; }
	// 结束
	inline Object*			end(){ return m_array + Num; }
	// 开始
	inline const Object*	begin()const{ return m_array; }
	// 结束
	inline const Object*	end()const{ return m_array + Num; }
	// new 操作
	void* operator			new(size_t size){ return MPoolEx.Alloc(size); }
		// delete 操作
	void operator			delete(void* p)	{ MPoolEx.Free(p); }
	// new[] 操作
	void* operator			new[](size_t size) = delete;
	// delete[] 操作
	void operator			delete[](void* p) = delete;
private:
	// 数组成员
	Object					m_array[Num];
};

template<typename Object>
class DArray{
public:
	// 迭代器定义
	typedef		DArray*			Iterator;
public:
	// 构造函数
	DArray(size_t init_capacity);
	// 析构函数
	~DArray();
	// 禁止复制构造
	DArray(DArray&) = delete;
	// 初始化列表 构造
	DArray(std::initializer_list);
	// move 构造
	DArray(DArray&&);
	// 禁止 = 赋值
	DArray& operator &= (DArray) = delete;
	DArray& operator &= (DArray&) = delete;
	// 但是允许 = move赋值
	DArray& operator &= (DArray&&);
	// 数组末尾 PUSH 操作
	Iterator			push_back(const Object&);
	// 扩容
	BOOL				resize(size_t expand_size);
	// 尝试无 Cost 扩容
	BOOL				try_no_cost_resize(size_t expand_size);
private:
	// 数据指针
	Object*				data = nullptr;
	// 数组大小
	size_t				size = 0;
	// 数组容量
	size_t				capacity = 0;
};

// 静态栈
template<typename Object, int Num>
class SStack{
	
};




// 动态栈
template<typename Object>
class DStack{
public:
	// 构造函数
	DStack();
	// 析构函数
	~DStack();
	// 压入栈顶 并返回位置指针
	void push(Object&e);
	// 弹出栈顶
	BOOL pop(Object&e);
	// 获取长度
	int size(){ return nowStackIndex*EACH_STACK_MAX + pEachStackTop[nowStackIndex]; }
	// 是否为空
	BOOL empty(){ return pEachStackTop[nowStackIndex] == 0 && nowStackIndex == 0; }
private:
	// 每组栈大小
	static const int EACH_STACK_MAX = 256;
private:
	// 基栈
	Object**		ppStack;
	// 每组栈
	int*			pEachStackTop;
	// 当前栈数量
	int				stackSize;
	// 当前栈编号
	int				nowStackIndex;
};

















// 正式实现



// SArray实现
// 对象引用
template<typename Object, UINT Num>
inline Object&	SArray<Object, Num>::operator[](const int index){
#ifdef _DEBUG
	if (index >= Num)
		throw("index 超过范围!", this);
#endif
	// 允许逆序浏览
#ifdef CONTAINER_ALLOW_REV
	if (index < 0){
		if (Num + index >= 0)
			return m_array[Num + index];
#ifdef _DEBUG
		else
			throw("逆序引用index 超过范围!");
#endif
#elif defined(_DEBUG)
	if (index < 0)
		throw("index 小于0!", this);
#endif
	return m_array[index];
}


// 对象引用
template<typename Object, UINT Num>
inline const Object&	SArray<Object, Num>::operator[](const int index)const{
#ifdef _DEBUG
	if (index >= Num)
		throw("index 超过范围!", this);
#endif
	// 允许逆序浏览
#ifdef CONTAINER_ALLOW_REV
	if (index < 0){
		if (Num + index >= 0)
			return m_array[Num + index];
#ifdef _DEBUG
		else
			throw("逆序引用index 超过范围!");
#endif
#elif defined(_DEBUG)
	if (index < 0)
		throw("index 小于0!", this);
#endif
	return m_array[index];
}
