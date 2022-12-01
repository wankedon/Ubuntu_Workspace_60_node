#pragma once


	class CTimeStamp
	{
	public:
		CTimeStamp(void);
		CTimeStamp(uint32_t sec, uint32_t nSec);
		CTimeStamp(uint64_t time);
		CTimeStamp(const Timestamp& ts);
		~CTimeStamp(void);
	public:
		bool operator<(const CTimeStamp& rh) const;
		bool operator>(const CTimeStamp& rh) const;
		bool operator<=(const CTimeStamp& rh) const;
		bool operator>=(const CTimeStamp& rh) const;
		bool operator==(const CTimeStamp& rh) const;
		void operator-=(uint64_t span);
		void operator+=(uint64_t span);
		CTimeStamp operator-(uint64_t span) const;
		CTimeStamp operator=(const CTimeStamp& rh);
		CTimeStamp operator+(uint64_t span) const;
		int64_t operator-(const CTimeStamp& rh) const;
		uint64_t ToNSec() const;
		uint64_t ToUSec() const;
		uint64_t ToMSec() const;
		Timestamp ToTS() const;
		void IncSeconds(uint32_t sec);
	private:
		uint32_t second;
		uint32_t nanoSecond;
	};

