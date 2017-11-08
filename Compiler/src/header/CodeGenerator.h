#pragma once
#include "SemanticChecker.h"

class CodeGenerator {
public:
	CodeGenerator(ASTNode AstTree, map<string, Function> GeneratedSymbolTable) {
		Tree = AstTree;
		SymbolTable = GeneratedSymbolTable;
	}

	void writeOutTargetCode();

private:
	ASTNode Tree = ASTNode(PROGRAM_NODE_TYPE);
	map<string, Function> SymbolTable;
	map<string, int> FunctionLocation;
	vector<string> Instructions;
	int InstructionCount = 0;
	string CurrentFunction;

	void writeInstructionsToFile();
	void setUpRuntimeEnvironment();
	void addInstruction(string Instruction);
	void addWhiteSpace();
	void generateMainFunction();
	void generatePrintFunction();
	void walkTree(ASTNode ASTTree);
	
	void GenerateFunction();
	void setRegistersInDmem();
	void restoreRegistersFromDmem();
	void returnFromFunction();
	void callFunction(string functionName);
};