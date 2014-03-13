#include "stdafx.h"
#include "include.h"




// ʵ��

// DStack���캯��
template<typename Object> DStack<Object>::DStack(){
	// ʹ��ջ��ĵ�һλջ
	nowStackIndex = 0;
	// Ĭ��ʹ��һ��ջ *��������޸�*
	stackSize = 1;
	// ����ָ��ռ�
	//ppStack = new Object*[stackSize];
	ppStack = (Object**)MPool.Alloc(stackSize * sizeof(Object*))
	//pEachStackTop = new int[stackSize];
	pEachStackTop = (int*)MPool.Alloc(stackSize * sizeof(int))
	for (int i = 0; i < stackSize; ++i){
		// ����ջ�ռ�
		// ppStack[i] = new Object[EACH_STACK_MAX];
		ppStack[i] = (Object*)MPoolEx.Alloc(EACH_STACK_MAX * sizeof(Object))
		// �趨ջ��λ�� ʵ��λ��+1 ��0Ϊ�� 1Ϊ�±�Ϊ0��Ԫ��
		pEachStackTop[i] = 0;
	}
}


// ��������
template<typename Object> DStack<Object>::~DStack(){
	// ɾ��ջ��
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
	// ɾ����λ����Ϣ����
	if (pEachStackTop){
		MPool.Free(pEachStackTop);
		pEachStackTop = nullptr;
	}
}


// ѹ��ջ�� ������λ��ָ��
template<typename Object> void DStack<Object>::push(Object&e){
	// ��ǰջ���˵Ļ�
	if (pEachStackTop[nowStackIndex] >= EACH_STACK_MAX){
		// ջ���ǲ����Ļ�
		if (++nowStackIndex >= stackSize){
			//Object** new_ppStack = new Object*[stackSize + 1];
			Object** new_ppStack = (Object**)MPool.Alloc((stackSize + 1) * sizeof(Object*));
			//int* new_pEachStackTop = new int[stackSize + 1];
			int* new_pEachStackTop = (int*)MPool.Alloc((stackSize + 1) * sizeof(int));

			// ��ֵ�����µط�
			for (int i = 0; i < stackSize; ++i){
				new_ppStack[i] = ppStack[i];
				new_pEachStackTop[i] = pEachStackTop[i];
			}
			// �ͷž�ֵ
			//delete[] ppStack;
			MPool.Free(ppStack);
			//delete[] pEachStackTop;
			MPool.Free(pEachStackTop);
			// ����ֵ
			ppStack = new_ppStack;
			pEachStackTop = new_pEachStackTop;
			// �ٷ���һ���ռ�
			//ppStack[stackSize] = new Object[EACH_STACK_MAX];
			ppStack[stackSize] = (Object*)MPoolEx.Alloc(EACH_STACK_MAX * sizeof(Object))
			// ջ����Ϊ0
			pEachStackTop[stackSize] = 0;
			// ����ջ������
			++stackSize;
		}
		pEachStackTop[nowStackIndex] = 0;
	}
	// ջ����Ҫ���
	ppStack[nowStackIndex][pEachStackTop[nowStackIndex]] = e;
	++pEachStackTop[nowStackIndex];
}


// ����ջ��
template<typename Object> BOOL DStack<Object>::pop(Object&e){
	// ��ǰջ��ʱ
	if (pEachStackTop[nowStackIndex] == 0){
		// ���Լ����һ��ջ
		if (nowStackIndex == 0)
			// û����һ��ջ�ͷ���false
			return FALSE;
		--nowStackIndex;
		pEachStackTop[nowStackIndex] = EACH_STACK_MAX;
	}
	// ջ����Ҫ���
	e = ppStack[nowStackIndex][--pEachStackTop[nowStackIndex]];
	return TRUE;
}
