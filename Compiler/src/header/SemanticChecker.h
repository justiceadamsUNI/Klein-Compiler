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
		vector<ASTNode*> list = astTree.getDefinitions()->getDefNodes();

		for (int i = 0; i <list.size(); i++)
		{
			Function(*list.at(i));
			//add function and it's name to the map. {"function1" --->  FuntionObj}
		}
	}

private:
	ASTNode tree = ASTNode(BodyNodeTYPE);
};