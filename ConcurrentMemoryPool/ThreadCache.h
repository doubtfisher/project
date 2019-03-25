#pragma once
#include "Common.h"

class ThreadCache
{
public:
	//�����ڴ����
	void* Allocate(size_t size);

	//�ͷ��ڴ����
	void Deallocate(void* ptr, size_t size);

	//�����Ļ����ȡ����
	void* FetchFromCentralCache(size_t index, size_t size);

	//�ͷŶ���ʱ���������������Ͷ��󽫷Żص����Ļ���
	void ListTooLong(FreeList* list, size_t size);

private:
	FreeList _FreeList[NLISTS];//��������
	
};
//TLS
_declspec (thread) static ThreadCache* tlslist = nullptr;//��������Ϊ��̬�ģ���Ϊ��ֻ�ڵ�ǰ�ļ��ɼ�