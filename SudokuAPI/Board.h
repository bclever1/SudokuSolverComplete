#pragma once
#include <vector>
#include <mutex>
#include <bitset>
#include "Macros.h"
#include <list>

using namespace std;

class displayGrid;
class Square;

struct link;

class Board
{
public:

	typedef enum {Row, Column, Block} SquareGroupType_e;
	enum solvingTechniques { ROW_REDUCE, COL_REDUCE, BLK_REDUCE, STRANDED_SINGLES, POINTING_PAIRS, NAKED_PAIRS, X_WING, COLORING, ALL };

	Board();
	~Board();

	Board(Board& b);
	Board(Board* b);

	void BuildBoardStructure();

	/* Return the state of square[i,j]... a summation of all its values */
	int GetValue(int i, int j);

	/* Set a particular value in square[i,j]. This will remove all other values.*/
	void SetValue(int i, int j, int theVal);

	/* Do simple row, column and block reduction based on squares containing a known value.*/
	void Reduce(Board::SquareGroupType_e theType, int theIdx);

	/* Search for entries that must be known but hidden in a square with multiple values. */
	void RemoveStrandedSingles(Board::SquareGroupType_e theGrpType, int theItem);

	/* Eliminate entries in rows, columns and blocks because a naked pair is known. */
	void RemoveNakedPairs(Board::SquareGroupType_e theGrpType, int theItem);

	/* Eliminate entries in rows and columns because a block has a certain configuration*/
	void PointingPairs(Board::SquareGroupType_e theGrpType, int theItem);

	/* Eliminate entries using the X-WING technique*/
	void XWing();

	void Coloring();

	/* Returns an int that indicates the current state of the board. */
	int GetBoardState();

	/* Is the current state valid?*/
	bool CheckValid();

	/* Is the current state a solution?*/
	bool Solved();

	/* Get the contents of square[row,col]*/
	bitset<10>& GetSquareValues(int row, int col);

	/* Remove the value at position thePos from square[row,col]. */
	void RemoveValueAtPosFromSquare(int row, int col, int thePos);

	/* Remove theVal from square[row,col]. */
	void RemoveValueFromSquare(int row, int col, int theVal);

	/* Given a block, what are the other blocks row-wise? */
	static void GetBlockRowComplement(int theBlock, int& compl_1, int& compl_2);

	/* Given a block, what are the other blocks column-wise? */
	static void GetBlockColumnComplement(int theBlock, int& compl_1, int& compl_2);

	/* Get the block number of square[theRow, theCol]. */
	static int GetBlock(int theRow, int theCol)
	{
		return square_to_block_map[theRow][theCol];
	}

	/* Set the value of the given square */
	void SetSquareValue(int row, int col, int theval);

	/* How many values are in the given square */
	int GetSquareCount(int row, int col);

	/* Does square contain the value? */
	bool Contains(int row, int col, int theval);

	/* Clear out all values */
	void Reset();

	Board& operator=(const Board& orig);

	void ResetColoringInfo();

	void SetValues(int r, int c, tuple<int, int, int, int, int, int, int, int, int>);

	void ColorSquares(Square* theRoot, list<std::shared_ptr<link>>& theLinks);

	void DisplayForDebug();
	

	void GetBoardData(int* theData, int theStartIndex)
	{
		for (uint i = 0; i < solvingTechniques::ALL; ++i)
		{
			theData[i+theStartIndex] = myReduceCounters[i];
		}
	}

private:

	int myBoardId;
	Square* mySquares[10][10];

	vector<Square*> myBlocks[10];
	vector<Square*> myRows[10];
	vector<Square*> myColumns[10];

	std::mutex myMutex;

	uint myReduceCounters[solvingTechniques::ALL];

	static int square_to_block_map[10][10];
	static int MY_BOARD_COUNTER;

	void CleanUpLinks(list<link*>& theLinks);

};
