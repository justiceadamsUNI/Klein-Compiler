#include "../header/CodeGenerator.h"
#include <iostream>
#include <fstream>

using namespace std;

void CodeGenerator::writeOutTargetCode()
{
	addInitialComments();
	setUpRuntimeEnvironment();
	generatePrintFunction();
	generateMainFunction();
	writeInstructionsToFile();
}

void CodeGenerator::addInitialComments()
{
	Instructions.push_back("*");
	Instructions.push_back("*\tRUN TIME!  gets command line args and calls main");
	Instructions.push_back("*\t           R1 is used as temp register");
	Instructions.push_back("*");
}

/*Sets up the runtime environment and makes the call to main*/
void CodeGenerator::setUpRuntimeEnvironment()
{
	vector<tuple<string, ReturnTypes>>Params = SymbolTable.at("main").getParameters();

	int LastParamLocation = 1;
	if (!Params.empty())
	{
		LastParamLocation = Params.size() + 2;

		// Loads all command line args into stack frame.
		for (int i = Params.size(); i >= 0; i--)
		{
			addInstruction("LD  1, " + to_string(i + 1) + "(0)");
			addInstruction("ST  1, " + to_string(i + 2) + "(0)   ; Storing Command line arg number " + to_string(i));
		}
	}

	// Setup main stack frame before call to function
	addInstruction("ST  0, " + to_string(LastParamLocation + 2) + "(0)   ; Storing access link before call to main");
	addInstruction("LDC  6, " + to_string(LastParamLocation + 3) + "(0)   ; Adjust status pointer before call to main");
	addInstruction("LDA  1, 2(7)   ; Setting return adress and storing in temp register R1");
	addInstruction("ST  1, " + to_string(LastParamLocation + 1) + "(0)   ; Storing the return adress in DMEM at the control link slot");
	// 18 slots to skip Print function
	addInstruction("LDA  7, 18(7)   ; Jump to main"); 

	// Load into register 1 the value returned from main and print it.
	addInstruction("LD  1, 1(5)   ; Load main's return value");
	addInstruction("OUT  1, 0, 0   ; Printing main return value");

	// Quit
	addInstruction("HALT  0, 0, 0   ; End of program");
}

void CodeGenerator::addInstruction(string Instruction)
{
	Instructions.push_back(to_string(InstructionCount) + ": " +  Instruction);
	InstructionCount++;
}

void CodeGenerator::addWhiteSpace(string FunctionName)
{
	Instructions.push_back("");
	Instructions.push_back("*");
	Instructions.push_back("*\t" + FunctionName);
	Instructions.push_back("*");
}

void CodeGenerator::generateMainFunction()
{
	FunctionLocation.insert(pair<string, int>("main", InstructionCount));
	generateFunctionHeader("main");
	
	// Walk the AST tree for every def node
	vector<ASTNode*> DefNodes = Tree.getDefinitions()->getDefNodes();
	for (int i = 0; i < DefNodes.size(); i++) {
		CurrentFunction = DefNodes.at(i)->getIdentifierNode()->getIdentifierName();
		generateCodeForDefNode(*DefNodes.at(i));
	}

	generateFunctionReturnSequence();
}

void CodeGenerator::generatePrintFunction()
{
	FunctionLocation.insert(pair<string, int>("print", InstructionCount));
	generateFunctionHeader("print");
	addInstruction("LD  1, -3(6)   ; Loading the value of whatever argument is passed to print to R1");
	addInstruction("OUT 1, 0, 0   ; Printing the value of whatever argument is passed to print");
	generateFunctionReturnSequence();
}

void CodeGenerator::generateFunctionHeader(string FunctionName)
{
	addWhiteSpace(FunctionName);
	setRegistersInDmem();
}

/*Note that when setting registers for a function call, we expect to find the old status pointer 
  in the R1 register. This way we can store it properly.*/
void CodeGenerator::setRegistersInDmem()
{
	addInstruction("ST  2, 0(6)   ; Store register 2 into ( new status pointer) ");
	addInstruction("ST  3, 1(6)   ; Store register 3 into (new status pointer + 1) ");
	addInstruction("ST  4, 2(6)   ; Store register 4 into (new status pointer + 2) ");
	addInstruction("ST  1, 3(6)   ; Store register 1 (the old status pointer) into ( new status pointer + 3) ");

	addInstruction("LDC  1, 3(0)   ; Store constant 3 in R1 ");
	addInstruction("ADD  5, 1, 6   ; Adjust top of stack to be (new status pointer + 3) ");
}

void CodeGenerator::generateFunctionReturnSequence()
{
	// Stores temp status pointer so it isn't overriden when restoreRegistersFromDmem() is called.
	addInstruction("ADD  1,6,0   ; Storing a temp copy of status pointer in R1");
	restoreRegistersFromDmem();
	addInstruction("LD  5,-1(1)   ; Restoring the stack top variable");
	addInstruction("LD  7,-2(1)   ; Restoring the return adress from the control link");
}

void CodeGenerator::callFunction(string FunctionName)
{
	int ArgCount = FunctionName == "print" ? 1 : SymbolTable.find(FunctionName)->second.getParameters().size();

	// Set incoming args. We do this by transfering the last n slots from the temp objects
	// of the previous stack frame into the new stack frames arguments section.
	for (int i = 0; i < ArgCount; i++) {
		addInstruction("LD 1, -" + to_string(i) + "(5)   ; Moving Temp arg " + to_string(i) + " to R1");
		addInstruction("ST 1, " + to_string(i + 2) + "(5)   ; Storing Temp arg " + to_string(i) + " to Arg location in new stack frame");
	}

	// Always leave register one as the value of current status pointer right before we call a function. To avoid overwriten values.
	addInstruction("ST  5, " + to_string(ArgCount + 3) + "(5)   ; Storing Access Link, about to call " + FunctionName);
	addInstruction("LDA  1, 4(7)   ; Saving return adress in temp register, about to call " + FunctionName);
	addInstruction("ST  1, " + to_string(ArgCount + 2) + "(5)   ; Storing the return adress in DMEM at the control link slot");
	addInstruction("ADD  1, 6, 0   ; Copying current status pointer before function call, about to call " + FunctionName);
	addInstruction("LDA  6, " +  to_string(ArgCount + 4) + "(5)   ; Adjusting Status pointer, about to call " + FunctionName);
	addInstruction("LDA  7, " + to_string(FunctionLocation.find(FunctionName)->second) + "(0)   ; Jump to " + FunctionName);
}

void CodeGenerator::restoreRegistersFromDmem()
{
	addInstruction("LD  2, 0(6)   ; restore register 2 from (status pointer) ");
	addInstruction("LD  3, 1(6)   ; restore register 3 from (status pointer + 1) ");
	addInstruction("LD  4, 2(6)   ; restore register 4 from (status pointer + 2) ");
	addInstruction("LD  6, 3(6)   ; restore register 6 from (status pointer + 3) ");
}

void CodeGenerator::writeInstructionsToFile()
{
	ofstream TMFile;
	TMFile.open(OutFileName, ios::out);

	// Write to out file and close it.
	for (int i = 0; i < Instructions.size(); i++)
	{
		TMFile << Instructions.at(i) << endl;
	}

	TMFile.close();
}


//----------------------------------------------------------------
void CodeGenerator::generateCodeForDefNode(ASTNode Node)
{
	generateCodeForBodyNode(*Node.getBodyNode());
}

void CodeGenerator::generateCodeForBodyNode(ASTNode Node)
{
	vector<ASTNode*> PrintStatements = Node.getPrintStatements();
	int CurrentFunctionParamsSize = SymbolTable.find(CurrentFunction)->second.getParameters().size();

	// Assign types to print statments
	for (int i = PrintStatements.size() - 1; i >= 0; i--) {
		generateCodeForPrintStatementNode(*PrintStatements.at(i));
	}
	generateCodeForExpressionNode(*Node.getBaseExprNode());

	addInstruction("LD 1, 0(5)   ; Pushing the return value of " + CurrentFunction + " into a register");
	addInstruction("ST 1, -" + to_string(3 + CurrentFunctionParamsSize) + "(6)   ; Storing return value of " + CurrentFunction + " into its' stack frame");
}

void CodeGenerator::generateCodeForBaseExpressionNode(ASTNode Node)
{
	generateCodeForSimpleExpressionNode(*Node.getBaseSimpleExprNode());
}

void CodeGenerator::generateCodeForBaseSimpleExpressionNode(ASTNode Node)
{
	generateCodeForTermNode(*Node.getBaseTermNode());
}

void CodeGenerator::generateCodeForBaseTermNode(ASTNode Node)
{
	generateCodeForFactorNode(*Node.getFactorNode());
}

void CodeGenerator::generateCodeForLiteralFactorNode(ASTNode Node)
{
	generateCodeForLiteralNode(*Node.getLiteralNode());
}

void CodeGenerator::generateCodeForIntegerLiteralNode(ASTNode Node)
{
	addInstruction("LDC 1, " + Node.getLiteralValue() + "(0)   ; Pushing the literal value into a register.");
	addInstruction("ST 1, 1(5)   ; Storing integer literal into temp varaibles slot");
	addInstruction("LDC 1, 1(0)   ; Loading 1 into R1");
	addInstruction("ADD 5, 1, 5   ; Add 1 to Stack Top");
}

void CodeGenerator::generateCodeForPrintStatementNode(ASTNode Node)
{
	generateCodeForBaseExpressionNode(*Node.getBaseExprNode());
	callFunction("print");
}

void CodeGenerator::generateCodeForBooleanLiteralNode(ASTNode Node)
{
	//Stub
}

void CodeGenerator::generateCodeForSubtractionFactorNode(ASTNode Node)
{
	//Stub
}

void CodeGenerator::generateCodeForParenthesisedExpressionNode(ASTNode Node)
{
	generateCodeForExpressionNode(*Node.getBaseExprNode());
}

void CodeGenerator::generateCodeForFunctionCallNode(ASTNode Node)
{
	//Stub
}

void CodeGenerator::generateCodeForSingletonIdentifierFactorNode(ASTNode Node)
{
	//Stub
}

void CodeGenerator::generateCodeForIfFactorNode(ASTNode Node)
{
}

void CodeGenerator::generateCodeForNotFactorNode(ASTNode Node)
{
	//Stub
}

void CodeGenerator::generateCodeForAndNode(ASTNode Node)
{
	//Stub
}

void CodeGenerator::generateCodeForMultiplicatorNode(ASTNode Node)
{
	// Right Side
	generateCodeForFactorNode(*Node.getFactorNode());

	// Left Side
	if (Node.getBaseSimpleExprNode())
	{
		generateCodeForSimpleExpressionNode(*Node.getBaseSimpleExprNode());
	}
	else if (Node.getBaseTermNode())
	{
		generateCodeForTermNode(*Node.getBaseTermNode());
	}
	else {
		generateCodeForFactorNode(*Node.getFactorNode2());
	}

	addInstruction("LD 3, 0(5)   ; Getting left operand of multiplication");
	addInstruction("LD 4, -1(5)   ; Getting right operand of multiplication");
	addInstruction("MUL 2, 3, 4   ; Performing multiplication on R3 and R4");
	addInstruction("ST 2, -1(5)   ; Store result of multiplication as temp (overwrite left operand)");
	addInstruction("LDC 1, -1(0)   ; Store -1 ");
	addInstruction("ADD 5, 1, 5   ; Decrement stack top ");

}

void CodeGenerator::generateCodeForDividerNode(ASTNode Node)
{
	// Right Side
	generateCodeForFactorNode(*Node.getFactorNode());

	// Left Side
	if (Node.getBaseSimpleExprNode())
	{
		generateCodeForSimpleExpressionNode(*Node.getBaseSimpleExprNode());
	}
	else if (Node.getBaseTermNode())
	{
		generateCodeForTermNode(*Node.getBaseTermNode());
	}
	else {
		generateCodeForFactorNode(*Node.getFactorNode2());
	}

	addInstruction("LD 3, 0(5)   ; Getting left operand of division");
	addInstruction("LD 4, -1(5)   ; Getting right operand of division");
	addInstruction("DIV 2, 3, 4   ; Performing division on R3 and R4");
	addInstruction("ST 2, -1(5)   ; Store result of division as temp (overwrite left operand)");
	addInstruction("LDC 1, -1(0)   ; Store -1 ");
	addInstruction("ADD 5, 1, 5   ; Decrement stack top ");
}

void CodeGenerator::generateCodeForOrNode(ASTNode Node)
{
	//Stub
}

void CodeGenerator::generateCodeForAdditionNode(ASTNode Node)
{
	// Right Side
	generateCodeForTermNode(*Node.getBaseTermNode());

	// Left Side
	if (Node.getBaseSimpleExprNode())
	{
		generateCodeForSimpleExpressionNode(*Node.getBaseSimpleExprNode());
	}
	else {
		generateCodeForTermNode(*Node.getBaseTermNode2());
	}

	addInstruction("LD 3, 0(5)   ; Getting left operand of addition");
	addInstruction("LD 4, -1(5)   ; Getting right operand of addition");
	addInstruction("ADD 2, 3, 4   ; Performing addition on R3 and R4");
	addInstruction("ST 2, -1(5)   ; Store result of addition as temp (overwrite left operand)");
	addInstruction("LDC 1, -1(0)   ; Store -1 ");
	addInstruction("ADD 5, 1, 5   ; Decrement stack top ");
}

void CodeGenerator::generateCodeForSubtractionNode(ASTNode Node)
{
	// Right Side
	generateCodeForTermNode(*Node.getBaseTermNode());

	// Left Side
	if (Node.getBaseSimpleExprNode())
	{
		generateCodeForSimpleExpressionNode(*Node.getBaseSimpleExprNode());
	}
	else {
		generateCodeForTermNode(*Node.getBaseTermNode2());
	}

	addInstruction("LD 3, 0(5)   ; Getting left operand of subtraction");
	addInstruction("LD 4, -1(5)   ; Getting right operand of subtraction");
	addInstruction("SUB 2, 3, 4   ; Performing subtraction on R3 and R4");
	addInstruction("ST 2, -1(5)   ; Store result of subtraction as temp (overwrite left operand)");
	addInstruction("LDC 1, -1(0)   ; Store -1 ");
	addInstruction("ADD 5, 1, 5   ; Decrement stack top ");
}

void CodeGenerator::generateCodeForLessThanNode(ASTNode Node)
{
	//Stub
}

void CodeGenerator::generateCodeForEqualNode(ASTNode Node)
{
	//Stub
}

void CodeGenerator::generateCodeForIdentifierNode(ASTNode Node)
{
	//Stub
}



// helper methods -------------------------------------------------------
void CodeGenerator::generateCodeForExpressionNode(ASTNode Node) {
	if (Node.getAstNodeType() == BASE_EXPR_NODE_TYPE)
	{
		generateCodeForBaseExpressionNode(Node);
	}
	else if (Node.getAstNodeType() == LESS_THAN_EXPR_NODE_TYPE)
	{
		generateCodeForLessThanNode(Node);
	}
	else {
		generateCodeForEqualNode(Node);
	}
}

void CodeGenerator::generateCodeForSimpleExpressionNode(ASTNode Node) {
	if (Node.getAstNodeType() == BASE_SIMPLE_EXPR_NODE_TYPE)
	{
		generateCodeForBaseSimpleExpressionNode(Node);
	}
	else if (Node.getAstNodeType() == ADDITION_SIMPLE_EXPR_NODE_TYPE)
	{
		generateCodeForAdditionNode(Node);
	}
	else if (Node.getAstNodeType() == SUBTRACTOR_SIMPLE_EXPR_NODE_TYPE)
	{
		generateCodeForSubtractionNode(Node);
	}
	else {
		generateCodeForOrNode(Node);
	}
}

void CodeGenerator::generateCodeForTermNode(ASTNode Node)
{
	if (Node.getAstNodeType() == BASE_TERM_NODE_TYPE)
	{
		generateCodeForBaseTermNode(Node);
	}
	else if (Node.getAstNodeType() == MULTIPLICATOR_TERM_NODE_TYPE)
	{
		generateCodeForMultiplicatorNode(Node);
	}
	else if (Node.getAstNodeType() == DIVIDER_TERM_NODE_TYPE)
	{
		generateCodeForDividerNode(Node);
	}
	else {
		generateCodeForAndNode(Node);
	}
}

void CodeGenerator::generateCodeForFactorNode(ASTNode Node)
{
	if (Node.getAstNodeType() == PARENTHESISED_EXPR_FACTOR_NODE_TYPE)
	{
		generateCodeForParenthesisedExpressionNode(Node);
	}
	else if (Node.getAstNodeType() == SUBTRACTION_FACTOR_NODE_TYPE)
	{
		generateCodeForSubtractionFactorNode(Node);
	}
	else if (Node.getAstNodeType() == LITERAL_FACTOR_NODE_TYPE)
	{
		generateCodeForLiteralFactorNode(Node);
	}
	else if (Node.getAstNodeType() == FUNCTION_CALL_TYPE)
	{
		generateCodeForFunctionCallNode(Node);
	}
	else if (Node.getAstNodeType() == SINGLETON_IDENTIFIER_FACTOR_NODE_TYPE)
	{
		generateCodeForSingletonIdentifierFactorNode(Node);
	}
	else if (Node.getAstNodeType() == NOT_FACTOR_NODE_TYPE)
	{
		generateCodeForNotFactorNode(Node);
	}
	else {
		generateCodeForIfFactorNode(Node);
	}
}

void CodeGenerator::generateCodeForLiteralNode(ASTNode Node)
{
	if (Node.getAstNodeType() == INTEGER_LITERAL_NODE_TYPE)
	{
		generateCodeForIntegerLiteralNode(Node);
	}
	else {
		generateCodeForBooleanLiteralNode(Node);
	}
}