#pragma once
#include "SpectrumThreshold.h"
#include "node/spectrum.pb.h"

/*
* 单极点滤波器，用于平滑
*/
class SinglePoleFilter
{
public:
	SinglePoleFilter(float decayRatio); //decayRatio should between (0, 1)
	~SinglePoleFilter();
public:
	void smooth(float& prevY, SegmentData& data);
	void smooth(TraceDataHolder& dataHolder);//smooth inline
private:
	const float a0;
	const float b1;
};

class AdaptiveThreshold : public SpectrumThreshold
{
public:
	AdaptiveThreshold(const detection::AdaptiveThresholdParam& param, const spectrum::FrequencyBand& wholeBand);
	~AdaptiveThreshold() = default;
public:
	void compareTo(const TraceDataHolder& spectrumLine, std::vector<std::vector<uint8_t>>& compareResult) override;
	SegmentData uploadThresholdLine() override;
private:
	struct Slot
	{
		int32_t start;
		int32_t stop;
		std::list<float> historyVal;
	};
	std::vector<Slot> calcSlotPos(size_t segPoints);
	void training(const TraceDataHolder& spectrumLine);
	bool inWarmupStage() const;
private:
	const detection::AdaptiveThresholdParam param;
	std::vector<std::vector<Slot>> slots;
	SinglePoleFilter smoothFilter;
};

