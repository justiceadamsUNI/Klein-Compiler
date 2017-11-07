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
	int InstructionCount = 0;

	void writeInstructionsToFile();
	void setUpRuntimeEnvironment();
	void addInstruction(string Instruction);
	void addWhiteSpace();
	void generateMainFunction();
	void generatePrintFunction(string temp);
	void walkTree(ASTNode ASTTree);
	
	void GenerateFunction();
	void setRegistersInDmem();
	void restoreRegistersFromDmem();
	void returnFromFunction();
};