#include "CentralCache.h"
#include "PageCache.h"


CentralCache CentralCache::_inst;//�����ⶨ��

//��PageCache��ȡһ��span
Span* CentralCache::GetOneSpan(SpanList& spanlist, size_t byte_size)
{
	Span* span = spanlist.Begin();
	while (span != spanlist.End())
	{
		if (span->_list != nullptr)//�ж�һ��span�Ƿ�Ϊ��
		{
			return span;
		}
		else
		{
			span = span->_next;
		}			
	}

	//�ߵ������ʾspan�ǿյ�
	Span* newspan = PageCache::GetInstance()->NewSpan(1);
	//Span* newspan = new Span;
	//newspan->objsize = 16;
	//void* ptr = malloc(16 * 8);
	//void* cur = ptr;
	////�зֶ���
	//for (size_t i = 0; i < 7; i++)
	//{
	//	//���еĶ������������
	//	void* next = (char*)cur + 16;
	//	NEXT_OBJ(cur) = next;
	//	cur = next;
	//}

	//NEXT_OBJ(cur) = nullptr;
	//newspan->_list = ptr;

	//��span���뵽��ǰ���������
	spanlist.PushFront(newspan);

	return newspan;

}

//�����Ļ����ȡһ�������Ķ��󷵻ظ�thread cache
size_t CentralCache::FetchRangeObj(void*& start, void*& end, size_t n, size_t byte_size)
{
	//����

	size_t index = Size::Index(byte_size);//����������С��Ӧ��һ���spanlist
	SpanList& spanlist = _spanlist[index];
	Span* span = GetOneSpan(spanlist, byte_size);

	//��span�л�ȡһ����Χ�ڵĶ���
	size_t batchsize = 0;//ͳ�ƻ�ȡ����ĸ���
	void* prev = nullptr;
	void*cur = span->_list;
	for (size_t i = 0; i < n; i++)
	{
		prev = cur;
		cur = NEXT_OBJ(cur);
		batchsize++;
		if (cur == nullptr)//�����Ҫ��ȡ�Ķ������n����ʵ���еĶ��󣬾�Ҫ�жϣ��Է�Խ��
			break;
	}
	start = span->_list;
	end = prev;
	span->_list = cur;//���߶������span��_listָ���µĵ�һ������
	span->usecount += batchsize;
	
	if (span->_list == nullptr)//���spanΪ�գ��Ͱ���β�嵽��󣬱��ַǿյ�span��ǰ��
	{
		spanlist.Erase(span);//ֻ���Ƴ������ǳ���ɾ��
		spanlist.PushBack(span);
	}

	return batchsize;

}