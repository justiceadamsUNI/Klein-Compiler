#include "Parser.h"
#include <stack>
#include <stdexcept>

using namespace std;


//Throws Error for popping an empty stack
class PStack  //A Stack used to hold states of the parser
{
	
public:
	PStack();
	Parser::StackValues pop();
	Parser::StackValues top();
	void push(Parser::StackValues);
	bool isEmpty(); 

private:
	stack<Parser::StackValues> parserStack;
};
