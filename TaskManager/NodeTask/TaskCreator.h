/**
 * @file TaskCreator.h
 * @brief 任务创建
 * @author 装备事业部软件组 杨青 
 * @version 0.1
 * @date 2020-12-03
 * 
 * @copyright Copyright (c) 2020  中国电子科技集团公司第四十一研究所
 * 
 */
#pragma once
/**
 * @brief 任务创建器
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
	TaskDataSendFunc msgSendFunc;	///<任务结果发送器，由taskManager在creator构造时传入
	std::list<std::shared_ptr<NodeTaskFactory>> factories;
};

