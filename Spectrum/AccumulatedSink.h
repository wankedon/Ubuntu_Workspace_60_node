/**
 * @file AccumulatedSink.h
 * @brief �����ۻ�
 * @author װ����ҵ������� ����
 * @version 0.1
 * @date 2021-9-30
 *
 * @copyright Copyright (c) 2021  �й����ӿƼ����Ź�˾����ʮһ�о���
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