#include "pch.h"
#include "DeviceCluster.h"
#include "Device.h"


std::shared_ptr<Device> DeviceCluster::getDeviceById(const zb::dcts::node::DeviceId& did)
{
	std::lock_guard<std::mutex> lg(mtx);
	auto iter = devices.find(did.value());
	if (iter != devices.end())
	{
		return iter->second;
	}
	else
	{
		return nullptr;
	}
}

std::list<std::shared_ptr<Device>> DeviceCluster::getDeviceByType(zb::dcts::node::DeviceType type)
{
	std::lock_guard<std::mutex> lg(mtx);
	std::list<std::shared_ptr<Device>> result;
	for (auto& dev : devices)
	{
		if (dev.second->type() == type)
		{
			result.push_back(dev.second);
		}
	}
	return result;
}

void DeviceCluster::fillUpNodeInfo(NodeInfo& info)
{
	std::lock_guard<std::mutex> lg(mtx);
	auto deviceInfo = info.mutable_devices();
	for (auto& dev : devices)
	{
		auto devInfo = deviceInfo->Add();
		*devInfo = dev.second->info();
	}
}