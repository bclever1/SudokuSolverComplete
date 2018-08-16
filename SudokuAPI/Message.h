#pragma once

#include "Macros.h"
#include "Dispatcher.h"

#include <mutex>
#include <fstream>
#include <string>
#include <list>

using namespace std;

class Message
{
public:

	Message(const char* c, bool theAppend) 
	{
		myMessage = string(c);
		myAppend = theAppend;
	}

	Message(const Message& orig)
	{
		myMessage = orig.myMessage;
		myAppend = orig.myAppend;
	}

	~Message() {}

	string myMessage;
	bool myAppend;

	void Start()
	{
		if (MY_DEBUG)
		{
			if (myAppend)
			{
				std::ofstream outfile("C:\\Users\\Brian\\source\\repos\\SudokuSolver\\SudokuInterface\\bin\\Debug\\sudoku_debug.txt", ios::app);
				outfile << myMessage << endl;
			}
			else
			{
				std::ofstream outfile("C:\\Users\\Brian\\source\\repos\\SudokuSolver\\SudokuInterface\\bin\\Debug\\sudoku_debug.txt", ios::out);
				outfile << myMessage << endl;
			}
		}
	}
};