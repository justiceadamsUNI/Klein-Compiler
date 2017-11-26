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
	map<string, int> FunctionLocations;
	map<int, string> FunctionJumpReplacements;
	vector<string> Instructions;
	string OutFileName;
	string CurrentFunction;
	int InstructionCount = 0;

	void addInitialComments();
	void setUpRuntimeEnvironment();
	void addInstruction(string Instruction);
	void addWhiteSpace(string FunctionName);
	void writeInstructionsToFile();
	void generateFunctionHeader(string FunctionName);
	void generateMainFunction();
	void generatePrintFunction();
	void generateAllOtherFunctions();
	void setRegistersInDmem();
	void restoreRegistersFromDmem();
	void generateFunctionReturnSequence();
	void callFunction(string FunctionName);
	void backPatch();

	// AST node to target code helper functions.
	void generateCodeForIdentifierNode(ASTNode Node);
	void generateCodeForDefNode(ASTNode Node);
	void generateCodeForBodyNode(ASTNode Node);
	void generateCodeForLessThanNode(ASTNode Node);
	void generateCodeForEqualNode(ASTNode Node);
	void generateCodeForBaseExpressionNode(ASTNode Node);
	void generateCodeForOrNode(ASTNode Node);
	void generateCodeForAdditionNode(ASTNode Node);
	void generateCodeForSubtractionNode(ASTNode Node);
	void generateCodeForBaseSimpleExpressionNode(ASTNode Node);
	void generateCodeForAndNode(ASTNode Node);
	void generateCodeForMultiplicatorNode(ASTNode Node);
	void generateCodeForDividerNode(ASTNode Node);
	void generateCodeForBaseTermNode(ASTNode Node);
	void generateCodeForIfFactorNode(ASTNode Node);
	void generateCodeForNotFactorNode(ASTNode Node);
	void generateCodeForLiteralFactorNode(ASTNode Node);
	void generateCodeForNegatedFactorNode(ASTNode Node);
	void generateCodeForParenthesisedExpressionNode(ASTNode Node);
	void generateCodeForFunctionCallNode(ASTNode Node);
	void generateCodeForSingletonIdentifierFactorNode(ASTNode Node);
	void generateCodeForIntegerLiteralNode(ASTNode Node);
	void generateCodeForBooleanLiteralNode(ASTNode Node);
	void generateCodeForPrintStatementNode(ASTNode Node);


	//helper methods
	void generateCodeForExpressionNode(ASTNode Node);
	void generateCodeForSimpleExpressionNode(ASTNode Node);
	void generateCodeForTermNode(ASTNode Node);
	void generateCodeForFactorNode(ASTNode Node);
	void generateCodeForLiteralNode(ASTNode Node);
};