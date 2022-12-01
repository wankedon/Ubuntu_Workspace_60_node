/**
 * @file SpectrumDetector.cpp
 * @brief 频谱检波器
 * @author 装备事业部软件组 杨青 
 * @version 0.1
 * @date 2020-12-09
 * 
 * @copyright Copyright (c) 2020  中国电子科技集团公司第四十一研究所
 * 
 */
#include "pch.h"
#include "SpectrumDetector.h"

bool SpectrumDetector::detectable(Detector::DetectType detectType, size_t inputPoints, size_t outputPoints)
{
	if (outputPoints < 1)
		return false;
	auto outputBins = outputPoints;
	if (detectType == Detector::DetectType::AUTOPEAK)
	{
		outputBins /= 2;
	}
	return outputBins * 2 <= inputPoints;
}

SpectrumDetector::SpectrumDetector(Detector::DetectType detectType, size_t inputPoints, size_t outputPoints)
	:type(detectType),
	inputBins(inputPoints),
	outputBins(detectType != Detector::DetectType::AUTOPEAK ? outputPoints : (outputPoints / 2)),
	handle(0)
{
	if (outputBins * 2 <= inputBins)
	{
		Detector::init(&handle, detectType, inputBins, outputBins);
	}
}


SpectrumDetector::~SpectrumDetector(void)
{
	if (handle != 0)
	{
		Detector::deinit(handle);
		handle = 0;
	}
}

bool SpectrumDetector::isChanged(size_t ips, size_t ops) const
{
	if (inputBins != ips)
		return true;
	if (abs((double)this->outputPoints() - ops) > 1)
		return true;
	return false;
}

size_t SpectrumDetector::outputPoints() const
{
	if (type != Detector::DetectType::AUTOPEAK)
		return outputBins;
	else
		return outputBins * 2;
}

Detector::Buffer SpectrumDetector::detect(const std::vector<float>& input)
{
	Detector::Buffer outputBuffer{ nullptr, 0 };
	Detector::Buffer inputBuffer{ const_cast<float*>(input.data()), input.size() };
	if (Detector::input(handle, inputBuffer) == Detector::ErrorType::ERR_NONE)
	{
		Detector::getSweepDetectResult(handle, &outputBuffer, true);
		Detector::reset(handle);
	}
	return outputBuffer;
}

Detector::Buffer tryDetect(std::unique_ptr<SpectrumDetector>& detector, Detector::DetectType type, const std::vector<float>& input, size_t expectedOutput)
{
	if (SpectrumDetector::detectable(type, input.size(), expectedOutput))
	{
		if (detector == nullptr || detector->isChanged(input.size(), expectedOutput))
			detector = std::make_unique<SpectrumDetector>(type, input.size(), expectedOutput);
		return detector->detect(input);
	}
	else
	{
		return Detector::Buffer{ nullptr, 0 };
	}
}