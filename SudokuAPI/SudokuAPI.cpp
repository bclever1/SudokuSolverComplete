#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <bitset>

#include "Board.h"
#include "Dispatcher.h"
#include "SolverPair.h"
#include "SolverFactory.h"
#include "Macros.h"
#include "Message.h"

using namespace std;

extern "C" __declspec(dllexport) void Reset()
{
	std::unique_ptr<Message> m = std::make_unique<Message>( "Reset\n", false );
	Dispatcher<Message>::GetInst()->addElement(m, std::this_thread::get_id());
}

unsigned char* getBoardDisplay()
{
	std::unique_ptr<Message> m = std::make_unique<Message>("getBoardDisplay\n", false);
	Dispatcher<Message>::GetInst()->addElement(m, std::this_thread::get_id());

	unsigned char* values = new unsigned char[RESPONSE_SIZE];

	try
	{
		Board* B = SolverFactory::GetInst()->GetSolvedBoard();

		if (B != nullptr)
		{
			int byteIdx = 0;
			for (int i = 1; i <= 9; ++i)
			{
				for (int j = 1; j <= 9; ++j)
				{
					bitset<10>& sqr_values = B->GetSquareValues(i, j);

					for (int k = 1; k <= 9; ++k)
					{
						if (sqr_values[k] == 1)
						{
							values[byteIdx] = (unsigned char)(k);
							++byteIdx;
						}
					}
					values[byteIdx] = (unsigned char)0;
					++byteIdx;
				}
			}
		}
	}
	catch (...)
	{

	}

	return values;
}

extern "C" __declspec(dllexport) int* GetBoardData()
{
	//Message* m = new Message({ "getBoardData\n", true });
	//Dispatcher<Message>::GetInst()->addElement(m);

	int* values = new int[RESPONSE_SIZE];

	for (int i = 0; i < RESPONSE_SIZE; ++i)
	{
		values[i] = 0;
	}

	int idx = 0;
	values[idx] = (int)SolverFactory::GetInst()->GetSolvedYet();
	++idx;
	values[idx] = SolverFactory::GetInst()->GetNumGuesses();
	++idx;
	values[idx] = SolverFactory::GetInst()->GetNumInvalids();
	++idx;
	values[idx] = SolverFactory::GetInst()->GetNumSurrenders();
	++idx;
	values[idx] = SolverFactory::GetInst()->GetBestScore();
	++idx;
	Board* B = SolverFactory::GetInst()->GetSolvedBoard();

	if (B != nullptr)
	{
		B->GetBoardData(values, idx);
		++idx;
	}

	return values;
}

extern "C" __declspec(dllexport) bool Initialize(unsigned char bytes[])
{
	//Message* m1 = new Message(" New Starting...\n", true);
	//Dispatcher<Message>::GetInst()->addElement(m1);

	SolverFactory::GetInst()->Initialize();
	SolverFactory::GetInst()->CreateNewSolver(bytes);

	return true;
}

extern "C" __declspec(dllexport) unsigned char* GetCurrentBoard()
{
	//Message* m = new Message({ "GetCurrentBoard\n", true });
	//Dispatcher<Message>::GetInst()->addElement(m);

	return getBoardDisplay();
}

extern "C" __declspec(dllexport) void Solve()
{
	//Message* m = new Message("Solve\n", true);
	//Dispatcher<Message>::GetInst()->addElement(m);
}

extern "C" __declspec(dllexport) int GetActiveSolvers()
{	
	//Message* m = new Message({ "GetActiveSolvers\n", true });
	//Dispatcher<Message>::GetInst()->addElement(m);

	int theActiveSolvers = SolverFactory::GetInst()->GetNumGuesses() - SolverFactory::GetInst()->GetNumInvalids() - SolverFactory::GetInst()->GetNumSurrenders();
	return theActiveSolvers;
}

extern "C" __declspec(dllexport) void EnableGuessing()
{
	SolverFactory::GetInst()->EnableGuessing();
}

extern "C" __declspec(dllexport) void DisableGuessing()
{
	SolverFactory::GetInst()->DisableGuessing();
}

extern "C" __declspec(dllexport) char* GetNumGuesses()
{
	char* values = new char[RESPONSE_SIZE];

	unsigned long theGuesses = SolverFactory::GetInst()->GetNumGuesses();
	if (theGuesses > 0)
	{
		// Don't count the initial board.
		--theGuesses;
	}

	string myStr = std::to_string(theGuesses);
	
	for (uint i = 0; i < myStr.length(); ++i)
	{
		values[i] = myStr.c_str()[i];
	}
	values[myStr.length()] = '\0';

	return values;
}

extern "C" __declspec(dllexport) char* GetNumInvalids()
{
	char* values = new char[RESPONSE_SIZE];

	unsigned long theBestScore = SolverFactory::GetInst()->GetNumInvalids();

	string myStr = std::to_string(theBestScore);

	for (uint i = 0; i < myStr.length(); ++i)
	{
		values[i] = myStr.c_str()[i];
	}
	values[myStr.length()] = '\0';

	return values;
}

extern "C" __declspec(dllexport) char* GetBestScore()
{
	char* values = new char[RESPONSE_SIZE];

	unsigned long theBestScore = SolverFactory::GetInst()->GetBestScore();

	string myStr = std::to_string(theBestScore);

	for (uint i = 0; i < myStr.length(); ++i)
	{
		values[i] = myStr.c_str()[i];
	}
	values[myStr.length()] = '\0';

	return values;
}

extern "C" __declspec(dllexport) char* GetNumSurrenders()
{
	char* values = new char[RESPONSE_SIZE];

	unsigned long theBestScore = SolverFactory::GetInst()->GetNumSurrenders();

	string myStr = std::to_string(theBestScore);

	for (uint i = 0; i < myStr.length(); ++i)
	{
		values[i] = myStr.c_str()[i];
	}
	values[myStr.length()] = '\0';

	return values;
}

extern "C" __declspec(dllexport) bool GetIsSolved()
{
	//Message* m = new Message({ "GetIsSolved\n", true });
	//Dispatcher<Message>::GetInst()->addElement(m);

	return SolverFactory::GetInst()->GetSolvedYet();
}

extern "C" __declspec(dllexport) bool Shutdown()
{
	//Message* m = new Message({ "Shutdown\n", true });
	//Dispatcher<Message>::GetInst()->addElement(m);

	return SolverFactory::GetInst()->Shutdown();
}