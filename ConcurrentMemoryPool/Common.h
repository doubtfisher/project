#pragma once

#include <iostream>
#include <stdlib.h>
#include <thread>
#include <vector>

using std::cout;
using std::endl;

const size_t MAX_BYTES = 64 * 1024; //thread cache���������ڴ�
const size_t NLISTS = MAX_BYTES/8;//��������������ô�����������


inline static void*& NEXT_OBJ(void* obj)//��ȡ��ǰ�������һ������
{
	return *((void**)obj);
}

class FreeList
{
public:
	void PushRange(void* start, void* end, size_t n)//���������Ķ���
	{
		NEXT_OBJ(end) = _list;
		start = _list;
		_size += n;
	}

	void PopRange()
	{

	}

	void Push(void* obj)//����һ������
	{
		NEXT_OBJ(obj) = _list;
		_list = obj;
		_size++;
	}
	void* Pop()//����һ������
	{
		/*void* next = *((void**)_list);
		void* cur = _list;
		_list = next;*/

		void* obj = _list;
		_list = NEXT_OBJ(obj);
		_size--;

		return obj;//���ر������Ķ���
	}
	bool Empty()
	{
		return _list == nullptr;
	}
private:
	void* _list=nullptr;//����������һ��ָ�룬ָ�����Ķ����
	size_t _size=0;//��¼�������������ж��ٸ�����

};

class Size
{
public:
	inline static size_t Index(size_t size)//����һ��size���㣬���size������������ĸ�λ��
	{	
		return _Index(size, 3);
	}

	inline static size_t Roundup(size_t size)//���뵽8�ֽڵ�������
	{
		return _Roundup(size, 3);//����֮����Ҫ��װ������������Ϊ���ǵ�������ԽС���ڴ���Ƭ����Խ��
	}
private:
	inline static size_t _Index(size_t size, size_t align)//����һ��size���㣬���size������������ĸ�λ��
	{
		//9-16(+7��)  --->16-23
		//return ((size+7)>>3)-1;
		size_t _align = 1 << align;
		return ((size + _align - 1) >> align) - 1;
	}

	inline static size_t _Roundup(size_t size, size_t align)//���뵽8�ֽڵ�������
	{

		//return (size+7)&~7;
		size_t _align = 1 << align;
		return (size + _align - 1)&~_align - 1;//size+7�ٰ�λ����7ȡ�����պ�ÿ�ζ��ѵ���λ��Ϊ0���Ϳ��Դﵽÿ�ζ����뵽8����������
	}
};

typedef size_t PageID;

struct Span//���еģ������˷��ʵ�һ��Ͳ�����Ƴ�class��
{
	PageID _pageid;//ҳ��
	size_t _npage;//ҳ��
	
	Span* _next = nullptr;
	Span* _prev = nullptr;

	void* _list = nullptr;//���Ӷ������������(_listָ���һ������)
	size_t objsize = 0;//һ������Ĵ�С

	size_t usecount = 0;//�����ʹ�ü���

};

//˫���ͷѭ����span����
class SpanList
{
public:
	SpanList()//���캯��
	{
		_head = new Span;
		_head->_next = _head;
		_head->_prev = _head;
	}

	Span* Begin()
	{
		return _head->_next;
	}
	Span* End()
	{
		return _head;
	}

	void PushBack(Span* newspan)
	{
		Insert(End(), newspan);
	}

	void PushFront(Span* newspan)
	{
		Insert(Begin(), newspan);
	}

	void PopBack()
	{
		Erase(_head->_prev);
	}

	void PopFront()
	{
		Erase(_head->_next);
	}

	void Insert(Span* cur, Span* newspan)//��cur��ǰ�����һ��newspan
	{
		//prev newspan cur
		Span* prev = cur->_prev;

		newspan->_prev = prev;
		prev->_next = newspan;
		newspan->_next = cur;
	}

	void Erase(Span* cur)
	{
		//prev cur  next
		Span* prev = cur->_prev;
		Span* next = cur->_next;

		prev->_next = next;
		next->_prev = prev;
	}

	~SpanList()
	{
		Span* cur = _head->_next;
		while (cur != _head)
		{
			Span* next = cur->_next;
			delete cur;
			cur = next;
		}
		delete _head;
		_head = nullptr;
	}

	SpanList(const SpanList&) = delete;//û�п��������󣬵��ǲ�д�����������Զ�����ǳ���������Ծ�ֱ�Ӱѿ�����������Ϊdelete����
	SpanList& operator=(const SpanList&) = delete;//��ֵҲһ��

private:
	Span* _head;
};
