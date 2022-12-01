/**
 * @file TaskCreator.h
 * @brief ���񴴽�
 * @author װ����ҵ������� ���� 
 * @version 0.1
 * @date 2020-12-03
 * 
 * @copyright Copyright (c) 2020  �й����ӿƼ����Ź�˾����ʮһ�о���
 * 
 */
#pragma once
/**
 * @brief ���񴴽���
 * 
 */
class MessageBuilder;
class NodeTask;
class Device;
class NodeTaskFactory;
using DeviceRequest = std::pair<DeviceId, DeviceType>;
using DeviceRequestFunc = std::function<std::shared_ptr<Device>(DeviceRequest)>;
using TaskDataSendFunc = std::function<int(MessageBuilder&)>;

class TaskCreator
{
public:
	TaskCreator(DeviceRequestFunc, TaskDataSendFunc);
	~TaskCreator();
public:
	std::shared_ptr<NodeTask> createTask(const CmdHeader& header, TaskType type);
	void addFactory(std::shared_ptr<NodeTaskFactory> factory);
private:
	DeviceRequestFunc deviceFindFunc;
	TaskDataSendFunc msgSendFunc;	///<����������������taskManager��creator����ʱ����
	std::list<std::shared_ptr<NodeTaskFactory>> factories;
};

