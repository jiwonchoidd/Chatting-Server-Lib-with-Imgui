/*오브젝트 풀*/

#pragma once
#include	<iostream>
#define		POOL_MAX_SIZE 1024
#define		POOL_SIZE_MASK (POOL_MAX_SIZE - 1)
template <class type>
class KObjectPool
{
	//volatile 최적화(컴파일러) 방지, 변수의 cache 사용 방지
	static void* volatile m_ObjectPool[POOL_MAX_SIZE];
	static long long volatile g_iHead; // 생성 및 할당 인덱스
	static long long volatile g_iTail; // 소멸 및 해제 인덱스
	// 준비되어 있으면 준비된 오브젝트를 반환해주고
	// 준비된 오브젝트가 없으면 신규로 생성해서 반환해주자.
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
	//메모리 할당(메모리 주소)을 하고 생성자 나중에 불러옴 값은 바뀜
	static void* operator new(size_t size)
	{
		//스레드에서 돌아가게 되면 
		//통신에서는 Overlapped 구조체가 유저마다 있는데
		//메모리 변수 값 갱신 -> 그 값을 확인 ->(캐시)-> 또 확인 하면 중간에 캐시가 낀다.
		//실제 데이터와 캐시데이터는 주소가 다름, 고로 캐시를 못쓰게 바꿔놔야함
		//메모리에 직접가서 들어가있는 값만 확인하겠다
		//volatile 캐쉬 방지가 필요하다. 

		//동기화를 안써도 변수를 안전하게 바꾸는 함수 인터락 C++ 11 아토믹변수
		long long iCurrent = InterlockedIncrement64(&g_iHead)-1;
		long long iPos = iCurrent & POOL_SIZE_MASK;

		void* pObj = InterlockedExchangePointer(&m_ObjectPool[iPos], nullptr);
		//m_ObjectPool[iPos] = nullptr;
		if (pObj != nullptr) return pObj;
		//메모리를 정렬해서 정해진 16의 승수 단위로 말록해라
		//가상메모리가 아닌 물리적 메모리를 정렬한다.
		return _aligned_malloc(size, MEMORY_ALLOCATION_ALIGNMENT);
	}
	static void  operator delete(void* deleteObj)
	{
		long long iCurrent = InterlockedIncrement64(&g_iTail)-1;
		long long iPos = iCurrent & POOL_SIZE_MASK;
		void* pObj = InterlockedExchangePointer(&m_ObjectPool[iPos], deleteObj);
		if (pObj != nullptr)
		{
			//블록단위로 메모리 해제
			_aligned_free(pObj);
		}
	}
};
template<typename type>
void* volatile KObjectPool<type>::m_ObjectPool[POOL_MAX_SIZE] = { nullptr, };
template<typename type>
long long volatile KObjectPool<type>::g_iHead = 0; // 생성 및 할당 인덱스
template<typename type>
long long volatile KObjectPool<type>::g_iTail = 0; // 소멸 및 해제 인덱스