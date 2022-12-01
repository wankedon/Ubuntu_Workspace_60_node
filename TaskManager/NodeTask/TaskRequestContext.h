#pragma once

#include "ZMsgWrap.h"

class TaskRequestContext
{
public:
	TaskRequestContext(MessageExtractor&& extractor);
	~TaskRequestContext();
public:
	MessageBuilder makeReply();
	bool parsed() const;
	bool isGood() const;
	uint64_t getKey() const;
	CmdHeader cmdHeader() const { return header; }
	void setErrorCode(ErrorType err);
	TaskCmd taskCmdType() const { return cmdType; }
	void setDeviceId(uint32_t deviceId);
	std::string toString() const;
	MessageExtractor& messageExtractor();
public:
	template<class T>
	void serializeToTail(const T& data)
	{
		builder.serializeToTail(data);
	}
	void addString(const std::string& str)
	{
		builder.addString(str);
	}
	template <class T>
	void add(const T& data)
	{
		builder.add(data);
	}
private:
	bool extractDone;
	CmdHeader header;
	TaskCmd cmdType;
	MessageExtractor extractor;
	MessageBuilder builder;
};

