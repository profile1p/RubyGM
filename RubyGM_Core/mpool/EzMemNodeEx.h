// EzMemNodeEx类 内存池节点
// !*Unicode编码请注意*!

#pragma once

// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途:	简单的内存池实现


//#pragma pack(push) //保存对齐状态
//#pragma pack(1)

// 使用标准C++ 不使用空数组

class EzMemPoolEx;


// 内存池节点 不是一般的c++用new出来的，所以不写构造/析构
// 而是显式调用Init()/Destroy()代替
class EzMemNodeEx{
	// 友元类声明
	friend EzMemPoolEx;
	// MemoryInfo 定义
	union MemoryInfo{
		struct  {
			BYTE*		pointer;			// 指针
			size_t		size;				// 大小
		};
		__int64			full;				// 64位寄存器可以一次性处理
	};
public:
	// 内存节点字节
	static const int MEMORY_NODE_BYTE = 1024 * 1024 * 16;
public:
	// 初始化
	inline void		Init();
	// 摧毁
	inline void		Destroy();
private:
	// 构造函数 防止误生成
	EzMemNodeEx(){}
	// 析构函数 防止误生成
	~EzMemNodeEx(){}
	// 申请内存
	inline void*		alloc(size_t size);
	// 释放
	inline void			free(void* address);
private:
	// 下一个节点
	EzMemNodeEx*		m_pNextNode;
	// 每单元底指针
	BYTE*				m_pNodeBottom;
	// 每单元顶指针
	MemoryInfo*			m_pNodeTop;
#ifdef USING_STANDARD_C_PLUS_PLUS
	static const int	ADDIBLE_BLOCK = 16;
	// 基数组
	BYTE				m_aBase[ADDIBLE_BLOCK];
#else
	// 基数组 本对象不占空间 标准C++不支持
	BYTE				m_aBase[];
#endif
public:
#ifdef USING_STANDARD_C_PLUS_PLUS
	// 获取生成大小
	static const int GetMakeSize(){ return sizeof(EzMemNodeEx)+MEMORY_NODE_BYTE - ADDIBLE_BLOCK; }
#else
	// 获取生成大小
	static const int GetMakeSize(){ return sizeof(EzMemNodeEx)+MEMORY_NODE_BYTE; }
#endif
};


// 生成 EzMemNodeEx
inline EzMemNodeEx* new_EzMemNodeEx(){
	EzMemNodeEx* node = (EzMemNodeEx*)malloc(EzMemNodeEx::GetMakeSize());
	node->Init();
	return node;
}

// 释放 名称没问题 重了也是重载(模板怎么办  Σ(●ﾟдﾟ●)
inline void delete_(EzMemNodeEx* node){
	node->Destroy();
	free(node);
}

// 安全释放 名称没问题 重了也是重载(模板怎么办 Σ(●ﾟдﾟ●)
inline void safe_delete(EzMemNodeEx*& node){
	if (node){
		node->Destroy();
		free(node);
		node = NULL;
	}
}


// 初始化 EzMemNodeEx
inline void EzMemNodeEx::Init(){
	m_pNextNode = NULL;
	// 初始化指针
	m_pNodeBottom = m_aBase;
	m_pNodeTop = reinterpret_cast<MemoryInfo*>(m_aBase + EzMemNodeEx::MEMORY_NODE_BYTE);
#if defined(_DEBUG) || defined(_GAME_DEBUG)
	_cwprintf(L"<EzMemNodeEx::Init>创建新内存节点作为内存池使用 :new MemNode\n");
#endif
}

// 摧毁 
inline void	EzMemNodeEx::Destroy(){
#if defined(_DEBUG) || defined(_GAME_DEBUG)
	_cwprintf(L"<EzMemNodeEx::Destroy>释放内存节点. :delete MemNodeEx\n");
#endif
}

// 申请内存
inline void*	EzMemNodeEx::alloc(size_t size){
#ifdef _DEBUG
	assert(size < (EzMemNodeEx::MEMORY_NODE_BYTE - sizeof(EzMemNodeEx::MemoryInfo)) &&
		"<EzMemNodeEx::alloc> 申请字节超过EzMemNodeEx::MEMORY_NODE_BYTE");
#endif
	if ((m_pNodeBottom + sizeof(EzMemNodeEx::MemoryInfo) + size) <=
		reinterpret_cast<BYTE*>(m_pNodeTop)){
		--m_pNodeTop;
		m_pNodeTop->pointer = m_pNodeBottom;
		m_pNodeTop->size = size;
		m_pNodeBottom += size;
#if defined(_DEBUG) && defined(SHOW_MEMORY_POOL_ALLOC_OFFSET) 
		_cwprintf(L"<EzMemNodeEx::alloc> 申请内存偏移值:%d\n", m_pNodeTop->pointer-m_aBase);
#endif
		return m_pNodeTop->pointer;
	}
	return NULL;
}



// 释放内存
inline void	EzMemNodeEx::free(void* address){
	// 假设释放为最近一次释放的,即 m_pNodeTop->pointer == address
	if (m_pNodeTop->pointer == address){
		// 将m_pNodeTop移至最近一个非0位置
		// LOOP_START:
		while (true){
			m_pNodeBottom = m_pNodeTop->pointer;
			++m_pNodeTop;
			if (m_pNodeTop[-1].pointer == m_aBase||m_pNodeTop->size)
				break;
		}
		
	}
	// 否则仅仅标记0
	else{
		// 利用二分(折半)查找法搜索
		// 逆序升序，即顺序降序，与一般相反
		size_t low = reinterpret_cast<MemoryInfo*>(m_aBase + EzMemNodeEx::MEMORY_NODE_BYTE)
			- m_pNodeTop - 1, high = 0, mid;
		// 如果释放最先申请的
		/*if (m_pNodeTop[index][low].offset == offset){
			m_pNodeTop[index][low].offset = 0;
			// 标记下个Cell头4字节为非0,以免上面的LOOP_START处循环到下一个内存块
			reinterpret_cast<MemoryInfo*>(m_aBase + EzMemNodeEx::MEMORY_CELL_BYTE*(index+1))->full = 0xCDCDCDCD;
			return;
		}*/
		// 其他情况
		while (m_pNodeTop[low].pointer <= m_pNodeTop[high].pointer){
			mid = (low + high) / 2;
			if (m_pNodeTop[mid].pointer == address){
				m_pNodeTop[mid].size = 0;
				return;
			}
			else if (m_pNodeTop[mid].pointer < address){
				low = mid - 1;
			}
			else{
				high = mid + 1;
			}
		}
#ifdef _DEBUG
		assert(0 && "<EzMemNodeEx::free> offset: 404 Not Found.");
#endif
	}
}




//#pragma pack(pop)//恢复对齐状态