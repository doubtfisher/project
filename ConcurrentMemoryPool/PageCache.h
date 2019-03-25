#pragma once
#include "Common.h"

//PageCacheҲҪ��Ƴɵ���ģʽ

class PageCache
{
public:
 static PageCache* GetInstance()
	{
		return &_inst;
	}

 Span* NewSpan(size_t n);//����һ���µ�span

 PageCache(const PageCache&) = delete;//�������캯������Ϊdelete����
 PageCache& operator=(const PageCache&) = delete;//��ֵ��������Ϊdelete����

private:
	PageCache(){}//���캯��˽�л�
	static PageCache _inst;

};
