#include "pch.h"
#include "SpectrumThreshold.h"
#include "SpectrumUtil.h"
#include "AdaptiveThreshold.h"
#include "Logger.h"
#include "FrequencyScale.h"

using namespace std;
SpectrumThreshold::SpectrumThreshold(const spectrum::FrequencyBand& wholeBand, detection::ThresholdType t)
	:type(t),
	thresholdLine(wholeBand, 0.0f)
{
}

SpectrumThreshold::~SpectrumThreshold()
{
}

void SpectrumThreshold::compareTo(const TraceDataHolder& spectrumLine, std::vector<vector<uint8_t>>& compareResult)
{
	if (spectrumLine.segmentCount() != thresholdLine.segmentCount())
		return;
	for (auto i = 0; i < spectrumLine.segmentCount(); i++)
	{
		auto spectrumData = spectrumLine.getSegmentData(i);
		auto thresholdData = thresholdLine.getSegmentData(i);
		if (spectrumData->size() == thresholdData->size())
		{
			float* compareRef = thresholdData->data();
			float* compareSrc = spectrumData->data();
			uint8_t* compareDst = compareResult[i].data();
			for (auto j = 0; j < spectrumData->size(); j++)
			{
				*compareDst += (*compareSrc > *compareRef);
				++compareDst;
				++compareRef;
				++compareSrc;
			}
		}

	}
}

detection::ThresholdType SpectrumThreshold::getType() const
{
	return type;
}

UserDefThreshold::UserDefThreshold(const std::vector<detection::ThresholdSector>& sectors, const spectrum::FrequencyBand& wholeBand)
	:SpectrumThreshold(wholeBand, detection::ThresholdType::USER_DEFINE),
	uploaded(false)
{
	thresholdLine.offset(100);
	for (auto& sector : sectors)
	{
		setThresholdLevel(sector, wholeBand);
	}
}

UserDefThreshold::~UserDefThreshold() = default;

void UserDefThreshold::setThresholdLevel(const detection::ThresholdSector& sector, const spectrum::FrequencyBand& wholeBand)
{
	auto bandRange = bandSpan(wholeBand);
	auto sectorCopy = sector;
	if (sectorCopy.freq_span().start_freq() < bandRange.start_freq())
	{
		sectorCopy.mutable_freq_span()->set_start_freq(bandRange.start_freq());
	}
	if (sectorCopy.freq_span().stop_freq() > bandRange.stop_freq())
	{
		sectorCopy.mutable_freq_span()->set_stop_freq(bandRange.stop_freq());
	}
	auto range = frequencySpanToRange(sectorCopy.freq_span(), wholeBand);
	if (range)
	{
		for (auto i = 0; i < thresholdLine.segmentCount(); i++)
		{
			auto segmentPart = segmentPartInRange(i, *range, wholeBand);
			if (segmentPart)
			{
				auto segData = thresholdLine.getSegmentData(i);
				for (int j = segmentPart->first; j <= segmentPart->second; j++)
				{
					segData->at(j) = sectorCopy.level();
				}
			}
		}
	}
}

SegmentData UserDefThreshold::uploadThresholdLine()
{
	if (uploaded)
		return SegmentData(0);
	uploaded = true;
	return thresholdLine.integrate();
}

#include "SpectrumDetector.h"
#include "SpectrumDataHolder.h"
#include "HistoryThreshold.h"

ThresholdManager::ThresholdManager(const spectrum::FrequencyBand& wb, HTCreateFunctor htCreator)
	:wholeBand(wb),
	 compareCounter(0),
	 historyThresholdCreator(htCreator)
{

}

ThresholdManager::~ThresholdManager() = default;

void ThresholdManager::receive(const TraceDataHolder& data)
{
	if (thresholds.empty())
		return;
	compare(data);
}

void ThresholdManager::compare(const TraceDataHolder& data)
{
	std::vector<std::vector<uint8_t>> compareResult;	//初始化结果数组
	for (auto band : wholeBand.segments())
	{
		compareResult.emplace_back(band.points(), 0);
	}
	for (auto& t : thresholds)	//让结果依次送往门限进行检测
	{
		t->compareTo(data, compareResult);
	}
	updateCounter(compareResult);	//根据本次检测结果更新计数
}

SegmentData ThresholdManager::getPanorama(size_t expectedOutput)
{
	if (hitCounter == nullptr)
		return SegmentData{ 0 };
	SegmentData raw(hitCounter->integrate());
	auto buffer = tryDetect(panoramaDetector, Detector::DetectType::POSIPEAK, raw, expectedOutput);
	if (buffer.ptr != nullptr && buffer.size != 0)
	{
		return SegmentData{ buffer.ptr, buffer.ptr + buffer.size };
	}
	else
	{
		return raw;
	}
}

SegmentData ThresholdManager::getCloseshot(size_t expectedOutput, const FrequencyScale& scale)
{
	if (hitCounter == nullptr)
		return SegmentData{ 0 };
	SegmentData zoominPart(hitCounter->getZoominPart(scale));
	auto buffer = tryDetect(zoomInDetector, Detector::DetectType::POSIPEAK, zoominPart, expectedOutput);
	if (buffer.ptr != nullptr && buffer.size != 0)
	{
		return SegmentData{ buffer.ptr, buffer.ptr + buffer.size };
	}
	else
	{
		return zoominPart;
	}
}

void ThresholdManager::onDetectionRequest(const detection::EnergyDetectionParam& param)
{
	resetToDefault();
	thresholds.clear();
	detectParam = param;
	if (detectParam.has_user_threshold())
	{
		vector<detection::ThresholdSector> sectors{ detectParam.user_threshold().sectors().begin(), detectParam.user_threshold().sectors().end() };
		thresholds.push_back(make_shared<UserDefThreshold>(sectors, wholeBand));
	}
	if (detectParam.has_adaptive_threshold())
	{
		thresholds.push_back(make_shared<::AdaptiveThreshold>(detectParam.adaptive_threshold(), wholeBand));
	}
	if (detectParam.has_history_threshold() && historyThresholdCreator != nullptr)
	{
		auto newHistory = historyThresholdCreator(wholeBand);
		if (newHistory)
		{
			if (newHistory->loadHistory(detectParam.history_threshold()))	//加载成功才加入队列
			{
				thresholds.push_back(newHistory);
				CLOG("{} records loaded on history threshold request", newHistory->loadedDataCount());
			}
			else
			{
				CLOG("no record matching on history threshold request");
			}
		}
		
	}
}

void ThresholdManager::onCloseRequest()
{
	thresholds.clear();
}

void ThresholdManager::sendHits()
{
	send(*hitCounter);
}

void ThresholdManager::resetToDefault()	//重置，以便重新开始比较，注意：门限对象不重置
{
	hitCounter= make_unique<TraceDataHolder>(wholeBand, 0);
	compareCounter = 0;
}

void ThresholdManager::updateCounter(const std::vector<std::vector<uint8_t>>& curCompareResult)
{
	uint8_t compareValue = 1;
	if (detectParam.logic() == detection::EnergyDetectionParam::LT_AND)
	{
		compareValue = thresholds.size();
	}
	for (auto i = 0; i < curCompareResult.size(); i++)
	{
		auto segCounter = hitCounter->getSegmentData(i);
		auto& segCompareResult = curCompareResult[i];
		if (segCounter->size() == segCompareResult.size())
		{
			for (auto j = 0; j < segCompareResult.size(); j++)
			{
				segCounter->at(j) += (segCompareResult[j] >= compareValue);		//更新每个频点上的计数
			}
		}
	}
	compareCounter++;	//更新总的比较次数
}

void ThresholdManager::fillThresholdLine(google::protobuf::RepeatedPtrField<detection::ThresholdRefLine>& lines)
{
	if (thresholds.empty())
		return;
	for (auto& threshold : thresholds)
	{
		auto thresholdLine = threshold->uploadThresholdLine();
		if (!thresholdLine.empty())
		{
			auto newLine = lines.Add();
			newLine->set_type(threshold->getType());
			if (panoramaDetector)
			{
				auto output = panoramaDetector->detect(thresholdLine);
				*newLine->mutable_threshold_trace() = { output.ptr, output.ptr + output.size };
			}
			else
			{
				*newLine->mutable_threshold_trace() = { thresholdLine.begin(), thresholdLine.end() };
			}
		}
	}
}