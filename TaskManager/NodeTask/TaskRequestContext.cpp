#include "pch.h"
#include "TaskRequestContext.h"
#include "Miscellaneous.h"
#if _MSC_VER > 1900
#include "fmt_/core.h"
#else
#include "fmt/core.h"
#endif // _MSC_VER > 1900

TaskRequestContext::TaskRequestContext(MessageExtractor&& me)
	:extractDone(true),
	extractor(std::move(me))
{
	extractDone &= extractor.deserialize(header);
	extractDone &= extractor.extract(cmdType);
	if (!extractDone)
	{
		header.set_error_code(ERR_DATA_TYPE);
	}
}

TaskRequestContext::~TaskRequestContext()
{

}

MessageBuilder TaskRequestContext::makeReply()
{
	builder.serializeToTop(header);
	return std::move(builder);
}

bool TaskRequestContext::parsed() const
{
	return extractDone;
}

bool TaskRequestContext::isGood() const
{
	return header.error_code() == ERR_NONE;
}

uint64_t TaskRequestContext::getKey() const
{
	return combineId(header.task_id(), header.task_runner().device_id());
}

void TaskRequestContext::setErrorCode(ErrorType err)
{
	header.set_error_code(err);
}

void TaskRequestContext::setDeviceId(uint32_t deviceId)
{
	header.mutable_task_runner()->mutable_device_id()->set_value(deviceId);
}

std::string TaskRequestContext::toString() const
{
	return fmt::format("seq {}, task_id {:x}, device_id {:x}, error_code {}", header.sequence_number(), header.task_id().value(), header.task_runner().device_id().value(), header.error_code());
}

MessageExtractor& TaskRequestContext::messageExtractor()
{
	return extractor;
}
