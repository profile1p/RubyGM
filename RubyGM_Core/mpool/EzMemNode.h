// EzMemNode类 内存池节点
// !*Unicode编码请注意*!
#pragma once

// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途:	简单的内存池实现


//#pragma pack(push) //保存对齐状态
//#pragma pack(1)


class EzMemPool;


// 内存池节点 不是一般的c++用new出来的，所以不写构造/析构
// 而是显式调用Init()/Destroy()代替
class EzMemNode{
	// 友元类声明
	friend EzMemPool;
	// MemoryInfo 定义
	union MemoryInfo{
		struct {
			WORD offset;	// 偏移量
			WORD size;		// 大小
		};
		DWORD full;			// x86可以一次性处理32位数据,方便
	};
public:
	// 内存单元大小 4kb
	static const int MEMORY_CELL_BYTE = 4096;
	// 内存节点包含单元数量
	static const int MEMORY_CELL_SIZE = 4096;
	// 内存节点字节
	static const int MEMORY_NODE_BYTE = MEMORY_CELL_SIZE*MEMORY_CELL_BYTE;
	// 空间不足检查次数
	static const int NULL_CHECK_TIMES = MEMORY_CELL_SIZE / 32;
public:
	// 初始化
	inline void		Init();
	// 摧毁
	inline void		Destroy();
private:
	// 构造函数 防止误生成
	EzMemNode(){}
	// 析构函数 防止误生成
	~EzMemNode(){}
	// 申请内存
	inline void*		alloc(size_t size);
	// 释放
	inline void			free(void* address);
private:
	// 下一个节点
	EzMemNode*			m_pNextNode;
	// 当前处理索引(一次性索引)
	size_t				m_iIndex;
	// 第二次当前处理索引(而不是第二次，是第二次及其之后╮（￣▽￣）╭ )
	size_t				m_iIndex2nd;
	// 每单元底指针
	BYTE*				m_paCellBottom[MEMORY_CELL_SIZE];
	// 每单元顶指针
	MemoryInfo*			m_paCellTop[MEMORY_CELL_SIZE];
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
	static const int GetMakeSize(){ return sizeof(EzMemNode)+MEMORY_NODE_BYTE - ADDIBLE_BLOCK; }
#else
	// 获取生成大小
	static const int GetMakeSize(){ return sizeof(EzMemNode)+MEMORY_NODE_BYTE; }
#endif
};


// 生成 EzMemNode
inline EzMemNode* new_EzMemNode(){
	EzMemNode* node = (EzMemNode*)malloc(EzMemNode::GetMakeSize());
	node->Init();
	return node;
}

// 释放 名称没问题 重了也是重载(模板怎么办  Σ(●ﾟдﾟ●)
inline void delete_(EzMemNode* node){
	node->Destroy();
	free(node);
}

// 安全释放 名称没问题 重了也是重载(模板怎么办 Σ(●ﾟдﾟ●)
inline void safe_delete(EzMemNode*& node){
	if (node){
		node->Destroy();
		free(node);
		node = NULL;
	}
}


// 初始化 EzMemNode
inline void EzMemNode::Init(){
	m_pNextNode = NULL;
	m_iIndex = 0;
	m_iIndex2nd = 0;
	// 没必要
	/*ZeroMemory(m_paCellBottom, sizeof(m_paCellBottom));
	ZeroMemory(m_paCellTop, sizeof(m_paCellTop));
	ZeroMemory(m_aBase, EzMemNode::MEMORY_NODE_BYTE);*/
	// 初始化指针
	for (int i = 0; i < EzMemNode::MEMORY_CELL_SIZE; ++i){
		m_paCellBottom[i] = m_aBase + EzMemNode::MEMORY_CELL_BYTE*i;
		m_paCellTop[i] = reinterpret_cast<MemoryInfo*>(m_aBase + EzMemNode::MEMORY_CELL_BYTE*(i + 1));
	}
#if defined(_DEBUG) || defined(_GAME_DEBUG)
	_cwprintf(L"<EzMemNode::Init>创建新内存节点作为内存池使用 :new MemNode\n");
#endif
}

// 摧毁 
inline void	EzMemNode::Destroy(){
#if defined(_DEBUG) || defined(_GAME_DEBUG)
	_cwprintf(L"<EzMemNode::Destroy>释放内存节点. :delete MemNode\n");
#endif
}

// 申请内存
inline void*	EzMemNode::alloc(size_t size){
	void* result = NULL;
#ifdef _DEBUG
	assert(size < (EzMemNode::MEMORY_CELL_BYTE - sizeof(EzMemNode::MemoryInfo)) &&
		"<EzMemNode::alloc> 申请字节超过EzMemNode::MEMORY_CELL_BYTE");
#endif
	// 还在一次性分配中的话
	if (m_iIndex < EzMemNode::MEMORY_CELL_SIZE){
		// 分配完的话
		if ((m_paCellBottom[m_iIndex] + sizeof(EzMemNode::MemoryInfo) + size) > reinterpret_cast<BYTE*>(m_paCellTop[m_iIndex]))
			++m_iIndex;
		if (m_iIndex < EzMemNode::MEMORY_CELL_SIZE){
			--m_paCellTop[m_iIndex];
			m_paCellTop[m_iIndex]->offset = m_paCellBottom[m_iIndex] - m_aBase - EzMemNode::MEMORY_CELL_BYTE*m_iIndex;
			m_paCellTop[m_iIndex]->size = size;
			result = reinterpret_cast<void*>(m_paCellBottom[m_iIndex]);
			m_paCellBottom[m_iIndex] += size;

#if defined(_DEBUG) && defined(SHOW_MEMORY_POOL_ALLOC_OFFSET) 
			_cwprintf(L"<EzMemNode::alloc> 申请内存区:%d, 偏移值:%8d\n", m_iIndex,
				m_paCellTop[m_iIndex]->offset);
#endif

			return result;
		}
	}
	// 利用m_iIndex2nd分配
	for (int i = 0; i < EzMemNode::NULL_CHECK_TIMES; ++i) {
		if ((m_paCellBottom[m_iIndex2nd] + sizeof(EzMemNode::MemoryInfo) + size) <=
			reinterpret_cast<BYTE*>(m_paCellTop[m_iIndex2nd])){
			--m_paCellTop[m_iIndex2nd];
			m_paCellTop[m_iIndex2nd]->offset = m_paCellBottom[m_iIndex2nd] - m_aBase - EzMemNode::MEMORY_CELL_BYTE*m_iIndex2nd;
			m_paCellTop[m_iIndex2nd]->size = size;
			result = reinterpret_cast<void*>(m_paCellBottom[m_iIndex2nd]);
			m_paCellBottom[m_iIndex2nd] += size;

#if defined(_DEBUG) && defined(SHOW_MEMORY_POOL_ALLOC_OFFSET) 
			_cwprintf(L"<EzMemNode::alloc> 申请内存区:%d, 偏移值:%8d\n", m_iIndex2nd,
				m_paCellTop[m_iIndex2nd]->offset);
#endif
			return result;
		}
		++m_iIndex2nd;
		m_iIndex2nd %= EzMemNode::MEMORY_CELL_SIZE;
	}
	return result;
}



// 释放内存
inline void	EzMemNode::free(void* address){
	// 计算索引编号
	size_t index = (reinterpret_cast<BYTE*>(address)-m_aBase) / EzMemNode::MEMORY_CELL_SIZE;
	// 计算偏移值
	WORD offset = reinterpret_cast<BYTE*>(address)-m_aBase - EzMemNode::MEMORY_CELL_BYTE*index;
	// 假设释放为最近一次释放的,即 m_paCellTop[index]->offset == offset
	if (m_paCellTop[index]->offset == offset){
		// 将m_paCellTop移至最近一个非0位置
		//m_paCellTop[index][1].full += 0;
		// LOOP_START:
		while (true){
			m_paCellBottom[index] = reinterpret_cast<BYTE*>(m_aBase)+
				EzMemNode::MEMORY_CELL_BYTE*index + m_paCellTop[index]->offset;
			++m_paCellTop[index];
			if (!m_paCellTop[index]->offset || m_paCellTop[index]->size)
				break;
		}
		
	}
	// 否则仅仅标记0
	else{
		// 利用二分(折半)查找法搜索
		// 逆序升序，即顺序降序，与一般相反
		size_t low = reinterpret_cast<MemoryInfo*>(m_aBase + EzMemNode::MEMORY_CELL_BYTE*(index + 1))
			- m_paCellTop[index] - 1, high = 0, mid;
		// 如果释放最先申请的
		/*if (m_paCellTop[index][low].offset == offset){
			m_paCellTop[index][low].offset = 0;
			// 标记下个Cell头4字节为非0,以免上面的LOOP_START处循环到下一个内存块
			reinterpret_cast<MemoryInfo*>(m_aBase + EzMemNode::MEMORY_CELL_BYTE*(index+1))->full = 0xCDCDCDCD;
			return;
		}*/
		// 其他情况
		while (m_paCellTop[index][low].offset <= m_paCellTop[index][high].offset){
			mid = (low + high) / 2;
			if (m_paCellTop[index][mid].offset == offset){
				m_paCellTop[index][mid].size = 0;
				return;
			}
			else if (m_paCellTop[index][mid].offset < offset){
				low = mid - 1;
			}
			else{
				high = mid + 1;
			}
		}
#ifdef _DEBUG
		assert(0 && "<EzMemNode::free> offset: 404 Not Found.");
#endif
	}
}




//#pragma pack(pop)//恢复对齐状态