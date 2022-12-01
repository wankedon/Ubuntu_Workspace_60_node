/**
* @file Filters.h

* @par Copyright (c):�е�Ƶ���ʮһ�о��� װ����ҵ��
* ��Ȩ���У�δ����ɣ����ø��ơ�������ת��

* @brief ���������ࡣ

* @author �κ��

* @date 2022��5��9��

* @note

* @version <1.0.0>

*/
#pragma once

#include "StreamConnect.h"
#include "node/detection.pb.h"
#include "node/detectionInternal.pb.h"

class DataFilter :public StreamInOut<detection::RawData, detection::RawData>
{
public:
	DataFilter();
	virtual ~DataFilter();
public:
	void receive(detection::RawData& input) override; //�ض����const������receive������
	virtual void onParamChange(const detection::SegmentationParam& params) = 0; //���ù�����������
protected:
	virtual void filter(detection::RawData& input) = 0; //���й��ˣ��������������Ǿ���ʵ�ַ�����
};	

