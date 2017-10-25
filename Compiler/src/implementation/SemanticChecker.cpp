#include"../header/SemanticChecker.h"


void SemanticChecker::assignTypeForDefNode(ASTNode Node) {
	assignTypeForBodyNode(*Node.getBodyNode());
	assignTypeForTypeNode(*Node.getTypeNode());

	if (Node.getTypeNode()->getReturnType() == Node.getBodyNode()->getReturnType())
	{
		//happy
	}
	else {
		//ERROR: The return type for {function name} does not match it's definition
	}
}

void SemanticChecker::assignTypeForTypeNode(ASTNode Node) {
	if (Node.getDataType() == "integer")
	{
		Node.setReturnType(INTEGER_TYPE);
	}
	else if (Node.getDataType() == "boolean") {
		Node.setReturnType(BOOLEAN_TYPE);
	}
}

void SemanticChecker::assignTypeForLessThanNode(ASTNode Node)
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

void SemanticChecker::assignTypeForEqualNode(ASTNode Node)
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

void SemanticChecker::assignTypeForBaseExpressionNode(ASTNode Node)
{
	ReturnTypes Type = assignTypeForSimpleExpressionNode(*Node.getBaseSimpleExprNode());
	Node.setReturnType(Type);
}

void SemanticChecker::assignTypeForBodyNode(ASTNode Node) {
	vector<ASTNode*> PrintStatements = Node.getPrintStatements();

	// Assign types to print statments
	for (int i = 0; i < PrintStatements.size(); i++)
	{
		assignTypeForPrintStatementNode(*PrintStatements[i]);
	}

	ReturnTypes Type = assignTypeForExpressionNode(*Node.getBaseExprNode());
	Node.setReturnType(Type);
}

void SemanticChecker::assignTypeForBaseSimpleExpressionNode(ASTNode Node)
{
	ReturnTypes Type = assignTypeForTermNode(*Node.getBaseTermNode());
	Node.setReturnType(Type);
}

void SemanticChecker::assignTypeForAdditionNode(ASTNode Node)
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

void SemanticChecker::assignTypeForSubtractionNode(ASTNode Node)
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

void SemanticChecker::assignTypeForOrNode(ASTNode Node)
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

void SemanticChecker::assignTypeForBaseTermNode(ASTNode Node)
{
	ReturnTypes Type = assignTypeForFactorNode(*Node.getFactorNode());
	Node.setReturnType(Type);
}

void SemanticChecker::assignTypeForMultiplicatorNode(ASTNode Node)
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

void SemanticChecker::assignTypeForDividerNode(ASTNode Node)
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

void SemanticChecker::assignTypeForAndNode(ASTNode Node)
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

void SemanticChecker::assignTypeForParenthesisedExpressionNode(ASTNode Node)
{
	ReturnTypes Type = assignTypeForExpressionNode(*Node.getBaseExprNode());
	Node.setReturnType(Type);
}

void SemanticChecker::assignTypeForSubtractionFactorNode(ASTNode Node)
{
	ReturnTypes Type = assignTypeForFactorNode(*Node.getFactorNode());

	if (Type != INTEGER_TYPE)
	{
		errors.push_back("ERROR: you can't apply a negative operator to a non-integer data value. found within -  " + CurrentFunction + "()");
	}

	Node.setReturnType(INTEGER_TYPE);
}

void SemanticChecker::assignTypeForLiteralFactorNode(ASTNode Node)
{
	ReturnTypes Type = assignTypeForLiteralNode(*Node.getLiteralNode());
	
	Node.setReturnType(Type);
}

void SemanticChecker::assignTypeForFunctionCallNode(ASTNode Node)
{
	string FunctionName = Node.getIdentifierNode()->getIdentifierName();

	if (SymbolTable.find(FunctionName) != SymbolTable.end())
	{
		if (Node.getBaseActualsNode()->getAstNodeType() == BaseActualsNodeTYPE)
		{
			if (!(SymbolTable.find(FunctionName)->second.getParameters().empty()))
			{
				errors.push_back("ERROR: you attempted to call " + FunctionName + " with no paramaters.");
			}
		}
		//There are paramaters in the funciton call
		else {
			//this is a non empty actuals. It has variable many expression nodes.
			//loop over every expression node, call assignTypeForExpressionNode() and compare
			//its return value to that of the corresponding paramater (which are in the correct order).
			//i *think* you have to loop through the expression nodes in reverse order. Call 
			// Node.getBaseActualsNode() -> getExpressions() to get the expressions.

		}
	}
}




































//helper methods ------------------------------------------------------------------------
ReturnTypes SemanticChecker::assignTypeForExpressionNode(ASTNode Node) {
	if (Node.getAstNodeType() == BaseExprNodeType)
	{
		assignTypeForBaseExpressionNode(Node);
		return Node.getReturnType();
	}
	else if (Node.getAstNodeType() == LessThanExprNodeTYPE)
	{
		assignTypeForLessThanNode(Node);
		return BOOLEAN_TYPE;
	}
	else {
		assignTypeForEqualNode(Node);
		return BOOLEAN_TYPE;
	}
}

ReturnTypes SemanticChecker::assignTypeForSimpleExpressionNode(ASTNode Node) {
	if (Node.getAstNodeType() == BaseSimpleExprNodeType)
	{
		assignTypeForBaseSimpleExpressionNode(Node);
		return Node.getReturnType();
	}
	else if (Node.getAstNodeType() == AdditionSimpleExprNodeTYPE)
	{
		assignTypeForAdditionNode(Node);
		return INTEGER_TYPE;

	}
	else if (Node.getAstNodeType() == SubtractorSimpleExprNodeTYPE)
	{
		assignTypeForSubtractionNode(Node);
		return INTEGER_TYPE;

	}
	else {
		assignTypeForOrNode(Node);
		return BOOLEAN_TYPE;
	}

}

ReturnTypes SemanticChecker::assignTypeForTermNode(ASTNode Node)
{
	if (Node.getAstNodeType() == BaseTermNodeTYPE)
	{
		assignTypeForBaseTermNode(Node);
		return Node.getReturnType();

	}
	else if (Node.getAstNodeType() == MultiplicatorTermNodeTYPE)
	{
		assignTypeForMultiplicatorNode(Node);
		return INTEGER_TYPE;

	}
	else if (Node.getAstNodeType() == DividerTermNodeTYPE)
	{
		assignTypeForDividerNode(Node);
		return INTEGER_TYPE;
	}
	else {
		assignTypeForAndNode(Node);
		return BOOLEAN_TYPE;
	}
}

ReturnTypes SemanticChecker::assignTypeForFactorNode(ASTNode Node)
{
	if (Node.getAstNodeType() == ParenExprFactorNodeTYPE)
	{
		assignTypeForParenthesisedExpressionNode(Node);
		return Node.getReturnType();

	}
	else if (Node.getAstNodeType() == SubtractionFactorNodeTYPE)
	{
		assignTypeForSubtractionFactorNode(Node);
		return INTEGER_TYPE;
	}
	else if (Node.getAstNodeType() == LiteralFactorNodeTYPE)
	{
		assignTypeForLiteralFactorNode(Node);
		return Node.getReturnType();
	}
	else if (Node.getAstNodeType() == FunctionCallType)
	{
		assignTypeForFunctionCallNode(Node);
		return Node.getReturnType();
	}
	else if (Node.getAstNodeType() == SingletonIdentifierFactorNodeTYPE)
	{
		assignTypeForSingletonIdentifierFactorNode(Node);
		return Node.getReturnType();
	}
	else if (Node.getAstNodeType() == NotFactorNodeTYPE)
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