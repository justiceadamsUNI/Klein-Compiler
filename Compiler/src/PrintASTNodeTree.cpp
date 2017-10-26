#include <iostream>
#include "header/Parser.h"
#include "header/Scanner.h"

const map<ASTNodeType, string> ASTNodePrintMap{
	{ IDENTIFIER_NODE_TYPE, "Identifier Node" },
	{ FACTOR_NODE_TYPE, "Factor Node" },
	{ BASE_ACTUALS_NODE_TYPE, "Base Actuals Node" },
	{ BASE_TERM_NODE_TYPE, "Base Term Node" },
	{ BASE_SIMPLE_EXPR_NODE_TYPE, "Base Simple Expr Node" },
	{ BASE_EXPR_NODE_TYPE, "Base Expr Node" },
	{ FORMALS_NODE_TYPE, "Formals Node" },
	{ PRINT_STATEMENT_NODE_TYPE, "Print Statment Node" },
	{ LITERAL_NODE_TYPE, "Literal Node" },
	{ INTEGER_LITERAL_NODE_TYPE, "Integer Literal Node" },
	{ BOOLEAN_LITERAL_NODE_TYPE, "Boolean Literal Node" },
	{ NON_EMPTY_ACTUALS_NODE_TYPE, "Non Empty Actuals Node" },
	{ NON_BASE_ACTUALS_NODE_TYPE, "Non Base Actuals Node" },
	{ PARENTHESISED_EXPR_FACTOR_NODE_TYPE, "Parenthesised Expr Node" },
	{ SUBTRACTION_FACTOR_NODE_TYPE, "Subtraction Factor Node" },
	{ LITERAL_FACTOR_NODE_TYPE, "Literal Factor Node" },
	{ FUNCTION_CALL_TYPE, "Identifier Factor Node" },
	{ SINGLETON_IDENTIFIER_FACTOR_NODE_TYPE, "Singleton Identifier Factor Node" },
	{ NOT_FACTOR_NODE_TYPE, "Not Factor Node" },
	{ IF_FACTOR_NODE_TYPE, "If Factor Node" },
	{ MULTIPLICATOR_TERM_NODE_TYPE, "Mulitiplicator Term Node" },
	{ DIVIDER_TERM_NODE_TYPE, "Divider Term Node" },
	{ AND_TERM_NODE_TYPE, "And Term Node" },
	{ SUBTRACTOR_SIMPLE_EXPR_NODE_TYPE, "Subtraction Simple Expr Node" },
	{ OR_SIMPLE_EXPR_NODE_TYPE, "Or Simple Expr Node" },
	{ ADDITION_SIMPLE_EXPR_NODE_TYPE, "Addition Simple Expr Node" },
	{ EQUAL_TO_EXPR_NODE_TYPE, "Equal To Expr Node" },
	{ LESS_THAN_EXPR_NODE_TYPE, "Less Than Expr Node" },
	{ TYPE_NODE_TYPE, "Type Node" },
	{ BODY_NODE_TYPE, "Body Node" },
	{ FORMAL_NODE_TYPE, "Formal Node" },
	{ NON_EMPTY_FORMALS_NODE_TYPE, "Non Empty Formals Node" },
	{ DEF_NODE_TYPE, "Def Node" },
	{ DEFINITIONS_NODE_TYPE, "Definitions Node" },
	{ PROGRAM_NODE_TYPE, "Program Node" }
};

void printValueIfExist(ASTNode ASTNodeVar) {
	if (ASTNodeVar.getLiteralValue() != "NULL")
	{
		cout << ASTNodeVar.getLiteralValue() << endl;
	}
	else if (ASTNodeVar.getDataType() != "NULL")
	{
		cout << ASTNodeVar.getDataType() << endl;
	}
	else if (ASTNodeVar.getIdentifierName() != "NULL")
	{
		cout << ASTNodeVar.getIdentifierName() << endl;
	}
	else {
		cout << endl;
	}
}

void printASTNodeTree(ASTNode ASTNodeVar, int treelevel) {
	//Print tabs
	for (int tabs = 0; tabs < treelevel; tabs++)
	{
		cout << "    ";
	}

	//Print Node of level
	cout << ASTNodePrintMap.find(ASTNodeVar.getAstNodeType())->second << ": ";
	printValueIfExist(ASTNodeVar);

	//Print any children nodes if needed (next level of tree)
	if (ASTNodeVar.getFactorNode())
	{
		printASTNodeTree(*ASTNodeVar.getFactorNode(), treelevel + 1);
	}

	if (ASTNodeVar.getFactorNode2())
	{
		printASTNodeTree(*ASTNodeVar.getFactorNode2(), treelevel + 1);
	}

	if (ASTNodeVar.getBaseTermNode())
	{
		printASTNodeTree(*ASTNodeVar.getBaseTermNode(), treelevel + 1);
	}

	if (ASTNodeVar.getBaseTermNode2())
	{
		printASTNodeTree(*ASTNodeVar.getBaseTermNode2(), treelevel + 1);
	}

	if (ASTNodeVar.getBaseSimpleExprNode())
	{
		printASTNodeTree(*ASTNodeVar.getBaseSimpleExprNode(), treelevel + 1);
	}

	if (ASTNodeVar.getBaseSimpleExprNode2())
	{
		printASTNodeTree(*ASTNodeVar.getBaseSimpleExprNode2(), treelevel + 1);
	}

	if (ASTNodeVar.getBaseExprNode())
	{
		printASTNodeTree(*ASTNodeVar.getBaseExprNode(), treelevel + 1);
	}

	if (ASTNodeVar.getBaseExprNode2())
	{
		printASTNodeTree(*ASTNodeVar.getBaseExprNode2(), treelevel + 1);
	}

	if (ASTNodeVar.getBaseExprNode3())
	{
		printASTNodeTree(*ASTNodeVar.getBaseExprNode3(), treelevel + 1);
	}

	for (int i = 0; i < ASTNodeVar.getExpressions().size(); i++)
	{
		printASTNodeTree(*ASTNodeVar.getExpressions()[i], treelevel + 1);
	}

	if (ASTNodeVar.getNonEmptyActualsNode())
	{
		printASTNodeTree(*ASTNodeVar.getNonEmptyActualsNode(), treelevel + 1);
	}

	if (ASTNodeVar.getLiteralNode())
	{
		printASTNodeTree(*ASTNodeVar.getLiteralNode(), treelevel + 1);
	}

	if (ASTNodeVar.getIdentifierNode())
	{
		printASTNodeTree(*ASTNodeVar.getIdentifierNode(), treelevel + 1);
	}

	if (ASTNodeVar.getBaseActualsNode())
	{
		printASTNodeTree(*ASTNodeVar.getBaseActualsNode(), treelevel + 1);
	}

	if (ASTNodeVar.getTypeNode())
	{
		printASTNodeTree(*ASTNodeVar.getTypeNode(), treelevel + 1);
	}

	for (int i = 0; i < ASTNodeVar.getPrintStatements().size(); i++)
	{
		printASTNodeTree(*ASTNodeVar.getPrintStatements()[i], treelevel + 1);
	}

	for (int i = 0; i < ASTNodeVar.getFormalNodes().size(); i++)
	{
		printASTNodeTree(*ASTNodeVar.getFormalNodes()[i], treelevel + 1);
	}

	if (ASTNodeVar.getFormalsNode())
	{
		printASTNodeTree(*ASTNodeVar.getFormalsNode(), treelevel + 1);
	}

	if (ASTNodeVar.getBodyNode())
	{
		printASTNodeTree(*ASTNodeVar.getBodyNode(), treelevel + 1);
	}

	if (ASTNodeVar.getDefinitions())
	{
		printASTNodeTree(*ASTNodeVar.getDefinitions(), treelevel + 1);
	}

	for (int i = 0; i < ASTNodeVar.getDefNodes().size(); i++)
	{
		printASTNodeTree(*ASTNodeVar.getDefNodes()[i], treelevel + 1);
	}
}


int main(int argv, char* argc[])
{
	if (argv == 1) {
		cout << "Please insert a KLEIN file to print the AST structure for." << endl;
		return 1;
	}

	try
	{
		Scanner Scanner(argc[1]);
		Parser Parser(Scanner);

		Parser.parseProgram();
		ASTNode AST = Parser.getFinalASTNode();
		printASTNodeTree(AST, 0);
	}
	catch (const std::exception& e)
	{
		//Print Error Message
		cout << endl << e.what() << endl;
		return 1;
	}
	return 0;
}