#include "pch.h"
#include "TimeStamp.h"

static const uint32_t NS_IN_1_SEC = 1000000000;
static const uint32_t US_IN_1_SEC = 1000000;
static const uint32_t MS_IN_1_SEC = 1000;



	CTimeStamp::CTimeStamp(void)
		:second(0),
		nanoSecond(0)
	{
	}


	CTimeStamp::~CTimeStamp(void)
	{
	}

	CTimeStamp::CTimeStamp(uint32_t sec, uint32_t nSec)
		:second(sec),
		nanoSecond(nSec)
	{
		assert(nanoSecond < NS_IN_1_SEC);
	}

	CTimeStamp::CTimeStamp(uint64_t time)
	{
		second = (uint32_t)(time / NS_IN_1_SEC);
		nanoSecond = (uint32_t)(time % NS_IN_1_SEC);
	}

	CTimeStamp::CTimeStamp(const Timestamp& ts)
		:second(ts.seconds()),
		nanoSecond(ts.nanos())
	{

	}

	bool CTimeStamp::operator<(const CTimeStamp& rh) const
	{
		if (second == rh.second)
		{
			return nanoSecond < rh.nanoSecond;
		}
		else
		{
			return second < rh.second;
		}
	}

	bool CTimeStamp::operator>(const CTimeStamp& rh) const
	{
		if (second == rh.second)
		{
			return nanoSecond > rh.nanoSecond;
		}
		else
		{
			return second > rh.second;
		}
	}

	bool CTimeStamp::operator==(const CTimeStamp& rh) const
	{
		return (second == rh.second && nanoSecond == rh.nanoSecond);
	}

	bool CTimeStamp::operator<=(const CTimeStamp& rh) const
	{
		return (*this < rh) || (*this == rh);
	}

	bool CTimeStamp::operator>=(const CTimeStamp& rh) const
	{
		return (*this > rh) || (*this == rh);
	}

	CTimeStamp CTimeStamp::operator=(const CTimeStamp& rh)
	{
		second = rh.second;
		nanoSecond = rh.nanoSecond;
		return *this;
	}

	CTimeStamp CTimeStamp::operator+(uint64_t span) const
	{
		uint32_t sec = second + (uint32_t)(span / NS_IN_1_SEC);
		uint32_t nanoSec = nanoSecond + (uint32_t)(span % NS_IN_1_SEC);
		if (nanoSec >= NS_IN_1_SEC)
		{
			sec++;
			nanoSec -= NS_IN_1_SEC;
		}
		return CTimeStamp(sec, nanoSec);
	}

	void CTimeStamp::operator+=(uint64_t span)
	{
		second += (uint32_t)(span / NS_IN_1_SEC);
		nanoSecond += (uint32_t)(span % NS_IN_1_SEC);
		if (nanoSecond >= NS_IN_1_SEC)
		{
			second++;
			nanoSecond -= NS_IN_1_SEC;
		}
	}

	CTimeStamp CTimeStamp::operator-(uint64_t span) const
	{
		if (ToNSec() <= span)
			return CTimeStamp(0, 0);
		uint32_t sec = (uint32_t)(span / NS_IN_1_SEC);
		uint32_t nanoSec = (uint32_t)(span % NS_IN_1_SEC);
		if (nanoSecond >= nanoSec)
		{
			return CTimeStamp(second - sec, nanoSecond - nanoSec);
		}
		else
		{
			return CTimeStamp(second - sec - 1, nanoSecond + NS_IN_1_SEC - nanoSec);
		}
	}

	void CTimeStamp::operator-=(uint64_t span)
	{
		if (ToNSec() <= span)
		{
			second = nanoSecond = 0;
		}
		else
		{
			uint32_t sec = (uint32_t)(span / NS_IN_1_SEC);
			uint32_t nanoSec = (uint32_t)(span % NS_IN_1_SEC);
			second -= sec;
			if (nanoSecond >= nanoSec)
			{
				nanoSecond -= nanoSec;
			}
			else
			{
				second--;
				nanoSecond += (NS_IN_1_SEC - nanoSec);
			}
		}
	}

	int64_t CTimeStamp::operator-(const CTimeStamp& rh) const
	{
		uint64_t timeInNSec = ToNSec();
		if (timeInNSec >= rh.ToNSec())
		{
			return timeInNSec - rh.ToNSec();
		}
		else
		{
			return -(int64_t)(rh.ToNSec() - timeInNSec);
		}
	}

	uint64_t CTimeStamp::ToNSec() const
	{
		return (uint64_t)second * NS_IN_1_SEC + nanoSecond;
	}

	uint64_t CTimeStamp::ToUSec() const
	{
		return (uint64_t)second * US_IN_1_SEC + nanoSecond / 1000;
	}

	uint64_t CTimeStamp::ToMSec() const
	{
		return (uint64_t)second * MS_IN_1_SEC + nanoSecond / 1000000;
	}

	void CTimeStamp::IncSeconds(uint32_t sec)
	{
		second += sec;
	}

	Timestamp CTimeStamp::ToTS() const
	{
		Timestamp ts;
		ts.set_seconds(second);
		ts.set_nanos(nanoSecond);
		return ts;
	}

