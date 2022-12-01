#include "pch.h"
#include "NodeManager.h"
#include "ClientServer.h"
#include "DeviceManager.h"
#include "TaskManager.h"
#include "Logger.h"
#include "Whiteboard.h"
#include "AbnormalMsgInfo.h"
#include "Miscellaneous.h"
#include "node/nodeServer.pb.h"

using namespace std;

NodeManager::NodeManager(const NodeConfig& cfg, const std::string& path)
	:identity(cfg.identity())
{
	init(cfg, path);
}

NodeManager::~NodeManager()
{
	clientEp.reset();
	taskManager.reset();
	deviceManager.reset();
	whiteboard.reset();
}

std::string NodeManager::nodeName() const
{
	return identity.node_name();
}

void NodeManager::init(const NodeConfig& cfg, const std::string& path)
{
	//����CLIENTͨ�Ŷ˵�
	clientEp = make_unique<ClientEndpoint>(cfg.wan_conf().client());
	//�����豸������
	deviceManager = make_unique<DeviceManager>(cfg.lan_conf());
	//��ȡ�����豸�ĺ�������
	auto func = [this](const DeviceRequest& req) {
		return deviceManager->getIdleDevice(req);
	};
	//�������������
	taskManager = make_unique<TaskManager>(cfg.wan_conf().source(), path, func);
	//Ϊclient�˵����Ӷ�ʱ��������
	clientEp->setTimer(3000, 0, [this]() {onTimer(); });
	clientEp->bindClientMsgHandler([this](MessageExtractor& me) {onServerMsg(me); });

	//�����װ壬��������
	whiteboard = make_unique<Whiteboard>("MessageBus", cfg.wan_conf().pub(), cfg.wan_conf().sub());

	//��������
	setupTopic();	
}

void NodeManager::setupTopic()
{
	abnormalMsgInfo = make_shared<AbnormalMsgInfoClient>(identity.id().value());
	auto subTopic = concatString(list<string>{ "DCTS", "Node", "AbnormalTask", "Notify"}, "#");
	whiteboard->registerTopic(subTopic, abnormalMsgInfo);
	subTopic = concatString(list<string>{ "DCTS", "Node", "Control", "Request"}, "#");
	whiteboard->registerTopic(subTopic, [this](MessageExtractor& me) {onNodeControl(me); });
}

void NodeManager::onTimer()
{
	static const string pubTopic = concatString(list<string>{ "DCTS", "Node", "AbnormalTask", "Modify"}, "#");
	NodeInfo info;
	info.set_name(identity.node_name());
	*info.mutable_id() = identity.id();
	taskManager->onTimer(*abnormalMsgInfo);
	taskManager->updateInfo(info);
	deviceManager->onTimer();
	deviceManager->updateInfo(info);
	MessageBuilder builder;
	builder.serializeToTail(info)
		.push(CSTip::NODE_TICK);
	clientEp->sendToServer(builder);
	whiteboard->publish(pubTopic, abnormalMsgInfo);	//�ù�����Ϣ�����Է���"AbnormalTaskModify"������Ϣ
}

void NodeManager::onServerMsg(MessageExtractor& me)
{
	CSTip tip;
	if (!me.extract(tip))
		return;
	if (tip == NODE_CONTROL)
	{
		onNodeControl(me);
	}
	else if (tip == TASK_REQUEST)
	{
		auto reply = taskManager->onRemoteRequest(std::move(me));
		reply.push(REPLY);
		clientEp->sendToServer(reply);
	}
	else
	{
		LOG("unexpected task request msg");
	}
}

void NodeManager::onNodeControl(MessageExtractor& me)
{
	node::server::NodeControl nodeControl;
	if (!me.deserialize(nodeControl))
		return;
	if (nodeControl.node_id().value() != identity.id().value())
		return;
	auto type = nodeControl.control_type();
	switch (type)
	{
	case node::server::REBOOT_NODE:
		//system("shutdown -r -t 5");	//���windowsϵͳ
		//nodeControl.set_detail("OK");
		break;
	case node::server::SHUTDOWN_NODE:
		//system("shutdown -s -t 5");	//���windowsϵͳ
		//nodeControl.set_detail("OK");
		break;
	case node::server::ABORT_ALL:
		taskManager->taskAbortAll();
		nodeControl.set_detail("OK");
		break;
	case node::server::RENAME:
		if (!nodeControl.detail().empty())
		{
			identity.set_node_name(nodeControl.detail());
		}
		break;
	default:
		nodeControl.set_detail("Error");
		break;
	}
	auto pubTopic = concatString(list<string>{ "DCTS", "Node", "Control", "Reply"}, "#");
	MessageBuilder mb;
	mb.serializeToTop(nodeControl);
	whiteboard->publish(pubTopic, mb);
}