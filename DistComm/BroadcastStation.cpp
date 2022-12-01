#include "pch.h"
#include "BroadcastStation.h"
#include "SubPub.h"
#include "Whiteboard.h"

using namespace std;

BroadcastStation::BroadcastStation(const ZeromqLinkCfg& msgBusPubConf, const ZeromqLinkCfg& msgBusSubConf)
	:BUS_NAME("DCTS"),
	 messageBus(make_unique<Broker>(BUS_NAME, msgBusPubConf, msgBusSubConf)),
	 whiteboard(make_unique<Whiteboard>(BUS_NAME, msgBusPubConf, msgBusSubConf))
{
	messageBus->subTopic("");	//broker转发所有消息
}

BroadcastStation::~BroadcastStation()
{
	whiteboard.reset();
	messageBus.reset();
}

void BroadcastStation::registerTopic(const std::string& topic, std::shared_ptr<ISharedInfo> sinfo)
{
	whiteboard->registerTopic(topic, sinfo);
}

void BroadcastStation::unregisterTopic(const std::string& topic)
{
	whiteboard->unregisterTopic(topic);
}

void BroadcastStation::publish(const std::string& topic, std::shared_ptr<ISharedInfo> sinfo, const TimerParam* timer)
{
	whiteboard->publish(topic, sinfo, timer);
}