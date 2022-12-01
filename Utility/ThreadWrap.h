#pragma once

#include <functional>
#include <thread>

class ThreadWrap
{
public:
	ThreadWrap();
	~ThreadWrap();
public:
	bool start(std::function<void()>& loop, std::function<void()>* sf);
	bool stop();
	void onSelfQuit();
	bool joinable() const;
private:
	std::function<void()> loopFunc;
	std::function<void()> stopFunc;
	std::thread t;	//according to 'Effective Modern C++' item 37th, thread object should be the last member
					//this is unnecessary here because thread object is not passed by constructor, so thread context preparation is not needed.
					//but in case a move constructor is introduced, in which thread is passed from outside, this rule will make sense
};

