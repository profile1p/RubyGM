#include "stdafx.h"


// EzMemPoolEx ���캯��
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


// EzMemPoolEx ��������
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


// ʵ��


// �����ڴ�
void* EzMemPoolEx::Alloc(size_t size){
	void* result = m_pHeadNode->alloc(size);
	if (!result){
		// ��һ....
	ReCall:
		// ���Ӽ���
		++m_iNULLCounter;
		// ������->new ż����->����
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
			assert(pToTail&&"����ȥ.");
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
	check_pointer(result, "���Ӹɺ���.");
#endif
	return result;
}


// �ͷ��ڴ�
void	EzMemPoolEx::Free(void* address){
#ifdef _DEBUG
	if (address){
		--m_iAllocCounter;
		assert(m_iAllocCounter >= 0 && "<EzMemPoolEx::Free> ������С��0,֤����ͼ�ͷ�Ұָ��.");
	}
#endif
	// ���address���ĸ���
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