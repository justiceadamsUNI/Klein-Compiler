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
		type = convertToReturnType(functionDefinition);
		setParameters(functionDefinition);
	}
	Function() {
		type = NO_RETURN_TYPE;
	}
	
	ReturnTypes getReturnType() {
		return type;
	}

	vector<string> getFunctionCallers() {
		return functionCallers;
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
		if (in != "" && !(find(functionCallers.begin(), functionCallers.end(), in) != functionCallers.end())) {
			functionCallers.push_back(in);
		}
	}
	vector<tuple<string, ReturnTypes>> getParameters() {
		return parameters;
	}

private:
	ReturnTypes type;
	vector<tuple<string, ReturnTypes>> parameters = {};
	vector<string> functionCallers;
	vector<string> UsedVariables;
	vector<ASTNode*> temp;


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
			//Need to change implementation to add error messages into a list and then output
			//cout << "The node given was not a Def Node!!" << endl;
			return NO_RETURN_TYPE;
		}
	};
	void setParameters(ASTNode inputParameters) {
		ReturnTypes dataType = NO_RETURN_TYPE;
		//Assuming we are getting a Def Node
		//NOTE: The formals are in reverse order by the way we build that vector.
		if (inputParameters.getAstNodeType() == DEF_NODE_TYPE) {
			temp = inputParameters.getFormalsNode()->getFormalNodes();
			for (int i = temp.size() - 1; i >= 0; i--) {
				if (temp.at(i)->getTypeNode()->getDataType() == "integer") {
					dataType = INTEGER_TYPE;
				}
				if (temp.at(i)->getTypeNode()->getDataType() == "boolean") {
					dataType = BOOLEAN_TYPE;
				}

				//Need to chek if datatype still equals NO_RETURN_TYPE, then add an error to the list
				parameters.push_back(make_tuple((temp.at(i)->getIdentifierNode()->getIdentifierName()), dataType));
			}
		}
	}
};