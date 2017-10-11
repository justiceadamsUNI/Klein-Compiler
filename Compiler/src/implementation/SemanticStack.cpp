#pragma once
#include "../header/SemanticStack.h"

using namespace std;

SemanticStack::SemanticStack() {
	stack<ASTNode*> SStack;
}

ASTNode* SemanticStack::pop() {
	if (isEmpty()) throw out_of_range("ERROR: Can't call pop() on an Empty Stack");
	else {
		ASTNode* temp = top();
		SStack.pop();
		return temp;
	}
}

void SemanticStack::push(ASTNode* in) {
	SStack.push(in);
}

ASTNode* SemanticStack::top() {
	if (isEmpty()) {
		throw out_of_range("ERROR: Can't call top() on an Empty Stack");
	}
	else {
		return SStack.top();
	}
}

bool SemanticStack::isEmpty() {
	return SStack.empty();
}