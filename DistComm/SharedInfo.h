#pragma once

#include "ZMsgWrap.h"

class ISharedInfo
{
public:
	virtual std::unique_ptr<MessageBuilder> geneMessage() = 0;
	virtual void synchronize(MessageExtractor& me) = 0;
};
