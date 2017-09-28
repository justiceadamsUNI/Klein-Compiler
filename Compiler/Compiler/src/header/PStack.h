#include "StackValues.h"
#include <stack>
#include <stdexcept>

using namespace std;


//Throws Error for popping an empty stack
class PStack  //A Stack used to hold states of the parser
{
	
public:
	PStack();

	StackValues pop();

	StackValues top();

	void push(StackValues);

	bool isEmpty(); 

private:
	stack<StackValues> parserStack;
};
