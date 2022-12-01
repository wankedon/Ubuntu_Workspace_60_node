#include "pch.h"
#include "DataFilter.h"

DataFilter::DataFilter() = default;

DataFilter::~DataFilter() = default;

void DataFilter::receive(detection::RawData& input)
{
	filter(input); //各个过滤器的过程实现。
	send(input);
}
