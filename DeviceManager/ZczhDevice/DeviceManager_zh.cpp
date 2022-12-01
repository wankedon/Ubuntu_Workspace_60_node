#include "pch.h"
#include "DeviceManager.h"
#include "DeviceCluster.h"
#include "Device.h"
#include "ZhSensorCluster.h"

using namespace std;

DeviceManager::DeviceManager(const LANConfig& cfg)
{
	cluster.insert(make_shared<ZhSensorCluster>(cfg));
}

DeviceManager::~DeviceManager()
{

}

void DeviceManager::onTimer()
{
	for (auto& item : cluster)
	{
		item->refresh();
	}
}

void DeviceManager::updateInfo(NodeInfo& info) const
{
	for (auto& item : cluster)
	{
		item->fillUpNodeInfo(info);
	}
}

shared_ptr<Device> DeviceManager::getDeviceById(const zb::dcts::node::DeviceId& did)
{
	for (auto& item : cluster)
	{
		auto device = item->getDeviceById(did);
		if (device)
			return device;
	}
	return nullptr;
}

list<shared_ptr<Device>> DeviceManager::getDeviceByType(DeviceType type)
{
	for (auto& item : cluster)
	{
		auto devices = item->getDeviceByType(type);
		if (!devices.empty())
			return devices;
	}
	return list<shared_ptr<Device>>();
}

shared_ptr<Device> DeviceManager::getIdleDevice(const std::pair<DeviceId, DeviceType>& req)
{
	if (req.first.value() != 0)
	{
		auto device = getDeviceById(req.first);
		//��һ���ļ��ʷ�������������ڵ����������������UI�˻�δ���ü�ˢ�½ڵ��б���10sˢ��һ�Σ���
		//����UI���͵��豸ID�Ǿɵ�ID�����Ҳ����豸
		if ((device) && (device->isIdle()))
		{
			return device;
		}
	}
	else
	{
		auto devices = getDeviceByType(req.second);
		for (auto& device : devices)
		{
			if (device->isIdle())
			{
				return device;
			}
		}
	}
	return nullptr;
}
