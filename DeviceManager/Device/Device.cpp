#include "pch.h"
#include "Device.h"
#include "Miscellaneous.h"

Device::Device(DeviceType type, DeviceStatus initStatus)
{
	deviceInfo.set_type(type);
	deviceInfo.mutable_id()->set_value(geneHashcode());
	deviceInfo.set_status(initStatus);
}

Device::~Device()
{

}

DeviceType Device::type() const
{
	return deviceInfo.type();
}

DeviceId Device::id() const
{
	return deviceInfo.id();
}

DeviceInfo Device::info() const
{
	return deviceInfo;
}

DeviceStatus Device::status() const
{
	return deviceInfo.status();
}

void Device::engageWithTask(const zb::dcts::TaskId& tid)
{
	assert(isIdle());
	taskId = std::make_unique<zb::dcts::TaskId>(tid);
	deviceInfo.set_status(D_ENGAGED);
}

void Device::disengage()
{
	taskId.reset();
	deviceInfo.set_status(D_IDLE);
}

bool Device::isIdle() const
{
	return (status() == D_IDLE);
}

bool Device::isEngaged() const
{
	return (taskId != nullptr);
}

