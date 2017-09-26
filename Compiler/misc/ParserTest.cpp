#include <iostream>
#include "../header/PStack.h"


using namespace std;

int main(){
	cout << "This is creating an empty stack." << endl;
	PStack myStack;
	if (myStack.isEmpty()) cout << "TRUE"  << endl;
	else cout << "FALSE" << endl;
	Parser::StackValues emum = Parser::OR;
	myStack.push(emum);
	myStack.push(emum);
	if (myStack.isEmpty()) cout << "TRUE"  << endl;
	else cout << "FALSE" << endl;
	cout << "POP VAL: " << Parser::StackValues(myStack.pop()) << endl;
	myStack.pop();
	if (myStack.isEmpty()) cout << "TRUE"  << endl;
	else cout << "FALSE" << endl;

}
