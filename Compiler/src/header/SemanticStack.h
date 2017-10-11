#pragma once
#include "ASTNodes.h"
#include <stack>
#include <stdexcept>

using namespace std;


// Throws Error for popping an empty stack
// A Stack used to hold states of the parser
class SemanticStack
{

public:
	SemanticStack();

	ASTNode* pop();

	ASTNode* top();

	void push(ASTNode*);

	bool isEmpty();

private:
	stack<ASTNode*> SStack;
};

