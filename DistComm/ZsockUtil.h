#pragma once

namespace ZsockUtil
{
	void activeHeartbeat(zsock_t* s, int interval, int timeout, int ttl=0);	//all in ms unit
	void activeReconnect(zsock_t* s, int interval, int maxIvl = 0);
	void setSocketOption(zsock_t* s, const ZeromqLinkCfg& cfg);
	std::string makeConnectionString(const IPv4Address& address);
};

