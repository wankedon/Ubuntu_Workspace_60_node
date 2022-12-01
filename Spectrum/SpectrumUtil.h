#pragma once

#include "node/spectrum.pb.h"

enum class SegPosition
{
	OUT_OF_RANGE,
	UNIQUE,
	MULTI_FIRST,
	MULTI_LAST,
	MULTI_MID,
};

using SegmentData = std::vector<float>;			        ///< ×Ó¶ÎÆµÆ×
int frequencyBandPoints(const spectrum::FrequencyBand& band);
spectrum::FrequencySpan bandSpan(const spectrum::FrequencyBand& band);
spectrum::FrequencyRange bandRange(const spectrum::FrequencyBand& band);

double freqStep(const spectrum::FrequencySegment& freqSeg);
std::unique_ptr<int32_t> freqToIndex(const spectrum::FrequencySegment& freqSeg, const double frequency);
std::unique_ptr<double> indexToFreq(const spectrum::FrequencySegment& freqSeg, const int32_t idx);

SegPosition segmentPosInRange(int idx, const spectrum::FrequencyRange& range);
std::unique_ptr<std::pair<int, int>> segmentPartInRange(int32_t segIdx, const spectrum::FrequencyRange& range, const spectrum::FrequencyBand& band);
std::unique_ptr<double> posToFrequency(const spectrum::FrequencyPos pos, const spectrum::FrequencyBand& band);
std::unique_ptr<spectrum::FrequencyPos> frequencyToPos(double frequency, const spectrum::FrequencyBand& band);

std::unique_ptr<spectrum::FrequencyRange> frequencySpanToRange(const spectrum::FrequencySpan& span, const spectrum::FrequencyBand& band);

double intersection(const spectrum::FrequencySpan& spanA, const spectrum::FrequencySpan& spanB);
double unionsection(const spectrum::FrequencySpan& spanA, const spectrum::FrequencySpan& spanB);