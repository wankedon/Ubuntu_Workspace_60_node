#include "pch.h"
#include "HitsOverlap.h"
using namespace std;

HitsOverlap::HitsOverlap(int numOverlap)
:numOverlap(numOverlap),
 overlapCounter(0)
{
}

HitsOverlap::~HitsOverlap() = default;

/**
 * @brief �Դ�������ݽ��д���
 * @in���ϼ������SignalDetectionParam
 * @out�����Ӵ�����SignalDetectionParam
 */
void HitsOverlap::receive(detection::RawData& rawData)
{
	if (numOverlap <= 1)
	{
		noOverlapProcess(rawData);
	}
	else
	{	
		overlapProcess(rawData);
	}
}

void HitsOverlap::noOverlapProcess(detection::RawData& rawData)
{
	normalizeHits(rawData);
	send(rawData);
}

void HitsOverlap::overlapProcess(detection::RawData& rawData)
{
	if (overlapCounter == 0)
	{
		if (overlapData == nullptr)
		{
			overlapData = make_unique<detection::RawData>(rawData);
		}	
		else
		{
			*overlapData = rawData;
		}
	}
	else
	{
		auto hitsSrc = rawData.body().over_threshold_hits();//hitֵ����
		auto hitDst = overlapData->mutable_body()->mutable_over_threshold_hits();
		auto ampSrc = rawData.body().cur_trace();//ampֵȡ���...
		auto ampDst = overlapData->mutable_body()->mutable_cur_trace();
		for (int i = 0; i < hitsSrc.size(); i++)
		{
			(*hitDst)[i] += hitsSrc[i];
			(*ampDst)[i] = std::max((*ampDst)[i], ampSrc[i]);
		}
	}	
	overlapData->mutable_header()->set_sweep_count(rawData.header().sweep_count() + overlapData->header().sweep_count());
	if (++overlapCounter < numOverlap) //û�дﵽ���õ��������Ӵ�����������
		return;
	//���½���ʱ��
	*(overlapData->mutable_header()->mutable_time_span()->mutable_stop_time()) = rawData.header().time_span().stop_time();
	normalizeHits(*overlapData);
	send(*overlapData);
	overlapCounter = 0;
}

void HitsOverlap::normalizeHits(detection::RawData& rawData)
{
	auto sweepCount = rawData.header().sweep_count();
	auto thresholdHits = rawData.mutable_body()->mutable_over_threshold_hits();
	for (auto& hit : *thresholdHits)
	{
		hit /= sweepCount;
	}
}

void HitsOverlap::onParamChange(uint32_t num)
{
	numOverlap = num;
	overlapCounter = 0;
	overlapData.reset();
}