#pragma once

class Whiteboard;
class Broker;
class ISharedInfo;

class BroadcastStation
{
public:
	BroadcastStation(const ZeromqLinkCfg& msgBusPubConf, const ZeromqLinkCfg& msgBusSubConf);
	~BroadcastStation();
public:
	void registerTopic(const std::string& topic, std::shared_ptr<ISharedInfo> sinfo);
	void unregisterTopic(const std::string& topic);
	void publish(const std::string& topic, std::shared_ptr<ISharedInfo> sinfo, const TimerParam* timer = nullptr);
private:
	const std::string BUS_NAME;
	std::unique_ptr<Broker> messageBus;	//发布订阅的消息总线
	std::unique_ptr<Whiteboard> whiteboard;	//发布订阅白板
};

