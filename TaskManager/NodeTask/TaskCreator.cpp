#include "pch.h"
#include "TaskCreator.h"
#include "NodeTask.h"
#include "Device.h"
#include "NodeTaskFactory.h"

using namespace std;

/**
 * @brief Construct a new Task Creator:: Task Creator object
 * 
 * @param sendFunc ������Ϣ������
 */
TaskCreator::TaskCreator(DeviceRequestFunc drf, TaskDataSendFunc tdsf)
	:deviceFindFunc(drf),
	msgSendFunc(tdsf)
{
}


TaskCreator::~TaskCreator() = default;

/**
 * @brief ���Ÿ�taskManager�����񴴽��ӿ�
 * 
 * @param header ������Ϣ
 * @param type Ҫ��������������
 * @return shared_ptr<MeasTask> 
 */
shared_ptr<NodeTask> TaskCreator::createTask(const CmdHeader& header, TaskType type)
{
	NodeTask::TaskInitEntry entry{ header, type, msgSendFunc, nullptr };
	shared_ptr<NodeTask> task = nullptr;
	for (auto& factory : factories)
	{
		task = factory->createTask(entry, deviceFindFunc);
		if (task)
			break;
	}
	return task;
}

void TaskCreator::addFactory(std::shared_ptr<NodeTaskFactory> factory)
{
	factories.push_back(factory);
}