#pragma once

#include "node/nodeInternal.pb.h"

class Device;
class DeviceCluster;
class DeviceManager
{
public:
	DeviceManager(const LANConfig& cfg);
	~DeviceManager();
public:
	std::shared_ptr<Device> getIdleDevice(const std::pair<DeviceId, DeviceType>& request);
public:
	void onTimer();
	void updateInfo(NodeInfo& info) const;
	std::shared_ptr<Device> getDeviceById(const DeviceId& did);
	std::list<std::shared_ptr<Device>> getDeviceByType(DeviceType type);
private:
	std::set<std::shared_ptr<DeviceCluster>> cluster;
};

