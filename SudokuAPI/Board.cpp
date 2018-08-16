#include "stdafx.h"

#include <initializer_list>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>

#include "Board.h"
#include "Square.h"
#include "Solver.h"
#include "SolverFactory.h"
#include "Message.h"

using namespace std;

int Board::MY_BOARD_COUNTER = 1;

int Board::square_to_block_map[10][10] =
{
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 1, 1, 1, 2, 2, 2, 3, 3, 3 },
	{ 0, 1, 1, 1, 2, 2, 2, 3, 3, 3 },
	{ 0, 1, 1, 1, 2, 2, 2, 3, 3, 3 },
	{ 0, 4, 4, 4, 5, 5, 5, 6, 6, 6 },
	{ 0, 4, 4, 4, 5, 5, 5, 6, 6, 6 },
	{ 0, 4, 4, 4, 5, 5, 5, 6, 6, 6 },
	{ 0, 7, 7, 7, 8, 8, 8, 9, 9, 9 },
	{ 0, 7, 7, 7, 8, 8, 8, 9, 9, 9 },
	{ 0, 7, 7, 7, 8, 8, 8, 9, 9, 9 }
};

Board::Board()
{
	std::lock_guard<std::mutex> guard(myMutex);

	myBoardId = MY_BOARD_COUNTER;
	++MY_BOARD_COUNTER;

	BuildBoardStructure();

	for (int i = 0; i < solvingTechniques::ALL; ++i)
	{
		myReduceCounters[i] = 0;
	}
}

Board::~Board()
{
	for (int i = 1; i <= 9; ++i)
	{
		while (myRows[i].size() > 0)
		{
			myRows[i].clear();
		}

		while (myColumns[i].size() > 0)
		{
			myColumns[i].clear();
		}

		while (myBlocks[i].size() > 0)
		{
			myBlocks[i].clear();
		}
	}

	for (int i = 1; i <= 9; ++i)
	{
		for (int j = 1; j <= 9; ++j)
		{
			Square* temp = mySquares[i][j];
			mySquares[i][j] = 0;
			delete temp;
		}
	}
}

void Board::SetValue(int row, int col, int theVal)
{
	std::lock_guard<std::mutex> guard(myMutex);
	mySquares[row][col]->setValue(theVal);
}

int Board::GetValue(int i, int j)
{
	std::lock_guard<std::mutex> guard(myMutex);
	return 0;
}

struct mySquareFunctor
{
	bool operator()(Square* s)
	{
		if (s->getCount() > 1)
		{
			return false;
		}

		return true;
	}
};

struct myValidationFunctor
{
	bool operator()(vector<Square*>& v)
	{
		mySquareFunctor f;
		bool theResult = true;

		for (auto itr = v.begin();
			itr != v.end();
			++itr)
		{
			if (!f((*itr)))
			{
				return false;
			}
		}

		return true;
	}
};


bool Board::Solved()
{
	std::lock_guard<std::mutex> guard(myMutex);

	myValidationFunctor f;
	bool theResult = true;

	for (int i = 1; i <= 9; ++i)
	{
		if (!f(myRows[i]))
		{
			return false;
		}
    }
	for (int i = 1; i <= 9; ++i)
	{
		if (!f(myColumns[i]))
		{
			return false;
		}
	}
	for (int i = 1; i <= 9; ++i)
	{
		if (!f(myBlocks[i]))
		{
			return false;
		}
	}

	return true;

}

void Board::Reduce(Board::SquareGroupType_e theGrpType, int theItem)
{
	std::lock_guard<std::mutex> guard(myMutex);

	vector<Square*>* grpToReduce = nullptr;

	solvingTechniques theTechnique;

	if (theGrpType == Board::SquareGroupType_e::Row)
	{
		grpToReduce = &myRows[theItem];
		theTechnique = solvingTechniques::ROW_REDUCE;

	}
	else if (theGrpType == Board::SquareGroupType_e::Column)
	{
		grpToReduce = &myColumns[theItem];
		theTechnique = solvingTechniques::COL_REDUCE;
	}
	else if (theGrpType == Board::SquareGroupType_e::Block)
	{
		grpToReduce = &myBlocks[theItem];
		theTechnique = solvingTechniques::BLK_REDUCE;
	}

	if (grpToReduce == nullptr) return;

	for (int reducer = 1; reducer <= 9; ++reducer)
	{
		// See if we can reduce this number
		// If the square contains this number as a singleton we can remove it from the other squares

		int theCount = 0;

		for (auto itr = grpToReduce->begin();
			itr != grpToReduce->end();
			++itr)
		{
			if ((*itr)->getCount() == 1 && (*itr)->contains(reducer))
			{
				++theCount;
			}
		}

		if (theCount == 1)
		{
			// Remove the reducer from all the squares except the singleton
			for (auto itr1 = grpToReduce->begin();
				itr1 != grpToReduce->end();
				++itr1)
			{
				if ((*itr1) == nullptr || (*itr1) == 0)
				{
					return;
				}

				if ((*itr1)->contains(reducer) && (*itr1)->getCount() > 1)
				{
					(*itr1)->remove(reducer);
					++myReduceCounters[theTechnique];
				}
			}
		}
	}
}


void Board::RemoveStrandedSingles(Board::SquareGroupType_e theGrpType, int theItem)
{
	std::lock_guard<std::mutex> guard(myMutex);

	vector<Square*>* v = nullptr;

	if (theGrpType == Board::SquareGroupType_e::Row)
	{
		v = &myRows[theItem];
	}
	else if (theGrpType == Board::SquareGroupType_e::Column)
	{
		v = &myColumns[theItem];
	}
	else if (theGrpType == Board::SquareGroupType_e::Block)
	{
		v = &myBlocks[theItem];
	}

	for (int reducer = 1; reducer <= 9; ++reducer)
	{
		bool isSingleton = false;

		// Check the vector v for the reducer value.
		// If reducer only occurs once in the set of squares we can remove all other values
		//vector<Square*>* theContainer = new vector<Square*>();

		for (auto itr = v->begin();
			itr != v->end();
			++itr)

		{
			if ((*itr)->getCount() == 1 && (*itr)->contains(reducer))
			{
				// This value is a singleton, we don't need to go any further
				isSingleton = true;
				break;
			}
		}

		if (isSingleton) continue;

		int theCount = 0;
		Square* theGoodSquare = nullptr;

		for (auto itr = v->begin();
			 itr != v->end();
			 ++itr)

		{
			if ((*itr)->getCount() > 1 && (*itr)->contains(reducer))
			{
				theGoodSquare = *itr;
				++theCount;
			}
		}

		if (theCount == 1 && theGoodSquare != nullptr)
		{
			// This is the only square that has the value.
			myReduceCounters[solvingTechniques::STRANDED_SINGLES] += theGoodSquare->getCount() - 1;
			theGoodSquare->removeAllExcept(reducer);
		}
	}
}


void Board::RemoveNakedPairs(Board::SquareGroupType_e theGrpType, int theItem)
{
	std::lock_guard<std::mutex> guard(myMutex);

	vector<Square*> v;

	if (theGrpType == Board::SquareGroupType_e::Row)
	{
		v = myRows[theItem];
	}
	else if (theGrpType == Board::SquareGroupType_e::Column)
	{
		v = myColumns[theItem];
	}
	else if (theGrpType == Board::SquareGroupType_e::Block)
	{
		v = myBlocks[theItem];
	}

	for (int pair1 = 1; pair1 <= 8; ++pair1)
	{
		for (int pair2 = pair1 + 1; pair2 <= 9; ++pair2)
		{
			bitset<10> thePair;
			thePair[pair1] = 1;
			thePair[pair2] = 1;

			// Scan the squares of v to see if they equal this pair... if we find two we've got a naked pair

			int pairCount = 0;
			vector<Square*> thePairSet;

			for (auto itr = v.begin();
				itr != v.end();
				++itr)
			{
				if ((*itr)->compareValues(thePair))
				{
					thePairSet.push_back((*itr));
				}
			}

			if (thePairSet.size() == 2)
			{
				// Found a naked pair! We can remove pair1 and pair2 from all other squares

				for (auto itr = v.begin();
					itr != v.end();
					++itr)
				{
					if ((*itr) != thePairSet[0] && (*itr != thePairSet[1]))
					{
						if ((*itr)->contains(pair1))
						{
							(*itr)->remove(pair1);
							++myReduceCounters[solvingTechniques::NAKED_PAIRS];
						}
						if ((*itr)->contains(pair2))
						{
							(*itr)->remove(pair2);
							++myReduceCounters[solvingTechniques::NAKED_PAIRS];
						}
					}
				}
			}
		}
	}
}


void Board::PointingPairs(Board::SquareGroupType_e theGrpType, int theItem)
{
	std::lock_guard<std::mutex> guard(myMutex);

	for (int reducer = 1; reducer <= 9; ++reducer)
	{
		// See if we can reduce this number
	
		// Go through the blocks to see if the reducer must be in a particular row or column.
		// If it must be, then remove the reducer from the associated blocks' rows or columns.

		vector<Square*> theGoodSquares;

		for (auto sqr = myBlocks[theItem].begin();
			sqr != myBlocks[theItem].end();
			++sqr)
		{
			if ((*sqr)->contains(reducer))
			{
				theGoodSquares.push_back((*sqr));
			}
		}
		if (theGoodSquares.size() >= 2)
		{
			vector<int> theRowNums;
			vector<int> theColNums;

			for (auto good_sqr = theGoodSquares.begin();
				 good_sqr != theGoodSquares.end();
				 ++good_sqr)
			{
				theRowNums.push_back((*good_sqr)->GetRowNum());
				theColNums.push_back((*good_sqr)->GetColNum());
			}

			std::vector<int>::iterator rowItr = std::unique(theRowNums.begin(), theRowNums.end());
			theRowNums.resize(std::distance(theRowNums.begin(), rowItr));

			std::vector<int>::iterator colItr = std::unique(theColNums.begin(), theColNums.end());
			theColNums.resize(std::distance(theColNums.begin(), colItr));

			if (theRowNums.size() == 1)
			{
				// This means reducer only exists in this row in this block.
				// Therefore we can remove reducer from the intersection of this row
				// and the other blocks.

				int theRow = theRowNums[0];
				int the_complement_1;
				int the_complement_2;

				Board::GetBlockRowComplement(theItem, the_complement_1, the_complement_2);

				for (auto itx = myRows[theRow].begin();
					itx != myRows[theRow].end();
					++itx)
				{
					if (Board::GetBlock((*itx)->GetRowNum(), (*itx)->GetColNum()) == the_complement_1 ||
						Board::GetBlock((*itx)->GetRowNum(), (*itx)->GetColNum()) == the_complement_2)
					{
						if ((*itx)->contains(reducer))
						{
							(*itx)->remove(reducer);
							++myReduceCounters[solvingTechniques::POINTING_PAIRS];
						}
					}
				}
			}

			if (theColNums.size() == 1)
			{
				// This means reducer only exists in this column in this block.
				// Therefore we can remove reducer from the intersection of this row
				// and the other blocks.

				int theCol = theColNums[0];
				int the_complement_1;
				int the_complement_2;

				Board::GetBlockColumnComplement(theItem, the_complement_1, the_complement_2);

				for (auto itx = myColumns[theCol].begin();
					itx != myColumns[theCol].end();
					++itx)
				{
					if (Board::GetBlock((*itx)->GetRowNum(), (*itx)->GetColNum()) == the_complement_1 ||
						Board::GetBlock((*itx)->GetRowNum(), (*itx)->GetColNum()) == the_complement_2)
					{
						if ((*itx)->contains(reducer))
						{
							(*itx)->remove(reducer);
							++myReduceCounters[solvingTechniques::POINTING_PAIRS];
						}
					}
				}
			}
		}
	}
}

int Board::GetBoardState()
{
	int result = 0;

	for (int i = 1; i <= 9; ++i)
	{
		for (int j = 1; j <= 9; ++j)
		{
			result += mySquares[i][j]->getValue();
		}
	}

	return result;
}

Board::Board(Board& orig)
{
	std::lock_guard<std::mutex> guard(myMutex);

	myBoardId = MY_BOARD_COUNTER;
	++MY_BOARD_COUNTER;

	BuildBoardStructure();

	for (int i = 1; i <= 9; ++i)
	{
		for (int j = 1; j <= 9; ++j)
		{
			mySquares[i][j]->copyValues(orig.mySquares[i][j]->getValues());
		}
	}

	for (int i = 0; i < solvingTechniques::ALL; ++i)
	{
		myReduceCounters[i] = orig.myReduceCounters[i];
	}
}


Board::Board(Board* orig)
{
	std::lock_guard<std::mutex> guard(myMutex);

	myBoardId = MY_BOARD_COUNTER;
	++MY_BOARD_COUNTER;

	BuildBoardStructure();

	for (int i = 1; i <= 9; ++i)
	{
		for (int j = 1; j <= 9; ++j)
		{
			mySquares[i][j]->copyValues(orig->mySquares[i][j]->getValues());
		}
	}

	for (int i = 0; i < solvingTechniques::ALL; ++i)
	{
		myReduceCounters[i] = orig->myReduceCounters[i];
	}
}

struct validityChecker
{
	bool myResult;
	void operator()(vector<Square*>&v)
	{
		int theCounts[10]{ 0,0,0,0,0,0,0,0,0,0 };
	}
};

bool Board::CheckValid()
{
	clock_t start_run = clock();

	bool theResult = true;

	if (GetBoardState() < 405)
	{
		return false;
	}

	// If any row, column or block contains two or more singletons then invalid
	{
		for (int r = 1; r <= 9; ++r)
		{
			int theCounts[10]{ 0,0,0,0,0,0,0,0,0,0 };

			for (std::vector<Square*>::iterator sqr = myRows[r].begin();
				sqr != myRows[r].end();
				++sqr)
			{
				if ((*sqr)->getCount() == 1)
				{
					++theCounts[(*sqr)->getSingleton()];
				}
			}

			for (int i = 0; i < 10; ++i)
			{
				if (theCounts[i] > 1)
				{
					return false;
				}
			}
		}
	}

	{
		for (int c = 1; c <= 9; ++c)
		{
			int theCounts[10]{ 0,0,0,0,0,0,0,0,0,0 };

			for (std::vector<Square*>::iterator sqr = myColumns[c].begin();
				sqr != myColumns[c].end();
				++sqr)
			{
				if ((*sqr)->getCount() == 1)
				{
					++theCounts[(*sqr)->getSingleton()];
				}
			}

			for (int i = 0; i < 10; ++i)
			{
				if (theCounts[i] > 1)
				{
					return false;
				}
			}
		}
	}

	{

		for (int b = 1; b <= 9; ++b)
		{
			int theCounts[10]{ 0,0,0,0,0,0,0,0,0,0 };

			for (std::vector<Square*>::iterator sqr = myBlocks[b].begin();
				sqr != myBlocks[b].end();
				++sqr)
			{
				if ((*sqr)->getCount() == 1)
				{
					++theCounts[(*sqr)->getSingleton()];
				}
			}

			for (int i = 0; i < 10; ++i)
			{
				if (theCounts[i] > 1)
				{
					return false;
				}
			}
		}
	}

	return true;
}

bitset<10>& Board::GetSquareValues(int row, int col)
{
	return mySquares[row][col]->getValues();
}

void Board::RemoveValueAtPosFromSquare(int row, int col, int thePos)
{
	std::lock_guard<std::mutex> guard(myMutex);
	mySquares[row][col]->remove(thePos);
}

void Board::RemoveValueFromSquare(int row, int col, int thePos)
{
	std::lock_guard<std::mutex> guard(myMutex);
	mySquares[row][col]->remove(thePos);
}

void Board::SetValues(int r, int c, tuple<int, int, int, int, int, int, int, int, int> t)
{
	std::lock_guard<std::mutex> guard(myMutex);
	mySquares[r][c]->setValues(t);
}

void Board::GetBlockRowComplement(int theBlock, int& compl_1, int& compl_2)
{
	switch (theBlock)
	{
	case 1: compl_1 = 2; compl_2 = 3; return;
	case 2: compl_1 = 1; compl_2 = 3; return;
	case 3: compl_1 = 1; compl_2 = 2; return;
	case 4: compl_1 = 5; compl_2 = 6; return;
	case 5: compl_1 = 4; compl_2 = 6; return;
	case 6: compl_1 = 4; compl_2 = 5; return;
	case 7: compl_1 = 8; compl_2 = 9; return;
	case 8: compl_1 = 7; compl_2 = 9; return;
	case 9: compl_1 = 7; compl_2 = 8; return;
	default: compl_1 = -1; compl_2 = -1; return;
	};
}

void Board::GetBlockColumnComplement(int theBlock, int& compl_1, int& compl_2)
{
	switch (theBlock)
	{
	case 1: compl_1 = 4; compl_2 = 7; return;
	case 2: compl_1 = 5; compl_2 = 8; return;
	case 3: compl_1 = 6; compl_2 = 9; return;
	case 4: compl_1 = 1; compl_2 = 7; return;
	case 5: compl_1 = 2; compl_2 = 8; return;
	case 6: compl_1 = 3; compl_2 = 9; return;
	case 7: compl_1 = 1; compl_2 = 4; return;
	case 8: compl_1 = 2; compl_2 = 5; return;
	case 9: compl_1 = 3; compl_2 = 6; return;
	default: compl_1 = -1; compl_2 = -1; return;
	};
}

void Board::SetSquareValue(int row, int col, int theval)
{
	std::lock_guard<std::mutex> guard(myMutex);
	mySquares[row][col]->setValue(theval);
}

bool Board::Contains(int row, int col, int theval)
{
	return mySquares[row][col]->contains(theval);
}

int Board::GetSquareCount(int row, int col)
{
	return mySquares[row][col]->getCount();
}

void Board::Reset()
{
	for (int r = 1; r <= 9; ++r)
	{
		for (int c = 1; c <= 9; ++c)
		{
			mySquares[r][c]->clear();
		}
	}
}
Board& Board::operator=(const Board& orig)
{
	for (int r = 1; r <= 9; ++r)
	{
		for (int c = 1; c <= 9; ++c)
		{
			mySquares[r][c]->copyValues(orig.mySquares[r][c]->getValues());
		}
	}

	for (int i = 0; i < solvingTechniques::ALL; ++i)
	{
		myReduceCounters[i] = orig.myReduceCounters[i];
	}

	return *this;
}

void Board::BuildBoardStructure()
{
	for (int i = 0; i <= 9; ++i)
	{
		for (int j = 0; j <= 9; ++j)
		{
			mySquares[i][j] = nullptr;
		}
	}

	for (int i = 1; i <= 9; ++i)
	{
		for (int j = 1; j <= 9; ++j)
		{
			mySquares[i][j] = new Square(i, j);

			myRows[i].push_back(mySquares[i][j]);
			myColumns[j].push_back(mySquares[i][j]);

			if (1 <= i && i <= 3)
			{
				if (1 <= j && j <= 3)
				{
					myBlocks[1].push_back(mySquares[i][j]);
				}
				if (4 <= j && j <= 6)
				{
					myBlocks[2].push_back(mySquares[i][j]);
				}
				if (7 <= j && j <= 9)
				{
					myBlocks[3].push_back(mySquares[i][j]);
				}
			}
			if (4 <= i && i <= 6)
			{
				if (1 <= j && j <= 3)
				{
					myBlocks[4].push_back(mySquares[i][j]);
				}
				if (4 <= j && j <= 6)
				{
					myBlocks[5].push_back(mySquares[i][j]);
				}
				if (7 <= j && j <= 9)
				{
					myBlocks[6].push_back(mySquares[i][j]);
				}
			}
			if (7 <= i && i <= 9)
			{
				if (1 <= j && j <= 3)
				{
					myBlocks[7].push_back(mySquares[i][j]);
				}
				if (4 <= j && j <= 6)
				{
					myBlocks[8].push_back(mySquares[i][j]);
				}
				if (7 <= j && j <= 9)
				{
					myBlocks[9].push_back(mySquares[i][j]);
				}
			}
		}
	}
}

void Board::XWing()
{
	std::lock_guard<std::mutex> guard(myMutex);

	for (int reducer = 1; reducer <= 9; ++reducer)
	{
		// See if we can find an X-wing based on this number

        // Go through all the X-configurations to see if the reducer MUST be in one of the four squares.
		// If so, we can remove reducer from the other squares in the rows and columns.

		vector<Square*> theXWing[10];

		for (int row = 1; row <= 9; ++row)
		{
			for (int col = 1; col <= 9; ++col)
			{
				if (mySquares[row][col]->contains(reducer))
				{
					theXWing[row].push_back(mySquares[row][col]);
				}
			}
		}

		for (int row1 = 1; row1 <= 8; ++row1)
		{
			for (int row2 = row1 + 1; row2 <= 9; ++row2)
			{
				if (theXWing[row1].size() == 2 && theXWing[row2].size() == 2)
				{
					if ((theXWing[row1][0]->GetColNum() == theXWing[row2][0]->GetColNum()) &&
						(theXWing[row1][1]->GetColNum() == theXWing[row2][1]->GetColNum()))
					{
						int col1 = theXWing[row1][0]->GetColNum();
						int col2 = theXWing[row1][1]->GetColNum();

						// Found one! The reducer can be removed from these columns
						for (auto sqrItr = myColumns[col1].begin();
							sqrItr != myColumns[col1].end();
							++sqrItr)
						{
							if ((*sqrItr) != theXWing[row1][0] && (*sqrItr) != theXWing[row1][1] && (*sqrItr) != theXWing[row2][0] && (*sqrItr) != theXWing[row2][1])
							{
								if ((*sqrItr)->getCount() > 1 && (*sqrItr)->contains(reducer))
								{
									(*sqrItr)->remove(reducer);
									++myReduceCounters[solvingTechniques::X_WING];
								}
							}
						}

						for (auto sqrItr1 = myColumns[col2].begin();
							sqrItr1 != myColumns[col2].end();
							++sqrItr1)
						{
							if ((*sqrItr1) != theXWing[row1][0] && (*sqrItr1) != theXWing[row1][1] && (*sqrItr1) != theXWing[row2][0] && (*sqrItr1) != theXWing[row2][1])
							{
								if ((*sqrItr1)->getCount() > 1 && (*sqrItr1)->contains(reducer))
								{
									(*sqrItr1)->remove(reducer);
									++myReduceCounters[solvingTechniques::X_WING];
								}
							}
						}
					}
				}
			}
		}
	}
}

struct color
{
	bool onOff;
	bool set;
};

struct link
{
	Square* from;
	Square* to;
	bool traversed;

	~link()
	{

	}

	bool operator==(const link& lhs)
	{
		if ((from == lhs.from && to == lhs.to) || (from == lhs.to && to == lhs.from)) return true;
		return false;
	}
};

void Board::Coloring()
{
	std::lock_guard<std::mutex> guard(myMutex);

	if (myBoardId != 9) return;

	for (int reducer = 2; reducer <= 2; ++reducer)
	{
		ResetColoringInfo();

		list<std::shared_ptr<link>>myLinks;

		vector<Square*>* v = 0;

		for (int unitType = 1; unitType <= 3; ++unitType)
		{
			switch (unitType)
			{
			case 1: v = myRows; break;
			case 2: v = myColumns; break;
			case 3: v = myBlocks; break;
			};
			
			for (int unitNum = 1; unitNum <= 9; ++unitNum)
			{
				vector<Square*>myLinkCandidates;

				for (auto sqr = v[unitNum].begin();
					sqr != v[unitNum].end();
					++sqr)
				{
					if ((*sqr)->getCount() > 1 && (*sqr)->contains(reducer))
					{
						myLinkCandidates.push_back(*sqr);
					}
				}

				if (myLinkCandidates.size() == 2)
				{
					// Form a chain out of these two members
					std::shared_ptr<link> l = std::make_shared<link>(link({ myLinkCandidates[0], myLinkCandidates[1]}));

					bool found = false;
					for (auto linkItr = myLinks.begin();
						linkItr != myLinks.end();
						++linkItr)
					{
						if ((**linkItr == *l))
						{
							found = true;
							break;
						}
					}
					if (found == false)
					{
						myLinks.push_back(l);
					}
				}
			}
		}

		bool done = false;

		if (myLinks.size() > 0)
		{
			auto theRoot = (*myLinks.begin())->from;
			theRoot->myColor = true;
			theRoot->myColorSet = true;

			ColorSquares(theRoot, myLinks);
		}
		
		for (int unitType = 1; unitType <= 3; ++unitType)
		{
			switch (unitType)
			{
			case 1: v = myRows; break;
			case 2: v = myColumns; break;
			case 3: v = myBlocks; break;
			};

			for (int unitNum = 1; unitNum <= 9; ++unitNum)
			{
				vector<Square*>myReducerContainers;

				for (auto sqr = v[unitNum].begin();
					sqr != v[unitNum].end();
					++sqr)
				{
					if ((*sqr)->getCount() > 1 && (*sqr)->contains(reducer))
					{
						myReducerContainers.push_back(*sqr);
					}
				}

				int onColor = 0;
				int offColor = 0;

				for (auto sqr = myReducerContainers.begin();
					sqr != myReducerContainers.end();
					++sqr)
				{
					if ((*sqr)->myColorSet == true)
					{
						if ((*sqr)->myColor == true)
						{
							++onColor;
						}
						else
						{
							++offColor;
						}
					}
				}
				
				if (onColor == 2 && offColor != 2)
				{
					// Find the ON squares and remove the reducer
					for (auto sqr = myReducerContainers.begin();
						sqr != myReducerContainers.end();
						++sqr)
					{
						if ((*sqr)->myColorSet == true)
						{
							if ((*sqr)->myColor == true)
							{
								// Remove reducer from this square
								(*sqr)->remove(reducer);
								++myReduceCounters[solvingTechniques::COLORING];

#if 0
								string* theMessage = new string("(1) Removed: ");
								*theMessage += std::to_string(reducer);
								*theMessage += " from: [";
								*theMessage += std::to_string((*sqr)->GetRowNum());
								*theMessage += ",";
								*theMessage += std::to_string((*sqr)->GetColNum());
								*theMessage += "]\n";

								Dispatcher<Message>::GetInst()->addElement(theMessage, true);
#endif

							}
						}
					}
				}
				else if (offColor == 2 && onColor != 2)
				{
					// Find the OFF squares and remove the reducer
					for (auto sqr = myReducerContainers.begin();
						sqr != myReducerContainers.end();
						++sqr)
					{
						if ((*sqr)->myColorSet == true)
						{
							if ((*sqr)->myColor == false)
							{
								// Remove reducer from this square
								(*sqr)->remove(reducer);
								++myReduceCounters[solvingTechniques::COLORING];

#if 0
								string theMessage = "(2) Removed: ";
								theMessage += std::to_string(reducer);
								theMessage += " from: [";
								theMessage += std::to_string((*sqr)->GetRowNum());
								theMessage += ",";
								theMessage += std::to_string((*sqr)->GetColNum());
								theMessage += "]\n";

								Dispatcher<Message>::GetInst()->addElement(theMessage, true);
#endif

							}
						}
					}
				}
			}
		}
	}
}

void Board::ColorSquares(Square* theRoot, list<std::shared_ptr<link>>& theLinks)
{
	int linkIdx = 0;
	for (auto linkItr = theLinks.begin();
		linkItr != theLinks.end();
		++linkItr)
	{
		++linkIdx;

		if (theRoot->GetRowNum() == 2 && theRoot->GetColNum() == 5)
		{
			/*
			string theMessage = "Root: ";
			theMessage += "[";
			theMessage += std::to_string(theRoot->GetRowNum());
			theMessage += ",";
			theMessage += std::to_string(theRoot->GetColNum());
			theMessage += "] ";
			theMessage += "scanning link: ";
			theMessage += std::to_string(linkIdx);
			theMessage += " [";
			theMessage += std::to_string((*linkItr)->from->GetRowNum());
			theMessage += ",";
			theMessage += std::to_string((*linkItr)->from->GetColNum());
			theMessage += "] to [";
			theMessage += std::to_string((*linkItr)->to->GetRowNum());
			theMessage += ",";
			theMessage += std::to_string((*linkItr)->to->GetColNum());
			theMessage += "] traversed: ";
			theMessage += std::to_string((*linkItr)->traversed);
			theMessage += "\n";

			Dispatcher<Message>::GetInst()->addElement(theMessage, true);
			*/
		}

			if ((*linkItr)->from == theRoot)
			{
				if (theRoot->myColorSet)
				{
					if ((*linkItr)->to->myColorSet == false)
					{

						(*linkItr)->to->myColor = !theRoot->myColor;
						(*linkItr)->to->myColorSet = true;
						(*linkItr)->traversed = true;

						ColorSquares((*linkItr)->to, theLinks);
					}
				}
			}
			else if ((*linkItr)->to == theRoot)
			{
				if (theRoot->myColorSet)
				{
					if ((*linkItr)->from->myColorSet == false)
					{

						(*linkItr)->from->myColor = !theRoot->myColor;
						(*linkItr)->from->myColorSet = true;
						(*linkItr)->traversed = true;

						ColorSquares((*linkItr)->from, theLinks);
					}
				}
			}
	}
}

void Board::ResetColoringInfo()
{
	for (int row = 1; row <= 9; ++row)
	{
		for (int col = 1; col <= 9; ++col)
		{
			mySquares[row][col]->myColor = false;
			mySquares[row][col]->myColorSet = false;
		}
	}
}

void Board::DisplayForDebug()
{
	string theMessage = "";
	for (int r = 1; r <= 9; ++r)
	{
		for (int c = 1; c <= 9; ++c)
		{

		}
	}
}

void Board::CleanUpLinks(list<link*>& theLinks) 
{
	// It's possible our list got some duplicates
	theLinks.unique();
}