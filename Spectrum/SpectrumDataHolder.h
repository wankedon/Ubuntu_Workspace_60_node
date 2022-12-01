/**
 * @file SpectrumDataHolder.h
 * @brief 频谱数据保持
 * @author 装备事业部软件组 杨青
 * @version 0.1
 * @date 2021-9-30
 *
 * @copyright Copyright (c) 2021  中国电子科技集团公司第四十一研究所
 * 
 */
#pragma once

#include "SpectrumUtil.h"
#include "node/spectrum.pb.h"
#include "StreamConnect.h"

class TraceDataHolder;
class FrequencyScale;
class SpectrumDetector;
/**
 * @brief 频谱数据持有
 * 
 */
class SpectrumDataHolder : public StreamOut<TraceDataHolder>
{
public:
	SpectrumDataHolder(spectrum::DataHoldType dataHoldType);
	~SpectrumDataHolder();
public:
	void forwardData();	//转发自身数据
	void clear();
	void inputSegData(int32_t segIdx, const float* data, size_t count);
	void loadSweepData(const spectrum::FrequencyBand& band, const std::vector<char>& sweepData);
	size_t segmentCount() const;
	std::shared_ptr<SegmentData> getSegmentData(int32_t segIdx) const;
	SegmentData getPanorama(size_t expectedOutput);	//获得全景
	SegmentData getCloseshot(size_t expectedOutput, const FrequencyScale& scale);	//获得近景
private:
	const spectrum::DataHoldType DH_TYPE;
	std::unique_ptr<SpectrumDetector> panoramaDetector;
	std::unique_ptr<SpectrumDetector> zoomInDetector;
	std::unique_ptr<TraceDataHolder> dataHolder;
};