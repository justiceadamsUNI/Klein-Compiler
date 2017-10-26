#include "../header/SemanticChecker.h"
#include <string>
#include <tuple>


void SemanticChecker::assignTypeForDefNode(ASTNode& Node) {
	assignTypeForBodyNode(*Node.getBodyNode());
	assignTypeForTypeNode(*Node.getTypeNode());

	if (Node.getTypeNode()->getReturnType() == Node.getBodyNode()->getReturnType())
	{
		return;
	}
	else {
		//ERROR: The return type for {function name} does not match it's definition
		errors.push_back("ERROR: The return type found for " + CurrentFunction + "() does not match it's definition");
	}
}

void SemanticChecker::assignTypeForTypeNode(ASTNode& Node) {
	if (Node.getDataType() == "integer")
	{
		Node.setReturnType(INTEGER_TYPE);
	}
	else if (Node.getDataType() == "boolean") {
		Node.setReturnType(BOOLEAN_TYPE);
	}
}

void SemanticChecker::assignTypeForLessThanNode(ASTNode& Node)
{
	ReturnTypes RightSideType = assignTypeForSimpleExpressionNode(*Node.getBaseSimpleExprNode());
	ReturnTypes LeftSideType  =	assignTypeForSimpleExpressionNode(*Node.getBaseSimpleExprNode2());

	if (RightSideType != INTEGER_TYPE)
	{
		errors.push_back("ERROR: Right side of the < operator is not an integer. found within -  " + CurrentFunction + "()");
	}
	if (LeftSideType != INTEGER_TYPE)
	{
		errors.push_back("ERROR: Left side of the < operator is not an integer. found within -  " + CurrentFunction + "()");
	}

	Node.setReturnType(BOOLEAN_TYPE);
}

void SemanticChecker::assignTypeForEqualNode(ASTNode& Node)
{
	ReturnTypes RightSideType = assignTypeForSimpleExpressionNode(*Node.getBaseSimpleExprNode());
	ReturnTypes LeftSideType = assignTypeForSimpleExpressionNode(*Node.getBaseSimpleExprNode2());

	if (RightSideType != INTEGER_TYPE && RightSideType != BOOLEAN_TYPE)
	{
		errors.push_back("ERROR: Right side of the = operator is not an integer or a boolean. found within -  " + CurrentFunction + "()");
	}
	if (LeftSideType != INTEGER_TYPE && LeftSideType != BOOLEAN_TYPE)
	{
		errors.push_back("ERROR: Left side of the = operator is not an integer or a boolean. found within -  " + CurrentFunction + "()");
	}

	if (RightSideType != LeftSideType)
	{
		errors.push_back("ERROR: Left side of the = operator is of a different type than the right side! found within -  " + CurrentFunction + "()");
	}

	Node.setReturnType(BOOLEAN_TYPE);
}

void SemanticChecker::assignTypeForBaseExpressionNode(ASTNode& Node)
{
	ReturnTypes Type = assignTypeForSimpleExpressionNode(*Node.getBaseSimpleExprNode());
	Node.setReturnType(Type);
}

void SemanticChecker::assignTypeForBodyNode(ASTNode& Node) {
	vector<ASTNode*> PrintStatements = Node.getPrintStatements();

	// Assign types to print statments
	for (int i = 0; i < PrintStatements.size(); i++)
	{
		assignTypeForPrintStatementNode(*PrintStatements[i]);
	}

	ReturnTypes Type = assignTypeForExpressionNode(*Node.getBaseExprNode());
	Node.setReturnType(Type);
}

void SemanticChecker::assignTypeForBaseSimpleExpressionNode(ASTNode& Node)
{
	ReturnTypes Type = assignTypeForTermNode(*Node.getBaseTermNode());
	Node.setReturnType(Type);
}

void SemanticChecker::assignTypeForAdditionNode(ASTNode& Node)
{
	ReturnTypes RightSideType = assignTypeForTermNode(*Node.getBaseTermNode());
	if (RightSideType != INTEGER_TYPE)
	{
		errors.push_back("ERROR: Right side of the + operator is not an integer. found within -  " + CurrentFunction + "()");
	}

	ReturnTypes LeftSideType;
	if (Node.getBaseSimpleExprNode())
	{
		LeftSideType = assignTypeForSimpleExpressionNode(*Node.getBaseSimpleExprNode());
	}
	else {
		LeftSideType = assignTypeForTermNode(*Node.getBaseTermNode2());
	}

	if (LeftSideType != INTEGER_TYPE)
	{
		errors.push_back("ERROR: Left side of the + operator is not an integer. found within -  " + CurrentFunction + "()");
	}

	Node.setReturnType(INTEGER_TYPE);
}

void SemanticChecker::assignTypeForSubtractionNode(ASTNode& Node)
{
	ReturnTypes RightSideType = assignTypeForTermNode(*Node.getBaseTermNode());
	if (RightSideType != INTEGER_TYPE)
	{
		errors.push_back("ERROR: Right side of the - operator is not an integer. found within -  " + CurrentFunction + "()");
	}

	ReturnTypes LeftSideType;
	if (Node.getBaseSimpleExprNode())
	{
		LeftSideType = assignTypeForSimpleExpressionNode(*Node.getBaseSimpleExprNode());
	}
	else {
		LeftSideType = assignTypeForTermNode(*Node.getBaseTermNode2());
	}

	if (LeftSideType != INTEGER_TYPE)
	{
		errors.push_back("ERROR: Left side of the - operator is not an integer. found within -  " + CurrentFunction + "()");
	}

	Node.setReturnType(INTEGER_TYPE);
}

void SemanticChecker::assignTypeForOrNode(ASTNode& Node)
{
	ReturnTypes RightSideType = assignTypeForTermNode(*Node.getBaseTermNode());
	if (RightSideType != BOOLEAN_TYPE)
	{
		errors.push_back("ERROR: Right side of the 'or' operator is not an boolean. found within -  " + CurrentFunction + "()");
	}

	ReturnTypes LeftSideType;
	if (Node.getBaseSimpleExprNode())
	{
		LeftSideType = assignTypeForSimpleExpressionNode(*Node.getBaseSimpleExprNode());
	}
	else {
		LeftSideType = assignTypeForTermNode(*Node.getBaseTermNode2());
	}

	if (LeftSideType != BOOLEAN_TYPE)
	{
		errors.push_back("ERROR: Left side of the 'or' operator is not a boolean. found within -  " + CurrentFunction + "()");
	}

	Node.setReturnType(BOOLEAN_TYPE);
}

void SemanticChecker::assignTypeForBaseTermNode(ASTNode& Node)
{
	ReturnTypes Type = assignTypeForFactorNode(*Node.getFactorNode());
	Node.setReturnType(Type);
}

void SemanticChecker::assignTypeForMultiplicatorNode(ASTNode& Node)
{
	ReturnTypes RightSideType = assignTypeForFactorNode(*Node.getFactorNode());
	if (RightSideType != INTEGER_TYPE)
	{
		errors.push_back("ERROR: Right side of the * operator is not an integer. found within -  " + CurrentFunction + "()");
	}

	ReturnTypes LeftSideType;
	if (Node.getBaseSimpleExprNode())
	{
		LeftSideType = assignTypeForSimpleExpressionNode(*Node.getBaseSimpleExprNode());
	}
	else if (Node.getBaseTermNode())
	{
		LeftSideType = assignTypeForTermNode(*Node.getBaseTermNode());
	}
	else {
		LeftSideType = assignTypeForFactorNode(*Node.getFactorNode2());
	}

	if (LeftSideType != INTEGER_TYPE)
	{
		errors.push_back("ERROR: Left side of the * operator is not an integer. found within -  " + CurrentFunction + "()");
	}

Node.setReturnType(INTEGER_TYPE);
}

void SemanticChecker::assignTypeForDividerNode(ASTNode& Node)
{
	ReturnTypes RightSideType = assignTypeForFactorNode(*Node.getFactorNode());
	if (RightSideType != INTEGER_TYPE)
	{
		errors.push_back("ERROR: Right side of the / operator is not an integer. found within -  " + CurrentFunction + "()");
	}

	ReturnTypes LeftSideType;
	if (Node.getBaseSimpleExprNode())
	{
		LeftSideType = assignTypeForSimpleExpressionNode(*Node.getBaseSimpleExprNode());
	}
	else if (Node.getBaseTermNode())
	{
		LeftSideType = assignTypeForTermNode(*Node.getBaseTermNode());
	}
	else {
		LeftSideType = assignTypeForFactorNode(*Node.getFactorNode2());
	}

	if (LeftSideType != INTEGER_TYPE)
	{
		errors.push_back("ERROR: Left side of the / operator is not an integer. found within -  " + CurrentFunction + "()");
	}

	Node.setReturnType(INTEGER_TYPE);
}

void SemanticChecker::assignTypeForAndNode(ASTNode& Node)
{
	ReturnTypes RightSideType = assignTypeForFactorNode(*Node.getFactorNode());
	if (RightSideType != BOOLEAN_TYPE)
	{
		errors.push_back("ERROR: Right side of the 'and' operator is not a boolean. found within -  " + CurrentFunction + "()");
	}

	ReturnTypes LeftSideType;
	if (Node.getBaseTermNode())
	{
		LeftSideType = assignTypeForTermNode(*Node.getBaseTermNode());
	}
	else {
		LeftSideType = assignTypeForFactorNode(*Node.getFactorNode2());
	}

	if (LeftSideType != BOOLEAN_TYPE)
	{
		errors.push_back("ERROR: Left side of the 'and' operator is not a boolean. found within -  " + CurrentFunction + "()");
	}

	Node.setReturnType(BOOLEAN_TYPE);
}

void SemanticChecker::assignTypeForParenthesisedExpressionNode(ASTNode& Node)
{
	ReturnTypes Type = assignTypeForExpressionNode(*Node.getBaseExprNode());
	Node.setReturnType(Type);
}

void SemanticChecker::assignTypeForSubtractionFactorNode(ASTNode& Node)
{
	ReturnTypes Type = assignTypeForFactorNode(*Node.getFactorNode());

	if (Type != INTEGER_TYPE)
	{
		errors.push_back("ERROR: you can't apply a negative operator to a non-integer data value. found within -  " + CurrentFunction + "()");
	}

	Node.setReturnType(INTEGER_TYPE);
}

void SemanticChecker::assignTypeForLiteralFactorNode(ASTNode& Node)
{
	ReturnTypes Type = assignTypeForLiteralNode(*Node.getLiteralNode());

	Node.setReturnType(Type);
}

void SemanticChecker::assignTypeForFunctionCallNode(ASTNode& Node)
{
	string FunctionName = Node.getIdentifierNode()->getIdentifierName();

	if (SymbolTable.find(FunctionName) != SymbolTable.end())
	{
		if (Node.getBaseActualsNode()->getAstNodeType() == BASE_ACTUALS_NODE_TYPE) {
			if (!SymbolTable.find(FunctionName)->second.getParameters().empty()) {
				errors.push_back("ERROR: you attempted to call " + FunctionName + " with no paramaters. found within -  " + CurrentFunction + "()");
			}
		}
		//There are parameters in the function call
		else {
			vector<ASTNode*> functionParams = Node.getBaseActualsNode()->getNonEmptyActualsNode()->getExpressions();
			vector<tuple<string, ReturnTypes>> symbolTableParams = SymbolTable.find(FunctionName)->second.getParameters();
			ReturnTypes type = NO_RETURN_TYPE;
			if (functionParams.size() == symbolTableParams.size()) {
				for (int i = functionParams.size() - 1, j = 0; i >= 0, j < symbolTableParams.size(); i--, j++) {
					type = assignTypeForExpressionNode(*functionParams.at(i));
					if (type == get<1>(symbolTableParams.at(j))) {
						continue;
					}
					else
					{
						errors.push_back("ERROR: There was a type mismatch when calling function " + FunctionName + ".found within - " + CurrentFunction + "()");
						break;
					}
				}
			}
			else
			{
				errors.push_back("ERROR: You attempted to call " + FunctionName + "() with a mismatching number of arguments. This function expects " + to_string(functionParams.size())+  " parameters. found within - " + CurrentFunction + "()");
			}
		}

		//set this expression node type to that of the function call. Also put the CurrentFunction into the called functions list.
		SymbolTable.find(FunctionName)->second.addFunctionCallers(CurrentFunction);
		Node.setReturnType(SymbolTable.find(FunctionName)->second.getReturnType());
	}
	else {
		errors.push_back("ERROR: Call to function " + FunctionName + ": Function does not exist.");
	}
}


void SemanticChecker::assignTypeForSingletonIdentifierFactorNode(ASTNode& Node)
{
	assignTypeForIdentifierNode(*Node.getIdentifierNode());
	Node.setReturnType(Node.getIdentifierNode()->getReturnType());
}

void SemanticChecker::assignTypeForNotFactorNode(ASTNode& Node)
{
	ReturnTypes Type = assignTypeForFactorNode(*Node.getFactorNode());
	if (Type != BOOLEAN_TYPE)
	{
		errors.push_back("ERROR: you can't negate a non integer data value. found within -  " + CurrentFunction + "()");
	}
	Node.setReturnType(BOOLEAN_TYPE);
}

void SemanticChecker::assignTypeForIfFactorNode(ASTNode& Node)
{
	ReturnTypes ElseType = assignTypeForExpressionNode(*Node.getBaseExprNode());
	ReturnTypes ThenType = assignTypeForExpressionNode(*Node.getBaseExprNode2());
	ReturnTypes IfCheck = assignTypeForExpressionNode(*Node.getBaseExprNode3());
	
	if (IfCheck != BOOLEAN_TYPE)
	{
		errors.push_back("ERROR: If statement condition needs to be of type boolean. found within -  " + CurrentFunction + "()");
	}

	if (ThenType == ElseType)
	{
		Node.setReturnType(ThenType);
	}
	else {
		Node.setReturnType(OR_TYPE);
	}

}

void SemanticChecker::assignTypeForIdentifierNode(ASTNode& Node)
{
	vector<tuple<string, ReturnTypes>> symbolTableParams = SymbolTable.find(CurrentFunction)->second.getParameters();
	ReturnTypes type = NO_RETURN_TYPE;
	for (int i = 0; i < symbolTableParams.size(); i++) {
		if (Node.getIdentifierName() == get<0>(symbolTableParams.at(i))) {
			//add variable to used variable list and assign type to it
			SymbolTable.find(CurrentFunction)->second.variableUsed(Node.getIdentifierName());
			type = get<1>(symbolTableParams.at(i));
			break;
		}
	}
	if (type == NO_RETURN_TYPE)
	{
		errors.push_back("ERROR: variable " + Node.getIdentifierName() + " has not been declared.");
	}
	Node.setReturnType(type);
}

void SemanticChecker::assignTypeForIntegerLiteralNode(ASTNode& Node)
{
	Node.setReturnType(INTEGER_TYPE);
}

void SemanticChecker::assignTypeForBooleanLiteralNode(ASTNode& Node)
{
	Node.setReturnType(BOOLEAN_TYPE);
}

void SemanticChecker::assignTypeForPrintStatementNode(ASTNode& Node)
{
	assignTypeForExpressionNode(*Node.getBaseExprNode());
	SymbolTable.find("print")->second.addFunctionCallers(CurrentFunction);
	Node.setReturnType(NO_RETURN_TYPE);
}

// helper methods -------------------------------------------------------
ReturnTypes SemanticChecker::assignTypeForExpressionNode(ASTNode& Node) {
	if (Node.getAstNodeType() == BASE_EXPR_NODE_TYPE)
	{
		assignTypeForBaseExpressionNode(Node);
		return Node.getReturnType();
	}
	else if (Node.getAstNodeType() == LESS_THAN_EXPR_NODE_TYPE)
	{
		assignTypeForLessThanNode(Node);
		return BOOLEAN_TYPE;
	}
	else {
		assignTypeForEqualNode(Node);
		return BOOLEAN_TYPE;
	}
}

ReturnTypes SemanticChecker::assignTypeForSimpleExpressionNode(ASTNode& Node) {
	if (Node.getAstNodeType() == BASE_SIMPLE_EXPR_NODE_TYPE)
	{
		assignTypeForBaseSimpleExpressionNode(Node);
		return Node.getReturnType();
	}
	else if (Node.getAstNodeType() == ADDITION_SIMPLE_EXPR_NODE_TYPE)
	{
		assignTypeForAdditionNode(Node);
		return INTEGER_TYPE;

	}
	else if (Node.getAstNodeType() == SUBTRACTOR_SIMPLE_EXPR_NODE_TYPE)
	{
		assignTypeForSubtractionNode(Node);
		return INTEGER_TYPE;

	}
	else {
		assignTypeForOrNode(Node);
		return BOOLEAN_TYPE;
	}

}

ReturnTypes SemanticChecker::assignTypeForTermNode(ASTNode& Node)
{
	if (Node.getAstNodeType() == BASE_TERM_NODE_TYPE)
	{
		assignTypeForBaseTermNode(Node);
		return Node.getReturnType();

	}
	else if (Node.getAstNodeType() == MULTIPLICATOR_TERM_NODE_TYPE)
	{
		assignTypeForMultiplicatorNode(Node);
		return INTEGER_TYPE;

	}
	else if (Node.getAstNodeType() == DIVIDER_TERM_NODE_TYPE)
	{
		assignTypeForDividerNode(Node);
		return INTEGER_TYPE;
	}
	else {
		assignTypeForAndNode(Node);
		return BOOLEAN_TYPE;
	}
}

ReturnTypes SemanticChecker::assignTypeForFactorNode(ASTNode& Node)
{
	if (Node.getAstNodeType() == PARENTHESISED_EXPR_FACTOR_NODE_TYPE)
	{
		assignTypeForParenthesisedExpressionNode(Node);
		return Node.getReturnType();

	}
	else if (Node.getAstNodeType() == SUBTRACTION_FACTOR_NODE_TYPE)
	{
		assignTypeForSubtractionFactorNode(Node);
		return INTEGER_TYPE;
	}
	else if (Node.getAstNodeType() == LITERAL_FACTOR_NODE_TYPE)
	{
		assignTypeForLiteralFactorNode(Node);
		return Node.getReturnType();
	}
	else if (Node.getAstNodeType() == FUNCTION_CALL_TYPE)
	{
		assignTypeForFunctionCallNode(Node);
		return Node.getReturnType();
	}
	else if (Node.getAstNodeType() == SINGLETON_IDENTIFIER_FACTOR_NODE_TYPE)
	{
		assignTypeForSingletonIdentifierFactorNode(Node);
		return Node.getReturnType();
	}
	else if (Node.getAstNodeType() == NOT_FACTOR_NODE_TYPE)
	{
		assignTypeForNotFactorNode(Node);
		return BOOLEAN_TYPE;
	}
	else {
		assignTypeForIfFactorNode(Node);
		// not a boolean because it could be an OR type
		return Node.getReturnType();
	}
}

ReturnTypes SemanticChecker::assignTypeForLiteralNode(ASTNode& Node)
{
	if (Node.getAstNodeType() == INTEGER_LITERAL_NODE_TYPE)
	{
		assignTypeForIntegerLiteralNode(Node);
		return INTEGER_TYPE;
	}
	else {
		assignTypeForBooleanLiteralNode(Node);
		return BOOLEAN_TYPE;
	}
}
