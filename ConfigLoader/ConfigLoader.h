#pragma once

#include "node/nodeInternal.pb.h"
#include "pugixml.hpp"

class ConfigLoader
{
public:
	ConfigLoader(const std::wstring& filePath);
	~ConfigLoader(void) = default;
public:
	std::unique_ptr<NodeConfig> load();
	void rewriteName(const std::string& newName);
private:
	NodeIdentity loadIdentity(pugi::xml_node xmlIdentity);
	WANConfig loadWANConfig(pugi::xml_node xmlWAN);
	LANConfig loadLANConfig(pugi::xml_node xmlLAN);
private:
	ZeromqLinkCfg loadLinkCfg(pugi::xml_node xmlNode);
	std::vector<DeviceConfig> loadDeviceCluster(pugi::xml_node xmlNode);
	std::unique_ptr<DeviceType> parseDeviceType(const std::wstring& strType);
private:
	const std::wstring filePath;
	bool modified;
};

