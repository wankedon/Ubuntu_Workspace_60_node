#include "pch.h"
#include "DataFilter.h"

DataFilter::DataFilter() = default;

DataFilter::~DataFilter() = default;

void DataFilter::receive(detection::RawData& input)
{
	filter(input); //�����������Ĺ���ʵ�֡�
	send(input);
}
