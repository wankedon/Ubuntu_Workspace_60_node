#include "pch.h"
#include "PushPull.h"
#include "ZsockUtil.h"

using namespace std;

StreamSource::StreamSource(const ZeromqLinkCfg& sourceCfg)
	:push(nullptr),
	send_fail_times(0),
	sended_message_count(0),
	block_times(0)
{
	push = zsock_new(ZMQ_PUSH);
	assert(push);
	int rc = zsock_connect(push, ZsockUtil::makeConnectionString(sourceCfg.address()).c_str());
	
	ZsockUtil::setSocketOption(push, sourceCfg);
}

StreamSource::~StreamSource()
{
	zsock_destroy(&push);
}

int StreamSource::send(const MessageBuilder& mb)
{
	int rc = -1;
	if (!streamLock.try_lock_for(chrono::seconds(1)))
	{
		send_fail_times++;
		return rc;
	}
	if (push != nullptr)
	{
		rc = mb.send(push);
		if (rc == 0)
		{
			sended_message_count++;
		}
		else
		{
			block_times++;
		}
	}
	streamLock.unlock();
	if (rc != 0)
	{
		send_fail_times++;
	}
	return rc;
}

StreamSink::StreamSink(const ZeromqLinkCfg& sinkCfg)
	:pull(nullptr),
	msgHandler(nullptr)
{
	auto func = [this, sinkCfg]() {
		pull = zsock_new(ZMQ_PULL);
		int rc = zsock_bind(pull, "tcp://*:%d", sinkCfg.address().port());
		ZsockUtil::setSocketOption(pull, sinkCfg);
		addReader(pull, [this]() {onStreamEvent(); return 0; });
	};
	start(func);
}

StreamSink::~StreamSink()
{
	stop();
	zsock_destroy(&pull);
}

void StreamSink::bindStreamMsgHandler(std::function<void(MessageExtractor&)> handler)
{
	msgHandler = handler;
}

void StreamSink::onStreamEvent()
{
	auto extractor = MessageExtractor::createFromReceivedMsg(pull);
	if (extractor && msgHandler)
	{
		msgHandler(*extractor);
	}
}