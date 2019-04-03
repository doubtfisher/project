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

 //��ȡ�Ӷ���span��ӳ��
 Span* MapObjectToSpan(void* obj);

 //�ѿ���span�ͷŻ�PageCache,�������ڵ�span���кϲ�
 void ReleaseSpanToPageCache(Span* span);

 PageCache(const PageCache&) = delete;//�������캯������Ϊdelete����
 PageCache& operator=(const PageCache&) = delete;//��ֵ��������Ϊdelete����

private:
	PageCache(){}//���캯��˽�л�
	static PageCache _inst;
private:
	SpanList _spanlist[NPAGES];
	std::map<PageID, Span*> _idspanmap;
};
