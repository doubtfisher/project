#include "Common.h"
#include "ThreadCache.h"
#include "CentralCache.h"


//�����Ļ����ȡ����
void* ThreadCache::FetchFromCentralCache(size_t index, size_t size)
{
	//�����Ļ����ȡ�����Ķ���
	FreeList* freelist = &_FreeList[index];
	size_t maxsize = freelist->MaxSize();
	size_t numtomove =min( Size::NumMoveSize(size),maxsize);
	void* start = nullptr;
	void* end = nullptr;
	size_t batchsize = CentralCache::GetInstance()->FetchRangeObj(start, end, numtomove, size);//������cache�����������ڴ棬������ʵ���õ��Ķ������
	
	if (batchsize > 1)
	{
		freelist->PushRange(NEXT_OBJ(start), end, batchsize - 1);//��ʣ�µ�(batchsize-1)������һص���������
	}

	if (batchsize >= freelist->MaxSize())
	{
		freelist->SetMaxSize(maxsize + 1);
	}

	return start;
}



//�����ڴ����
void* ThreadCache::Allocate(size_t size)
{
	size_t index =Size::Index(size);
	FreeList* freelist = &_FreeList[index];
	if (!freelist->Empty())//�����������Ϊ��
	{
		return freelist->Pop();
	}
	else//�����������Ϊ��
	{
		return FetchFromCentralCache(index, Size::Roundup(size));
	}
}

//�ͷ��ڴ����
void ThreadCache::Deallocate(void* ptr, size_t size)
{
	size_t index = Size::Index(size);
	FreeList* freelist = &_FreeList[index];
	freelist->Push(ptr);

	//��������(�ͷŻ�һ���������ڴ�)���ͷŻ����Ļ���
	if (freelist->Size() >= freelist->MaxSize())
	{
		//ListTooLong(freelist, size);
	}

}

//�ͷŶ���ʱ������������������󽫷Żص����Ļ���
void ThreadCache::ListTooLong(FreeList* list, size_t size)
{
	void* start = list->PopRange();
	CentralCache::GetInstance()->ReleaseListToSpans(start, size);
}