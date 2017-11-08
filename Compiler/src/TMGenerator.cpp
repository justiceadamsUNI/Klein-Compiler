#include <iostream>
#include "header/Parser.h"
#include "header/SemanticChecker.h"
#include "header/CodeGenerator.h"

string GenerateFinalTmFileName(string KlnFilePath);

int main(int argv, char* argc[])
{
	if (argv == 1) {
		cout << "Please insert a KLEIN file to compile into TM." << endl;
		return 1;
	}

	try
	{
		Scanner Scanner(argc[1], 0);
		Parser Parser(Scanner);
		Parser.parseProgram();

		ASTNode AstTree = Parser.getFinalASTNode();
		SemanticChecker SemanticCheckerVar = SemanticChecker(AstTree);

		// print warnings
		for (int i = 0; i < SemanticCheckerVar.getWarnings().size(); i++)
		{
			cout << SemanticCheckerVar.getWarnings().at(i) << endl;
		}

		string OutFileName = GenerateFinalTmFileName(Scanner.getFinalFileName());

		// print Errors, and if there are none, then compile!
		if (SemanticCheckerVar.getErrors().empty())
		{
			CodeGenerator CodeGenerator(AstTree, SemanticCheckerVar.getSymbolTable(), OutFileName);
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

string GenerateFinalTmFileName(string KlnFilePath) {
	string OutFileName = KlnFilePath.replace(KlnFilePath.size() - 4, 4, ".tm");

	// Need to account for both Windows and Unix Directory seperators
	// We use 0 here to account for unsigned ints representation being a large
	// integer.
	size_t Position = OutFileName.rfind("/");
	Position = Position == string::npos ? 0 : Position;

	size_t Position2 = OutFileName.rfind("\\");
	Position2 = Position2 == string::npos ? 0 : Position2;

	size_t IndexOfSlash = std::max<size_t>(Position, Position2);
	if (IndexOfSlash != 0) {
		OutFileName.erase(0, IndexOfSlash + 1);
	}

	return OutFileName;
}