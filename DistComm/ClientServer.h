#pragma once
#include "ActorBase.h"

class ClientEndpoint : public Actor
{
public:
	ClientEndpoint(const ZeromqLinkCfg& clientCfg);
	~ClientEndpoint();
public:
	int sendToServer(MessageBuilder& mb);
	void bindClientMsgHandler(std::function<void(MessageExtractor&)> func);
private:
	void onClientEvent();
	void onPipeMsg(MessageExtractor&);
private:
	std::function<void(MessageExtractor&)> clientMsgHandler;
	zsock_t* client;
	uint32_t sendfail;
};

class ServerEndpoint : public Actor
{
public:
	ServerEndpoint(const ZeromqLinkCfg& serverCfg);
	~ServerEndpoint();
public:
	int sendToClient(uint32_t rid, MessageBuilder& mb);
	void bindServerMsgHandler(std::function<void(uint32_t, MessageExtractor&)> func);
private:
	void onServerEvent();
	void onPipeMsg(MessageExtractor&);
private:
	std::function<void(uint32_t, MessageExtractor&)> serverMsgHandler;
	zsock_t* server;
#ifdef PLATFROM_WINDOWS
	UINT sendfail;
#else
	uint32_t sendfail;
#endif 
};

