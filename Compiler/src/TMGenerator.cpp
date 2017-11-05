#include <iostream>
#include "header/Parser.h"
#include "header/SemanticChecker.h"
#include "header/CodeGenerator.h"

int main(int argv, char* argc[])
{
	if (argv == 1) {
		cout << "Please insert a KLEIN file to compile into TM." << endl;
		return 1;
	}

	try
	{
		Scanner Scanner(argc[1]);
		Parser Parser(Scanner);
		Parser.parseProgram();

		ASTNode AstTree = Parser.getFinalASTNode();
		SemanticChecker SemanticCheckerVar = SemanticChecker(AstTree);

		// print warnings
		for (int i = 0; i < SemanticCheckerVar.getWarnings().size(); i++)
		{
			cout << SemanticCheckerVar.getWarnings().at(i) << endl;
		}

		// print Errors, and if there are none, then print the symbol table
		if (SemanticCheckerVar.getErrors().empty())
		{
			CodeGenerator CodeGenerator(AstTree, SemanticCheckerVar.getSymbolTable());
			CodeGenerator.writeOutTargetCode();
		}
		else {
			for (int i = SemanticCheckerVar.getErrors().size() - 1; i >= 0; i--)
			{
				cout << SemanticCheckerVar.getErrors().at(i) << endl;
			}
		}
	}
	catch (const std::exception& e)
	{
		//Print Error Message (that isn't a semantic error)
		cout << endl << e.what() << endl;
		return 1;
	}
	return 0;
}