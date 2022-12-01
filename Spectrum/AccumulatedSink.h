/**
 * @file AccumulatedSink.h
 * @brief 数据累积
 * @author 装备事业部软件组 杨青
 * @version 0.1
 * @date 2021-9-30
 *
 * @copyright Copyright (c) 2021  中国电子科技集团公司第四十一研究所
 * 
 */
#pragma once

#include "StreamConnect.h"

class TraceDataHolder;

class AccumulatedSink : public StreamIn<TraceDataHolder>
{
public:
    AccumulatedSink();
    ~AccumulatedSink();
public:
    void receive(const TraceDataHolder& data) override;
    void enable();
    void disable();
    void reset();
    bool isEnabled() const { return enabled; }
    void fillDataBuffer(std::vector<char>& buffer);
    void fillDataBuffer(std::vector<float>& buffer);
private:
    std::unique_ptr<TraceDataHolder> dataCache;
    bool enabled;
};