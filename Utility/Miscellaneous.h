/**
 * @file Miscellaneous.h
 * @brief 杂项
 * @author 装备事业部软件组 杨青 
 * @version 0.1
 * @date 2021-07-13
 * 
 * @copyright Copyright (c) 2021  中国电子科技集团公司第四十一研究所
 * 
 */
#pragma once

template <class T1, class T2>
inline uint64_t combineId(const T1& id1, const T2& id2)
{
	return (uint64_t)id1.value() << 32 | id2.value();
}

inline uint64_t timestampToUINT64(const Timestamp& ts)
{
	return (uint64_t)ts.seconds() << 32 | ts.nanos();
}

inline Timestamp UINT64ToTimestamp(uint64_t u64Time)
{
	Timestamp ts;
	ts.set_seconds(u64Time >> 32);
	ts.set_nanos(u64Time & 0xFFFFFFFF);
	return ts;
}

inline Timestamp makeTimestamp(uint32_t sec, uint32_t nano)
{
	Timestamp ts;
	ts.set_seconds(sec);
	ts.set_nanos(nano);
	return ts;
}

inline Position makePosition(double ln, double lt, double ht)
{
	Position pos;
	pos.set_altitude(ht);
	pos.set_longitude(ln);
	pos.set_latitude(lt);
	return pos;
}

std::wstring LocateModulePath();

std::string genUUID();
uint32_t getUUID(zuuid_t* id);
uint32_t geneHashcode();

template <class StringContainer>
std::string concatString(const StringContainer& container, const std::string& separator)	//拼接字符串
{
	auto iter = container.begin();
	std::string result = *iter;
	for (++iter; iter != container.end(); ++iter)
	{
		result += (separator + *iter);
	}
	return result;
}
