#pragma once

#include "SubPub.h"

class ISharedInfo;

class Whiteboard
{
public:
	Whiteboard(const std::string& msgBusName, const ZeromqLinkCfg& msgBusPubConf, const ZeromqLinkCfg& msgBusSubConf);
	~Whiteboard();
public:
	void registerTopic(const std::string& topic, std::shared_ptr<ISharedInfo> sinfo);
	void registerTopic(const std::string& topic, std::function<void(MessageExtractor&)> func);
	void unregisterTopic(const std::string& topic);
	void publish(const std::string& topic, std::shared_ptr<ISharedInfo> sinfo, const TimerParam* timer = nullptr);
	void publish(const std::string& topic, MessageBuilder& mb);
private:
	bool isPeerMsg(const std::string& msgAddress);
	void onBusMessage(MessageExtractor& me);
	bool extractHeaderAndCheck(MessageExtractor& me);
	void sendMsgByPub(const std::string& topic, std::shared_ptr<ISharedInfo> sinfo);
private:
	const std::string address;
	std::unique_ptr<Subscriber> sub;
	std::unique_ptr<Publisher> pub;
};

