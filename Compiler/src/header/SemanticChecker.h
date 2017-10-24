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
			if (SymbolTable.find(list.at(i)->getIdentifierNode()->getIdentifierName()) != SymbolTable.end()) {
				errors.push_back("ERROR: Function " + list.at(i)->getIdentifierNode()->getIdentifierName() + " already defined.");
			}
			SymbolTable.insert(std::pair<string, Function>(list.at(i)->getIdentifierNode()->getIdentifierName(), Function(*list.at(i))));
			
		}
		//After loop, ensure there is a main
		//After loop, make sure there isn't a print function
		//Add generic print function
	}
	void checkValidTypesOnTree();

private:
	ASTNode tree = ASTNode(BodyNodeTYPE);
	map<string, Function> SymbolTable{ };
	vector<string> errors = {};

	void assignTypeForIdentifierNode(ASTNode Node);
	void assignTypeForDefNode(ASTNode Node);
	void assignTypeForFormalsNode(ASTNode Node);
	void assignTypeForNonEmptyFormalsNode(ASTNode Node);
	void assignTypeForFormalNode(ASTNode Node);
	void assignTypeForBodyNode(ASTNode Node);
	void assignTypeForTypeNode(ASTNode Node);
	void assignTypeForLessThanNode(ASTNode Node);
	void assignTypeForEqualNode(ASTNode Node);
	void assignTypeForBaseExpressionNode(ASTNode Node);
	void assignTypeForOrNode(ASTNode Node);
	void assignTypeForAdditionNode(ASTNode Node);
	void assignTypeForSubtractionNode(ASTNode Node);
	void assignTypeForBaseSimpleExpressionNode(ASTNode Node);
	void assignTypeForAndNode(ASTNode Node);
	void assignTypeForMultiplicatorNode(ASTNode Node);
	void assignTypeForDividerNode(ASTNode Node);
	void assignTypeForBaseTermNode(ASTNode Node);
	void assignTypeForIfFactorNode(ASTNode Node);
	void assignTypeForNotFactorNode(ASTNode Node);
	void assignTypeForLiteralFactorNode(ASTNode Node);
	void assignTypeForSubtractionFactorNode(ASTNode Node);
	void assignTypeForParenthesisedExpressionNode(ASTNode Node);
	void assignTypeForIdentifierActualsNode(ASTNode Node);
	void assignTypeForSingletonIdentifierFactorNode(ASTNode Node);
	void assignTypeForBaseActualsNode(ASTNode Node);
	void assignTypeForNonBaseActualsNode(ASTNode Node);
	void assignTypeForNonEmptyActualsNode(ASTNode Node);
	void assignTypeForIntegerLiteralNode(ASTNode Node);
	void assignTypeForBooleanLiteralNode(ASTNode Node);
	void assignTypeForPrintStatementNode(ASTNode Node);


	//helper methods
	ReturnTypes assignTypeForExpressionNode(ASTNode Node);
};