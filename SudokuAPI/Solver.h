#pragma once

#include "Board.h"
#include "Macros.h"
#include <memory>
#include <tuple>

class Solver
{
	public:

		static int DEBUG_SOLVER_COUNTER;

		enum SolverState {
			ACTIVE,
			READY,
			INITIALIZING,
			INVALID,
			RUNNING,
            SURRENDERED,
			SOLVED
		};

		Solver(unsigned char bytes[]);

#if 0
			int9 t_1_1(1, 0, 0, 4, 5, 0, 0, 0, 0);
			myBoard->SetValues(1, 1, t_1_1);

			int9 t_1_2(1, 0, 0, 0, 5, 0, 0, 0, 0);
			myBoard->SetValues(1, 2, t_1_2);

			int9 t_1_4(0, 2, 0, 0, 5, 0, 0, 0, 0);
			myBoard->SetValues(1, 4, t_1_4);

			int9 t_1_8(1, 0, 0, 4, 0, 0, 0, 0, 9);
			myBoard->SetValues(1, 8, t_1_8);

			int9 t_1_9(1, 2, 0, 4, 0, 0, 0, 0, 9);
			myBoard->SetValues(1, 9, t_1_9);

			//---------------------------------------------------------------------------------

			int9 t_2_1(1, 0, 0, 4, 5, 0, 0, 0, 0);
			myBoard->SetValues(2, 1, t_2_1);

			int9 t_2_5(0, 2, 0, 0, 5, 0, 0, 0, 0);
			myBoard->SetValues(2, 5, t_2_5);

			int9 t_2_8(1, 0, 0, 4, 0, 0, 0, 0, 0);
			myBoard->SetValues(2, 8, t_2_8);

			int9 t_2_9(1, 2, 0, 4, 0, 0, 0, 0, 0);
			myBoard->SetValues(2, 9, t_2_9);
			//---------------------------------------------------------------------------------

			int9 t_4_3(0, 2, 0, 0, 5, 0, 0, 0, 0);
			myBoard->SetValues(4, 3, t_4_3);

			int9 t_4_6(0, 2, 0, 0, 5, 0, 0, 0, 0);
			myBoard->SetValues(4, 6, t_4_6);

			//---------------------------------------------------------------------------------

			int9 t_5_1(1, 0, 0, 0, 5, 0, 0, 0, 9);
			myBoard->SetValues(5, 1, t_5_1);

			int9 t_5_3(1, 2, 0, 0, 0, 0, 0, 0, 0);
			myBoard->SetValues(5, 3, t_5_3);

			int9 t_5_7(0, 2, 0, 4, 5, 0, 0, 0, 0);
			myBoard->SetValues(5, 7, t_5_7);

			int9 t_5_8(1, 0, 0, 4, 0, 0, 0, 0, 9);
			myBoard->SetValues(5, 8, t_5_8);

			int9 t_5_9(1, 0, 0, 4, 5, 0, 0, 0, 9);
			myBoard->SetValues(5, 9, t_5_9);

			//---------------------------------------------------------------------------------

			int9 t_6_1(1, 0, 0, 0, 0, 0, 0, 0, 9);
			myBoard->SetValues(6, 1, t_6_1);

			int9 t_6_4(0, 2, 0, 0, 5, 0, 0, 0, 0);
			myBoard->SetValues(6, 4, t_6_4);

			int9 t_6_7(0, 2, 0, 0, 5, 0, 0, 0, 0);
			myBoard->SetValues(6, 7, t_6_7);

			int9 t_6_9(1, 0, 0, 0, 0, 0, 0, 0, 9);
			myBoard->SetValues(6, 9, t_6_9);

			//---------------------------------------------------------------------------------

			int9 t_7_3(1, 0, 0, 0, 5, 0, 0, 0, 0);
			myBoard->SetValues(7, 3, t_7_3);

			int9 t_7_6(1, 0, 0, 4, 0, 0, 0, 0, 0);
			myBoard->SetValues(7, 6, t_7_6);

			int9 t_7_7(0, 0, 0, 4, 5, 0, 0, 0, 0);
			myBoard->SetValues(7, 7, t_7_7);

			//---------------------------------------------------------------------------------

			int9 t_8_5(0, 2, 0, 0, 5, 0, 0, 0, 0);
			myBoard->SetValues(8, 5, t_8_5);

			int9 t_8_6(0, 2, 0, 0, 5, 0, 0, 0, 0);
			myBoard->SetValues(8, 6, t_8_6);

			//---------------------------------------------------------------------------------

			int9 t_9_2(1, 0, 0, 0, 5, 0, 0, 0, 0);
			myBoard->SetValues(9, 2, t_9_2);

			int9 t_9_6(1, 0, 0, 4, 0, 0, 0, 0, 0);
			myBoard->SetValues(9, 6, t_9_6);

			int9 t_9_9(0, 0, 0, 4, 5, 0, 0, 0, 0);
			myBoard->SetValues(9, 9, t_9_9);
#endif

		Solver(Board& theBoard) : mySolverState(SolverState::INITIALIZING)
		{
			myBoard = std::make_unique<Board>(theBoard);
			myConstructorId = 2;
			mySolverId = DEBUG_SOLVER_COUNTER++;
			myRunCounter = 0;
		}

		Solver(const Solver& s) : mySolverState(s.mySolverState)
		{
			myBoard = std::make_unique<Board>((*s.myBoard));
			myConstructorId = 3;
			mySolverId = DEBUG_SOLVER_COUNTER++;
			myRunCounter = 0;
		}
		
		~Solver()
		{
		//	delete myBoard;
		}

		void Initialize();

		void Run();
		
		void Reduce(Board::SquareGroupType_e theItemType, int theItem);
		void RemoveStrandedSingles(Board::SquareGroupType_e theItemType, int theItem);
		void RemoveNakedPairs(Board::SquareGroupType_e theItemType, int theItem);
		void PointingPairs(Board::SquareGroupType_e theItemType, int theItem);

		void ClearBoard()
		{
			myBoard = nullptr;
		}

//		void SetBoard(Board* theBoard) { myBoard = theBoard; }
		std::unique_ptr<Board>* GetBoard() { return &myBoard; }

		void SetState(Solver::SolverState theState) { mySolverState = theState; }
		Solver::SolverState GetState() { return mySolverState; }

private:

	void MakeGuesses();

	SolverState mySolverState;
	std::unique_ptr<Board> myBoard;
	std::mutex myMutex;
	int myConstructorId;
	int mySolverId;
	int myRunCounter;	
};