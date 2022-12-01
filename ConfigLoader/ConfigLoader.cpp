#include "pch.h"
#include "ConfigLoader.h"
#include "Miscellaneous.h"
#include "StringConv.h"
#include "Logger.h"

using namespace std;
using namespace StrConvert;

ConfigLoader::ConfigLoader(const std::wstring& fp)
	:filePath(fp),
	modified(false)
{

}

void ConfigLoader::rewriteName(const std::string& newName)
{
	pugi::xml_document doc;
	if (!doc.load_file(filePath.c_str()))
	{
		LOG("configuration load failed");
		return;
	}
	auto root = doc.child(L"配置");
	if (root == nullptr)
		return;
	auto xmlIdentity = root.child(L"通用");
	if (xmlIdentity == nullptr)
		return;
	auto wName = StrConvert::UTF8Towstring(newName);
	xmlIdentity.child(L"名字").text().set(wName.c_str());
	doc.save_file(filePath.c_str());
}

std::unique_ptr<NodeConfig> ConfigLoader::load()
{
	LOG("filePath: {}",wstringToUTF8(filePath));
	pugi::xml_document doc;
	if (!doc.load_file(filePath.c_str()))
	{
		LOG("configuration load failed");
		return nullptr;
	}
	auto root = doc.child(L"配置");
	if (root == nullptr)
		return nullptr;
	auto result = make_unique<NodeConfig>();
	auto xmlIdentity = root.child(L"通用");
	if (xmlIdentity)
	{
		*result->mutable_identity() = loadIdentity(xmlIdentity);
	}

	auto xmlWAN = root.child(L"广域");
	if (xmlWAN)
	{
		*result->mutable_wan_conf() = loadWANConfig(xmlWAN);
	}

	auto xmlLAN = root.child(L"本地");
	if (xmlLAN)
	{
		*result->mutable_lan_conf() = loadLANConfig(xmlLAN);
	}
	if (modified)
	{
		doc.save_file(filePath.c_str());
	}
	return result;
}

NodeIdentity ConfigLoader::loadIdentity(pugi::xml_node xmlIdentity)
{
	NodeIdentity identity;
	auto name = xmlIdentity.child(L"名字");
	if (name)
	{
		auto u8Name = StrConvert::wstringToUTF8(name.text().as_string());
		identity.set_node_name(u8Name);
	}

	auto position = xmlIdentity.child(L"位置");
	if (position)
	{
		auto defaultPos = identity.mutable_default_pos();
		defaultPos->set_longitude(position.child(L"经度").text().as_double() / 180 * M_PI);
		defaultPos->set_latitude(position.child(L"纬度").text().as_double() / 180 * M_PI);
		defaultPos->set_altitude(position.child(L"高度").text().as_double());
	}

	auto id = xmlIdentity.child(L"id");
	if (id != nullptr)
	{
		identity.mutable_id()->set_value(id.text().as_uint());
	}
	else
	{
		auto idValue = geneHashcode();
		identity.mutable_id()->set_value(idValue);
		xmlIdentity.append_child(L"id").text().set(idValue);
		modified = true;
	}
	LOG("node id = {:x}", identity.id().value());

	return identity;
}

WANConfig ConfigLoader::loadWANConfig(pugi::xml_node xmlWAN)
{
	WANConfig cfg;
	auto xmlClient = xmlWAN.child(L"请求响应");
	if (xmlClient)
	{
		*cfg.mutable_client() = loadLinkCfg(xmlClient);
	}

	auto xmlSource = xmlWAN.child(L"数据管线");
	if (xmlSource)
	{
		*cfg.mutable_source() = loadLinkCfg(xmlSource);
	}

	auto xmlPub = xmlWAN.child(L"发布");
	if (xmlPub)
	{
		*cfg.mutable_pub() = loadLinkCfg(xmlPub);
	}

	auto xmlSub = xmlWAN.child(L"订阅");
	if (xmlSub)
	{
		*cfg.mutable_sub() = loadLinkCfg(xmlSub);
	}
	return cfg;
}

LANConfig ConfigLoader::loadLANConfig(pugi::xml_node xmlLAN)
{
	LANConfig cfg;
	auto xml3900 = xmlLAN.child(L"Sensor");
	if (xml3900)
	{
		auto deviceConf(loadDeviceCluster(xml3900));
		*cfg.mutable_cluster_3900() = { deviceConf.begin(),  deviceConf.end()};
	}

	auto xmlRadarDF = xmlLAN.child(L"RadarDF");
	if (xmlRadarDF)
	{
		auto deviceConf(loadDeviceCluster(xmlRadarDF));
		*cfg.mutable_cluster_radardf() = { deviceConf.begin(),  deviceConf.end() };
	}
	return cfg;
}

ZeromqLinkCfg ConfigLoader::loadLinkCfg(pugi::xml_node xmlNode)
{
	ZeromqLinkCfg cfg;

	auto xmlName = xmlNode.child(L"名字");
	if (xmlName)
	{
		cfg.set_link_name(StrConvert::wstringToUTF8(xmlName.text().as_string()));
	}

	auto xmlAddress = xmlNode.child(L"地址");
	if (xmlAddress)
	{
		auto address = cfg.mutable_address();
		auto u8Ip = StrConvert::wstringToUTF8(xmlAddress.child(L"ip").text().as_string());
		address->set_ip(u8Ip);
		address->set_port(xmlAddress.child(L"端口").text().as_uint());
	}
	auto xmlHeartbeat = xmlNode.child(L"心跳");
	if (xmlHeartbeat)
	{
		auto heartbeat = cfg.mutable_heartbeat_cfg();
		heartbeat->set_heartbeat_interval(xmlHeartbeat.child(L"间隔").text().as_int());
		heartbeat->set_heartbeat_timeout(xmlHeartbeat.child(L"超时").text().as_int());
		heartbeat->set_heartbeat_ttl(xmlHeartbeat.child(L"TTL").text().as_int());
	}
	auto xmlReconnection = xmlNode.child(L"重连");
	if (xmlReconnection)
	{
		auto reconnection = cfg.mutable_reconnect_cfg();
		reconnection->set_reconnect_interval(xmlReconnection.child(L"间隔").text().as_int());
		reconnection->set_reconnect_interval_max(xmlReconnection.child(L"最大间隔").text().as_int());
	}

	auto xmlSendOption = xmlNode.child(L"发送");
	if (xmlSendOption)
	{
		auto sendOption = cfg.mutable_msg_send_cfg();
		sendOption->set_hwm(xmlSendOption.child(L"高水位").text().as_int());
		sendOption->set_linger(xmlSendOption.child(L"悬挂").text().as_int());
		sendOption->set_timeout(xmlSendOption.child(L"超时").text().as_int());
	}
	return cfg;
}

vector<DeviceConfig> ConfigLoader::loadDeviceCluster(pugi::xml_node xmlNode)
{
	vector<DeviceConfig> cluster;
	for (auto& item : xmlNode.children(L"设备"))
	{
		auto type = parseDeviceType(item.child(L"类型").text().as_string());
		if (type)
		{
			DeviceConfig dc;
			dc.set_type(*type);
			dc.mutable_address()->set_ip(StrConvert::wstringToUTF8(item.child(L"ip").text().as_string()));
			auto xmlPort = item.child(L"端口");
			if (xmlPort != nullptr)
			{
				dc.mutable_address()->set_port(xmlPort.text().as_uint());
			}
			auto xmlId = item.child(L"id");
			if (xmlId != nullptr)
			{
				dc.mutable_id()->set_value(xmlId.text().as_uint());
			}
			cluster.push_back(dc);
		}
	}
	return cluster;
}

unique_ptr<DeviceType> ConfigLoader::parseDeviceType(const std::wstring& strType)
{
	static const vector<wstring> BUILD_IN_TYPE_NAME = {
		L"SENSOR_3900",
		L"RADAR_DF",
		L"CZC_SM",
		L"CZC_DF",
		L"CZC_QJ"
	};
	for (int i = 0; i < BUILD_IN_TYPE_NAME.size(); i++)
	{
		if (BUILD_IN_TYPE_NAME[i] == strType)
		{
			return make_unique<DeviceType>(DeviceType(i));
		}
	}
	return nullptr;
}

