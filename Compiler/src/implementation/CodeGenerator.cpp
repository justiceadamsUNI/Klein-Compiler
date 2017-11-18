#include "../header/CodeGenerator.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

void CodeGenerator::writeOutTargetCode()
{
	addInitialComments();
	setUpRuntimeEnvironment();
	generatePrintFunction();
	generateMainFunction();
	generateAllOtherFunctions();
	backPatch();
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
		LastParamLocation = Params.size() + 1;

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
	FunctionLocations.insert(pair<string, int>("main", InstructionCount));
	generateFunctionHeader("main");
	
	// Find Main def node and generate code for it
	vector<ASTNode*> DefNodes = Tree.getDefinitions()->getDefNodes();
	for (int i = 0; i < DefNodes.size(); i++) {
		if (DefNodes.at(i)->getIdentifierNode()->getIdentifierName() == "main")
		{
			CurrentFunction = "main";
			generateCodeForDefNode(*DefNodes.at(i));
		}
	}

	generateFunctionReturnSequence();
}

void CodeGenerator::generateAllOtherFunctions()
{
	// Walk the AST tree for every def node and generate it's code while storing it's location in IMEM
	vector<ASTNode*> DefNodes = Tree.getDefinitions()->getDefNodes();
	for (int i = 0; i < DefNodes.size(); i++) {
		if (DefNodes.at(i)->getIdentifierNode()->getIdentifierName() != "main")
		{
			CurrentFunction = DefNodes.at(i)->getIdentifierNode()->getIdentifierName();
			FunctionLocations.insert(pair<string, int>(CurrentFunction, InstructionCount));
			generateFunctionHeader(CurrentFunction);
			generateCodeForDefNode(*DefNodes.at(i));
			generateFunctionReturnSequence();
		}
	}
}

void CodeGenerator::generatePrintFunction()
{
	FunctionLocations.insert(pair<string, int>("print", InstructionCount));
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
	addInstruction("LDA  6, " + to_string(ArgCount + 4) + "(5)   ; Adjusting Status pointer, about to call " + FunctionName);

	if (FunctionLocations.find(FunctionName) != FunctionLocations.end()) {
		addInstruction("LDA  7, " + to_string(FunctionLocations.find(FunctionName)->second) + "(0)   ; Jump to " + FunctionName);
	}
	else {
		// Mark line for later backpatching
		FunctionJumpReplacements.insert(pair<int, string>(Instructions.size(), FunctionName));
		addInstruction("LDA  7, " + FunctionName +  "(0)   ; Jump to " + FunctionName);
	}
}

void CodeGenerator::backPatch()
{
	map<int, string>::iterator iter = FunctionJumpReplacements.begin();
	while (iter != FunctionJumpReplacements.end())
	{
		int InstructionIndex = iter->first;
		string Placeholder = iter->second;

		// Do replacement on instruction string
		string Temp = Instructions.at(InstructionIndex);
		Temp.replace(
			Temp.find(Placeholder),
			Placeholder.size(), 
			to_string(FunctionLocations.find(Placeholder)->second));

		// Actually do replacement
		Instructions.at(InstructionIndex) = Temp;

		// Incrememnt Iterator
		iter++;
	}
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

	// generate jumps to print statments
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
	generateCodeForExpressionNode(*Node.getBaseExprNode());
	callFunction("print");
}

void CodeGenerator::generateCodeForBooleanLiteralNode(ASTNode Node)
{
	// We treat "true" as == 1 and "false" as == 0
	if (Node.getLiteralValue() == "true")
	{
		addInstruction("LDC 1, 1(0)   ; Pushing the boolean value (true == 1) into a register.");
		addInstruction("ST 1, 1(5)   ; Storing boolean literal into temp varaibles slot");
	}
	else {
		addInstruction("LDC 1, 0(0)   ; Pushing the boolean value (false == 0) into a register.");
		addInstruction("ST 1, 1(5)   ; Storing boolean literal into temp varaibles slot");
	}

	addInstruction("LDC 1, 1(0)   ; Loading 1 into R1");
	addInstruction("ADD 5, 1, 5   ; Add 1 to Stack Top");
}

void CodeGenerator::generateCodeForNegatedFactorNode(ASTNode Node)
{
	generateCodeForFactorNode(*Node.getFactorNode());

	addInstruction("LD 3, 0(5)   ; Getting left operand of negation multiplication");
	addInstruction("LDC 4, -1(0)   ; setting right operand of negation to -1");
	addInstruction("MUL 2, 3, 4   ; Performing negation multiplication on R3 and R4");
	addInstruction("ST 2, 0(5)   ; Store result of negation multiplication as temp (overwrite original value)");
}

void CodeGenerator::generateCodeForParenthesisedExpressionNode(ASTNode Node)
{
	generateCodeForExpressionNode(*Node.getBaseExprNode());
}

void CodeGenerator::generateCodeForFunctionCallNode(ASTNode Node)
{
	string CalledFunction = Node.getIdentifierNode()->getIdentifierName();
	int CalledFunctionsParamSize = SymbolTable.find(CalledFunction)->second.getParameters().size();

	// Evaluate arguments and store as temp vaiables before jumping to function call (if there are any)
	if (Node.getBaseActualsNode()->getAstNodeType() == NON_BASE_ACTUALS_NODE_TYPE) {
		vector<ASTNode*> FunctionCallParams = Node.getBaseActualsNode()->getNonEmptyActualsNode()->getExpressions();
		for (int i = 0; i < FunctionCallParams.size(); i++)
		{
			generateCodeForExpressionNode(*FunctionCallParams.at(i));
		}
	}
	
	callFunction(CalledFunction);
	addInstruction("LD 1, 1(5)   ; Loading the return value of " + CalledFunction + " into R1");

	// Overwrite arguments on the stack frame (if they exist)
	if (CalledFunctionsParamSize == 0)
	{
		addInstruction("LDC 1, 1(0)   ; Loading 1 into R1");
		addInstruction("ADD 5, 1, 5   ; Add 1 to Stack Top");
	}
	else {
		addInstruction("ST 1, -" + to_string(CalledFunctionsParamSize - 1) + "(5)   ; Storing return value of " + CalledFunction + " as a temp variable (overwrites other params)");
		addInstruction("LDC 1, " + to_string(CalledFunctionsParamSize - 1) + "(0)   ; Loading decrement value into R1");
		addInstruction("SUB 5, 5, 1   ; Decrement Stack Top if needed");
	}
}

void CodeGenerator::generateCodeForSingletonIdentifierFactorNode(ASTNode Node)
{
	generateCodeForIdentifierNode(*Node.getIdentifierNode());
}

void CodeGenerator::generateCodeForIfFactorNode(ASTNode Node)
{
	// Evaluate if check
	generateCodeForExpressionNode(*Node.getBaseExprNode3());

	addInstruction("LD 1, 0(5)   ; Getting value of boolean expression for if statment");
	int BeforeThenCaseInstructionIndex = Instructions.size();
	// If value of check == 0, jump to the else clause
	addInstruction("JEQ 1, X(7)   ; Jump to else clause");

	int BeforeThenCaseInstruction = InstructionCount;
	generateCodeForExpressionNode(*Node.getBaseExprNode2());
	int BeforeElseCaseInstructionIndex = Instructions.size();
	addInstruction("LDC 1, Y(0)   ; Store value to skip");
	addInstruction("ADD 7, 7, 1   ; Skip over then clause (since then clause was triggered)");
	int AfterThenCaseInstruction = InstructionCount;

	int BeforeElseCaseInstruction = InstructionCount;
	generateCodeForExpressionNode(*Node.getBaseExprNode());
	int AfterElseCaseInstruction = InstructionCount;

	// Move value of evaluated if expression and overwrite the boolean expression
	addInstruction("LD 1, 0(5)   ; Getting evaluated value of if expression");
	addInstruction("ST 1, -1(5)   ; Moving evaluated value of if expression to overwrite boolean expression (if check)");
	addInstruction("LDC 1, -1(0)   ; Store -1 ");
	addInstruction("ADD 5, 1, 5   ; Decrement stack top ");


	// We do the 'backpatching' here as opposed to the backpatch() method since we already have all the information
	// we would need right here in the scope of this method. It makes more sense to do it here then to persist that
	// info and wait until a later time.

	// Do replacement on instruction string
	string Temp = Instructions.at(BeforeThenCaseInstructionIndex);
	Temp.replace(
		Temp.find("X"),
		1,
		to_string(AfterThenCaseInstruction - BeforeThenCaseInstruction));

	Instructions.at(BeforeThenCaseInstructionIndex) = Temp;

	// Do replacement on instruction string
	Temp = Instructions.at(BeforeElseCaseInstructionIndex);
	Temp.replace(
		Temp.find("Y"),
		1,
		to_string(AfterElseCaseInstruction - BeforeElseCaseInstruction));

	Instructions.at(BeforeElseCaseInstructionIndex) = Temp;
}

void CodeGenerator::generateCodeForNotFactorNode(ASTNode Node)
{
	generateCodeForFactorNode(*Node.getFactorNode());

	// We can simply subtract 1 then multiply by -1 when negating boolean statements.
	// Not(true) = (1-1) * 0. Not(False) = (0-1)*-1
	addInstruction("LD 3, 0(5)   ; Getting original boolean value");
	addInstruction("LDC 4, 1(0)   ; setting right operand to 1 for subtraction");
	addInstruction("SUB 2, 3, 4   ; Performing subtraction on R3 and R4");
	addInstruction("LDC 4, -1(0)   ; setting right operand of negation to -1");
	addInstruction("MUL 2, 2, 4   ; Performing negation multiplication on R3 and R4");
	addInstruction("ST 2, 0(5)   ; Store result of negating boolean statement as temp (overwrite original value)");
}

void CodeGenerator::generateCodeForAndNode(ASTNode Node)
{
	// Right Side
	generateCodeForFactorNode(*Node.getFactorNode());

	// Left Side
	if (Node.getBaseTermNode())
	{
		generateCodeForTermNode(*Node.getBaseTermNode());
	}
	else {
		generateCodeForFactorNode(*Node.getFactorNode2());
	}

	// We simple multiply here! 1*0 = true and false = 0 = false.
	// 1 * 1 = 1 = true.
	addInstruction("LD 3, 0(5)   ; Getting left operand of and operator");
	addInstruction("LD 4, -1(5)   ; Getting right operand of and operator");
	addInstruction("MUL 2, 3, 4   ; Performing multiplication (and operation) on R3 and R4");
	addInstruction("ST 2, -1(5)   ; Store result of and operation as temp (overwrite left operand)");
	addInstruction("LDC 1, -1(0)   ; Store -1 ");
	addInstruction("ADD 5, 1, 5   ; Decrement stack top ");
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

	// We do an addition and if the result is >0 we have a true value.
	addInstruction("LD 3, 0(5)   ; Getting left operand of addition");
	addInstruction("LD 4, -1(5)   ; Getting right operand of addition");
	addInstruction("ADD 2, 3, 4   ; Performing addition on R3 and R4");

	// Adjust value by subtracting 1
	addInstruction("ADD 1, 2, 0   ; Store result into temp R1");
	addInstruction("LDC 3, 1(0)   ; Store 1 into R3 ");
	addInstruction("SUB 1, 1, 3   ; Performing subtraction on R1 and R3");

	// If result is > 1 then store result - 1 as the temp variable.
	addInstruction("JGT 1, 2(7)   ; Performing subtraction on R1 and R3");
	addInstruction("ST 2, -1(5)   ; Store the result of OR operation as temp (overwrite left operand)");
	addInstruction("ADD 7, 7, 3   ; Skip 1 instruction");

	// Store adjusted value
	addInstruction("ST 1, -1(5)   ; Store adjusted result of OR operation as temp (overwrite left operand)");

	// Adjust stack top
	addInstruction("LDC 1, -1(0)   ; Store -1 ");
	addInstruction("ADD 5, 1, 5   ; Decrement stack top ");
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
	// Right Side
	generateCodeForSimpleExpressionNode(*Node.getBaseSimpleExprNode());

	// Left Side
	generateCodeForSimpleExpressionNode(*Node.getBaseSimpleExprNode2());

	// We do Right - Left then compare the results to 0.
	addInstruction("LD 3, 0(5)   ; Getting left operand of <");
	addInstruction("LD 4, -1(5)   ; Getting right operand of <");
	addInstruction("SUB 2, 4, 3   ; Performing Subtraction on R4 and R3");

	// If result is > 0 then store 1 as the value (since left was less than right).
	addInstruction("JGT 2, 4(7)   ; Jumping if R2 greater than 0");
	addInstruction("LDC 1, 0(0)   ; Load 0 (false) ");
	addInstruction("ST 1, -1(5)   ; Store the result of < as temp (overwrite left operand)");
	addInstruction("LDC 1, 2(0)   ; Load 2 into R1 ");
	addInstruction("ADD 7, 7, 1   ; Skip 2 instructions");

	// Store adjusted value
	addInstruction("LDC 1, 1(0)   ; Load 1 (true) ");
	addInstruction("ST 1, -1(5)   ; Store the result of < as temp (overwrite left operand)");

	// Adjust stack top
	addInstruction("LDC 1, -1(0)   ; Store -1 ");
	addInstruction("ADD 5, 1, 5   ; Decrement stack top ");
}

void CodeGenerator::generateCodeForEqualNode(ASTNode Node)
{
	// Right Side
	generateCodeForSimpleExpressionNode(*Node.getBaseSimpleExprNode());

	// Left Side
	generateCodeForSimpleExpressionNode(*Node.getBaseSimpleExprNode2());

	// We do Right - Left then compare the results to 0.
	addInstruction("LD 3, 0(5)   ; Getting left operand of =");
	addInstruction("LD 4, -1(5)   ; Getting right operand of =");
	addInstruction("SUB 2, 4, 3   ; Performing Subtraction on R4 and R3");

	// If result is = 0 then store 1 as the value (since left was eqaul to right).
	addInstruction("JEQ 2, 4(7)   ; Jumping if R2 equal than 0");
	addInstruction("LDC 1, 0(0)   ; Load 0 (false) ");
	addInstruction("ST 1, -1(5)   ; Store the result of = as temp (overwrite left operand)");
	addInstruction("LDC 1, 2(0)   ; Load 2 into R1 ");
	addInstruction("ADD 7, 7, 1   ; Skip 2 instructions");

	// Store adjusted value
	addInstruction("LDC 1, 1(0)   ; Load 1 (true) ");
	addInstruction("ST 1, -1(5)   ; Store the result of = as temp (overwrite left operand)");

	// Adjust stack top
	addInstruction("LDC 1, -1(0)   ; Store -1 ");
	addInstruction("ADD 5, 1, 5   ; Decrement stack top ");
}

void CodeGenerator::generateCodeForIdentifierNode(ASTNode Node)
{
	vector<tuple<string, ReturnTypes>> Params = SymbolTable.at(CurrentFunction).getParameters();
	ptrdiff_t Position = find_if(
		Params.begin(), 
		Params.end(), 
		[&](const tuple<string, ReturnTypes>& tuple) {return get<0>(tuple) == Node.getIdentifierName(); }) - Params.begin();

	int ArgumentPointer = -1 * (2 + Params.size()) + int(Position);
	addInstruction("LD 1, " + to_string(ArgumentPointer) + "(6)   ; Loading actual value of argument into R1 from stack");

	addInstruction("ST 1, 1(5)   ; Store the argument into the temp variables slot");
	addInstruction("LDC 1, 1(0)   ; Store 1 ");
	addInstruction("ADD 5, 1, 5   ; Increment stack top ");
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
		generateCodeForNegatedFactorNode(Node);
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