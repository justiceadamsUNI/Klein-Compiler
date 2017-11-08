//This Program Prints Out Tokens For A Given Klein File
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
		Scanner Scanner(argc[1], 1);
		Token Token = Scanner.next();
		while (Token.getTokenType() != END_OF_FILE)
		{
			cout << TokenTypePrintMap.find(Token.getTokenType())->second << " : " << Token.getValue() << endl;
			Token = Scanner.next();
		}
		cout << TokenTypePrintMap.find(END_OF_FILE)->second << endl;
	}
	catch (const std::exception& e)
	{
		//Print Error Message
		cout << endl << e.what() << endl;
		return 1;
	}
    return 0;
}

