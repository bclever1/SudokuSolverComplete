#include "stdafx.h"

#include "Timer.h"
#include "TimerFactory.h"

#include <thread>
#include <chrono>

void Timer::Run()
{
	if (myWorkComplete == false || isRecurring == true)
	{
		std::chrono::nanoseconds ms(myTimer);

	    begin: std::this_thread::sleep_for(ms);

		if (!(myCallback == nullptr))
		{
			myCallback();
		}

		myWorkComplete = true;

		if (isRecurring)
		{
			goto begin;
		}
		//else
		//{
		//	TimerFactory::GetInst()->TimerFired(myThread, this);
		//}
	}
}