#pragma once
#include "Function.h"
#include <map>
#include <algorithm>

class SemanticChecker {
public:
	SemanticChecker(ASTNode astTree) {
		// Order is very important here.
		Tree = astTree;
		SetupInitialSymbolTable(Tree);
		checkValidTypesOnTree(Tree);
		checkForUnncalledFunctions(SymbolTable);
		checkForUnusedVariables(SymbolTable);
	}

	vector<string> getErrors() {
		return Errors;
	}

	vector<string> getWarnings() {
		return Warings;
	}

	map<string, Function> getSymbolTable() {
		return SymbolTable;
		
	}

private:
	ASTNode Tree = ASTNode(BODY_NODE_TYPE);
	string CurrentFunction;
	map<string, Function> SymbolTable{ };
	vector<string> Errors = {};
	vector<string> Warings = {};

	void checkForUnncalledFunctions(map<string, Function> SymbolTable) {
		map<string, Function>::iterator Iterator;
		for (Iterator = SymbolTable.begin(); Iterator != SymbolTable.end(); Iterator++) {
			if (Iterator->first == "main" || Iterator->first == "print")
			{
				continue;
			}

			if (Iterator->second.getFunctionCallers().empty())
			{
				Warings.push_back("WARNING: Unused function " + Iterator->first + "() found. Did you mean to call this function somewhere?");
			}
		}
	}

	void checkForUnusedVariables(map<string, Function> SymbolTable) {
		map<string, Function>::iterator Iterator;

		// loop through variables
		for (Iterator = SymbolTable.begin(); Iterator != SymbolTable.end(); Iterator++) {
			if (Iterator->first == "print")
			{
				continue;
			}

			if (Iterator->second.getUsedVariables().size() != Iterator->second.getParameters().size())
			{
				// if mismatch in variables/params sizes, an element is not used, find which one.
				for (int i = 0; i < Iterator->second.getParameters().size(); i++)
				{
					vector<string> UsedVariables = Iterator->second.getUsedVariables();
					string ParamToSearchFor = get<0>(Iterator->second.getParameters().at(i));
					if (find(UsedVariables.begin(), UsedVariables.end(), ParamToSearchFor) == UsedVariables.end())
					{
						Warings.push_back("WARNING: Unused variable '" + ParamToSearchFor
							+ "' found in function " + Iterator->first 
							+ "(). Did you mean to use this variable somewhere?");
					}
				}
			}
		}
	}

	void SetupInitialSymbolTable(ASTNode AstTree) {
		// does initial pass through the functions in a program
		// and creates an initial symbol table. Detects duplicate variables/functions as well.
		vector<ASTNode*> List = AstTree.getDefinitions()->getDefNodes();

		for (int i = 0; i <List.size(); i++)
		{
			// check if function already defined.
			if (SymbolTable.find(List.at(i)->getIdentifierNode()->getIdentifierName()) != SymbolTable.end()) {
				Errors.push_back("ERROR: Function " + List.at(i)->getIdentifierNode()->getIdentifierName() + " already defined.");
			}

			// put pair into the map
			Function Temp = Function(*List.at(i));
			SymbolTable.insert(std::pair<string, Function>(List.at(i)->getIdentifierNode()->getIdentifierName(), Temp));

			//check for duplicate variables. Only print error for each duplicate variable once.
			vector<string> DupVars = {};
			int FunctionParamsSize = Temp.getParameters().empty() ? 0 : Temp.getParameters().size() - 1;
			for (int k = 0; k < FunctionParamsSize; k++)
			{
				if (find(DupVars.begin(), DupVars.end(), get<0>(Temp.getParameters().at(k))) != DupVars.end()) {
					continue;
				}

				for (int j = k + 1; j < Temp.getParameters().size(); j++) {
					if (get<0>(Temp.getParameters().at(j)) == get<0>(Temp.getParameters().at(k))) {
						Errors.push_back("ERROR: Duplicate variable '" + get<0>(Temp.getParameters().at(k))
							+ "' found in function "
							+ List.at(i)->getIdentifierNode()->getIdentifierName() + " definition.");

						DupVars.push_back(get<0>(Temp.getParameters().at(k)));
						break;
					}
				}

			}
		}
		//After loop, ensure there is a main
		if (SymbolTable.find("main") == SymbolTable.end()) {
			Errors.push_back("ERROR: Program does not have a function named main");
		}
		//After loop, make sure there isn't a print function
		if (SymbolTable.find("print") != SymbolTable.end()) {
			Errors.push_back("ERROR: print function cannot be overloaded");
		}
		//Add generic print function
		else {
			SymbolTable.insert(std::pair<string, Function>("print", Function()));
		}
	}

	// the method that actually kicks off type checking for our AST tree
	void checkValidTypesOnTree(ASTNode ASTNodeTree) {
		vector<ASTNode*> List = ASTNodeTree.getDefinitions()->getDefNodes();

		for (int i = 0; i <List.size(); i++)
		{
			CurrentFunction = List.at(i)->getIdentifierNode()->getIdentifierName();
			assignTypeForDefNode(*List.at(i));
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