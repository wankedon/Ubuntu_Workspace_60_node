
#include "pch.h"
#include "AdaptiveThreshold.h"
#include "boostUtil.h"

SinglePoleFilter::SinglePoleFilter(float decayRatio)
	:a0(1-decayRatio),
	b1(decayRatio)
{

}

SinglePoleFilter::~SinglePoleFilter()
{

}

void SinglePoleFilter::smooth(float& prevY, SegmentData& data)
{
	for (int i = 1; i < data.size(); ++i)
	{
		data[i] = data[i] * a0 + prevY * b1;
		prevY = data[i];
	}
}

void SinglePoleFilter::smooth(TraceDataHolder& dataHolder)
{
	float prevY = dataHolder.getSegmentData(0)->at(0);
	for (int i = 0; i < dataHolder.segmentCount(); ++i)
	{
		smooth(prevY, *dataHolder.getSegmentData(i));
	}
}

AdaptiveThreshold::AdaptiveThreshold(const detection::AdaptiveThresholdParam& param, const spectrum::FrequencyBand& wholeBand)
	:SpectrumThreshold(wholeBand, detection::ThresholdType::ADAPTIVE),
	param(param),
	smoothFilter(exp(-5.0 / wholeBand.segments().Get(0).points()))
{
	for (auto& seg : wholeBand.segments())
	{
		slots.emplace_back(calcSlotPos(seg.points()));	//计算每频段的直方图统计区间
	}
}

void AdaptiveThreshold::compareTo(const TraceDataHolder& spectrumLine, std::vector<std::vector<uint8_t>>& compareResult)
{
	training(spectrumLine);	//训练门限
	if (!inWarmupStage())
	{
		smoothFilter.smooth(thresholdLine);	//平滑
		SpectrumThreshold::compareTo(spectrumLine, compareResult);	//当前值与门限比较
	}
}

std::vector<AdaptiveThreshold::Slot> AdaptiveThreshold::calcSlotPos(size_t segPoints)
{
	static const int MIN_POINTS_IN_SLOT = 64;
	static const int32_t SLOT_COUNT[] = {8, 16, 4};//不同模式下频段分割统计的slot个数，标准8个，激进16个，快速4个
	auto slotCount = SLOT_COUNT[param.partition_mode()];
	if (segPoints / slotCount < MIN_POINTS_IN_SLOT)
	{
		slotCount = (segPoints + MIN_POINTS_IN_SLOT - 1) / MIN_POINTS_IN_SLOT;
	}
	std::vector<Slot> slotPos(slotCount);
	const double realSlotWidth = static_cast<double>(segPoints) / slotCount;
	int32_t startPos = 0;
	for (int i = 0; i < slotPos.size(); i ++)
	{
		slotPos[i].start = startPos;
		slotPos[i].stop = static_cast<int>((i + 1) * realSlotWidth + 0.5);
		startPos = slotPos[i].stop;
	}
	slotPos.back().stop = segPoints;
	return slotPos;
}

void AdaptiveThreshold::training(const TraceDataHolder& spectrumLine)
{
	for (int i = 0; i < spectrumLine.segmentCount(); i++)
	{
		auto spectrumDataPtr = spectrumLine.getSegmentData(i);
		auto thresholdDataPtr = thresholdLine.getSegmentData(i);
		auto& segSlotPos = slots[i];
		for (auto& slot : segSlotPos)
		{
			float baseVal = calcBaselineLevel(10, spectrumDataPtr->begin() + slot.start, spectrumDataPtr->begin() + slot.stop);
			baseVal += param.offset();
			if (param.average_count() == 0)
			{
				for (auto iter = thresholdDataPtr->begin() + slot.start; iter != thresholdDataPtr->begin() + slot.stop; iter++)
				{
					*iter = baseVal;
				}
			}
			else
			{
				if (slot.historyVal.size() >= param.average_count())
				{
					slot.historyVal.pop_front();
				}
				slot.historyVal.push_back(baseVal);
				if (slot.historyVal.size() == param.average_count())
				{
					float sum = 0;
					for (auto val : slot.historyVal)
					{
						sum += val;
					}
					sum /= param.average_count();
					for (auto iter = thresholdDataPtr->begin() + slot.start; iter != thresholdDataPtr->begin() + slot.stop; iter++)
					{
						*iter = sum;
					}
				}
			}
		}
	}
}

bool AdaptiveThreshold::inWarmupStage() const
{
	if (param.average_count() != 0)
	{
		if (slots.front().front().historyVal.size() < param.average_count())
			return true;
	}
	return false;
}

SegmentData AdaptiveThreshold::uploadThresholdLine()
{
	if (inWarmupStage())
	{
		return SegmentData(0);
	}
	else
	{
		return thresholdLine.integrate();
	}
}