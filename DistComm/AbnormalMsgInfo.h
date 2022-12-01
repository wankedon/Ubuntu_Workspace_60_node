#pragma once
#include "SharedInfo.h"

class AbnormalMsgInfoBase : public ISharedInfo
{
public:
	std::unique_ptr<MessageBuilder> geneMessage() override;
	AbnormalMsgRecord receivedRecord();
	size_t count();
protected:
	AbnormalMsgRecord syncInfo;
	std::mutex mtx;
};

class AbnormalMsgInfoClient : public AbnormalMsgInfoBase
{
public:
	AbnormalMsgInfoClient(uint32_t nid);
public:
	void synchronize(MessageExtractor& me) override;
private:
	const uint32_t nid;
};

class AbnormalMsgInfoServer : public AbnormalMsgInfoBase
{
public:
	void add(const TaskMsgId& id);
	void synchronize(MessageExtractor& me) override;
};

bool isEqual(const TaskMsgId& lhs, const TaskMsgId& rhs);
bool remove(AbnormalMsgRecord& base, const TaskMsgId& idToRemove);
