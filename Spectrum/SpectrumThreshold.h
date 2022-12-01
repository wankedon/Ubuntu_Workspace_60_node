#pragma once

#include "SpectrumUtil.h"
#include "node/detection.pb.h"
#include "TraceDataHolder.h"
#include "StreamConnect.h"

class SpectrumThreshold
{
public:
	SpectrumThreshold(const spectrum::FrequencyBand& wholeBand, detection::ThresholdType type);
	virtual ~SpectrumThreshold();
public:
	virtual void compareTo(const TraceDataHolder& spectrumLine, std::vector<std::vector<uint8_t>>& compareResult);
	virtual SegmentData uploadThresholdLine() = 0;
	detection::ThresholdType getType() const;
protected:
	const detection::ThresholdType type;
	TraceDataHolder thresholdLine;
};

class FrequencyScale;
class UserDefThreshold : public SpectrumThreshold
{
public:
	UserDefThreshold(const std::vector<detection::ThresholdSector>& sector, const spectrum::FrequencyBand& wholeBand);
	virtual ~UserDefThreshold();
public:
	SegmentData uploadThresholdLine() override;
private:
	void setThresholdLevel(const detection::ThresholdSector& sector, const spectrum::FrequencyBand& wholeBand);
private:
	bool uploaded;
};

#include "StreamConnect.h"

class FrequencyScale;
class SpectrumDetector;
class HistoryThresholdBase;
using HTCreateFunctor = std::function<std::shared_ptr<HistoryThresholdBase>(const spectrum::FrequencyBand&)>;

class ThresholdManager : public StreamInOut<TraceDataHolder, TraceDataHolder>
{
public:
	ThresholdManager(const spectrum::FrequencyBand& wholeBand, HTCreateFunctor htCreator);
	~ThresholdManager();
public:
	void receive(const TraceDataHolder& data) override;
public:
	void onDetectionRequest(const detection::EnergyDetectionParam& param);
	void onCloseRequest();
	void sendHits();
	void resetToDefault();
	SegmentData getPanorama(size_t expectedOutput);	//获得全景
	SegmentData getCloseshot(size_t expectedOutput, const FrequencyScale& scale);	//获得近景
	void fillThresholdLine(google::protobuf::RepeatedPtrField<detection::ThresholdRefLine>& lines);
	bool isOpened() const { return !thresholds.empty(); }
private:
	void updateCounter(const std::vector<std::vector<uint8_t>>& curCompareResult);
	void compare(const TraceDataHolder& data);
private:
	const spectrum::FrequencyBand wholeBand;
	int32_t compareCounter;
	detection::EnergyDetectionParam detectParam;
	std::list<std::shared_ptr<SpectrumThreshold>> thresholds;
	std::unique_ptr<TraceDataHolder> hitCounter;
	std::unique_ptr<SpectrumDetector> panoramaDetector;
	std::unique_ptr<SpectrumDetector> zoomInDetector;
	HTCreateFunctor historyThresholdCreator;
};