#pragma once

#include "StreamConnect.h"
#include "node/detection.pb.h"
#include "node/detectionInternal.pb.h"
#include "MergeHelper.h"
class DetectedSignal;
class MergePolicyManager;
class SignalManager : public StreamIn<detection::RawData>
{
public:
	SignalManager(const detection::DetectionParam& defaultParam);
	~SignalManager();
public:
	void receive(detection::RawData& data) override;
	void onCommitDetectedSignal(detection::SignalList& signalList);
	void onParamChange(const detection::DetectionParam& param);
	void setSignalKeeping(const detection::DetailKeeping& detailKeeping);
private:
	void fillSignalList(detection::SignalList& signalList);
	void moveStagedToHistory();
	std::list<std::string> mergeSignal();
	void clear();
private:
	detection::MergeParam mergeParam;
	int64_t commitPeriod;
	int64_t mergeCounter;
	std::unique_ptr<MergePolicyManager> policyManager;
	std::list<std::shared_ptr<DetectedSignal>> historySignals; //所有历史信号合集。
	std::list<std::shared_ptr<DetectedSignal>> mergedSignals; //被合并的历史信号合集。在historySignals中已被删除。	
};

