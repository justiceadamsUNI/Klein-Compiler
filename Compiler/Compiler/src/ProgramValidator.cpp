#include <iostream>
#include "header/Parser.h"
#include "header/Scanner.h"

int main()
{
	try
	{
		Scanner Scanner("programs/fractionAddition.kln");
		Parser Parser(Scanner);
		
		Parser.parseProgram();

		cout << endl << "IT'S VALID" << endl;
	}
	catch (const std::exception& e)
	{
		//Print Error Message
		cout << endl << e.what() << endl;
		return 1;
	}
	return 0;
}
