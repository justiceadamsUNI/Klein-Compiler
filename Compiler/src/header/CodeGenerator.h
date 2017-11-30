#pragma once
#include "SemanticChecker.h"
#include "InstructionManager.h"

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
	string OutFileName;
	string CurrentFunction;
	InstructionManager InstructionManagerInstance;

	void addInitialComments();
	void addFunctionLabel(string FunctionName);
	void setUpRuntimeEnvironment();
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