#include <iostream>
#include "header/Parser.h"
#include "header/Scanner.h"

const map<ASTNodeType, string> ASTNodePrintMap{
	{ IDENTIFIERNODETYPE, "Identifier Node" },
	{ FactorNodeTYPE, "Factor Node" },
	{ BaseActualsNodeTYPE, "Base Actuals Node" },
	{ BaseTermNodeTYPE, "Base Term Node" },
	{ BaseSimpleExprNodeType, "Base Simple Expr Node" },
	{ BaseExprNodeType, "Base Expr Node" },
	{ FormalsNodeType, "Formals Node" },
	{ PrintStatemetNodeTYPE, "Print Statment Node" },
	{ LiteralNodeTYPE, "Literal Node" },
	{ IntegerLiteralNodeTYPE, "Integer Literal Node" },
	{ BooleanLiteralNodeTYPE, "Boolean Literal Node" },
	{ NonEmptyActualsNodeTYPE, "Non Empty Actuals Node" },
	{ NonBaseActualsNodeTYPE, "Non Base Actuals Node" },
	{ ParenExprFactorNodeTYPE, "Parenthesised Expr Node" },
	{ SubtractionFactorNodeTYPE, "Subtraction Factor Node" },
	{ LiteralFactorNodeTYPE, "Literal Factor Node" },
	{ IdentifierFactorNodeTYPE, "Identifier Factor Node" },
	{ SingletonIdentifierFactorNodeTYPE, "Singleton Identifier Factor Node" },
	{ NotFactorNodeTYPE, "Not Factor Node" },
	{ IfFactorNodeTYPE, "If Factor Node" },
	{ MultiplicatorTermNodeTYPE, "Mulitiplicator Term Node" },
	{ DividerTermNodeTYPE, "Divider Term Node" },
	{ AndTermNodeType, "And Term Node" },
	{ SubtractorSimpleExprNodeTYPE, "Subtraction Simple Expr Node" },
	{ OrSimpleExprNodeTYPE, "Or Simple Expr Node" },
	{ AdditionSimpleExprNodeTYPE, "Addition Simple Expr Node" },
	{ EqualToExprNodeTYPE, "Equal To Expr Node" },
	{ LessThanExprNodeTYPE, "Less Than Expr Node" },
	{ TypeNodeTYPE, "Type Node" },
	{ BodyNodeTYPE, "Body Node" },
	{ FormalNodeTYPE, "Formal Node" },
	{ NonEmptyFormalsNodeTYPE, "Non Empty Formals Node" },
	{ DefNodeTYPE, "Def Node" },
	{ DefinitionsNodeTYPE, "Definitions Node" },
	{ ProgramNodeTYPE, "Program Node" }
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

	for (int i = 0; i < ASTNodeVar.getDefNodes().size(); i++)
	{
		printASTNodeTree(*ASTNodeVar.getDefNodes()[i], treelevel + 1);
	}

	for (int i = 0; i < ASTNodeVar.getDefinitions().size(); i++)
	{
		printASTNodeTree(*ASTNodeVar.getDefinitions()[i], treelevel + 1);
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