#pragma once

#include <mutex>
#include <fstream>
#include <string>
#include <functional>
#include <thread>
#include <memory>
#include <list>
#include "Timer.h"
#include "Macros.h"

using namespace std;

class TimerFactory
{
private:

	static TimerFactory* my_instance;
	static std::once_flag my_once_flag;
	std::mutex myMutex[100];
	std::list<thread*> myThreads;
	std::list<thread::id> myCompletedThreads;
	bool myTerminated;

	TimerFactory()
	{
		my_instance = this;
		myTerminated = true;
	}

	void CreateTimerOnThread(std::function<void()>theCallback, uint theTimer, bool recurring);
	void Clear();

public:

	static TimerFactory* GetInst()
	{
		std::call_once(TimerFactory::my_once_flag, []() {new TimerFactory(); });
		return TimerFactory::my_instance;
	}

	void myFcn()
	{
		std::lock_guard<std::mutex> guard(myMutex[0]);

		// Do something...
	}

	void CreateTimer(std::function<void()>theCallback, uint theTimer, bool recurring);
	void TimerFired(std::thread::id theThreadId, Timer* t);

	void Terminate()
	{
		std::lock_guard<std::mutex> guard(myMutex[1]);
		myTerminated = true;
		Clear();
	}

	void Initialize();

};