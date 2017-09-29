#include "../header/PStack.h"

using namespace std;

PStack::PStack(){
	stack<StackValues> parserStack;
}

StackValues PStack::pop(){
	if(isEmpty()) throw out_of_range("ERROR: Can't call pop() on an Empty Stack");
	else {
		StackValues temp = top();
		parserStack.pop();
		return temp;
	}
}

void PStack::push(StackValues in){
	parserStack.push(in);	
}

StackValues PStack::top() {
	if (isEmpty()) throw out_of_range("ERROR: Can't call top() on an Empty Stack");
	else {
		StackValues temp = parserStack.top();
		return temp;
	}
}

bool PStack::isEmpty(){
	return parserStack.empty();
}