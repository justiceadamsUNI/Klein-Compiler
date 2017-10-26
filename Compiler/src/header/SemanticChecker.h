#pragma once
#include "Function.h"
#include <map>
#include <algorithm>

class SemanticChecker {
public:
	SemanticChecker(ASTNode astTree) {
		//Order is very important here.
		tree = astTree;
		SetupInitialSymbolTable(tree);
		checkForUnncalledFunctions(SymbolTable);
		checkValidTypesOnTree(tree);
		checkForUnusedVariables(SymbolTable);
	}

	vector<string> getErrors() {
		return errors;
	}

	vector<string> getWarnings() {
		return warings;
	}

private:
	ASTNode tree = ASTNode(BODY_NODE_TYPE);
	string CurrentFunction;
	map<string, Function> SymbolTable{ };
	vector<string> errors = {};
	vector<string> warings = {};

	void checkForUnncalledFunctions(map<string, Function> SymbolTable) {
		//Assumes a def node type is passsed in.
		map<string, Function>::iterator Iterator;
		for (Iterator = SymbolTable.begin(); Iterator != SymbolTable.end(); Iterator++) {
			if (Iterator->first == "main" || Iterator->first == "print")
			{
				continue;
			}

			if (Iterator->second.getFunctionCallers().empty())
			{
				warings.push_back("WARNING: Unused function " + Iterator->first + "() found. Did you mean to call this function somewhere?");
			}
		}
	}

	void checkForUnusedVariables(map<string, Function> SymbolTable) {
		//Assumes a def node type is passsed in.
		map<string, Function>::iterator Iterator;
		for (Iterator = SymbolTable.begin(); Iterator != SymbolTable.end(); Iterator++) {
			if (Iterator->first == "print")
			{
				continue;
			}

			if (Iterator->second.getUsedVariables().size() != Iterator->second.getParameters().size())
			{
				for (int i = 0; i < Iterator->second.getParameters().size(); i++)
				{
					vector<string> usedVariables = Iterator->second.getUsedVariables();
					string ParamToSearchFor = get<0>(Iterator->second.getParameters().at(i));
					if (find(usedVariables.begin(), usedVariables.end(), ParamToSearchFor) == usedVariables.end())
					{
						warings.push_back("WARNING: Unused variable '" + ParamToSearchFor
							+ "' found in function " + Iterator->first 
							+ "(). Did you mean to use this variable somewhere?");
					}
				}
			}
		}
	}

	void SetupInitialSymbolTable(ASTNode astTree) {
		vector<ASTNode*> list = astTree.getDefinitions()->getDefNodes();

		for (int i = 0; i <list.size(); i++)
		{
			if (SymbolTable.find(list.at(i)->getIdentifierNode()->getIdentifierName()) != SymbolTable.end()) {
				errors.push_back("ERROR: Function " + list.at(i)->getIdentifierNode()->getIdentifierName() + " already defined.");
			}
			Function temp = Function(*list.at(i));
			SymbolTable.insert(std::pair<string, Function>(list.at(i)->getIdentifierNode()->getIdentifierName(), temp));

			vector<string> dupVars = {};
			int functionParamssize = temp.getParameters().empty() ? 0 : temp.getParameters().size() - 1;
			for (int k = 0; k < functionParamssize; k++)
			{
				if (find(dupVars.begin(), dupVars.end(), get<0>(temp.getParameters().at(k))) != dupVars.end()) {
					continue;
				}

				for (int j = k + 1; j < temp.getParameters().size(); j++) {
					if (get<0>(temp.getParameters().at(j)) == get<0>(temp.getParameters().at(k))) {
						errors.push_back("ERROR: Duplicate Variable " + get<0>(temp.getParameters().at(k)) 
							+ " found in function "
							+ list.at(i)->getIdentifierNode()->getIdentifierName() + " definition.");

						dupVars.push_back(get<0>(temp.getParameters().at(k)));
						break;
					}
				}

			}
		}
		//After loop, ensure there is a main
		if (SymbolTable.find("main") == SymbolTable.end()) {
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


	void assignTypeForIdentifierNode(ASTNode& Node);
	void assignTypeForDefNode(ASTNode& Node);
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
	void assignTypeForIntegerLiteralNode(ASTNode& Node);
	void assignTypeForBooleanLiteralNode(ASTNode& Node);
	void assignTypeForPrintStatementNode(ASTNode& Node);


	//helper methods
	ReturnTypes assignTypeForExpressionNode(ASTNode& Node);
	ReturnTypes assignTypeForSimpleExpressionNode(ASTNode& Node);
	ReturnTypes assignTypeForTermNode(ASTNode& Node);
	ReturnTypes assignTypeForFactorNode(ASTNode& Node);
	ReturnTypes assignTypeForLiteralNode(ASTNode& Node);
};