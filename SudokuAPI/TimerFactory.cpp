#include "stdafx.h"
#include "TimerFactory.h"
#include "Timer.h"
#include "Dispatcher.h"

TimerFactory* TimerFactory::my_instance = nullptr;
std::once_flag TimerFactory::my_once_flag;

void TimerFactory::Initialize()
{
	std::lock_guard<std::mutex> guard(myMutex[2]);
	myTerminated = false;
	Clear();
}

void TimerFactory::CreateTimerOnThread(std::function<void()>theCallback, uint theTimer, bool recurring)
{
	if (recurring)
	{
		Timer p(theCallback, theTimer, std::this_thread::get_id(), recurring);
		p.Start();
	}
	else
	{
		std::unique_ptr<Timer> p = std::make_unique<Timer>(Timer(theCallback, theTimer, std::this_thread::get_id(), recurring));

		while (!Dispatcher<Timer>::GetInst()->addElement(p, std::this_thread::get_id()));
	}
}

void TimerFactory::TimerFired(std::thread::id theThreadId, Timer* t)
{
	std::lock_guard<std::mutex> guard(myMutex[3]);
	myCompletedThreads.push_back(theThreadId);

	// We can't join and delete the thread in this method because it's called from the thread we are trying to delete.
}

void TimerFactory::CreateTimer(std::function<void()>theCallback, uint theInterval, bool recurring)
{
	std::lock_guard<std::mutex> guard(myMutex[4]);

	if (myTerminated == true)
	{
		return;
	}

	if (recurring)
	{
		Timer* p = new Timer(theCallback, theInterval, std::this_thread::get_id(), recurring);

		thread* t = new thread(&Timer::Start, p);
		myThreads.push_back(t);
	}

	else
	{
		std::unique_ptr<Timer> p = std::make_unique<Timer>(Timer(theCallback, theInterval, std::this_thread::get_id(), recurring));
		Dispatcher<Timer>::GetInst()->addElement(p, std::this_thread::get_id());
	}
}

void TimerFactory::Clear()
{
	bool found = false;

	while (myCompletedThreads.size() > 0)
	{
		auto temp2 = (*myCompletedThreads.begin());

		found = false;
		for (auto th = myThreads.begin();
			th != myThreads.end();
			++th)
		{
			if ((*th)->get_id() == temp2)
			{
				(*th)->join();
				auto temp = (*th);
				myThreads.erase(th);
				delete temp;
				break;
			}
		}

		myCompletedThreads.pop_front();
	}
}