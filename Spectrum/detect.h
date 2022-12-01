/**
 * @file detect.h
 * @brief Ƶ�׼첨��
 * @author װ����ҵ�������� ���� 
 * @version 0.1
 * @date 2022-03-16
 * 
 * @copyright Copyright (c) 2022  �й����ӿƼ����Ź�˾����ʮһ�о���
 * 
 */
#pragma once

#define DETECTOR_API Detector::ErrorType
#include "stdint.h"

namespace Detector
{
	enum class DetectType
	{
		AUTOPEAK,	//R&S combine pos and neg peak, NOTE: produce 2 points for each detect bin
		NEGAPEAK,	//min val in each bin
		POSIPEAK,	//max val in each bin
		AVG,		//volt average of all samples in each bin
		RMS,		//RMS of all samples in each bin
		SAMPLE,		//last sample val in each bin
	};

	enum class ErrorType
	{
		ERR_NONE,				//no error
		ERR_NOINSTANCE,			//handler error i.e. can not find instance
		ERR_PARAM,				//function param
		ERR_DETECT_END,			//cur sweep detection already done
		ERR_NOT_ENOUGH_POINTS,	//segment data not enough to produce detect result
		ERR_RESULT_INCOMPLETE,	//cur sweep detection undone
		ERR_BUFFER_TOO_SMALL,	//invalid buffer size
	};

	struct Buffer
	{
		float* ptr;
		size_t size;
	};

	//�첨����ʼ��
	DETECTOR_API init(uint32_t* detHandle, DetectType type, size_t fftBins, size_t detectBins);

	//����һ��Ƶ�εķ���
	DETECTOR_API input(uint32_t detHandle, Buffer buffer);

	//���ü첨������һ������ɨ��ȫ������֮�����
	DETECTOR_API reset(uint32_t detHandle);

	//��õ�ǰƵ�εļ첨���������input֮��,reset֮ǰ����
	DETECTOR_API getSegmentDetectResult(uint32_t detHandle, Buffer* buffer, bool nocopy);

	//��õ�ǰ����ɨ��ļ첨����������һ��Ƶ��input֮�����
	DETECTOR_API getSweepDetectResult(uint32_t detHandle, Buffer* buffer, bool nocopy);

	//���ټ첨������ɨ����������
	DETECTOR_API deinit(uint32_t detHandle);

	/*pseudo-code demo
	init(&detHandle, DetectType::POSIPEAK, 3029432, 1000);	//create a positive peak detector with 3029432 points input(raw spectrum) and 1000 points output(detect result)
	do
	{
		for(seg = 0; seg != segEnd; ++seg)
		{
			input(detHandle, segSpectrum);		//input each segment spectrum
			getSegmentDetectResult(detHandle, segDetOutput);	//optional
		}
		getSweepDetectResult(detHandle, sweepDetOutput);	//get detect result when sweep done
		reset(detHandle);									//reset detector to get ready for next sweep detection
		
	}while(!sweepEnd);						//do detection till user stop sweep 
	deinit(detHandle);						//free handle object
	*/
}


