#pragma once

#include "TaskCreator.h"
#include "NodeTask.h"

class NodeTaskFactory
{
public:
	NodeTaskFactory() = default;
	~NodeTaskFactory() = default;
public:
	virtual std::shared_ptr<NodeTask> createTask(NodeTask::TaskInitEntry&, const DeviceRequestFunc&) = 0;
};

