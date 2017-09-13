// Compiler.cpp : Main file that we will compile to run the program.
#include "header/Scanner.h"
#include <iostream>

int main(int argv, char* argc[])
{
	if(argv == 1){
		cout << "Please insert a KLEIN file to scan for tokens." << endl;
		return 1;
	}
	try
	{
		Scanner Scanner(argc[1]);
		Token Token = Scanner.getNextToken();
		while (Token.getTokenType() != END_OF_FILE)
		{
			cout << TokenTypePrintMap.find(Token.getTokenType())->second << " : " << Token.getValue() << endl;
			Token = Scanner.getNextToken();
		}
		cout << TokenTypePrintMap.find(END_OF_FILE)->second << endl;
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
		return 1;
	}
    return 0;
}

