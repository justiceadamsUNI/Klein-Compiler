#pragma once
#include "ReturnType.h"
#include "ASTNodes.h"
#include <tuple>
#include <vector>
#include <algorithm>

using namespace std;

class Function {
public:
	Function(ASTNode functionDefinition) {
		Type = convertToReturnType(functionDefinition);
		setParameters(functionDefinition);
	}

	// below constructor is only used for print statements
	Function() {
		Type = NO_RETURN_TYPE;
	}
	
	ReturnTypes getReturnType() {
		return Type;
	}

	vector<string> getFunctionCallers() {
		return FunctionCallers;
	}

	vector<string> getUsedVariables() {
		return UsedVariables;
	}

	void variableUsed(string variable) {
		if (find(UsedVariables.begin(), UsedVariables.end(), variable) == UsedVariables.end()) {
			UsedVariables.push_back(variable);
		}
	}

	void addFunctionCallers(string in) {
		if (in != "" && !(find(FunctionCallers.begin(), FunctionCallers.end(), in) != FunctionCallers.end())) {
			FunctionCallers.push_back(in);
		}
	}

	vector<tuple<string, ReturnTypes>> getParameters() {
		return Parameters;
	}

private:
	ReturnTypes Type;
	vector<tuple<string, ReturnTypes>> Parameters = {};
	vector<string> FunctionCallers;
	vector<string> UsedVariables;


	ReturnTypes convertToReturnType(ASTNode returnType) {
		//This is assuming that returnType is a Def Node
		if (returnType.getAstNodeType() == DEF_NODE_TYPE) {
			 if (returnType.getTypeNode()->getDataType() == "integer") {
				 return INTEGER_TYPE;
			 }
			 if (returnType.getTypeNode()->getDataType() == "boolean") {
				 return BOOLEAN_TYPE;
			 }
			 else {
				 return NO_RETURN_TYPE;
			 }
		}
		else
		{
			throw runtime_error("ERROR: attempted to call ConvertToReturnType on a node that isn't a Def Node!");
		}
	};

	void setParameters(ASTNode inputParameters) {
		// Assuming we are getting a Def Node
		ReturnTypes DataType = NO_RETURN_TYPE;
		vector<ASTNode*> Temp;
		
		// NOTE: The formals are in reverse order by the way we build that vector.
		if (inputParameters.getAstNodeType() == DEF_NODE_TYPE) {
			Temp = inputParameters.getFormalsNode()->getFormalNodes();
			for (int i = Temp.size() - 1; i >= 0; i--) {
				if (Temp.at(i)->getTypeNode()->getDataType() == "integer") {
					DataType = INTEGER_TYPE;
				}
				if (Temp.at(i)->getTypeNode()->getDataType() == "boolean") {
					DataType = BOOLEAN_TYPE;
				}
				// creates a list of tuples representing the function paramaters
				Parameters.push_back(make_tuple((Temp.at(i)->getIdentifierNode()->getIdentifierName()), DataType));
			}
		}
	}
};