/**
 * @file SpectrumDetector.h
 * @brief Ƶ�׼첨��
 * @author װ����ҵ������� ���� 
 * @version 0.1
 * @date 2020-12-09
 * 
 * @copyright Copyright (c) 2020  �й����ӿƼ����Ź�˾����ʮһ�о���
 * 
 */
#pragma once

#include "detect.h"

class SpectrumDetector
{
public:
	SpectrumDetector(Detector::DetectType detectType, size_t inputPoints, size_t outputPoints);
	~SpectrumDetector(void);
public:
	static bool detectable(Detector::DetectType detectType, size_t inputPoints, size_t outputPoints);
public:
	Detector::Buffer detect(const std::vector<float>& input);
	size_t inputPoints() const { return inputBins; }
	size_t outputPoints() const;
	Detector::DetectType detType() const { return type; }
	bool isValid() const { return handle != 0; }
	bool isChanged(size_t inputPoints, size_t outputPoints) const;
private:
	const Detector::DetectType type;
	const size_t inputBins;
	const size_t outputBins;
	uint32_t handle;			///< �첨���
};

Detector::Buffer tryDetect(std::unique_ptr<SpectrumDetector>& detector, Detector::DetectType, const std::vector<float>& input, size_t expectedOutput);