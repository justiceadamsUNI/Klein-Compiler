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

	//stuff
	//if getBaseExprnode, then stuff, else if getAndexpression, then somehting else, 
}