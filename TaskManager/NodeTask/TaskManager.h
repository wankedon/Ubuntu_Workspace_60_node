#pragma once

#include "ZMsgWrap.h"
#include "Miscellaneous.h"
#include "TaskCreator.h"

class StreamSource;
class TaskCreator;
class NodeTask;
class Device;
class AbnormalMsgInfoClient;
class TaskRequestContext;

class TaskManager
{
	friend class NodeManager;
public:
	TaskManager(const ZeromqLinkCfg& sourceCfg, const std::string& path, DeviceRequestFunc);
	~TaskManager();
private:
	//约束：TaskManager所有方法调用的唯一上下文环境是NodeManager的后台线程
	void onTimer(AbnormalMsgInfoClient& abnormalMsgInfo);
	void updateInfo(NodeInfo& info) const;
	MessageBuilder onRemoteRequest(MessageExtractor&&);
	void taskAbortAll();
private:
	void taskStart(TaskRequestContext& context);
	void taskCmd(TaskRequestContext& context);
	void taskStop(TaskRequestContext& context);
private:
	std::unique_ptr<StreamSource> dataSender;
	std::unique_ptr<TaskCreator> taskCreator;								///< 任务工厂
	std::map<uint64_t, std::shared_ptr<NodeTask>> tasks;		///< 当前的任务集合
};

