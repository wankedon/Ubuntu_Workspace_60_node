#pragma once

#include "node/node.pb.h"

using namespace zb::dcts::node;

class Device
{
public:
	DeviceType type() const;
	DeviceId id() const;
	DeviceInfo info() const;
	node::DeviceStatus status() const;

	void engageWithTask(const zb::dcts::TaskId& tid);
	void disengage();	
	bool isIdle() const;
	bool isEngaged() const;
public:
	virtual void onTimer() = 0;
protected:
	Device(DeviceType type, node::DeviceStatus initStatus = D_OFFLINE);
	~Device();
protected:
	DeviceInfo deviceInfo;
	std::unique_ptr<zb::dcts::TaskId> taskId;
};

