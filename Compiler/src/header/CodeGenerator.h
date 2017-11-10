#pragma once
#include "SemanticChecker.h"

class CodeGenerator {
public:
	CodeGenerator(ASTNode AstTree, map<string, Function> GeneratedSymbolTable, string FileName) {
		Tree = AstTree;
		SymbolTable = GeneratedSymbolTable;
		OutFileName = FileName;
	}

	void writeOutTargetCode();

private:
	ASTNode Tree = ASTNode(PROGRAM_NODE_TYPE);
	map<string, Function> SymbolTable;
	map<string, int> FunctionLocation;
	vector<string> Instructions;
	string OutFileName;
	string CurrentFunction;
	int InstructionCount = 0;

	void addInitialComments();
	void setUpRuntimeEnvironment();
	void addInstruction(string Instruction);
	void addWhiteSpace(string FunctionName);
	void walkTree(ASTNode ASTTree);
	void writeInstructionsToFile();
	void generateFunctionHeader(string FunctionName);
	void generateMainFunction();
	void generatePrintFunction();
	void setRegistersInDmem();
	void restoreRegistersFromDmem();
	void generateFunctionReturnSequence();
	void callFunction(string FunctionName);
};