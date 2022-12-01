#include "pch.h"
#include "ThreadWrap.h"

using namespace std;

ThreadWrap::ThreadWrap()
{
}


ThreadWrap::~ThreadWrap()
{
	stop();
}

bool ThreadWrap::start(function<void()>& loop, function<void()>* sf)
{
	loopFunc = loop;
	if (sf)
	{
		stopFunc = *sf;
	}
	t = thread(loopFunc);
	return true;
}

bool ThreadWrap::stop()
{
	if (stopFunc != nullptr)
	{
		stopFunc();
		stopFunc = nullptr;
	}
	if (t.joinable())
	{
		t.join();
		return true;
	}
	else
	{
		return false;
	}
}

void ThreadWrap::onSelfQuit()
{
	stopFunc = nullptr;
}

bool ThreadWrap::joinable() const
{
	return t.joinable();
}
