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
			//add function and it's name to the map. {"function1" --->  FuntionObj}
			//Check if string exists in the map here
			SymbolTable.insert(std::pair<string, Function>(list.at(i)->getIdentifierNode()->getIdentifierName(), Function(*list.at(i))));
			
		}
	}

private:
	ASTNode tree = ASTNode(BodyNodeTYPE);
	map<string, Function> SymbolTable{ };
};