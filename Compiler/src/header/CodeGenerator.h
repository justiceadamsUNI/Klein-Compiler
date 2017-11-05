#pragma once
#include "SemanticChecker.h"

class CodeGenerator {
public:
	CodeGenerator(ASTNode AstTree, map<string, Function> GeneratedSymbolTable) {
		AstTree = Tree;
		SymbolTable = GeneratedSymbolTable;
	}

	void writeOutTargetCode();

private:
	ASTNode Tree = ASTNode(PROGRAM_NODE_TYPE);
	map<string, Function> SymbolTable;
	vector<string> Instructions;

	void writeInstructionsToFile();
};