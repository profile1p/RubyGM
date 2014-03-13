// EzMemPoolEx类 简单易懂的现代内存池——EasyMemoryPoolEx

#pragma once

// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途:	简单的内存池实现

#ifndef USING_MULTITHREAD_MEMORY_POOL_FLAG
#define MPoolEx EzMemPoolEx::s_instance
#endif

// 内存池
class EzMemPoolEx{
public:
	// 申请内存
	void*				Alloc(size_t size);
	// 释放内存
	void				Free(void* address);
#ifdef _DEBUG
	// 申请-释放 计数器
	int					m_iAllocCounter;
#endif


#ifdef USING_MULTITHREAD_MEMORY_POOL_FLAG
public:
#else
private:
#endif
	// 强行单例
	EzMemPoolEx();
	// 析构
	~EzMemPoolEx();
private:
	// 链表头指针
	EzMemNodeEx*			m_pHeadNode;
	// 为空次数
	size_t					m_iNULLCounter;
#ifndef USING_MULTITHREAD_MEMORY_POOL_FLAG
public:
	// 唯一实例
	static	EzMemPoolEx		s_instance;
#endif
};