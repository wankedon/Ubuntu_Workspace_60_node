#pragma once

#include "node/nodeInternal.pb.h"
#include "ZMsgWrap.h"

class ClientEndpoint;
class DeviceManager;
class TaskManager;
class Device;
class Whiteboard;
class AbnormalMsgInfoClient;

class NodeManager
{
public:
	NodeManager(const NodeConfig& cfg, const std::string& path);
	~NodeManager();
public:
	std::string nodeName() const;
private:
	void init(const NodeConfig& cfg, const std::string& path);
	void onTimer();
	void onServerMsg(MessageExtractor& me);
private:
	void onNodeControl(MessageExtractor& me);
	void setupTopic();
private:
	mutable NodeIdentity identity;
	std::unique_ptr<ClientEndpoint> clientEp;
	std::unique_ptr<TaskManager> taskManager;
	std::unique_ptr<DeviceManager> deviceManager;
	std::unique_ptr<Whiteboard> whiteboard;
	std::shared_ptr<AbnormalMsgInfoClient> abnormalMsgInfo;
};

