/*������Ʈ Ǯ*/

#pragma once
#include	<iostream>
#define		POOL_MAX_SIZE 1024
#define		POOL_SIZE_MASK (POOL_MAX_SIZE - 1)
template <class type>
class KObjectPool
{
	//volatile ����ȭ(�����Ϸ�) ����, ������ cache ��� ����
	static void* volatile m_ObjectPool[POOL_MAX_SIZE];
	static long long volatile g_iHead; // ���� �� �Ҵ� �ε���
	static long long volatile g_iTail; // �Ҹ� �� ���� �ε���
	// �غ�Ǿ� ������ �غ�� ������Ʈ�� ��ȯ���ְ�
	// �غ�� ������Ʈ�� ������ �űԷ� �����ؼ� ��ȯ������.
public:
	static void AllFree()
	{
		for (int i = 0; i < POOL_MAX_SIZE; i++)
		{
			void* data = InterlockedExchangePointer(&m_ObjectPool[i], nullptr);
			if (data != nullptr)
			{
				_aligned_free(m_ObjectPool[i]);
			}
		}
	}
	//�޸� �Ҵ�(�޸� �ּ�)�� �ϰ� ������ ���߿� �ҷ��� ���� �ٲ�
	static void* operator new(size_t size)
	{
		//�����忡�� ���ư��� �Ǹ� 
		//��ſ����� Overlapped ����ü�� �������� �ִµ�
		//�޸� ���� �� ���� -> �� ���� Ȯ�� ->(ĳ��)-> �� Ȯ�� �ϸ� �߰��� ĳ�ð� ����.
		//���� �����Ϳ� ĳ�õ����ʹ� �ּҰ� �ٸ�, ��� ĳ�ø� ������ �ٲ������
		//�޸𸮿� �������� ���ִ� ���� Ȯ���ϰڴ�
		//volatile ĳ�� ������ �ʿ��ϴ�. 

		//����ȭ�� �Ƚᵵ ������ �����ϰ� �ٲٴ� �Լ� ���Ͷ� C++ 11 ����ͺ���
		long long iCurrent = InterlockedIncrement64(&g_iHead)-1;
		long long iPos = iCurrent & POOL_SIZE_MASK;

		void* pObj = InterlockedExchangePointer(&m_ObjectPool[iPos], nullptr);
		//m_ObjectPool[iPos] = nullptr;
		if (pObj != nullptr) return pObj;
		//�޸𸮸� �����ؼ� ������ 16�� �¼� ������ �����ض�
		//����޸𸮰� �ƴ� ������ �޸𸮸� �����Ѵ�.
		return _aligned_malloc(size, MEMORY_ALLOCATION_ALIGNMENT);
	}
	static void  operator delete(void* deleteObj)
	{
		long long iCurrent = InterlockedIncrement64(&g_iTail)-1;
		long long iPos = iCurrent & POOL_SIZE_MASK;
		void* pObj = InterlockedExchangePointer(&m_ObjectPool[iPos], deleteObj);
		if (pObj != nullptr)
		{
			//��ϴ����� �޸� ����
			_aligned_free(pObj);
		}
	}
};
template<typename type>
void* volatile KObjectPool<type>::m_ObjectPool[POOL_MAX_SIZE] = { nullptr, };
template<typename type>
long long volatile KObjectPool<type>::g_iHead = 0; // ���� �� �Ҵ� �ε���
template<typename type>
long long volatile KObjectPool<type>::g_iTail = 0; // �Ҹ� �� ���� �ε���