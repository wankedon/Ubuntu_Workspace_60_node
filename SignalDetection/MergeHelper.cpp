#include "pch.h"
#include "MergeHelper.h"
#include "TimeStamp.h"
#include <czmq.h>
#include <numeric>

using namespace std;
using namespace spectrum;

namespace Helper
{
    const std::string genUUID()
    {
        auto uuid = zuuid_new();
        string result(zuuid_str(uuid));
        zuuid_destroy(&uuid);
        return result;
    }
    /**
     * @brief 求两个信号的交集∩
     */
    double intersection(const FrequencySpan& spanA, const FrequencySpan& spanB)
    {
        auto intersectionStart = fmax(spanA.start_freq(), spanB.start_freq());
        auto intersectionStop = fmin(spanA.stop_freq(), spanB.stop_freq());
        auto diff = intersectionStop - intersectionStart;
        return diff > 0 ? diff : 0;
    }
    /**
     * @brief 求两个信号的并集∪
     */
    double unionsection(const FrequencySpan& spanA, const FrequencySpan& spanB)
    {
        auto unionStart = fmin(spanA.start_freq(), spanB.start_freq());
        auto unionStop = fmax(spanA.stop_freq(), spanB.stop_freq());
        auto maxSpanSum = spanA.stop_freq() + spanB.stop_freq() - spanA.start_freq() - spanB.start_freq();
        auto sum = unionStop - unionStart;
        return sum < maxSpanSum ? sum : maxSpanSum;
    }

    bool isIntersected(const spectrum::SignalBand& lhs, const spectrum::SignalBand& rhs, float minIntersectionRatio)
    {
        auto lSpan = toFrequencySpan(lhs);
        auto rSpan = toFrequencySpan(rhs);
        auto interPart = intersection(lSpan, rSpan);
        auto unionPart = unionsection(lSpan, rSpan);
        return interPart / unionPart > minIntersectionRatio;
    }

    /**
     * @brief 计算新、旧信号合并带宽与中心频率。
     */
    spectrum::SignalBand calMergeSigParam(const FrequencySpan& nowFreq, const FrequencySpan& newFreq)
    {
        spectrum::SignalBand signalBand;
        if (!intersection(nowFreq, newFreq))
        {
            signalBand.set_center_frequency(nowFreq.start_freq() + nowFreq.stop_freq()/ 2);
            signalBand.set_band_width(nowFreq.stop_freq() - nowFreq.start_freq());
        }
        else
        {
            auto span = unionsection(nowFreq, newFreq);
            auto start = fmin(nowFreq.start_freq(), newFreq.start_freq());
            signalBand.set_center_frequency(start + span / 2);
            signalBand.set_band_width(span);
        }
        return signalBand;
    }

    int64_t calTimeDiff(const Timestamp& pre, const Timestamp& now)
    {
        //TDOA::CTimeStamp timeTool;
        return CTimeStamp(now) - CTimeStamp(pre);
    }

    bool isBiggerThan(const Timestamp& pre, const Timestamp& now, int timeGapInMs)
    {
        return (CTimeStamp(now) - CTimeStamp(pre)) > (timeGapInMs * 1e6);
    }

    StatisticVal initStatisticVal(double val)
    {
        StatisticVal statVal;
        statVal.set_cur(val);
        statVal.set_down(val);
        statVal.set_mean(val);
        statVal.set_up(val);
        return statVal;
    }

    void updateStatisticVal(StatisticVal& statVal, size_t statCount, double now)
    {
        statVal.set_up(fmax(statVal.up(), now));
        statVal.set_down(fmin(statVal.down(), now));
        statVal.set_cur(now);
        statVal.set_mean((statVal.mean() * statCount + now) / (statCount + 1));
    }

    void updateStatisticVal(StatisticVal& dst, const StatisticVal& src, size_t destStatCount, size_t srcStatCount)
    {
        dst.set_up(fmax(dst.up(), src.up()));
        dst.set_down(fmin(dst.down(), src.down()));
        dst.set_cur(src.cur());
        dst.set_mean((dst.mean() * destStatCount + src.mean() * srcStatCount) / (destStatCount + srcStatCount));
    }

    spectrum::FrequencySpan toFrequencySpan(const spectrum::SignalBand& signalBand)
    {
        FrequencySpan span;
        span.set_start_freq(signalBand.center_frequency() - signalBand.band_width() / 2);
        span.set_stop_freq(signalBand.center_frequency() + signalBand.band_width() / 2);
        return span;
    }

    spectrum::SignalBand toCenterBandwidth(const FrequencySpan& sp)
    {
        spectrum::SignalBand band;
        band.set_band_width(sp.stop_freq() - sp.start_freq());
        band.set_center_frequency((sp.stop_freq() + sp.start_freq()) / 2);
        return band;
    }

    list<detection::SignalFeature> extractSignalFeatures(const detection::RawData& data)
    {
        list<detection::SignalFeature> features;      
        auto startstop = data.body().freq_span();
        auto freStep = (startstop.stop_freq() - startstop.start_freq()) / data.body().cur_trace_size();
        for (auto pos : data.signal_segments())
        {
            spectrum::FrequencySpan sp;
            sp.set_start_freq(startstop.start_freq() + pos.start() * freStep);
            sp.set_stop_freq(startstop.start_freq() + (pos.stop() - 1) * freStep); //左闭右开区间。
            detection::SignalFeature newFeature;
            *newFeature.mutable_time_span() = data.header().time_span();
            *newFeature.mutable_signal_band() = toCenterBandwidth(sp);
            newFeature.set_num_sweep(data.header().sweep_count());
            *newFeature.mutable_hits_detail() = {
                data.body().over_threshold_hits().begin() + pos.start(),
                data.body().over_threshold_hits().begin() + pos.stop(),
            };
            *newFeature.mutable_spectrum_detail() = {
                data.body().cur_trace().begin() + pos.start(),
                data.body().cur_trace().begin() + pos.stop()
            };
            *newFeature.mutable_amplitude() = doStatistic(newFeature.spectrum_detail().begin(), newFeature.spectrum_detail().end()); //统计幅度。
            *newFeature.mutable_hits() = doStatistic(newFeature.hits_detail().begin(), newFeature.hits_detail().end()); //统计命中。
            features.emplace_back(std::move(newFeature));
        }
        return features;
    }
}

