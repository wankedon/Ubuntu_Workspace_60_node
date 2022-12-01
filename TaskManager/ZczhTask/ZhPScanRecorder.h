#pragma once

#include "ZhPScanPersistence.h"
#include "node/zczh/ZhPScan.pb.h"
#include "node/spectrum.pb.h"
#include "AccumulatedSink.h"

#include <future>

class PScanDataAsyncWriter
{
public:
    PScanDataAsyncWriter(std::shared_ptr<persistence::PScanDBAccessor> accessor);
    ~PScanDataAsyncWriter();
public:
    bool isReady();
    bool writeItem(const persistence::PScanDataItem& item);
    int recordCount() const; 
private:
    std::shared_ptr<persistence::PScanDBAccessor> accessor;
    std::atomic<int> writeItemCounter;
    std::future<void> asyncWriteProc;
};

class PScanRecorder
{
    friend class ZhPScanProcessor;    //��¶˽�г�Ա��processor���Ա��ܹ��������ӳ�������
public:
    PScanRecorder(std::shared_ptr<persistence::PScanDBAccessor> accessor);
    ~PScanRecorder();
public:
    void writeTaskInfo(const persistence::PScanTaskItem& taskItem);
    void onResultReport(const spectrum::Header& header);
    void enable(int timesOfResultPeriod);
    void disable();
    void rewriteTaskInfo(const zczh::zhpscan::OperationStatus& lastStatus);
    int recordCount() const { return dataWriter.recordCount(); }
private:
    bool isEnabled() const;
    bool isNeedRecord(int sequenceNum) const;
    void makeDataItem();
    void resetAcc();
private:
    std::shared_ptr<persistence::PScanDBAccessor> accessor;
    std::unique_ptr<persistence::PScanTaskItem> taskInfo;
    std::unique_ptr<spectrum::Header> accumulatedStatus;
    AccumulatedSink accumulatedSpectrum;
    AccumulatedSink accumulatedHits;
    persistence::PScanDataItem dataItem;
    PScanDataAsyncWriter dataWriter;
    int timesOfResultPeriod;
};

