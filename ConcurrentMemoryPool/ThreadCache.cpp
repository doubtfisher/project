#include "ThreadCache.h"
#include "CentralCache.h"

//�����Ļ����ȡ����
void* ThreadCache::FetchFromCentralCache(size_t index, size_t size)
{
	//�����Ļ����ȡ�����Ķ���
	size_t numtomove = 5;
	void* start = nullptr;
	void* end = nullptr;
	size_t batchsize = CentralCache::GetInstance()->FetchRangeObj(start, end, numtomove, size);//������cache�����������ڴ棬������ʵ���õ��Ķ������
	
	if (batchsize > 1)
	{
		FreeList* freelist = &_FreeList[index];
		freelist->PushRange(NEXT_OBJ(start), end, batchsize - 1);//��ʣ�µ�(batchsize-1)������һص���������
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

	//�����������ͷŻ����Ļ���


}

//�ͷŶ���ʱ������������������󽫷Żص����Ļ���
void ThreadCache::ListTooLong(FreeList* list, size_t size)
{

}