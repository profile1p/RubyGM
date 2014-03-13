#include "stdafx.h"
#include "include.h"




// 实现

// DStack构造函数
template<typename Object> DStack<Object>::DStack(){
	// 使用栈组的第一位栈
	nowStackIndex = 0;
	// 默认使用一个栈 *这里可以修改*
	stackSize = 1;
	// 申请指针空间
	//ppStack = new Object*[stackSize];
	ppStack = (Object**)MPool.Alloc(stackSize * sizeof(Object*))
	//pEachStackTop = new int[stackSize];
	pEachStackTop = (int*)MPool.Alloc(stackSize * sizeof(int))
	for (int i = 0; i < stackSize; ++i){
		// 申请栈空间
		// ppStack[i] = new Object[EACH_STACK_MAX];
		ppStack[i] = (Object*)MPoolEx.Alloc(EACH_STACK_MAX * sizeof(Object))
		// 设定栈顶位置 实际位置+1 即0为空 1为下标为0的元素
		pEachStackTop[i] = 0;
	}
}


// 析构函数
template<typename Object> DStack<Object>::~DStack(){
	// 删除栈组
	if (ppStack){
		for (int i = 0; i < stackSize; ++i){
			if (ppStack[i]){
				// delete[] ppStack[i];
				MPoolEx.Free(ppStack[i]);
			}
		}
		// delete[] ppStack;
		MPool.Free(ppStack);
		ppStack = nullptr;
	}
	// 删除顶位置信息数组
	if (pEachStackTop){
		MPool.Free(pEachStackTop);
		pEachStackTop = nullptr;
	}
}


// 压入栈顶 并返回位置指针
template<typename Object> void DStack<Object>::push(Object&e){
	// 当前栈满了的话
	if (pEachStackTop[nowStackIndex] >= EACH_STACK_MAX){
		// 栈还是不够的话
		if (++nowStackIndex >= stackSize){
			//Object** new_ppStack = new Object*[stackSize + 1];
			Object** new_ppStack = (Object**)MPool.Alloc((stackSize + 1) * sizeof(Object*));
			//int* new_pEachStackTop = new int[stackSize + 1];
			int* new_pEachStackTop = (int*)MPool.Alloc((stackSize + 1) * sizeof(int));

			// 旧值赋到新地方
			for (int i = 0; i < stackSize; ++i){
				new_ppStack[i] = ppStack[i];
				new_pEachStackTop[i] = pEachStackTop[i];
			}
			// 释放旧值
			//delete[] ppStack;
			MPool.Free(ppStack);
			//delete[] pEachStackTop;
			MPool.Free(pEachStackTop);
			// 更新值
			ppStack = new_ppStack;
			pEachStackTop = new_pEachStackTop;
			// 再分配一定空间
			//ppStack[stackSize] = new Object[EACH_STACK_MAX];
			ppStack[stackSize] = (Object*)MPoolEx.Alloc(EACH_STACK_MAX * sizeof(Object))
			// 栈顶记为0
			pEachStackTop[stackSize] = 0;
			// 增加栈计数器
			++stackSize;
		}
		pEachStackTop[nowStackIndex] = 0;
	}
	// 栈符合要求后
	ppStack[nowStackIndex][pEachStackTop[nowStackIndex]] = e;
	++pEachStackTop[nowStackIndex];
}


// 弹出栈顶
template<typename Object> BOOL DStack<Object>::pop(Object&e){
	// 当前栈空时
	if (pEachStackTop[nowStackIndex] == 0){
		// 尝试检查上一组栈
		if (nowStackIndex == 0)
			// 没有上一组栈就返回false
			return FALSE;
		--nowStackIndex;
		pEachStackTop[nowStackIndex] = EACH_STACK_MAX;
	}
	// 栈符合要求后
	e = ppStack[nowStackIndex][--pEachStackTop[nowStackIndex]];
	return TRUE;
}
