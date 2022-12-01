#include "pch.h"
#include "HitsSegmentationFilter.h"

HitsSegmentationFilter::HitsSegmentationFilter(const detection::HitsSegmentation& param)
	:parameter(param),
	curInput(nullptr)
{
}

HitsSegmentationFilter::~HitsSegmentationFilter() = default;

void HitsSegmentationFilter::filter(detection::RawData& input)
{
	curInput = &input;
	excludeHits();	//step1. 根据用户设置的命中区间把不符合的hits置零
	eraseGap();		//step2. 抹去连续性很短的零值hits区间
	segmentation();	//step3. 根据连续hits分段
}

bool HitsSegmentationFilter::hitInRange(float hit, const DRange& range)
{
	return hit >= range.start() && hit <= range.stop();
}

void HitsSegmentationFilter::excludeHits()
{
	auto ranges = parameter.hits_prob();
	auto thresholdHits = curInput->mutable_body()->mutable_over_threshold_hits();
	for (auto& hit : *thresholdHits)
	{
		if (hit > 0)
		{
			bool setToZero = true;
			for (auto& range : ranges)
			{
				if (hitInRange(hit, range))
				{
					setToZero = false;
					break;
				}
			}
			if (setToZero) hit = 0;
		}
	}
}

int HitsSegmentationFilter::spanToPoints(double span)
{
	double step = (curInput->body().freq_span().stop_freq() - curInput->body().freq_span().start_freq()) / (curInput->body().cur_trace_size() - 1);
	return static_cast<int>(ceil(span / step));
}

void HitsSegmentationFilter::segmentation()
{
	auto biggerThanZero = [](float val) {return val > 0; };
	auto isZero = [](float val) {return val == 0; };
	const auto begin = curInput->body().over_threshold_hits().begin();
	const auto end = curInput->body().over_threshold_hits().end();
	auto iter = begin;
	int minContinal = 3;
	int maxContinal = curInput->body().cur_trace_size();
	if (parameter.has_bandwidth_range())
	{
		minContinal = std::max(minContinal, spanToPoints(parameter.bandwidth_range().start()));
		maxContinal = spanToPoints(parameter.bandwidth_range().stop());
	}
	do
	{
		if (!findMatch(iter, end, biggerThanZero))
			break;
		auto segStart = iter;
		if (!findMatch(++iter, end, isZero))
			break;
		iter++;//闭开区间
		auto diff = iter - segStart;
		if (diff > minContinal && diff <= maxContinal)
		{
			auto* pos = curInput->add_signal_segments();
			pos->set_start(segStart - begin);
			pos->set_stop(iter - begin);
		}
	} while (true);
}

void HitsSegmentationFilter::eraseGap()
{
	if (!parameter.has_bandwidth_range())
		return;
	int maxGapPoints = std::max(2, spanToPoints(parameter.bandwidth_range().start() / 2));
	auto isZero = [](float val) {return val == 0; };
	auto biggerThanZero = [](float val) {return val > 0; };
	const auto origin = curInput->mutable_body()->mutable_over_threshold_hits()->begin();
	const auto end = curInput->mutable_body()->mutable_over_threshold_hits()->end();
	auto iter = origin;
	do
	{
		if (!findMatch(iter, end, isZero))
			break;
		auto gapStart = iter;
		if (!findMatch(++iter, end, biggerThanZero))
			break;
		if (iter - gapStart <= maxGapPoints)	//把该区间内所有值设为非零
		{
			for (auto innerIter = gapStart; innerIter <= iter; ++innerIter)
			{
				*innerIter = *iter;
			}
		}
		++iter;
	} while (true);
}

void HitsSegmentationFilter::onParamChange(const detection::SegmentationParam& params)
{
	if (params.enable_filters() & detection::SegmentationFilterType::HITS)
	{
		if (params.has_hits_param())
		{
			parameter = params.hits_param();
		}
	}
}


/*void HitsQualityFilter::segmentationByContinuousHits(detection::RawData& input)
{
	auto startIndex = 0;
	auto stopIndex = 0;
	auto hits = input.body().over_threshold_hits();
	auto numMinContinal = parameter.num_min_continal();
	for (int i = 0; i < hits.size(); i++)
	{
		if (hits[i] > 0)
		{
			stopIndex = i;
			if (stopIndex - startIndex >= numMinContinal)
			{
				auto* pos = input.add_signal_segments();
				pos->set_start(startIndex);
				pos->set_stop(stopIndex - 1);
				stopIndex = startIndex = ++i;
				for (int j = startIndex; j < hits.size(); j++)
				{
					if (hits[j])
					{
						i = j - 1;
						break;
					}
					stopIndex = startIndex = ++i;
				}
			}
			else
				startIndex = stopIndex = i + 1;
		}
		else
		{
			stopIndex++;
			if (i + 1 == hits.size())
			{
				if (stopIndex - startIndex >= numMinContinal)
				{
					auto* pos = input.add_signal_segments();
					pos->set_start(startIndex);
					pos->set_stop(stopIndex - 1);
				}
			}
		}

	}
}*/
/**
 * @brief 对已经获得到得符合持续个数的hit索引集合再根据黏连个数设置更新有效hit的索引值。
 */
/*void HitsQualityFilter::eraseGap(detection::RawData& input)
{
	int32_t thisStop = 0;
	int32_t nextStart = 0;
	auto hits = input.body().over_threshold_hits();
	auto numMaxGap= parameter.num_max_gap();
	for (int i = 0; i < input.signal_segments().size(); i++)
	{
		thisStop = input.signal_segments().at(i).stop();
		if (i + 1 < input.signal_segments().size())
		{
			nextStart = input.signal_segments().at(i + 1).start();
			if ((nextStart > thisStop) && (nextStart - thisStop <= numMaxGap))
			{
				auto value = input.signal_segments().at(i).start();
				input.mutable_signal_segments()->at(i + 1).set_start(value);
				input.mutable_signal_segments()->at(i).set_start(-1);//说明此值已被下一个值包含；将其起始值置-1。
				for (int j = thisStop + 1; j < nextStart; j++)
				{
					if (hits[j] == 0)
					{
						input.mutable_body()->mutable_over_threshold_hits()->at(j) = input.body().over_threshold_hits().at(j - 1);
					}

				}
			}
		}
	}
}*/

