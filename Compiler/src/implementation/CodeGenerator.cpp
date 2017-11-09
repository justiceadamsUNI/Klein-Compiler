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
		walkTree(*DefNodes.at(i));
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

/*Walks the AST Tree and adds corresponding code for each node
  Assumes that we are given a def node.*/
void CodeGenerator::walkTree(ASTNode ASTTree)
{
	int CurrentFunctionParamsSize = SymbolTable.find(CurrentFunction)->second.getParameters().size();
	string LiteralValue;
	
	// This code below will need to change drastically to actually walk whatever tree it's given in project 6.
	// For now, we know the structure, so this is hard coded. Consider this a ToDo:
	if (ASTTree.getAstNodeType() == DEF_NODE_TYPE) {
		vector<ASTNode*> printStatements = ASTTree.getBodyNode()->getPrintStatements();
		for (int i = printStatements.size() - 1; i >= 0; i--) {
			LiteralValue = 
				printStatements.at(i)->
				getBaseExprNode()->
				getBaseSimpleExprNode()->
				getBaseTermNode()->
				getFactorNode()->
				getLiteralNode()->
				getLiteralValue();

			addInstruction("LDC 1, 1(0)   ; Push 1 into the temp reg R1");
			addInstruction("ADD 5, 1, 5   ; Incrementing Stack top by 1");
			addInstruction("LDC 1, " + LiteralValue + "(0)   ; Pushing the value of the print statement into a temp reg R1");
			addInstruction("ST 1, 0(5)   ; Storing into DMEM in the temp data objects slot. Print statement will grab the arg from here.");
			callFunction("print");
		}
	}

	// We know what the base expression node looks like So we can directly walk down it 
	// to get the literal node. Will change with project 6.
	LiteralValue = 
		ASTTree.getBodyNode()->
		getBaseExprNode()->
		getBaseSimpleExprNode()->
		getBaseTermNode()->
		getFactorNode()->
		getLiteralNode()->
		getLiteralValue();

	addInstruction("LDC 1, " + LiteralValue + "(0)   ; Pushing the return value of main into a register.");
	addInstruction("ST 1, -" + to_string(3 + CurrentFunctionParamsSize) + "(6)   ; Storing return value of main into its' stack frame");
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