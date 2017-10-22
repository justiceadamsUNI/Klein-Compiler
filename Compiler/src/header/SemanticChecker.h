#pragma once
#include "Function.h"
#include <map>

class SemanticChecker {
public:
	SemanticChecker(ASTNode astTree) {
		tree = astTree;
		SetupInitialSymbolTable(tree);
	}

	void SetupInitialSymbolTable(ASTNode astTree) {

	}
private:
	ASTNode tree(ProgramNodeTYPE);
};