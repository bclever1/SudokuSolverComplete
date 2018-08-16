#pragma once

#include <iostream>
#include <vector>
#include <mutex>
#include <bitset>

using namespace std;

class displayRegion;
class displayGrid;

class Square
{
	// This class represents one of 81 squares of the sudoku board

public:

	Square(int theRow, int theCol);
	~Square();

	void setValue(int theval);
	void setValues(std::tuple<int, int, int, int, int, int, int, int, int> theValues);

	bool contains(int theVal);
	void remove(int theVal);
	void removeAllExcept(int theVal);

	int getValue();
	int getCount();

	bool compareValues(bitset<10>& v);
	int getSingleton();

	void copyValues(bitset<10>& v);
	bitset<10>& getValues();

	int GetRowNum() { return myRow; }
	int GetColNum() { return myCol; }

	void clear();

	bool myColor;
	bool myColorSet;

private:

	//vector<int> myValues;
	std::bitset<10> myValues;
	
	std::mutex myMutex;
	int myRow;
	int myCol;

};