/**
 * @file SpectrumDataHolder.h
 * @brief Ƶ�����ݱ���
 * @author װ����ҵ������� ����
 * @version 0.1
 * @date 2021-9-30
 *
 * @copyright Copyright (c) 2021  �й����ӿƼ����Ź�˾����ʮһ�о���
 * 
 */
#pragma once

#include "SpectrumUtil.h"
#include "node/spectrum.pb.h"
#include "StreamConnect.h"

class TraceDataHolder;
class FrequencyScale;
class SpectrumDetector;
/**
 * @brief Ƶ�����ݳ���
 * 
 */
class SpectrumDataHolder : public StreamOut<TraceDataHolder>
{
public:
	SpectrumDataHolder(spectrum::DataHoldType dataHoldType);
	~SpectrumDataHolder();
public:
	void forwardData();	//ת����������
	void clear();
	void inputSegData(int32_t segIdx, const float* data, size_t count);
	void loadSweepData(const spectrum::FrequencyBand& band, const std::vector<char>& sweepData);
	size_t segmentCount() const;
	std::shared_ptr<SegmentData> getSegmentData(int32_t segIdx) const;
	SegmentData getPanorama(size_t expectedOutput);	//���ȫ��
	SegmentData getCloseshot(size_t expectedOutput, const FrequencyScale& scale);	//��ý���
private:
	const spectrum::DataHoldType DH_TYPE;
	std::unique_ptr<SpectrumDetector> panoramaDetector;
	std::unique_ptr<SpectrumDetector> zoomInDetector;
	std::unique_ptr<TraceDataHolder> dataHolder;
};