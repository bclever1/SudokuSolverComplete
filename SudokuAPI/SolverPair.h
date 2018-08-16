#pragma once

#include <memory>
#include "Solver.h"
#include "Board.h"

class SolverPair
{
public:

#if 0
	//explicit SolverPair(Board* theBoard, Solver* theSolver) : myBoard(theBoard), mySolver(theSolver) {}
	//explicit SolverPair(SolverPair* theSp) : myBoard(theSp->myBoard), mySolver(theSp->mySolver) {}

	SolverPair(std::unique_ptr<Board>& theBoard) {}
	SolverPair(unsigned char bytes[])
	{
		mySolver = std::make_unique<Solver*>(new Solver());
		myBoard = std::make_unique<Board*>(new Board());

		int byteIdx = 0;
		for (int i = 1; i <= 9; ++i)
		{
			for (int j = 1; j <= 9; ++j)
			{
				(*myBoard)->SetValue(i, j, bytes[byteIdx]);
				++byteIdx;
			}
		}
	}

	~SolverPair() 
	{
		//delete myBoard;
		//delete mySolver;
	}

	//std::unique_ptr<Board>* GetBoardPtr() { return &myBoard; }
	//std::unique_ptr<Solver>* GetSolverPtr() { return &mySolver; }

	void Initialize() 
	{
		(*mySolver)->Initialize();
	}

private:
	
	std::unique_ptr<Solver*> mySolver;
	std::unique_ptr<Board*> myBoard;
#endif
	
};
