/**
 * @file SweepZoomRange.h
 * @brief Ƶ���������
 * @author װ����ҵ������� ���� 
 * @version 0.1
 * @date 2020-12-09
 * 
 * @copyright Copyright (c) 2020  �й����ӿƼ����Ź�˾����ʮһ�о���
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
	const spectrum::FrequencyBand band_;					///< �β���
	const int32_t totalRawPoints_;						///< ����Ƶ�ε�ԭʼ����
	const spectrum::FrequencySpan wholeSpan_;
	const spectrum::FrequencyRange wholeRange_;
	spectrum::FrequencySpan curSpan_;					///< ��ǰƵ�ʷ�Χ
	spectrum::FrequencyRange curRange_;					///< ��ǰƵ�㷶Χ
	int32_t curRawPoints_;								///< ��ǰԭʼ����
};

