#include "pch.h"
#include "DetectionDataBuffer.h"

using namespace std;

DetectionDataBuffer::DetectionDataBuffer()
:runFlag(false)
{
	processFuture = async([this] {dispatchLoop(); });
}

DetectionDataBuffer::~DetectionDataBuffer()
{
	runFlag = false;
	if (processFuture.valid())
		processFuture.get();
}

void DetectionDataBuffer::receive(detection::RawData& data)
{
	push(data);
}

void DetectionDataBuffer::push(detection::RawData& data)
{
	lock_guard<mutex> lg(mtx);
	bufferList.push_back(data);
}

void DetectionDataBuffer::dispatchLoop()
{
	runFlag = true;
	while (runFlag)
	{
		bool executed = false;
		{
			lock_guard<mutex> lg(mtx); //此处若处理时间长，会一直处于“锁”的状态，影响新数据压入。
			if (!bufferList.empty())
			{
				send(bufferList.front());
				bufferList.pop_front();
				executed = true;
			}
		}
		if (!executed)
		{
			std::this_thread::sleep_for(chrono::milliseconds(10)); //让出时间片
		}
	}
}