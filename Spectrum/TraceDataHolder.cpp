#include "pch.h"
#include "TraceDataHolder.h"
#include "SpectrumThreshold.h"
#include "FrequencyScale.h"

using namespace std;

static void pickBigOne(SegmentData& old, const float* data, size_t count)
{
	if (old.size() != count)
		return;
	for (int i = 0; i < count; i++)
	{
		old[i] = (data[i] > old[i]) ? data[i] : old[i];
	}
}

static void pickSmallOne(SegmentData& old, const float* data, size_t count)
{
	if (old.size() != count)
		return;
	for (int i = 0; i < count; i++)
	{
		old[i] = (data[i] < old[i]) ? data[i] : old[i];
	}
}

static std::function<void(SegmentData& old, const float* data, size_t count)> autoPickFunc(spectrum::DataHoldType dhType)
{
	if (dhType == spectrum::MAX_HOLD)
	{
		return pickBigOne;
	}
	else if (dhType == spectrum::MIN_HOLD)
	{
		return pickSmallOne;
	}
	else
	{
		return nullptr;
	}
}

std::unique_ptr<TraceDataHolder> TraceDataHolder::makeTraceDataHolder(spectrum::DataHoldType dataHoldType)
{
	return make_unique<TraceDataHolder>(autoPickFunc(dataHoldType));
}

TraceDataHolder::TraceDataHolder(DataHoldFunc func)
	:dataHoldFunc(func)
{

}

TraceDataHolder::TraceDataHolder(const spectrum::FrequencyBand& band, float level)
{
	for (auto& seg : band.segments())
	{
		segmentsData.emplace_back(make_shared<SegmentData>(seg.points(), level));
	}
}

TraceDataHolder::~TraceDataHolder()
{

}

void TraceDataHolder::clear()
{
	segmentsData.clear();
}

void TraceDataHolder::input(int32_t segIdx, const float* data, size_t count)
{
	if (segIdx >= segmentsData.size())
	{
		initSegData(segIdx, data, count);
	}
	else
	{
		updateSegData(segIdx, data, count);
	}
}

void TraceDataHolder::offset(float offsetVal)
{
	for (auto& segData : segmentsData)
	{
		if (segData != nullptr)
		{
			for (auto& val : *segData)
			{
				val += offsetVal;
			}
		}
	}
}

SegmentData TraceDataHolder::integrate() const
{
	SegmentData result;
	for (auto& seg : segmentsData)
	{
		result.insert(result.end(), seg->begin(), seg->end());
	}
	return result;
}

SegmentData TraceDataHolder::getZoominPart(const FrequencyScale& scale) const
{
	auto sweepRange = scale.currentRange();
	if (sweepRange.start_pos().segment_idx() == sweepRange.stop_pos().segment_idx())	//就一段
	{
		auto seg = segmentsData[sweepRange.start_pos().segment_idx()];
		return SegmentData{
			seg->begin() + sweepRange.start_pos().seg_internal_offset(),
			seg->begin() + sweepRange.stop_pos().seg_internal_offset() + 1,
		};
	}
	else
	{
		auto firstSeg = segmentsData[sweepRange.start_pos().segment_idx()];
		SegmentData jointResult{
			firstSeg->begin() + sweepRange.start_pos().seg_internal_offset(),
			firstSeg->end()
		};
		for (auto i = sweepRange.start_pos().segment_idx() + 1; i < sweepRange.stop_pos().segment_idx(); i++)//中间段
		{
			jointResult.insert(jointResult.end(), segmentsData[i]->begin(), segmentsData[i]->end());
		}
		auto& lastSeg = segmentsData[sweepRange.stop_pos().segment_idx()];//最后一段
		jointResult.insert(jointResult.end(), lastSeg->begin(), lastSeg->begin() + sweepRange.stop_pos().seg_internal_offset() + 1);
		return jointResult;
	}
}

void TraceDataHolder::initSegData(int32_t segIdx, const float* data, size_t count)
{
	assert(segIdx == segmentsData.size());
	segmentsData.emplace_back(make_shared<SegmentData>(data, data + count));
}

shared_ptr<SegmentData> TraceDataHolder::getSegmentData(int32_t segIdx) const
{
	if (segIdx >= segmentsData.size())
		return nullptr;
	return segmentsData[segIdx];
}

void TraceDataHolder::updateSegData(int32_t segIdx, const float* data, size_t count)
{
	auto curSeg = getSegmentData(segIdx);
	if (curSeg->size() != count)
		return;
	if (dataHoldFunc)
	{
		dataHoldFunc(*segmentsData[segIdx], data, count);
	}
	else
	{
		std::copy(data, data + count, curSeg->begin());
	}
}

void TraceDataHolder::operator+=(const TraceDataHolder& src)
{
	for (int i = 0; i < src.segmentCount(); i++)
	{
		const auto& srcSeg = src.getSegmentData(i);
		input(i, srcSeg->data(), srcSeg->size());
	}
}

void TraceDataHolder::operator+=(const std::vector<char>& src)
{
	const float* ptr = (const float*)src.data();
	for (int i = 0; i < segmentCount(); i++)
	{
		auto segData = getSegmentData(i);
		updateSegData(i, ptr, segData->size());
		ptr += segData->size();
	}
}