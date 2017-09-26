#include "../header/PStack.h"

using namespace std;

PStack::PStack(){
	stack<Parser::StackValues> parserStack;
}

Parser::StackValues PStack::pop(){
	if(PStack::isEmpty()) throw out_of_range("Empty Stack");
	else {
		Parser::StackValues temp = PStack::parserStack.top();
		PStack::parserStack.pop();
		return temp;
	}
}

void PStack::push(Parser::StackValues in){
	PStack::parserStack.push(in);	
}

bool PStack::isEmpty(){
	return PStack::parserStack.empty();
}



