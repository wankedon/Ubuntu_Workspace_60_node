/**
 * @file StringConv.h
 * @brief 字符串格式转换
 * @author 装备事业部软件组 杨青 
 * @version 0.1
 * @date 2020-12-09
 * 
 * @copyright Copyright (c) 2020  中国电子科技集团公司第四十一研究所
 * 
 */
#pragma once

#include <string>
#include <codecvt>
#include <locale>
#include <vector>

namespace StrConvert {
	// string的编码方式为utf8，则采用：
	std::string wstringToUTF8(const std::wstring& str);
	std::wstring UTF8Towstring(const std::string& str);
	std::string GbkToUTF8(const std::string& str);
	std::string UTF8ToGbk(const std::string& str);
	long strToLong(const std::wstring& str);
	double strToDouble(const std::wstring& str);
	void extractWords(const std::wstring& context, const std::wstring& seps, std::vector<std::wstring>& result);
}

