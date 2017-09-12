// Compiler.cpp : Main file that we will compile to run the program.
#include "header/Scanner.h"
#include <iostream>
#include <conio.h>

int main()
{
	try
	{
		Scanner Scanner("C:/Users/khars/Desktop/test.txt");
		Token Token = Scanner.getNextToken();
		while (Token.getTokenType() != END_OF_FILE)
		{
			cout << TokenTypePrintMap.find(Token.getTokenType())->second << " : " << Token.getValue() << endl;
			Token = Scanner.getNextToken();

			if (Token.getTokenType() == END_OF_FILE)
			{
				cout << "END OF FILE" << endl;
			}
		}
		

	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
		getch();
		return 1;

	}

    return 0;
}

