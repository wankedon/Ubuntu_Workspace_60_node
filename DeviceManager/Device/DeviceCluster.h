#pragma once

#include "node/node.pb.h"

class Device;
class DeviceCluster
{
public:
	DeviceCluster() = default;
	~DeviceCluster() = default;
public:
	std::shared_ptr<Device> getDeviceById(const zb::dcts::node::DeviceId& did);
	std::list<std::shared_ptr<Device>> getDeviceByType(zb::dcts::node::DeviceType type);
	virtual void refresh() = 0;
	void fillUpNodeInfo(NodeInfo& info);
protected:
	std::mutex mtx;
	std::map<uint32_t, std::shared_ptr<Device>> devices;
};

