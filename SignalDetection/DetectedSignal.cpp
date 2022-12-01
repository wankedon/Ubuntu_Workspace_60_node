#include "pch.h"
#include "DetectedSignal.h"
#include "MergeHelper.h"

using namespace std;

DetectedSignal::DetectedSignal()
    :uuid(Helper::genUUID()),
    activity(detection::UNKNOWN),
    keepDetail(false)
{
}

DetectedSignal::~DetectedSignal() = default;

void DetectedSignal::setDetailKeeping(bool keep)
{
    keepDetail = keep;
}

void DetectedSignal::onNewFeature(detection::SignalFeature&& feature, int32_t timeFactor)
{
    if (!keepDetail)
    {
        feature.clear_spectrum_detail();
        feature.clear_hits_detail();
    }
    auto lastSeg = lastStagedSegment();
    if (lastSeg == nullptr)    //没有上一段
    {
        staged.emplace_back(newSignalSegment(std::move(feature)));
    }
    else
    {
        auto lastStopTime = lastSeg->time_span().stop_time();
        auto curStartTime = feature.time_span().start_time();
        if (Helper::isBiggerThan(lastStopTime, curStartTime, timeFactor))
        {
            staged.emplace_back(newSignalSegment(std::move(feature)));
        }
        else
        {
            update(*lastSeg, std::move(feature)); 
        }
    }
}

/**
 * @brief 根据信号特征生成一个信号片。
 */
detection::SignalSegment DetectedSignal::newSignalSegment(detection::SignalFeature&& feature)
{
    detection::SignalSegment segment;
    *segment.mutable_center_freq() = Helper::initStatisticVal(feature.signal_band().center_frequency());
    *segment.mutable_band_width() = Helper::initStatisticVal(feature.signal_band().band_width());
    *segment.mutable_amplitude() = feature.amplitude();
    *segment.mutable_time_span() = feature.time_span();
    segment.set_total_sweep(feature.num_sweep());
    segment.set_total_hits(segment.total_sweep() * feature.hits().up());
    *segment.add_feature() = feature;
    return segment;
}

void DetectedSignal::update(detection::SignalSegment& last, detection::SignalFeature&& curFeature)
{
    auto statisticCount = last.feature_size();
    Helper::updateStatisticVal(*last.mutable_center_freq(), statisticCount, curFeature.signal_band().center_frequency());
    Helper::updateStatisticVal(*last.mutable_band_width(), statisticCount, curFeature.signal_band().band_width());
    Helper::updateStatisticVal(*last.mutable_amplitude(), curFeature.amplitude(), statisticCount, 1);
    *last.mutable_time_span()->mutable_stop_time() = curFeature.time_span().stop_time();
    last.set_total_sweep(last.total_sweep() + curFeature.num_sweep());
    last.set_total_hits(last.total_hits() + curFeature.hits().up() * curFeature.num_sweep());
    *last.add_feature() = std::move(curFeature);
}

void DetectedSignal::update(detection::SignalSegment& dst, detection::SignalSegment& src)
{
    auto dstStatisticCount = dst.feature_size();
    auto srcStatisticCount = src.feature_size();
    Helper::updateStatisticVal(*dst.mutable_center_freq(), src.center_freq(), dstStatisticCount, srcStatisticCount);
    Helper::updateStatisticVal(*dst.mutable_band_width(), src.band_width(), dstStatisticCount, srcStatisticCount);
    Helper::updateStatisticVal(*dst.mutable_amplitude(), src.amplitude(), dstStatisticCount, srcStatisticCount);
    *dst.mutable_time_span()->mutable_stop_time() = src.time_span().stop_time();
    dst.set_total_sweep(dst.total_sweep() + src.total_sweep());
    dst.set_total_hits(dst.total_hits() + src.total_sweep());
    for (auto& f : *src.mutable_feature())
    {
        *dst.add_feature() = std::move(f); //使用右值拷贝
    }
}

detection::SignalSegment* DetectedSignal::lastStagedSegment()
{
    if (!staged.empty())
        return &staged.back();
    else
        return nullptr;
}

detection::SignalSegment* DetectedSignal::lastHistorySegment()
{
    if (!history.empty())
        return &history.back();
    else
        return nullptr;
}

int DetectedSignal::stagedHeaderSequence(int32_t timeFactor)
{
    auto lastHistory = lastHistorySegment();
    if (lastHistory == nullptr)
        return 0;
    auto lastSeq = lastHistory->segment_seq();
    auto lastStopTime = lastHistory->time_span().stop_time();
    auto curStartTime = staged.front().time_span().start_time();
    return Helper::isBiggerThan(lastStopTime, curStartTime, timeFactor) ? (lastSeq + 1) : lastSeq;
}

bool DetectedSignal::prepareForFill(int32_t timeFactor)
{
    if (staged.empty())
        return false;
    auto startSeq = stagedHeaderSequence(timeFactor);
    for (auto& segment : staged)  
    {
        segment.set_segment_seq(startSeq++); 
    }
    return true;
}

void DetectedSignal::fillSignalList(detection::SignalList& signalList, int32_t timeFactor)
{
    if (!prepareForFill(timeFactor))
        return;
    for (const auto& segment : staged)
    {
        auto* ds = signalList.add_detected_signals();
        ds->set_id(uuid);   //在拷贝给用户时，才设置id
        ds->set_segment_seq(segment.segment_seq());
        ds->set_total_hits(segment.total_hits());
        ds->set_total_sweep(segment.total_sweep());
        *ds->mutable_center_freq() = segment.center_freq();
        *ds->mutable_band_width() = segment.band_width();
        *ds->mutable_amplitude() = segment.amplitude();
        *ds->mutable_time_span() = segment.time_span();
        *ds->mutable_feature() = segment.feature();  
    }
}

void DetectedSignal::moveStagedToHistory()
{
    if (staged.empty())
        return;
    auto lastHistory = lastHistorySegment();
    if (lastHistory != nullptr)
    {
        auto& firstStaged = staged.front();
        if (lastHistory->segment_seq() == firstStaged.segment_seq())
        {
            update(*lastHistory, firstStaged);
            staged.pop_front();
        }
    }
    for (auto& segment : staged)
    {
        history.emplace_back(std::move(segment));    //右值传递，减少开销
    }

    staged.clear();  //需要将staged清空！
}

void DetectedSignal::mergeFrom(DetectedSignal& other)
{
    auto selfLastHistory = lastHistorySegment();
    auto otherLastHistory = other.lastHistorySegment();
    if (selfLastHistory == nullptr || otherLastHistory == nullptr)
        return;
    update(*selfLastHistory, *otherLastHistory);
    other.activity = detection::MERGED; //标记为merged
}

spectrum::SignalBand DetectedSignal::signalBand()
{
    spectrum::SignalBand band;
    auto seg = lastStagedSegment();
    if (seg == nullptr)
        seg = lastHistorySegment();
    if (seg != nullptr)
    {
        band.set_band_width(seg->band_width().mean());
        band.set_center_frequency(seg->center_freq().mean());
    }
    return band;
}

Timestamp DetectedSignal::lastEmergeTime()
{
    auto seg = lastStagedSegment();
    if (seg != nullptr)
        return seg->time_span().stop_time();
    else
        return lastHistorySegment()->time_span().stop_time();
}

Timestamp DetectedSignal::firstEmergeTime()
{
    if (!history.empty())
        return history.front().time_span().start_time();
    else
        return staged.front().time_span().start_time();
}

