#include "StackValues.h"
#include <stack>
#include <stdexcept>

using namespace std;


// Throws Error for popping an empty stack
// A Stack used to hold states of the parser
class PStack 
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
