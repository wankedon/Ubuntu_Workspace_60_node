#include "pch.h"
#include "ClientServer.h"
#include "ZsockUtil.h"
#include "Logger.h"

using namespace std;
const uint32_t MAX_SEND_FAIL = 50;

ClientEndpoint::ClientEndpoint(const ZeromqLinkCfg& clientCfg)
	:client(nullptr),
	sendfail(0)
{
	auto func = [this, clientCfg]() {
		client = zsock_new(ZMQ_CLIENT);
		assert(client);
		int rc = zsock_connect(client, ZsockUtil::makeConnectionString(clientCfg.address()).c_str());
		ZsockUtil::setSocketOption(client, clientCfg);
		addReader(client, [this]() {onClientEvent(); return 0; });
		auto handler = [this](MessageExtractor& msg) {onPipeMsg(msg); };
		bindActorMsgHandler(handler);
	};
	start(func);
}

ClientEndpoint::~ClientEndpoint()
{
	stop();
	zsock_destroy(&client);
}

int ClientEndpoint::sendToServer(MessageBuilder& mb)
{
	return Actor::sendToFrontend(mb);
}

void ClientEndpoint::bindClientMsgHandler(std::function<void(MessageExtractor&)> func)
{
	clientMsgHandler = func;
}

void ClientEndpoint::onClientEvent()
{
	zframe_t* frame = zframe_recv(client);	//Client仅接收单Frame消息
	if (frame == nullptr)
	{
		LOG("client socket received null frame");
		return;
	}
	auto msg = zmsg_decode(frame);
	zframe_destroy(&frame);
	if (msg == nullptr)
	{
		LOG("frame decode error");
		return;
	}
	MessageExtractor extractor(msg); //帧解码，还原成多帧消息	
	if (clientMsgHandler)
	{
		clientMsgHandler(extractor);
	}
}

void ClientEndpoint::onPipeMsg(MessageExtractor& me)
{
	MessageBuilder builder(me.dup());
	auto rc = builder.sendFrame(client);
	if (rc != 0)
	{
		if (sendfail < MAX_SEND_FAIL)
		{
			sendfail++;
			LOG("client fail to send msg");
		}
		else if (sendfail == MAX_SEND_FAIL)
		{
			sendfail++;
			LOG("client fail to send msg too many times");
		}
		else
			CLOG("client fail to send msg");
	}
	else
	{
		sendfail = 0;
	}
}

ServerEndpoint::ServerEndpoint(const ZeromqLinkCfg& serverCfg)
	:server(nullptr),
	sendfail(0)
{
	auto func = [this, serverCfg]() {
		server = zsock_new(ZMQ_SERVER);
		int rc = zsock_bind(server, "tcp://*:%d", serverCfg.address().port());
		ZsockUtil::setSocketOption(server, serverCfg);
		addReader(server, [this]() {onServerEvent(); return 0; });

		auto handler = [this](MessageExtractor& msg) {onPipeMsg(msg); };
		bindActorMsgHandler(handler);
	};
	start(func);
}

ServerEndpoint::~ServerEndpoint()
{
	stop();
	zsock_destroy(&server);
}

void ServerEndpoint::onServerEvent()
{
	zframe_t* frame = zframe_recv(server);	//SERVER socket 只接受单帧消息
	if (frame == nullptr)
	{
		LOG("server socket received null frame");
		return;
	}
	auto msg = zmsg_decode(frame);
	uint32_t rid = zframe_routing_id(frame);
	zframe_destroy(&frame);
	if (msg == nullptr)
	{
		LOG("frame decode error");
		return;
	}
	MessageExtractor extractor(msg);	//帧解码，形成多帧消息
	if (serverMsgHandler)
	{
		serverMsgHandler(rid, extractor);
	}
}

int ServerEndpoint::sendToClient(uint32_t rid, MessageBuilder& mb)
{
	mb.push(rid);
	return Actor::sendToFrontend(mb);
}

void ServerEndpoint::bindServerMsgHandler(std::function<void(uint32_t, MessageExtractor&)> func)
{
	serverMsgHandler = func;
}

void ServerEndpoint::onPipeMsg(MessageExtractor& extractor)
{
	uint32_t rid;
	if (extractor.extract(rid))
	{
		MessageBuilder builder(extractor.dup());
		auto rc = builder.sendFrame(server, rid);
		if (rc != 0)
		{
			if (sendfail < MAX_SEND_FAIL)
			{
				sendfail++;
				LOG("server fail to send msg to {:x}", rid);
			}
			else if (sendfail == MAX_SEND_FAIL)
			{
				sendfail++;
				LOG("server fail to send msg to {:x} too many times", rid);
			}
			else
				CLOG("server fail to send msg to {:x}", rid);
		}
		else
		{
			sendfail = 0;
		}
	}
}