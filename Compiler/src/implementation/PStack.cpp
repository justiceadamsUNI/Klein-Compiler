#include "../header/PStack.h"

using namespace std;

PStack::PStack(){
	stack<Parser::StackValues> parserStack;
}

Parser::StackValues PStack::pop(){
	if(isEmpty()) throw out_of_range("ERROR: Empty Stack");
	else {
		Parser::StackValues temp = parserStack.top();
		parserStack.pop();
		return temp;
	}
}

void PStack::push(Parser::StackValues in){
	parserStack.push(in);	
}

bool PStack::isEmpty(){
	return parserStack.empty();
}



