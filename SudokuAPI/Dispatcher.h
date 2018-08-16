#pragma once


#include <mutex>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <fstream>
#include <string>
#include <list>
#include <utility>

#include "Board.h"
#include "Timer.h"
#include "Solver.h"
#include "Macros.h"
#include "TimerFactory.h"

template <typename T>
class Dispatcher
{
private:

	static Dispatcher<T>* myInstance;
	static std::once_flag myOnceFlag;
	std::list<std::unique_ptr<T>> myData;
	std::recursive_mutex myMutex[1];

	Dispatcher()
	{
		myInstance = this;
	}

public:

	static Dispatcher* GetInst()
	{
		std::call_once(Dispatcher::myOnceFlag, []() {new Dispatcher(); });
		return Dispatcher::myInstance;
	}

	virtual void Initialize()
	{
		std::lock_guard<std::recursive_mutex> guard(myMutex[0]);

		while (myData.size() > 0)
		{
			std::unique_ptr<T> temp = std::move(*myData.begin());
			myData.pop_front();
			(*temp).Start();
		}

		std::function<void()> run_callback = std::bind(&Dispatcher::Run, this);
		TimerFactory::GetInst()->CreateTimer(run_callback, MY_DISPATCHER_CLOCK_RATE, true);
	}

	/* Add an element to my list */
	bool addElement(std::unique_ptr<T>& theElement, std::thread::id theCaller)
	{
		std::lock_guard<std::recursive_mutex> guard(myMutex[0]);

		myData.push_back(std::move(theElement));
		return true;
	}

	/* Just a pattern for future work */
	void myFcn()
	{
		std::lock_guard<std::recursive_mutex> guard(myMutex[0]);

		// Do something...
	}

	/* Clear my list */
	void Reset()
	{
		std::lock_guard<std::recursive_mutex> guard(myMutex[0]);

		while (myData.size() > 0)
		{
			myData.pop_back();
		}
	}


	virtual void Run()
	{
		std::unique_ptr<T> temp = nullptr;

		// Set up a little local scope to lock myData.
		// The chain of events coming from (*temp).Start() will need access.
		{
		    std::lock_guard<std::recursive_mutex> guard(myMutex[0]);

			if (myData.size() > 0)
			{
				temp = std::move(*myData.begin());
				myData.pop_front();
			}
		}

		if (temp != nullptr)
		{
			(*temp).Start();
			//NO delete of temp! These guys are unique_ptrs.
		}

	}
};

template <typename T>
Dispatcher<T>* Dispatcher<T>::myInstance = nullptr;

template <typename T>
std::once_flag Dispatcher<T>::myOnceFlag;