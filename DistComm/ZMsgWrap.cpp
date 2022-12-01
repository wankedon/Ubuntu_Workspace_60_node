/**
 * @file ZMsgWrap.cpp
 * @brief zeromq消息串行化工具
 * @author 装备事业部软件组 杨青 
 * @version 0.1
 * @date 2021-07-13
 * 
 * @copyright Copyright (c) 2021  中国电子科技集团公司第四十一研究所
 * 
 */
#include "pch.h"
#include "ZMsgWrap.h"

using namespace std;

string extractString(zmsg_t* msg)
{
	string result;
	char* str = zmsg_popstr(msg);
	if (str)
	{
		result = str;
		zstr_free(&str);
	}
	return result;
}