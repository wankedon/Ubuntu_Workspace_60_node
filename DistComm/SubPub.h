#pragma once

#include "ActorBase.h"

class Subscriber : public Actor
{
public:
	Subscriber(const ZeromqLinkCfg& subCfg);
	~Subscriber();
public:
	void subTopic(const std::string& topic, std::function<void(MessageExtractor&)> func);
	void unsubTopic(const std::string& topic);
private:
	void onSubEvent();
private:
	zsock_t* sub;
	std::mutex mapMutex;
	std::map<std::string, std::function<void(MessageExtractor&)>> handlers;
};

class Publisher
{
public:
	Publisher(const ZeromqLinkCfg& pubCfg);
	~Publisher();
public:
	int broadcast(MessageBuilder& mb);
private:
	zsock_t* pub;
	std::mutex mtx;
};


class Broker : public Actor
{
public:
	Broker(const std::string& name, const ZeromqLinkCfg& pubCfg, const ZeromqLinkCfg& subCfg);
	~Broker();
public:
	std::string name() const { return myName; }
	void notifyAll(MessageBuilder& mb);
	void subTopic(const std::string& topic);
	void unsubTopic(const std::string& topic);
private:
	void onSubEvent();
	void onPubEvent();
	void onPipeMsg(MessageExtractor&);
private:
	const std::string myName;
	zsock_t* xsub;
	zsock_t* xpub;
	std::map<std::string, int32_t> status;
};

