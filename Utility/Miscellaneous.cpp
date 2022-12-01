/**
 * @file Miscellaneous.cpp
 * @brief zeromq��Ϣ���л�����
 * @author װ����ҵ�������� ���� 
 * @version 0.1
 * @date 2021-07-13
 * 
 * @copyright Copyright (c) 2021  �й����ӿƼ����Ź�˾����ʮһ�о���
 * 
 */
#include "pch.h"
#include "Miscellaneous.h"

using namespace std;

wstring LocateModulePath()
{
#ifdef PLATFROM_WINDOWS
	TCHAR szbufPath[MAX_PATH] = TEXT("");
	TCHAR szLongPath[MAX_PATH] = TEXT("");
	::GetModuleFileName(NULL, szbufPath, MAX_PATH);
	::GetLongPathName(szbufPath, szLongPath, MAX_PATH);
	wstring fileName(szLongPath);
	wstring::size_type pos = fileName.find_last_of(L"\\");
	return fileName.substr(0, pos);
#endif 
}

uint32_t getUUID(zuuid_t* id)
{
	const byte* data = zuuid_data(id);
	vector<uint32_t> copyTo(zuuid_size(id) >> 2);
	memcpy(copyTo.data(), data, copyTo.size() * sizeof(uint32_t));
	return (copyTo[0] ^ ((copyTo[1] << 16) | copyTo[2])) ^ copyTo[3];
}

uint32_t geneHashcode()
{
	auto uuid = zuuid_new();
	uint32_t hashcode = getUUID(uuid);
	zuuid_destroy(&uuid);
	return hashcode;
}

string genUUID()
{
	auto uuid = zuuid_new();
	string result(zuuid_str(uuid));
	zuuid_destroy(&uuid);
	return result;
}