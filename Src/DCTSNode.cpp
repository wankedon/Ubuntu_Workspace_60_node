// DCTSNode.cpp : Implementation of WinMain
#include "pch.h"
#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"
/*
#define _ATL_FREE_THREADED

#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit


#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
*/
//#include "DCTSNode_i.h"
#include "NodeManager.h"
#include "ConfigLoader.h"

// using namespace ATL;
using namespace std;

#include <stdio.h>
#include "Logger.h"
#include "Miscellaneous.h"
#include "StringConv.h"
#include "ConfigLoader.h"
#include <time.h>
#if _MSC_VER > 1900
#include "fmt_/core.h"
#else
#include "fmt/core.h"
#endif // _MSC_VER > 1900

static atomic_bool int_flag(false);
static std::unique_ptr<NodeManager> nodeManager;
static void SignHandler(int sig)
{
	if (sig == SIGINT)
	{
		nodeManager.reset();
		zsys_shutdown();
		int_flag = true;
	}
}

int main()
{
	signal(SIGINT, SignHandler);
	char *path = getcwd(NULL, 0);
	string strPath = path;
	time_t ttnow = time(NULL);
	tm *tmnow;
	tmnow = localtime(&ttnow);
	Logger::PATH = fmt::format("{}/nodelog/nodelog{:04}-{:02}-{:02}.log", strPath, tmnow->tm_year + 1900, tmnow->tm_mon + 1, tmnow->tm_mday);
	ConfigLoader cl(StrConvert::UTF8Towstring(path) + L"/NodeConfigFile.xml");
	auto config = cl.load();
	if (!config)
		return -1;
	zsys_init();
	nodeManager = make_unique<NodeManager>(*config, strPath);
	LOG("---------------node control start----------------");
	while (!int_flag)
	{
		std::this_thread::sleep_for(chrono::milliseconds(100));
	}
	LOG("---------------node control quit------------------");
	LOGFlush();
}