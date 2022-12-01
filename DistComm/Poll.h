#pragma once

#include <mutex>
#include <functional>

class Poll
{
public:
	Poll();
	~Poll(void);
public:
	void start();
	void addReader(zsock_t*, std::function<int()> callback);
	int setTimer(size_t interval, size_t times, std::function<void()> func);
	void killTimer(int id);
private:
	static int eventHandler(zloop_t* loop, zsock_t* handle, void* arg);
	static int timerHandler(zloop_t* loop, int timer_id, void* arg);
	int onEvent(zsock_t* handle);
	void onTimer(int timerId);
private:
	zloop_t* zloop;			///< zloop¶ÔÏó
	std::map<zsock_t*, std::function<int()>> socketCb;
	std::map<int, std::function<void()>> timerCb;
};
