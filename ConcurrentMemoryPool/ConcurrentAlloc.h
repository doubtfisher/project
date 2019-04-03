#pragma once
#include "Common.h"
#include "ThreadCache.h"

inline static void* ConcurrentAlloc(size_t size)//����̲߳��������ڴ�
{
	if (size > MAX_BYTES)
	{
		//��pagecache����
		return malloc(size);
	}
	else//ÿ���߳�����Ҫ���ҵ������Լ���ThreadCache����ʱ�򣬾�������TLS
	{
		if (tlslist == nullptr)
		{
			//cout << std::this_thread::get_id() << endl;//��ȡ�߳�id
			tlslist = new ThreadCache;
			//cout << tlslist << endl;
		}
		//cout << tlslist << endl;


		return tlslist->Allocate(size);
	}
}

inline static void ConcurrentFree(void* ptr, size_t size)//�����ͷ��ڴ�
{
	if (size > MAX_BYTES)
	{
		free(ptr);
	}
	else
	{
		tlslist->Deallocate(ptr,size);
	}
}