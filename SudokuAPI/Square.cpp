#include "stdafx.h"
#include "Square.h"
#include <memory>
#include <string>
#include "Dispatcher.h"
#include "SolverPair.h"
#include <tuple>


Square::Square(int theRow, int theCol) : myRow(theRow), myCol(theCol)
{
	for (int i = 1; i <= 9; ++i)
	{
		myValues[i] = 1;
	}
}

Square::~Square()
{
}

bool Square::contains(int theval)
{
	std::lock_guard<std::mutex> guard(myMutex);

	if (myValues[theval] == 1) return true;
	return false;
}


void Square::remove(int theval)
{
	std::lock_guard<std::mutex> guard(myMutex);

	int theResult = 0;

	for (int i = 1; i <= 9; ++i)
	{
		if (myValues[i] == 1)
		{
			theResult += 1;
		}
	}

	if (theResult > 1)
	{
		myValues[theval] = 0;
	}
}


void Square::removeAllExcept(int theval)
{
	// First make sure I contain the specified value

	std::lock_guard<std::mutex> guard(myMutex);

	if (myValues[theval] == 1)
	{
		for (int i = 1; i <= 9; ++i)
		{
			if (i != theval)
			{
				myValues[i] = 0;
			}
		}
	}
}

void Square::setValue(int theval)
{
	if (theval != 0)
	{
		for (int i = 1; i <= 9; ++i)
		{
			if (i != theval)
			{
				myValues[i] = 0;
			}
		}

		myValues[theval] = 1;
	}
}

void Square::setValues(tuple<int, int, int, int, int, int, int, int, int> theval)
{
	std::lock_guard<std::mutex> guard(myMutex);

	myValues[1] = get<0>(theval);
	myValues[2] = get<1>(theval);
	myValues[3] = get<2>(theval);
	myValues[4] = get<3>(theval);
	myValues[5] = get<4>(theval);
	myValues[6] = get<5>(theval);
	myValues[7] = get<6>(theval);
	myValues[8] = get<7>(theval);
	myValues[9] = get<8>(theval);
}

int Square::getValue()
{
	std::lock_guard<std::mutex> guard(myMutex);
	int theResult = 0;

	for (int i = 1; i <= 9; ++i)
	{
		if (myValues[i] == 1)
		{
			theResult += i;
		}
	}

	return theResult;
}


int Square::getCount()
{
	std::lock_guard<std::mutex> guard(myMutex);
	int theResult = 0;

	for (int i = 1; i <= 9; ++i)
	{
		if (myValues[i] == 1)
		{
			theResult += 1;
		}
	}

	return theResult;
}

bool Square::compareValues(bitset<10>& v)
{
	std::lock_guard<std::mutex> guard(myMutex);

	for (int i = 1; i <= 9; ++i)
	{
		if (myValues[i] != v[i])
		{
			return false;
		}
	}

	return false;
}

int Square::getSingleton()
{
	std::lock_guard<std::mutex> guard(myMutex);

	for (int i = 1; i <=9; ++i)
	{
		if (myValues[i] == 1)
		{
			return i;
		}
	}
	return 0;
}

void Square::copyValues(bitset<10>& v)
{
	std::lock_guard<std::mutex> guard(myMutex);
	
	for (int i = 1; i <= 9; ++i)
	{
		myValues[i] = v[i];
	}
}
bitset<10>& Square::getValues()
{
	std::lock_guard<std::mutex> guard(myMutex);
	return myValues;
}

void Square::clear()
{
	for (int i = 1; i <= 9; ++i)
	{
		myValues[i] = 0;
	}
}