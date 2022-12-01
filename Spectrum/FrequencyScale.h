/**
 * @file SweepZoomRange.h
 * @brief 频谱区间管理
 * @author 装备事业部软件组 杨青 
 * @version 0.1
 * @date 2020-12-09
 * 
 * @copyright Copyright (c) 2020  中国电子科技集团公司第四十一研究所
 * 
 */
#pragma once

#include <functional>
#include "node/spectrum.pb.h"

using namespace spectrum;

class FrequencyScale
{
public:
	FrequencyScale(const FrequencyBand& whole);
	~FrequencyScale(void);
public:
	void onSpanChange(const FrequencySpan& span);
	bool isLastSegment(int32_t segIdx) const;
	int32_t sweepPoints() const{return curRawPoints_;}
	int32_t totalPoints() const { return totalRawPoints_; }
	FrequencyRange currentRange() const { return curRange_; }
	FrequencyRange wholeRange() const { return wholeRange_; }
	std::unique_ptr<FrequencyRange> calcRange(const FrequencySpan& span) const;
	FrequencySpan currentSpan() const{return curSpan_;}
	FrequencySpan wholeSpan() const { return wholeSpan_; }
	FrequencyBand wholeBand() const{return band_;}
	FrequencySpan adjustSpan(const FrequencySpan& range);
	double freqStep() const;
private:
	void resetCurrent(const FrequencySpan& range);
private:
	const spectrum::FrequencyBand band_;					///< 段参数
	const int32_t totalRawPoints_;						///< 整个频段的原始点数
	const spectrum::FrequencySpan wholeSpan_;
	const spectrum::FrequencyRange wholeRange_;
	spectrum::FrequencySpan curSpan_;					///< 当前频率范围
	spectrum::FrequencyRange curRange_;					///< 当前频点范围
	int32_t curRawPoints_;								///< 当前原始点数
};

