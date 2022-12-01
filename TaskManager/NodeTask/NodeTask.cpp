#include "pch.h"
#include "NodeTask.h"
#include "Device.h"
#include "Logger.h"

NodeTask::NodeTask(const TaskInitEntry& entry)
	:nd(entry.header.task_runner()),
	 sendFunctor(entry.sender),
	 device(entry.device)
{
	summary.mutable_starttime()->set_seconds(time(nullptr));
	*summary.mutable_id() = entry.header.task_id();
	*summary.mutable_type() = entry.type;
	summary.set_status(TaskStatus::T_READY);
	if (device)
	{
		*summary.mutable_device() = device->id();
		device->engageWithTask(taskId());
	}
}

NodeTask::~NodeTask()
{
	if (device)
	{
		device->disengage();	//释放设备资源
	}
	CLOG("taks type:{}-{} id:{:x} deleted", 
		summary.type().pri_task_type(), 
		summary.type().sec_task_type(),
		summary.id().value());
}

void NodeTask::config(TaskRequestContext& context)
{
	context.setErrorCode(ERR_NONE);
}

ErrorType NodeTask::start()
{
	return ERR_NONE;
}

void NodeTask::onCmd(TaskRequestContext& context)
{
	context.setErrorCode(ERR_NOTIMPLEMENTED);
}

bool NodeTask::isMyType(const TaskType& type) const
{
	return (type.pri_task_type() == summary.type().pri_task_type() &&
		type.sec_task_type() == summary.type().sec_task_type());
}

TaskSummary NodeTask::getSummary() const
{
	return summary;
}

bool NodeTask::canErase(const AbnormalMsgRecord& record) const
{
	if (summary.status() == TaskStatus::T_ABORT || summary.status() == TaskStatus::T_FINISH)
		return true;
	for (auto& msgId : record.task_msg_id())	//遍历服务器端的异常消息id，看是否是自己发出去的
	{
		if (isMyMessage(msgId))
			return true;
	}
	return false;
}

int NodeTask::sendTaskData(MessageBuilder& builder)
{
	if (sendFunctor)
	{
		builder.serializeToTop(nd)
			.serializeToTop(summary.id());
		return sendFunctor(builder);
	}
	else
	{
		return -1;
	}
}

void NodeTask::setTaskStatus(TaskStatus status)
{
	summary.set_status(status);
}

bool NodeTask::isMyMessage(const TaskMsgId& msgId) const
{
	if (msgId.nid() != nd.node_id().value())
		return false;
	if (msgId.tid() != taskId().value())
		return false;
	if (msgId.did() != nd.device_id().value())
		return false;
	return true;
}
