#pragma once
#include "Common.h"



//CentralCacheҪ���Ϊ����ģʽ
class CentralCache
{
public:
	static CentralCache* GetInstance()//��ȡһ��ʵ������
	{
		return &_inst;
	}
	//��page cache��ȡһ��span
	Span* GetOneSpan(SpanList& spanlist, size_t byte_size);

	//�����Ļ����ȡһ�������Ķ��󷵻ظ�thread cache
	size_t FetchRangeObj(void*& start, void*& end, size_t n, size_t byte_size);

	//��һ�������Ķ����ͷŵ�span���
	void ReleaseListToSpans(void* start, size_t size);



	CentralCache(const CentralCache&) = delete;//��ֹ����
	CentralCache& operator=(const CentralCache&) = delete;//����ֵ

private:
	SpanList _spanlist[NLISTS];

	CentralCache(){}//��������˽�л�

	//����ģʽ�ڵ�һ�ε���ʱ�Ŵ���������̵߳�һ�ε���ʱ����Ҫ������һ����Ч�ʾͻή�ͣ����Բ�����ʹ������ģʽ
	static CentralCache _inst;//����д���Ƕ���ģʽ����������̰߳�ȫ�ģ�����main����֮ǰ�ʹ��������߳�����main����֮��Ŵ����ģ������Ļ����Ͳ��ü����ˣ�Ҳû��˫�ؼ��
};
