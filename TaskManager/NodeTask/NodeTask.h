#pragma once

#include "TaskRequestContext.h"

class Device;
class NodeTask
{
public:
	/**
	 * @brief �����ʼ������
	 *
	 */
	struct TaskInitEntry
	{
		CmdHeader header;	///< ִ������Ϣ 
		TaskType type;		///< ��������
		std::function<int(MessageBuilder&)> sender; ///< ���������͹���
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
	virtual void stop() = 0;	//�÷���Ӧ��֤�ݵ���
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
	TaskSummary summary;		///< ����״̬
	std::function<int(MessageBuilder&)> sendFunctor;	///< ���͹���
	std::shared_ptr<Device> device;
};

