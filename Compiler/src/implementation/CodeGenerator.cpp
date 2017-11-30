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
	InstructionManagerInstance.AddCommentToInstructions("*");
	InstructionManagerInstance.AddCommentToInstructions("*\tRUN TIME!  gets command line args and calls main");
	InstructionManagerInstance.AddCommentToInstructions("*\t           R1 is used as temp register");
	InstructionManagerInstance.AddCommentToInstructions("*");
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
			InstructionManagerInstance.AddInstructionRM(LD, REGISTER_1, REGISTER_0, i + 1);
			InstructionManagerInstance.AddInstructionRM(ST, REGISTER_1, REGISTER_0, i + 2, "Storing Command line arg number " + to_string(i));
		}
	}

	// Setup main stack frame before call to function
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_0, REGISTER_0, LastParamLocation + 2, "Storing access link before call to main");
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_6, REGISTER_0, LastParamLocation + 3, "Adjust status pointer before call to main");
	InstructionManagerInstance.AddInstructionRM(LDA, REGISTER_1, REGISTER_7, 2, "Setting return adress and storing in temp register R1");
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_1, REGISTER_0, LastParamLocation + 1, "Storing the return adress in DMEM at the control link slot");

	// 18 slots to skip Print function
	InstructionManagerInstance.AddInstructionRM(LDA, REGISTER_7, REGISTER_7, 18, "Jump to main");

	// Load into register 1 the value returned from main and print it.
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_1, REGISTER_5, 1, "Load main's return value");
	InstructionManagerInstance.AddInstructionRO(OUT, REGISTER_1, REGISTER_0, REGISTER_0, "Printing main return value");

	// Quit
	InstructionManagerInstance.AddInstructionRO(HALT, REGISTER_0, REGISTER_0, REGISTER_0, "End of program");
}

void CodeGenerator::addFunctionLabel(string FunctionName)
{
	InstructionManagerInstance.AddCommentToInstructions("");
	InstructionManagerInstance.AddCommentToInstructions("*");
	InstructionManagerInstance.AddCommentToInstructions("*\t" + FunctionName);
	InstructionManagerInstance.AddCommentToInstructions("*");
}

void CodeGenerator::generateMainFunction()
{
	FunctionLocations.insert(pair<string, int>("main", InstructionManagerInstance.GetInstructionCount()));
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
			FunctionLocations.insert(pair<string, int>(CurrentFunction, InstructionManagerInstance.GetInstructionCount()));
			generateFunctionHeader(CurrentFunction);
			generateCodeForDefNode(*DefNodes.at(i));
			generateFunctionReturnSequence();
		}
	}
}

void CodeGenerator::generatePrintFunction()
{
	FunctionLocations.insert(pair<string, int>("print", InstructionManagerInstance.GetInstructionCount()));
	generateFunctionHeader("print");
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_1, REGISTER_6, -3, "Loading the value of whatever argument is passed to print to R1");
	InstructionManagerInstance.AddInstructionRO(OUT, REGISTER_1, REGISTER_0, REGISTER_0, "Printing the value of whatever argument is passed to print");
	generateFunctionReturnSequence();
}

void CodeGenerator::generateFunctionHeader(string FunctionName)
{
	addFunctionLabel(FunctionName);
	setRegistersInDmem();
}

/*Note that when setting registers for a function call, we expect to find the old status pointer 
  in the R1 register. This way we can store it properly.*/
void CodeGenerator::setRegistersInDmem()
{
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_2, REGISTER_6, 0, "Store register 2 into (new status pointer)");
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_3, REGISTER_6, 1, "Store register 3 into (new status pointer + 1)");
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_4, REGISTER_6, 2, "Store register 4 into (new status pointer + 2)");
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_1, REGISTER_6, 3, "Store register 1 (the old status pointer) into (new status pointer + 3)");
	
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, 3, "Store constant 3 in R1");
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_5, REGISTER_1, REGISTER_6, "Adjust top of stack to be (new status pointer + 3)");
}

void CodeGenerator::generateFunctionReturnSequence()
{
	// Stores temp status pointer so it isn't overriden when restoreRegistersFromDmem() is called.
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_1, REGISTER_6, REGISTER_0, "Storing a temp copy of status pointer in R1");
	restoreRegistersFromDmem();
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_5, REGISTER_1, -1, "Restoring the stack top variable");
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_7, REGISTER_1, -2, "Restoring the return adress from the control link");
}

void CodeGenerator::callFunction(string FunctionName)
{
	int ArgCount = FunctionName == "print" ? 1 : SymbolTable.find(FunctionName)->second.getParameters().size();

	// Set incoming args. We do this by transfering the last n slots from the temp objects
	// of the previous stack frame into the new stack frames arguments section.
	for (int i = 0; i < ArgCount; i++) {
		InstructionManagerInstance.AddInstructionRM(LD, REGISTER_1, REGISTER_5, -i, "Moving Temp arg " + to_string(i) + " to R1");
		InstructionManagerInstance.AddInstructionRM(ST, REGISTER_1, REGISTER_5, i + 2, "Storing Temp arg " + to_string(i) + " to Arg location in new stack frame");
	}

	// Always leave register one as the value of current status pointer right before we call a function. To avoid overwriten values.
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_5, REGISTER_5, ArgCount + 3, "Storing Access Link, about to call " + FunctionName);
	InstructionManagerInstance.AddInstructionRM(LDA, REGISTER_1, REGISTER_7, 4, "Saving return adress in temp register, about to call " + FunctionName);
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_1, REGISTER_5, ArgCount + 2, "Storing the return adress in DMEM at the control link slot");
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_1, REGISTER_6, REGISTER_0, "Copying current status pointer before function call, about to call " + FunctionName);
	InstructionManagerInstance.AddInstructionRM(LDA, REGISTER_6, REGISTER_5, ArgCount + 4, "Adjusting Status pointer, about to call " + FunctionName);

	if (FunctionLocations.find(FunctionName) != FunctionLocations.end()) {
		InstructionManagerInstance.AddInstructionRM(LDA, REGISTER_7, REGISTER_0, FunctionLocations.find(FunctionName)->second, "Jump to " + FunctionName);
	}
	else {
		// Mark line for later backpatching
		FunctionJumpReplacements.insert(pair<int, string>(InstructionManagerInstance.GetInstructionVectorSize(), FunctionName));
		InstructionManagerInstance.AddInstructionRMForBackPatching(LDA, REGISTER_7, REGISTER_0, FunctionName, "Jump to " + FunctionName);
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
		string Temp = InstructionManagerInstance.getInstructions().at(InstructionIndex);
		Temp.replace(
			Temp.find(Placeholder),
			Placeholder.size(), 
			to_string(FunctionLocations.find(Placeholder)->second));

		// Actually do replacement
		InstructionManagerInstance.replaceInstructionAtIndex(InstructionIndex, Temp);

		// Incrememnt Iterator
		iter++;
	}
}

void CodeGenerator::restoreRegistersFromDmem()
{

	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_2, REGISTER_6, 0, "restore register 2 from (status pointer)");
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_3, REGISTER_6, 1, "restore register 3 from (status pointer + 1)");
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_4, REGISTER_6, 2, "restore register 4 from (status pointer + 2)");
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_6, REGISTER_6, 3, "restore register 6 from (status pointer + 3)");
}

void CodeGenerator::writeInstructionsToFile()
{
	vector<string> Instructions = InstructionManagerInstance.getInstructions();
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

	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_1, REGISTER_5, 0, "pushing the return value of " + CurrentFunction + " into a register");
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_1, REGISTER_6, -(3 + CurrentFunctionParamsSize), "Storing return value of " + CurrentFunction + " into its stack frame");
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
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, stoi(Node.getLiteralValue()), "Pushing the literal value into a register");
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_1, REGISTER_5, 1, "Storing integer literal into temp varaibles slot");
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, 1, "Loading 1 into R1");
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_5, REGISTER_1, REGISTER_5, "Add 1 to Stack Top");
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
		InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, 1, "Pushing the boolean value (true == 1) into a register");
		InstructionManagerInstance.AddInstructionRM(ST, REGISTER_1, REGISTER_5, 1, "Storing boolean literal into temp varaibles slot");
	}
	else {
		InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, 0, "Pushing the boolean value (false == 0) into a register");
		InstructionManagerInstance.AddInstructionRM(ST, REGISTER_1, REGISTER_5, 1, "Storing boolean literal into temp varaibles slot");
	}

	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, 1, "Loading 1 into R1");
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_5, REGISTER_1, REGISTER_5, "Add 1 to Stack Top");
}

void CodeGenerator::generateCodeForNegatedFactorNode(ASTNode Node)
{
	generateCodeForFactorNode(*Node.getFactorNode());

	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_3, REGISTER_5, 0, "Getting left operand of negation multiplication");
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_4, REGISTER_0, -1, "setting right operand of negation to -1");
	InstructionManagerInstance.AddInstructionRO(MUL, REGISTER_2, REGISTER_3, REGISTER_4, "Performing negation multiplication on R3 and R4");
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_2, REGISTER_5, 0, "Store result of negation multiplication as temp (overwrite original value)");
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
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_1, REGISTER_5, 1, "Loading the return value of " + CalledFunction + " into R1");

	// Overwrite arguments on the stack frame (if they exist)
	if (CalledFunctionsParamSize == 0)
	{
		InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, 1, "Loading 1 into R1");
		InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_5, REGISTER_1, REGISTER_5, "Add 1 to Stack Top");
	}
	else {
		InstructionManagerInstance.AddInstructionRM(ST, REGISTER_1, REGISTER_5, -(CalledFunctionsParamSize - 1), "Storing return value of " + CalledFunction + " as a temp variable(overwrites other params)");
		InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, (CalledFunctionsParamSize - 1), "Loading decrement value into R1");
		InstructionManagerInstance.AddInstructionRO(SUB, REGISTER_5, REGISTER_5, REGISTER_1, "Decrement Stack Top if needed");
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

	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_1, REGISTER_5, 0, "Getting value of boolean expression for if statment");
	int BeforeThenCaseInstructionIndex = InstructionManagerInstance.GetInstructionVectorSize();
	// If value of check == 0, jump to the else clause
	InstructionManagerInstance.AddInstructionRMForBackPatching(JEQ, REGISTER_1, REGISTER_7, "X", "Jump to else clause");

	int BeforeThenCaseInstruction = InstructionManagerInstance.GetInstructionCount();
	generateCodeForExpressionNode(*Node.getBaseExprNode2());
	int BeforeElseCaseInstructionIndex = InstructionManagerInstance.GetInstructionVectorSize();
	InstructionManagerInstance.AddInstructionRMForBackPatching(LDC, REGISTER_1, REGISTER_0, "Y", "Store value to skip");
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_7, REGISTER_7, REGISTER_1, "Skip over then clause (since then clause was triggered)");
	int AfterThenCaseInstruction = InstructionManagerInstance.GetInstructionCount();

	int BeforeElseCaseInstruction = InstructionManagerInstance.GetInstructionCount();
	generateCodeForExpressionNode(*Node.getBaseExprNode());
	int AfterElseCaseInstruction = InstructionManagerInstance.GetInstructionCount();

	// Move value of evaluated if expression and overwrite the boolean expression
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_1, REGISTER_5, 0, "Getting evaluated value of if expression");
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_1, REGISTER_5, -1, "Moving evaluated value of if expression to overwrite boolean expression (if check)");
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, -1, "Store -1");
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_5, REGISTER_1, REGISTER_5, "Decrement stack top");

	// We do the 'backpatching' here as opposed to the backpatch() method since we already have all the information
	// we would need right here in the scope of this method. It makes more sense to do it here then to persist that
	// info and wait until a later time.

	// Do replacement on instruction string
	string Temp = InstructionManagerInstance.getInstructions().at(BeforeThenCaseInstructionIndex);
	Temp.replace(
		Temp.find("X"),
		1,
		to_string(AfterThenCaseInstruction - BeforeThenCaseInstruction));

	InstructionManagerInstance.replaceInstructionAtIndex(BeforeThenCaseInstructionIndex, Temp);

	// Do replacement on instruction string
	Temp = InstructionManagerInstance.getInstructions().at(BeforeElseCaseInstructionIndex);
	Temp.replace(
		Temp.find("Y"),
		1,
		to_string(AfterElseCaseInstruction - BeforeElseCaseInstruction));

	InstructionManagerInstance.replaceInstructionAtIndex(BeforeElseCaseInstructionIndex, Temp);
}

void CodeGenerator::generateCodeForNotFactorNode(ASTNode Node)
{
	generateCodeForFactorNode(*Node.getFactorNode());

	// We can simply subtract 1 then multiply by -1 when negating boolean statements.
	// Not(true) = (1-1) * 0. Not(False) = (0-1)*-1
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_3, REGISTER_5, 0, "Getting original boolean value");
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_4, REGISTER_0, 1, "setting right operand to 1 for subtraction");
	InstructionManagerInstance.AddInstructionRO(SUB, REGISTER_2, REGISTER_3, REGISTER_4, "Performing subtraction on R3 and R4");
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_4, REGISTER_0, -1, "setting right operand of negation to -1");
	InstructionManagerInstance.AddInstructionRO(MUL, REGISTER_2, REGISTER_2, REGISTER_4, "Performing negation multiplication on R3 and R4");
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_2, REGISTER_5, 0, "Store result of negating boolean statement as temp (overwrite original value)");
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
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_3, REGISTER_5, 0, "Getting left operand of and operator");
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_4, REGISTER_5, -1, "Getting right operand of and operator");
	InstructionManagerInstance.AddInstructionRO(MUL, REGISTER_2, REGISTER_3, REGISTER_4, "Performing multiplication (and operation) on R3 and R4");
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_2, REGISTER_5, -1, "Store result of and operation as temp (overwrite left operand)");
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, -1, "Store -1");
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_5, REGISTER_1, REGISTER_5, "Decrement stack top");
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

	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_3, REGISTER_5, 0, "Getting left operand of multiplication");
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_4, REGISTER_5, -1, "Getting right operand of multiplication");
	InstructionManagerInstance.AddInstructionRO(MUL, REGISTER_2, REGISTER_3, REGISTER_4, "Performing multiplication on R3 and R4");
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_2, REGISTER_5, -1, "Store result of multiplication as temp (overwrite left operand)");
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, -1, "Store -1");
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_5, REGISTER_1, REGISTER_5, "Decrement stack top");
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

	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_3, REGISTER_5, 0, "Getting left operand of division");
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_4, REGISTER_5, -1, "Getting right operand of division");
	InstructionManagerInstance.AddInstructionRO(DIV, REGISTER_2, REGISTER_3, REGISTER_4, "Performing division on R3 and R4");
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_2, REGISTER_5, -1, "Store result of division as temp (overwrite left operand)");
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, -1, "Store -1");
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_5, REGISTER_1, REGISTER_5, "Decrement stack top");
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
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_3, REGISTER_5, 0, "Getting left operand of addition");
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_4, REGISTER_5, -1, "Getting right operand of addition");
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_2, REGISTER_3, REGISTER_4, "Performing addition on R3 and R4");

	// Adjust value by subtracting 1
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_1, REGISTER_2, REGISTER_0, "Store result into temp R1");
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_3, REGISTER_0, 1, "Store 1 into R3");
	InstructionManagerInstance.AddInstructionRO(SUB, REGISTER_1, REGISTER_1, REGISTER_3, "Performing subtraction on R1 and R3");

	// If result is > 1 then store result - 1 as the temp variable.
	InstructionManagerInstance.AddInstructionRM(JGT, REGISTER_1, REGISTER_7, 2, "Performing subtraction on R1 and R3");
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_2, REGISTER_5, -1, "Store the result of OR operation as temp (overwrite left operand)");
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_7, REGISTER_7, REGISTER_3, "Skip 1 instruction");

	// Store adjusted value
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_1, REGISTER_5, -1, "Store adjusted result of OR operation as temp (overwrite left operand)");

	// Adjust stack top
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, -1, "Store -1");
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_5, REGISTER_1, REGISTER_5, "Decrement stack top");
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
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_3, REGISTER_5, 0, "Getting left operand of addition");
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_4, REGISTER_5, -1, "Getting right operand of addition");
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_2, REGISTER_3, REGISTER_4, "Performing addition on R3 and R4");
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_2, REGISTER_5, -1, "Store result of addition as temp (overwrite left operand)");
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, -1, "Store -1");
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_5, REGISTER_1, REGISTER_5, "Decrement stack top");
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

	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_3, REGISTER_5, 0, "Getting left operand of subtraction");
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_4, REGISTER_5, -1, "Getting right operand of subtraction");
	InstructionManagerInstance.AddInstructionRO(SUB, REGISTER_2, REGISTER_3, REGISTER_4, "Performing subtraction on R3 and R4");
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_2, REGISTER_5, -1, "Store result of subtraction as temp (overwrite left operand)");
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, -1, "Store -1");
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_5, REGISTER_1, REGISTER_5, "Decrement stack top");
}

void CodeGenerator::generateCodeForLessThanNode(ASTNode Node)
{
	// Right Side
	generateCodeForSimpleExpressionNode(*Node.getBaseSimpleExprNode());

	// Left Side
	generateCodeForSimpleExpressionNode(*Node.getBaseSimpleExprNode2());

	// We do Right - Left then compare the results to 0.
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_3, REGISTER_5, 0, "Getting left operand of <");
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_4, REGISTER_5, -1, "Getting right operand of <");
	InstructionManagerInstance.AddInstructionRO(SUB, REGISTER_2, REGISTER_4, REGISTER_3, "Performing subtraction on R4 and R3");

	// If result is > 0 then store 1 as the value (since left was less than right).
	InstructionManagerInstance.AddInstructionRM(JGT, REGISTER_2, REGISTER_7, 4, "Jumping if R2 greater than 0");
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, 0, "Load 0 (false) ");
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_1, REGISTER_5, -1, "Store the result of < as temp (overwrite left operand)");
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, 2, "Load 2 into R1 ");
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_7, REGISTER_7, REGISTER_1, "Skip 2 instructions");

	// Store adjusted value
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, 1, "Load 1 (true) ");
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_1, REGISTER_5, -1, "Store the result of < as temp (overwrite left operand)");


	// Adjust stack top
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, -1, "Store -1");
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_5, REGISTER_1, REGISTER_5, "Decrement stack top");
}

void CodeGenerator::generateCodeForEqualNode(ASTNode Node)
{
	// Right Side
	generateCodeForSimpleExpressionNode(*Node.getBaseSimpleExprNode());

	// Left Side
	generateCodeForSimpleExpressionNode(*Node.getBaseSimpleExprNode2());

	// We do Right - Left then compare the results to 0.
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_3, REGISTER_5, 0, "Getting left operand of =");
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_4, REGISTER_5, -1, "Getting right operand of =");
	InstructionManagerInstance.AddInstructionRO(SUB, REGISTER_2, REGISTER_4, REGISTER_3, "Performing subtraction on R4 and R3"); 

	// If result is = 0 then store 1 as the value (since left was eqaul to right).
	InstructionManagerInstance.AddInstructionRM(JEQ, REGISTER_2, REGISTER_7, 4, "Jumping if R2 equal to 0");
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, 0, "Load 0 (false) ");
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_1, REGISTER_5, -1, "Store the result of = as temp (overwrite left operand)");
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, 2, "Load 2 into R1 ");
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_7, REGISTER_7, REGISTER_1, "Skip 2 instructions");
	
	// Store adjusted value
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, 1, "Load 1 (true) ");
	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_1, REGISTER_5, -1, "Store the result of = as temp (overwrite left operand)");

	// Adjust stack top
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, -1, "Store -1");
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_5, REGISTER_1, REGISTER_5, "Decrement stack top");
}

void CodeGenerator::generateCodeForIdentifierNode(ASTNode Node)
{
	vector<tuple<string, ReturnTypes>> Params = SymbolTable.at(CurrentFunction).getParameters();
	ptrdiff_t Position = find_if(
		Params.begin(), 
		Params.end(), 
		[&](const tuple<string, ReturnTypes>& tuple) {return get<0>(tuple) == Node.getIdentifierName(); }) - Params.begin();

	int ArgumentPointer = -1 * (2 + Params.size()) + int(Position);
	InstructionManagerInstance.AddInstructionRM(LD, REGISTER_1, REGISTER_6, ArgumentPointer, "Loading actual value of argument into R1 from stack");

	InstructionManagerInstance.AddInstructionRM(ST, REGISTER_1, REGISTER_5, 1, "Store the argument into the temp variables slot");
	InstructionManagerInstance.AddInstructionRM(LDC, REGISTER_1, REGISTER_0, 1, "Store 1 ");
	InstructionManagerInstance.AddInstructionRO(ADD, REGISTER_5, REGISTER_1, REGISTER_5, "Increment stack top");
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