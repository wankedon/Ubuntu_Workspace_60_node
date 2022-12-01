#pragma once

#include "SpectrumUtil.h"

class FrequencyScale;
class TraceDataHolder
{
	using DataHoldFunc = std::function<void(SegmentData&, const float*, size_t)>;
public:
	TraceDataHolder(DataHoldFunc dataHoldFunc);
	TraceDataHolder(const spectrum::FrequencyBand& band, float level);
	~TraceDataHolder();
public:
	static std::unique_ptr<TraceDataHolder> makeTraceDataHolder(spectrum::DataHoldType dataHoldType);
public:
	void clear();
	void input(int32_t segIdx, const float* data, size_t count);
	void offset(float offsetVal);
	SegmentData integrate() const;
	SegmentData getZoominPart(const FrequencyScale& scale) const;
	std::shared_ptr<SegmentData> getSegmentData(int32_t segIdx) const;
	size_t segmentCount() const {return segmentsData.size();}
	void operator+=(const TraceDataHolder& src);
	void operator+=(const std::vector<char>& src);
private:
	void initSegData(int32_t segIdx, const float* data, size_t count);
	void updateSegData(int32_t segIdx, const float* data, size_t count);
private:
	std::vector<std::shared_ptr<SegmentData>> segmentsData;		///< 原始数据
	DataHoldFunc dataHoldFunc;		///< 数据保持算法
};

