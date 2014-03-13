// EzMemPoolEx�� ���׶����ִ��ڴ�ء���EasyMemoryPoolEx

#pragma once

// Author  : DustPG
// License : MIT: see more in "License.txt"

// ��;:	�򵥵��ڴ��ʵ��

#ifndef USING_MULTITHREAD_MEMORY_POOL_FLAG
#define MPoolEx EzMemPoolEx::s_instance
#endif

// �ڴ��
class EzMemPoolEx{
public:
	// �����ڴ�
	void*				Alloc(size_t size);
	// �ͷ��ڴ�
	void				Free(void* address);
#ifdef _DEBUG
	// ����-�ͷ� ������
	int					m_iAllocCounter;
#endif


#ifdef USING_MULTITHREAD_MEMORY_POOL_FLAG
public:
#else
private:
#endif
	// ǿ�е���
	EzMemPoolEx();
	// ����
	~EzMemPoolEx();
private:
	// ����ͷָ��
	EzMemNodeEx*			m_pHeadNode;
	// Ϊ�մ���
	size_t					m_iNULLCounter;
#ifndef USING_MULTITHREAD_MEMORY_POOL_FLAG
public:
	// Ψһʵ��
	static	EzMemPoolEx		s_instance;
#endif
};