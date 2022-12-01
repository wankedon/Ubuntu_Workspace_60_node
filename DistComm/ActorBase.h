#pragma once

#include <memory>
#include <functional>
#include <mutex>
#include "ZMsgWrap.h"

class Poll;
class Actor
{
public:
	~Actor();
public:
	int setTimer(size_t interval, size_t times, std::function<void()> func);
	void killTimer(int id);
protected:
	Actor();
protected:
	void start(std::function<void()> initFunc);
	void stop();
	void bindActorMsgHandler(std::function<void(MessageExtractor&)> func);
	int sendToFrontend(MessageBuilder& builder);
	void addReader(zsock_t* s, std::function<int()> cb);
protected:
	void startLoop(zsock_t* pipe);
	int onPipeEvent();
private:
	static void backendThreadFunc(zsock_t* pipe, void* arg);
private:
	std::mutex mtx;
	std::unique_ptr<Poll> poll;
	std::function<void()> initFunc;
	std::function<void(MessageExtractor&)> actorMsgHandler;
	zactor_t* actor;		///< zactor对象
	zsock_t* pipeSock;		///< zactor前后台交互的pipe sock
};

