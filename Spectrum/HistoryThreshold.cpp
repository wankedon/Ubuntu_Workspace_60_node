#include "pch.h"
#include "HistoryThreshold.h"

HistoryThresholdBase::HistoryThresholdBase(const spectrum::FrequencyBand& wholeBand)
	:SpectrumThreshold(wholeBand, detection::ThresholdType::HISTORY),
	band(wholeBand),
	uploaded(false),
	loadedCount(0)
{

}

HistoryThresholdBase::~HistoryThresholdBase()
{

}

SegmentData HistoryThresholdBase::uploadThresholdLine()
{
	if (uploaded)
		return SegmentData(0);
	uploaded = true;
	return thresholdLine.integrate();
}