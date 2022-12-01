#pragma once

#include "TaskRequestContext.h"

class Device;
class NodeTask
{
public:
	/**
	 * @brief 任务初始化参数
	 *
	 */
	struct TaskInitEntry
	{
		CmdHeader header;	///< 执行者信息 
		TaskType type;		///< 任务类型
		std::function<int(MessageBuilder&)> sender; ///< 任务结果发送工具
		std::shared_ptr<Device> device;
	};
public:
	virtual ~NodeTask();
public:
	bool isMyType(const TaskType& type) const;
	bool canErase(const AbnormalMsgRecord& record) const;
	DeviceId deviceId() const { return summary.device(); }
	TaskId taskId() const { return summary.id(); }
	TaskSummary getSummary() const;
public:
	virtual void config(TaskRequestContext& context);
	virtual ErrorType start();
	virtual void onCmd(TaskRequestContext& context);
	virtual void stop() = 0;	//该方法应保证幂等性
protected:
	NodeTask(const TaskInitEntry& entry);
	int sendTaskData(MessageBuilder& builder);
	void setTaskStatus(TaskStatus status);
	template <class DeviceT>
	std::shared_ptr<DeviceT> downcast()
	{
		return std::dynamic_pointer_cast<DeviceT>(device);
	}
	NodeDevice taskRunner() const { return nd; }
private:
	bool isMyMessage(const TaskMsgId&) const;
private:
	const NodeDevice nd;
	TaskSummary summary;		///< 任务状态
	std::function<int(MessageBuilder&)> sendFunctor;	///< 发送工具
	std::shared_ptr<Device> device;
};

