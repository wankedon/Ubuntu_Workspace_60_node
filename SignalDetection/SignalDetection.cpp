#include "pch.h"

#include "StreamConnect.h"
#include "SignalDetection.h"
//#include "DetectionDataBuffer.h"
#include "RawDataPreprocessor.h"
#include "MergePolicyManager.h"
#include "SignalManager.h"

using namespace std;

class SignalDetection::Impl
{
public:
	Impl()
	{
		detection::DetectionParam defaultParam;
		defaultParam.mutable_segmentation_param()->set_enable_filters(detection::HITS);
		DRange hitRange;
		hitRange.set_start(0.1);
		hitRange.set_stop(1);
		auto hitsParam = defaultParam.mutable_segmentation_param()->mutable_hits_param();
		*hitsParam->add_hits_prob() = hitRange;
		DRange bandwidthRange;
		bandwidthRange.set_start(10e3);
		bandwidthRange.set_stop(1e6);
		*hitsParam->mutable_bandwidth_range() = bandwidthRange;

		auto mergeParam = defaultParam.mutable_merge_param();
		mergeParam->set_policy(detection::SPECTRUM_INTERSECT_POLICY);
		mergeParam->mutable_feature_merge_param()->set_min_intersection_ratio(0.4);
		mergeParam->mutable_feature_merge_param()->set_max_time_gap(10000);
		mergeParam->mutable_signal_merge_param()->set_min_intersection_ratio(0.4);
		mergeParam->mutable_signal_merge_param()->set_max_time_gap(10000);
		
		defaultParam.mutable_options()->mutable_commit_result_period()->set_value(5);	//默认每5次检测更新一次历史库
		init(defaultParam);
	}

	Impl(const detection::DetectionParam& param)
	{
		init(param);
	}

	~Impl() = default;
public:
	void setDetectionParam(const detection::DetectionParam& params)
	{
		std::lock_guard<mutex> lg(paramMutex);
		detectionParam = make_unique<detection::DetectionParam>(params);
	}

	void detect(detection::RawData& rawData)
	{
		if (detectionParam != nullptr)
			onParamChange();
		preprocessor->preprocess(rawData);
	}

	void onCommitDetectedSignal(detection::SignalList& signalList)
	{
		sigManager->onCommitDetectedSignal(signalList);
	}

	void setDetailKeeping(const detection::DetailKeeping& detailKeeping)
	{
		sigManager->setSignalKeeping(detailKeeping);
	}

private:
	void init(const detection::DetectionParam& param)
	{
		sigManager = make_unique<SignalManager>(param);
		preprocessor = make_unique<RawDataPreprocessor>(param.segmentation_param(), *sigManager);
	}

	void onParamChange()
	{
		std::lock_guard<mutex> lg(paramMutex);
		if (detectionParam == nullptr)
			return;
		if (detectionParam->has_segmentation_param())
			preprocessor->onParamChange(detectionParam->segmentation_param());
		if (detectionParam->has_merge_param() || detectionParam->has_options())
			sigManager->onParamChange(*detectionParam);
		detectionParam.reset();
	}
private:
	unique_ptr<detection::DetectionParam> detectionParam;
	unique_ptr<SignalManager> sigManager;			//实体
	unique_ptr<RawDataPreprocessor> preprocessor;	//处理模块		
	mutex paramMutex;
};


SignalDetection::SignalDetection() //由各处理单元提供所需的默认参数。
	:impl(make_unique<Impl>())
{
}

SignalDetection::SignalDetection(const detection::DetectionParam& param)
	:impl(make_unique<Impl>(param))
{

}

SignalDetection::~SignalDetection() = default;

void SignalDetection::setDetectionParam(const detection::DetectionParam& params)
{
	impl->setDetectionParam(params);
}

void SignalDetection::detect(detection::RawData& rawData)
{
	impl->detect(rawData);
}

void SignalDetection::onCommitDetectedSignal(detection::SignalList& signalList)
{
	impl->onCommitDetectedSignal(signalList);
}

void SignalDetection::setDetailKeeping(const detection::DetailKeeping& detailKeeping)
{
	impl->setDetailKeeping(detailKeeping);
}
