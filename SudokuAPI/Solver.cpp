#include "stdafx.h"


#include <thread>
#include <time.h>

#include "Solver.h"
#include "Board.h"
#include "SolverFactory.h"
#include "Dispatcher.h"
#include "TimerFactory.h"
#include "Message.h"

int Solver::DEBUG_SOLVER_COUNTER = 0;

Solver::Solver(unsigned char bytes[])
{
	myBoard = std::make_unique<Board>(new Board());

	int byteIdx = 0;
	for (int i = 1; i <= 9; ++i)
	{
		for (int j = 1; j <= 9; ++j)
		{
			myBoard->SetValue(i, j, bytes[byteIdx]);
			++byteIdx;
		}
	}

	myConstructorId = 1;
	mySolverId = DEBUG_SOLVER_COUNTER++;
	myRunCounter = 0;
}

void Solver::Initialize()
{
	mySolverState = SolverState::READY;
	std::function<void()> run_callback = std::bind(&Solver::Run, this);
	TimerFactory::GetInst()->CreateTimer(run_callback, MY_SOLVER_CLOCK_RATE, false);
}

void Solver::Run()
{
	if (mySolverState == SolverState::SURRENDERED)
	{
		return;
	}

	mySolverState = SolverState::RUNNING;
	++myRunCounter;

	if (!(myBoard->CheckValid()))
	{
		// I was created invalid. Bummer.
		mySolverState = SolverState::INVALID;
		SolverFactory::GetInst()->RegisterCompld(mySolverState);
		return;
	}

	if (SolverFactory::GetInst()->GetSolvedYet())
	{
		mySolverState = SolverState::SURRENDERED;
		SolverFactory::GetInst()->RegisterCompld(mySolverState);
		return;
	}

	int myBoardState = myBoard->GetBoardState();

	// Do the obvious reductions of the rows, columns and blocks
	for (int theItem = 1; theItem <= 9; ++theItem)
	{
		Reduce(Board::SquareGroupType_e::Row, theItem);
		Reduce(Board::SquareGroupType_e::Column, theItem);
		Reduce(Board::SquareGroupType_e::Block, theItem);
	}

	for (int theItem = 1; theItem <= 9; ++theItem)
	{
		RemoveStrandedSingles(Board::SquareGroupType_e::Row, theItem);
		RemoveStrandedSingles(Board::SquareGroupType_e::Column, theItem);
		RemoveStrandedSingles(Board::SquareGroupType_e::Block, theItem);
	}

	for (int theItem = 1; theItem <= 9; ++theItem)
	{
		RemoveNakedPairs(Board::SquareGroupType_e::Row, theItem);
		RemoveNakedPairs(Board::SquareGroupType_e::Column, theItem);
		RemoveNakedPairs(Board::SquareGroupType_e::Block, theItem);
	}

	for (int theItem = 1; theItem <= 9; ++theItem)
	{
		PointingPairs(Board::SquareGroupType_e::Block, theItem);
	}

	myBoard->XWing();
	//myBoard->Coloring();

	if (!(myBoard->CheckValid()))
	{
		// After the reductions I ended up invalid.
		mySolverState = SolverState::INVALID;
		SolverFactory::GetInst()->RegisterCompld(mySolverState);
		return;
	}
	else if (myBoard->Solved())
	{
		// Notify the DataManager that a solution has been found

		mySolverState = SolverState::SOLVED;
		myBoardState = myBoard->GetBoardState();

		myBoard->CheckValid();

		SolverFactory::GetInst()->SetScore(myBoardState);
		SolverFactory::GetInst()->SolutionFound(&myBoard);
		
		return;
	}
	else
	{
		if (myBoardState == myBoard->GetBoardState())
		{
			// Our reduction techniques have done as much as they can.
			// Now we make a guess.

			if (SolverFactory::GetInst()->IsGuessingEnabled())
			{
				mySolverState = SolverState::SURRENDERED;
				SolverFactory::GetInst()->RegisterCompld(mySolverState);
				SolverFactory::GetInst()->SetScore(myBoardState);
				MakeGuesses();
				return;
			}
			else
			{
				// This is as far as we can go without guessing, so tell the manager we are done.
				mySolverState = SolverState::SOLVED;
				SolverFactory::GetInst()->SolutionFound(&myBoard);
				return;
			}
		}
		else
		{
			SolverFactory::GetInst()->SetScore(myBoardState);

			std::function<void()> run_callback = std::bind(&Solver::Run, this);
			TimerFactory::GetInst()->CreateTimer(run_callback, MY_SOLVER_CLOCK_RATE, false);
			mySolverState = SolverState::READY;

			return;
		}
	}
}

void Solver::Reduce(Board::SquareGroupType_e theItemType, int theItem)
{
	myBoard->Reduce(theItemType, theItem);
}

void Solver::RemoveStrandedSingles(Board::SquareGroupType_e theItemType, int theItem)
{
	myBoard->RemoveStrandedSingles(theItemType, theItem);
}

void Solver::RemoveNakedPairs(Board::SquareGroupType_e theItemType, int theItem)
{
	myBoard->RemoveNakedPairs(theItemType, theItem);
}

void Solver::PointingPairs(Board::SquareGroupType_e theItemType, int theItem)
{
	myBoard->PointingPairs(theItemType, theItem);
}

void Solver::MakeGuesses()
{
	// Here is where we make our guesses. 
	// If you think about it, making all the guesses for one square MUST
	// produce the correct answer eventually, so lets do that.

	bool finished = false;
	for (int i = 1; i <= 9; ++i)
	{
		for (int j = 1; j <= 9; ++j)
		{
			if (myBoard->GetSquareCount(i, j) > 1)
			{
				for (int k = 1; k <= 9; ++k)
				{
					if (myBoard->Contains(i,j,k))
					{
						Board B = *myBoard;
						B.SetSquareValue(i, j, k);

						SolverFactory::GetInst()->CreateNewSolver(B);
					}
				}

				// One of these guys must be correct, so get outta here and let the solving continue.
				finished = true;
				break;
			}
		}

		if (finished == true)
		{
			break;
		}
	}

	return;
}
