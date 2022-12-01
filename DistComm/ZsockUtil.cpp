#include "pch.h"
#include "ZsockUtil.h"
#if _MSC_VER > 1900
#include "fmt_/core.h"
#else
#include "fmt/core.h"
#endif // _MSC_VER > 1900



namespace ZsockUtil
{
	void activeHeartbeat(zsock_t* s, int interval, int timeout, int ttl)
	{
		zsock_set_heartbeat_ivl(s, interval);
		zsock_set_heartbeat_timeout(s, timeout);
		zsock_set_heartbeat_ttl(s, ttl);
	}

	void activeReconnect(zsock_t* s, int interval, int maxIvl)
	{
		zsock_set_reconnect_ivl(s, interval);
		zsock_set_reconnect_ivl_max(s, maxIvl);
	}

	void setSocketOption(zsock_t* s, const ZeromqLinkCfg& cfg)
	{
		if (cfg.has_heartbeat_cfg())
		{
			auto& hbcfg = cfg.heartbeat_cfg();
			activeHeartbeat(
				s,
				hbcfg.heartbeat_interval(),
				hbcfg.heartbeat_timeout(),
				hbcfg.heartbeat_ttl()
			);
		}
		if (cfg.has_reconnect_cfg())
		{
			auto& rccfg = cfg.reconnect_cfg();
			activeReconnect(
				s,
				rccfg.reconnect_interval(),
				rccfg.reconnect_interval_max()
			);
		}
		if (cfg.has_msg_send_cfg())
		{
			auto& sendCfg = cfg.msg_send_cfg();
			zsock_set_sndhwm(s, sendCfg.hwm());
			zsock_set_linger(s, sendCfg.linger());
			zsock_set_sndtimeo(s, sendCfg.timeout());
		}
	}

	std::string makeConnectionString(const IPv4Address& address)
	{
		return fmt::format("tcp://{}:{}", address.ip(), address.port());
	}
}
