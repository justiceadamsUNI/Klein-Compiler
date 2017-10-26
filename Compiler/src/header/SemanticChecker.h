#pragma once
#include "Function.h"
#include <map>
#include <algorithm>

class SemanticChecker {
public:
	SemanticChecker(ASTNode astTree) {
		tree = astTree;
		SetupInitialSymbolTable(tree);
		checkValidTypesOnTree(tree);
	}

	void SetupInitialSymbolTable(ASTNode astTree) {
		vector<ASTNode*> list = astTree.getDefinitions()->getDefNodes();

		for (int i = 0; i <list.size(); i++)
		{
			//add function and it's name to the map. {"function1" --->  FuntionObj}
			if (SymbolTable.find(list.at(i)->getIdentifierNode()->getIdentifierName()) != SymbolTable.end()) {
				errors.push_back("ERROR: Function " + list.at(i)->getIdentifierNode()->getIdentifierName() + " already defined.");
			}
			Function temp = Function(*list.at(i));
			SymbolTable.insert(std::pair<string, Function>(list.at(i)->getIdentifierNode()->getIdentifierName(), temp));
			vector<string> dupVars = {};
			for (int k = 0; k < temp.getParameters("").size()-1; k++)
			{
				if (find(dupVars.begin(), dupVars.end(), get<0>(temp.getParameters("").at(k))) != dupVars.end()) {
					continue;
				}
				vector<tuple<string, ReturnTypes>>::iterator start = std::next(temp.getParameters("").begin(),0);
									
				for (int j = k + 1; j < temp.getParameters("").size(); j++) {
					if (get<0>(temp.getParameters("").at(j)) == get<0>(temp.getParameters("").at(k))) {
						errors.push_back("ERROR: Duplicate Variable " + get<0>(temp.getParameters("").at(k)) + " found in function " + list.at(i)->getIdentifierNode()->getIdentifierName() + " definition.");
						dupVars.push_back(get<0>(temp.getParameters("").at(k)));
						break;
					}
				}

			}
		}
		//After loop, ensure there is a main
		if (SymbolTable.find("main") == SymbolTable.end()){
			errors.push_back("ERROR: Program does not have a function named main");
		}
		//After loop, make sure there isn't a print function
		if (SymbolTable.find("print") != SymbolTable.end()) {
			errors.push_back("ERROR: print function cannot be overloaded");
		}
		//Add generic print function
		else {
			SymbolTable.insert(std::pair<string, Function>("print", Function()));
		}
		
		
	}

	void checkValidTypesOnTree(ASTNode ASTNodeTree) {
		vector<ASTNode*> list = ASTNodeTree.getDefinitions()->getDefNodes();

		for (int i = 0; i <list.size(); i++)
		{
			CurrentFunction = list.at(i)->getIdentifierNode()->getIdentifierName();
			assignTypeForDefNode(*list.at(i));
		}
	}

	vector<string> getErrors() {
		return errors;
	}

private:
	ASTNode tree = ASTNode(BodyNodeTYPE);
	map<string, Function> SymbolTable{ };
	vector<string> errors = {};
	string CurrentFunction;


	void assignTypeForIdentifierNode(ASTNode& Node);
	void assignTypeForDefNode(ASTNode& Node);
	//void assignTypeForFormalsNode(ASTNode& Node); // not needed *i think*
	//void assignTypeForNonEmptyFormalsNode(ASTNode& Node); // not needed *i think*
	//void assignTypeForFormalNode(ASTNode& Node); // not needed because not called by anything
	void assignTypeForBodyNode(ASTNode& Node);
	void assignTypeForTypeNode(ASTNode& Node);
	void assignTypeForLessThanNode(ASTNode& Node);
	void assignTypeForEqualNode(ASTNode& Node);
	void assignTypeForBaseExpressionNode(ASTNode& Node);
	void assignTypeForOrNode(ASTNode& Node);
	void assignTypeForAdditionNode(ASTNode& Node);
	void assignTypeForSubtractionNode(ASTNode& Node);
	void assignTypeForBaseSimpleExpressionNode(ASTNode& Node);
	void assignTypeForAndNode(ASTNode& Node);
	void assignTypeForMultiplicatorNode(ASTNode& Node);
	void assignTypeForDividerNode(ASTNode& Node);
	void assignTypeForBaseTermNode(ASTNode& Node);
	void assignTypeForIfFactorNode(ASTNode& Node);
	void assignTypeForNotFactorNode(ASTNode& Node);
	void assignTypeForLiteralFactorNode(ASTNode& Node);
	void assignTypeForSubtractionFactorNode(ASTNode& Node);
	void assignTypeForParenthesisedExpressionNode(ASTNode& Node);
	void assignTypeForFunctionCallNode(ASTNode& Node);
	void assignTypeForSingletonIdentifierFactorNode(ASTNode& Node);
	//void assignTypeForBaseActualsNode(ASTNode& Node); // not needed *i think*
	//void assignTypeForNonBaseActualsNode(ASTNode& Node); //we need nonemptyactuals implemented before doing this fucker
	//void assignTypeForNonEmptyActualsNode(ASTNode& Node); // not needed *i think*
	void assignTypeForIntegerLiteralNode(ASTNode& Node);
	void assignTypeForBooleanLiteralNode(ASTNode& Node);
	void assignTypeForPrintStatementNode(ASTNode& Node); // not needed *i think*


	//helper methods
	ReturnTypes assignTypeForExpressionNode(ASTNode& Node);
	ReturnTypes assignTypeForSimpleExpressionNode(ASTNode& Node);
	ReturnTypes assignTypeForTermNode(ASTNode& Node);
	ReturnTypes assignTypeForFactorNode(ASTNode& Node);
	ReturnTypes assignTypeForLiteralNode(ASTNode& Node);
};