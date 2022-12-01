#pragma once

#include "SpectrumThreshold.h"

class HistoryThresholdBase : public SpectrumThreshold
{
public:
	HistoryThresholdBase(const spectrum::FrequencyBand& wholeBand);
	virtual ~HistoryThresholdBase();
public:
	SegmentData uploadThresholdLine() override;
	int loadedDataCount() const { return loadedCount; }
	virtual bool loadHistory(const detection::HistoryThresholdParam& param) = 0;
protected:
	const spectrum::FrequencyBand& getBand() const { return band; }
	void updateLoadedCount(int count) { loadedCount = count; }
private:
	const spectrum::FrequencyBand band;
	bool uploaded;
	int loadedCount;
};