#include "pch.h"
#include "AbnormalMsgInfo.h"
#include "Logger.h"

using namespace std;

std::unique_ptr<MessageBuilder> AbnormalMsgInfoBase::geneMessage()
{
	lock_guard<mutex> lg(mtx);
	if (syncInfo.task_msg_id_size() > 0)
	{
		auto mb = make_unique<MessageBuilder>();
		mb->serializeToTail(syncInfo);
		syncInfo.clear_task_msg_id();
		return mb;
	}
	else
	{
		return nullptr;
	}
}

AbnormalMsgRecord AbnormalMsgInfoBase::receivedRecord()
{
	lock_guard<mutex> lg(mtx);
	return syncInfo;
}

size_t AbnormalMsgInfoBase::count()
{
	lock_guard<mutex> lg(mtx);
	return syncInfo.task_msg_id_size();
}


AbnormalMsgInfoClient::AbnormalMsgInfoClient(uint32_t nid)
	:nid(nid)
{

}

void AbnormalMsgInfoClient::synchronize(MessageExtractor& me)
{
	AbnormalMsgRecord newMsg;
	if (me.deserialize(newMsg))
	{
		lock_guard<mutex> lg(mtx);
		syncInfo.clear_task_msg_id();
		for (auto& r : newMsg.task_msg_id())
		{
			if (r.nid() == nid)
			{
				*syncInfo.add_task_msg_id() = r;
			}
		}
	}
}


bool isEqual(const TaskMsgId& lhs, const TaskMsgId& rhs)
{
	if (lhs.tid() != rhs.tid())
		return false;
	if (lhs.nid() != rhs.nid())
		return false;
	if (lhs.did() != rhs.did())
		return false;
	return true;
}

bool remove(AbnormalMsgRecord& base, const TaskMsgId& idToRemove)
{
	bool removed = false;
	auto prevMsgs = base.mutable_task_msg_id();
	for (int i = 0; i < prevMsgs->size(); i++)
	{
		if (isEqual(prevMsgs->at(i), idToRemove))
		{
			prevMsgs->DeleteSubrange(i, 1);
			removed = true;
			break;
		}
	}
	return removed;
}


void AbnormalMsgInfoServer::add(const TaskMsgId& mid)
{
	lock_guard<mutex> lg(mtx);
	for (auto& item : syncInfo.task_msg_id())
	{
		if (isEqual(item, mid))
			return;
	}
	*syncInfo.add_task_msg_id() = mid;
	CLOG("add abnormal task {:x} msg id", mid.tid());
}

void AbnormalMsgInfoServer::synchronize(MessageExtractor& me)
{
	AbnormalMsgRecord newMsg;
	if (me.deserialize(newMsg))
	{
		lock_guard<mutex> lg(mtx);
		for (auto& item : newMsg.task_msg_id())
		{
			if (remove(syncInfo, item))
			{
				CLOG("remove abnormal task {:x} msg id", item.tid());
			}
		}
	}
}

