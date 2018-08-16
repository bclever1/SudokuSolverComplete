#include "stdafx.h"

#include "SolverFactory.h"
#include "Dispatcher.h"
#include "Timer.h"
#include "TimerFactory.h"
#include "Message.h"

SolverFactory* SolverFactory::my_instance = nullptr;
std::once_flag SolverFactory::my_once_flag;

void SolverFactory::Initialize()
{
	std::lock_guard<std::mutex> guard(myMutex[11]);

	TimerFactory::GetInst()->Initialize();
	Dispatcher<Timer>::GetInst()->Initialize();
	Dispatcher<Message>::GetInst()->Initialize();

	guessingEnabled = true;
	solutionFound = false;
	numGuesses = 0;
	numInvalids = 0;
	bestScore = 100000;
	numSurrenders = 0;
	mySolvedBoard.Reset();

	while (mySolvers.size() > 0)
	{
        // My elements are unique_ptrs, so no explicit delete for them!
		mySolvers.erase(mySolvers.begin());
	}
}

void SolverFactory::CreateNewSolver(unsigned char theBoard[])
{
	std::lock_guard<std::mutex> guard(myMutex[12]);

	std::unique_ptr<Solver> newSolver = std::make_unique<Solver>(theBoard);

	mySolvers.push_front(std::move(newSolver));
	(*mySolvers.begin())->Initialize();

	++numGuesses;
}

void SolverFactory::CreateNewSolver(Board& theBoard)
{
	std::lock_guard<std::mutex> guard(myMutex[13]);

	std::unique_ptr<Solver> newSolver = std::make_unique<Solver>(theBoard);
	mySolvers.push_front(std::move(newSolver));
	(*mySolvers.begin())->Initialize();

	++numGuesses;
}

bool SolverFactory::Shutdown()
{
	std::lock_guard<std::mutex> guard(myMutex[14]);
	TimerFactory::GetInst()->Terminate();
	TimerFactory::GetInst()->Initialize();
	Dispatcher<Timer>::GetInst()->Reset();

	while (mySolvers.size() > 0)
	{
		// My elements are unique_ptrs, so no explicit delete for them!
		mySolvers.erase(mySolvers.begin());
	}

	return true;
}