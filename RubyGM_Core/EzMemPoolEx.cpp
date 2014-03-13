#include "stdafx.h"


// EzMemPoolEx 构造函数
EzMemPoolEx::EzMemPoolEx() :
#ifdef _DEBUG
m_iAllocCounter(0),
#endif
m_iNULLCounter(0),
m_pHeadNode(NULL)
{
	m_pHeadNode = new_EzMemNodeEx();
	check_pointer(m_pHeadNode, "<EzMemPoolEx::EzMemPoolEx>");
}


// EzMemPoolEx 析构函数
EzMemPoolEx::~EzMemPoolEx()
{
	EzMemNodeEx* node = m_pHeadNode;
	while (node)
	{
		m_pHeadNode = m_pHeadNode->m_pNextNode;
		delete_(node);
		node = m_pHeadNode;
	}
}


// 实现


// 申请内存
void* EzMemPoolEx::Alloc(size_t size){
	void* result = m_pHeadNode->alloc(size);
	if (!result){
		// 万一....
	ReCall:
		// 增加计数
		++m_iNULLCounter;
		// 奇数次->new 偶数次->调换
		if (m_iNULLCounter & 1){
			EzMemNodeEx* node = new_EzMemNodeEx();
			check_pointer(node, "<EzMemPoolEx::Alloc>");
			if (!node)
				return nullptr;
			node->m_pNextNode = m_pHeadNode;
			m_pHeadNode = node;
			result = m_pHeadNode->alloc(size);
		}
		else{
			EzMemNodeEx *pToTail = NULL, *pTail = m_pHeadNode;
			while (pTail){
				if (!pTail->m_pNextNode)
					break;
				pToTail = pTail;
				pTail = pTail->m_pNextNode;
			}
#ifdef _DEBUG
			assert(pToTail&&"我再去.");
#endif
			pTail->m_pNextNode = NULL;
			m_pHeadNode = pTail;
			result = m_pHeadNode->alloc(size);
			if (!result)
				goto ReCall;
		}
	}
#ifdef _DEBUG
	if (result)
		++m_iAllocCounter;
	check_pointer(result, "池子干涸了.");
#endif
	return result;
}


// 释放内存
void	EzMemPoolEx::Free(void* address){
#ifdef _DEBUG
	if (address){
		--m_iAllocCounter;
		assert(m_iAllocCounter >= 0 && "<EzMemPoolEx::Free> 计数器小于0,证明试图释放野指针.");
	}
#endif
	// 检查address在哪个区
	EzMemNodeEx* node = m_pHeadNode;
	while (node)
	{
		if (address > node&&address < node + EzMemNodeEx::GetMakeSize()){
			return node->free(address);
		}
		else
			node = node->m_pNextNode;
	}
#ifdef _DEBUG
	assert(0 && "<EzMemPoolEx::Free> address:404 Not Found.");
#endif
}