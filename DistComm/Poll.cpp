
#include "pch.h"
#include "Poll.h"

using namespace std;


Poll::Poll()
	:zloop(zloop_new())
{
}

Poll::~Poll(void)
{
	if (zloop)
	{
		zloop_destroy(&zloop);
	}
}

void Poll::addReader(zsock_t* s, std::function<int()> cb)
{
	auto rc = zloop_reader(zloop, s, eventHandler, this);
	if (rc == 0)
	{
		zloop_reader_set_tolerant(zloop, s);
		socketCb[s] = cb;
	}
}

int Poll::eventHandler(zloop_t *loop, zsock_t *handle, void *arg)
{
	auto self = (Poll*)arg;
	return self->onEvent(handle);
}

int Poll::timerHandler(zloop_t* loop, int timer_id, void* arg)
{
	auto self = (Poll*)arg;
	self->onTimer(timer_id);
	return 0;
}

int Poll::onEvent(zsock_t* handle)
{
	auto iter = socketCb.find(handle);
	if (iter != socketCb.end())
	{
		return iter->second();
	}
	return 0;
}

void Poll::onTimer(int timerId)
{
	auto iter = timerCb.find(timerId);
	if (iter != timerCb.end())
	{
		iter->second();
	}
}

int Poll::setTimer(size_t interval, size_t times, std::function<void()> func)
{
	auto timerId = zloop_timer(zloop, interval, times, timerHandler, this);
	if (timerId != -1)
	{
		timerCb[timerId] = func;
	}
	return timerId;
}

void Poll::start()
{
	assert(zloop);
	zloop_timer(zloop, 1000, 1, timerHandler, this);	//δ֪ԭ��loop�������������߳�����timerû��Ӧ����������ǰ����һ��timer����������Ҳ��Ч
	int returnCode = zloop_start(zloop);
	zloop_destroy(&zloop);
}

void Poll::killTimer(int id)
{
	if (timerCb.find(id) != timerCb.end())
	{
		zloop_timer_end(zloop, id);
		timerCb.erase(id);
	}
}
