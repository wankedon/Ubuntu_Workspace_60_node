#include "pch.h"
#include "SubPub.h"
#include "Logger.h"
#include "ZMsgWrap.h"
#include "ZsockUtil.h"

using namespace std;

Subscriber::Subscriber(const ZeromqLinkCfg& subCfg)
{
	auto func = [this, subCfg]() {
		sub = zsock_new(ZMQ_SUB);
		assert(sub);
		int rc = zsock_connect(sub, ZsockUtil::makeConnectionString(subCfg.address()).c_str());
		ZsockUtil::setSocketOption(sub, subCfg);
		addReader(sub, [this]() {onSubEvent(); return 0; });
	};
	start(func);
}

Subscriber::~Subscriber()
{
	stop();
	zsock_destroy(&sub);
}

void Subscriber::subTopic(const string& topic, std::function<void(MessageExtractor&)> func)
{
	std::lock_guard<mutex> lg(mapMutex);
	zsock_set_subscribe(sub, topic.c_str());
	handlers[topic] = func;
}

void Subscriber::unsubTopic(const string& topic)
{
	std::lock_guard<mutex> lg(mapMutex);
	zsock_set_unsubscribe(sub, topic.c_str());
	handlers.erase(topic);
}

void Subscriber::onSubEvent()
{
	auto extractor = MessageExtractor::createFromReceivedMsg(sub);
	if (extractor == nullptr)
		return;
	std::lock_guard<mutex> lg(mapMutex);
	auto topic = extractor->popString();
	auto iter = handlers.find(topic);
	if (iter != handlers.end())
	{
		iter->second(*extractor);
	}
}

Publisher::Publisher(const ZeromqLinkCfg& pubCfg)
{
	pub = zsock_new(ZMQ_PUB);
	assert(pub);
	int rc = zsock_connect(pub, ZsockUtil::makeConnectionString(pubCfg.address()).c_str());
	ZsockUtil::setSocketOption(pub, pubCfg);
}

Publisher::~Publisher()
{
	zsock_destroy(&pub);
}

int Publisher::broadcast(MessageBuilder& mb)
{
	std::lock_guard<mutex> lg(mtx);
	return mb.send(pub);
}

Broker::Broker(const string& n, const ZeromqLinkCfg& pubCfg, const ZeromqLinkCfg& subCfg)
	:myName(n),
	xsub(nullptr),
	xpub(nullptr)
{
	auto initFunc = [this, pubCfg, subCfg]() {
		xsub = zsock_new(ZMQ_XSUB);
		assert(xsub);
		xpub = zsock_new(ZMQ_XPUB);
		assert(xpub);
		int rc = zsock_bind(xsub, "tcp://*:%d", subCfg.address().port());
		rc = zsock_bind(xpub, "tcp://*:%d", pubCfg.address().port());
		zsock_set_xpub_welcome_msg(xpub, myName.c_str());
		zsock_set_xpub_verboser(xpub, 1);
		ZsockUtil::setSocketOption(xsub, subCfg);
		ZsockUtil::setSocketOption(xpub, pubCfg);
		auto subFunc = [this]() {onSubEvent(); return 0; };
		auto pubFunc = [this]() {onPubEvent(); return 0; };

		addReader(xsub, subFunc);
		addReader(xpub, pubFunc);
		bindActorMsgHandler([this](MessageExtractor& me) {onPipeMsg(me); });
	};
	start(initFunc);
}

Broker::~Broker()
{
	stop();
	zsock_destroy(&xsub);
	zsock_destroy(&xpub);
}

void Broker::subTopic(const string& topic)
{
	vector<char> buffer(1);
	buffer[0] = 1;
	buffer.insert(buffer.end(), topic.begin(), topic.end());
	auto f = zframe_new(buffer.data(), buffer.size());
	int rc = zframe_send(&f, xsub, 0);
}

void Broker::unsubTopic(const string& topic)
{
	vector<char> buffer(1);
	buffer[0] = 0;
	buffer.insert(buffer.end(), topic.begin(), topic.end());
	auto f = zframe_new(buffer.data(), buffer.size());
	int rc = zframe_send(&f, xsub, 0);
}

void Broker::notifyAll(MessageBuilder& mb)
{
	sendToFrontend(mb);
}

void Broker::onSubEvent()
{
	auto msg = zmsg_recv(xsub);
	MessageBuilder mb(msg);
	mb.send(xpub);
}

void Broker::onPipeMsg(MessageExtractor& me)
{
	MessageBuilder mb(me.dup());
	mb.pushString(myName);
	mb.send(xpub);
}

void Broker::onPubEvent()
{
	auto f = zframe_recv(xpub);
	if (f == nullptr)
		return;
	if (zframe_size(f) == 0)
		return;
	vector<char> buffer(zframe_size(f));
	memcpy(buffer.data(), zframe_data(f), zframe_size(f));
	zframe_destroy(&f);
	auto onoff = buffer.front();
	string topic(buffer.begin() + 1, buffer.end());
	if (onoff)
	{
		status[topic] += 1;
		CLOG("'{}' subscriber join", topic);
	}
	else
	{
		status[topic] -= 1;
		CLOG("'{}' subscriber leave", topic);
		if (status[topic] <= 0)
		{
			status.erase(topic);
			CLOG("'{}' erased", topic);
		}
	}
}
