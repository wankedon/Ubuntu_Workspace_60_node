/**
 * @file StringConv.cpp
 * @brief 字符串格式转换
 * @author 装备事业部软件组 杨青 
 * @version 0.1
 * @date 2020-12-09
 * 
 * @copyright Copyright (c) 2020  中国电子科技集团公司第四十一研究所
 * 
 */
#include "pch.h"
#include "StringConv.h"
using namespace std;

#ifdef _WIN32
#include <windows.h>
#endif

namespace StrConvert {
	// string的编码方式为utf8，则采用：
	std::string wstringToUTF8(const std::wstring& str)
	{
		static std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
		return strCnv.to_bytes(str);
	}

	std::wstring UTF8Towstring(const std::string& str)
	{
		static std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;
		return strCnv.from_bytes(str);
	}

#ifdef _WIN32
	string GbkToUTF8(const string& str)
	{
		const char* src_str = str.c_str();
		int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0);
		wchar_t* wstr = new wchar_t[len + 1];
		memset(wstr, 0, len + 1);
		MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
		len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
		char* convStr = new char[len + 1];
		memset(convStr, 0, len + 1);
		WideCharToMultiByte(CP_UTF8, 0, wstr, -1, convStr, len, NULL, NULL);
		string strTemp = convStr;
		if (wstr) delete[] wstr;
		if (convStr) delete[] convStr;
		return strTemp;
	}

	string UTF8ToGbk(const string& str)
	{
		const char* src_str = str.c_str();
		int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
		wchar_t* wszGBK = new wchar_t[len + 1];
		memset(wszGBK, 0, len * 2 + 2);
		MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
		len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
		char* szGBK = new char[len + 1];
		memset(szGBK, 0, len + 1);
		WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
		string strTemp(szGBK);
		if (wszGBK) delete[] wszGBK;
		if (szGBK) delete[] szGBK;
		return strTemp;
	}
#else
#include <iconv.h>

	int GbkToUtf8(char* str_str, size_t src_len, char* dst_str, size_t dst_len)
	{
		iconv_t cd;
		char** pin = &str_str;
		char** pout = &dst_str;

		cd = iconv_open("utf8", "gbk");
		if (cd == 0)
			return -1;
		memset(dst_str, 0, dst_len);
		if (iconv(cd, pin, &src_len, pout, &dst_len) == -1)
			return -1;
		iconv_close(cd);
		**pout = '\0';

		return 0;
	}

	int Utf8ToGbk(char* src_str, size_t src_len, char* dst_str, size_t dst_len)
	{
		iconv_t cd;
		char** pin = &src_str;
		char** pout = &dst_str;

		cd = iconv_open("gbk", "utf8");
		if (cd == 0)
			return -1;
		memset(dst_str, 0, dst_len);
		if (iconv(cd, pin, &src_len, pout, &dst_len) == -1)
			return -1;
		iconv_close(cd);
		**pout = '\0';

		return 0;
	}
#endif

	long strToLong(const wstring& str)
	{
		if (str.length() == 0)
			return 0;
		wchar_t* stopstring;
		int base = 10;
		return wcstol(str.c_str(), &stopstring, base);
	}

	double strToDouble(const wstring& str)
	{
		if (str.length() == 0)
			return 0;
		wchar_t* stopstring;
		return wcstod(str.c_str(), &stopstring);
	}

	void extractWords(const wstring& context, const wstring& seps, vector<wstring>& result)
	{
		wstring::size_type startPos = 0;
		wstring::size_type stopPos = context.find_first_of(seps, startPos);
		while (stopPos != string::npos)
		{
			result.push_back(context.substr(startPos, stopPos - startPos));
			startPos = stopPos + 1;
			stopPos = context.find_first_of(seps, startPos);
		}
	}
}