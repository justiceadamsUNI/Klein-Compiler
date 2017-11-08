#include "../header/CodeGenerator.h"
#include <iostream>
#include <fstream>

using namespace std;

void CodeGenerator::writeOutTargetCode()
{
	setUpRuntimeEnvironment();
	generatePrintFunction();
	generateMainFunction();
	writeInstructionsToFile();
}

void CodeGenerator::setUpRuntimeEnvironment()
{
	vector<tuple<string, ReturnTypes>>Params = SymbolTable.at("main").getParameters();

	int LastParamLocation = 1;
	if (!Params.empty())
	{
		LastParamLocation = Params.size() + 2;
		for (int i = Params.size(); i >= 0; i--)
		{

			addInstruction("LD  1, " + to_string(i + 1) + "(0)");
			addInstruction("ST  1, " + to_string(i + 2) + "(0)   ; Storing Command line arg " + to_string(i));
		}
	}

	// 1 slots past the last command line arg = access link (top of stack)
	addInstruction("ST  0, " + to_string(LastParamLocation + 2) + "(0)");


	//adjust status pointer by loading constant into R6.
	addInstruction("LDC  " + to_string(6) + ", " + to_string(LastParamLocation + 3) + "(0)");


	//set return adress
	// control link = return adress. Store it into Dmem at 2 slots past last command line arg.
	// USING REGISTER 1 AS TEMP REGISTER.
	addInstruction("LDA  " + to_string(1) + ", " + to_string(2) + "(7)");
	addInstruction("ST  " + to_string(1) + ", " + to_string(LastParamLocation + 1) + "(0)   ; Storing the return adress in DMEM at the control link slot");

	//jump
	addInstruction("LDA  " + to_string(7) + ", 18(7)   ; Jump to main");  //16 slots to skip Print function

	// load into register 1 the value returned from main and print it.
	addInstruction("LD  1, 1(5)   ; load return value");
	addInstruction("OUT  1,0,0   ; Printing main return value");

	//quit
	addInstruction("HALT  0,0,0   ;End of program");
}

void CodeGenerator::addInstruction(string Instruction)
{
	Instructions.push_back(to_string(InstructionCount) + ": " +  Instruction);
	InstructionCount++;
}

void CodeGenerator::addWhiteSpace()
{
	Instructions.push_back("");
	Instructions.push_back("");
	Instructions.push_back("");
}

void CodeGenerator::generateMainFunction()
{
	FunctionLocation.insert(pair<string, int>("main", InstructionCount));
	GenerateFunction();
	vector<ASTNode*> DefNodes = Tree.getDefinitions()->getDefNodes();
	for (int i = 0; i < DefNodes.size(); i++) {
		CurrentFunction = DefNodes.at(i)->getIdentifierNode()->getIdentifierName();
		walkTree(*DefNodes.at(i));
	}
	returnFromFunction();
}

void CodeGenerator::generatePrintFunction()
{
	FunctionLocation.insert(pair<string, int>("print", InstructionCount));
	GenerateFunction();
	addInstruction("LD  1, -3(6)   ; Loading the value of whatever argument is passed to print to R1");
	addInstruction("OUT 1,0,0   ; Printing the value of whatever argument is passed to print");
	returnFromFunction();
}

void CodeGenerator::walkTree(ASTNode ASTTree)
{
	//Assume that we have a Def Node
	//Handle code for the print function
	int CurrentFunctionParamsSize = SymbolTable.find(CurrentFunction)->second.getParameters().size();
	string temp;
	
	
	// This code below will need to change drastically to actually walk whatever tree it's given in project 6.
	// For now, we know the structure.
	if (ASTTree.getAstNodeType() == DEF_NODE_TYPE) {
		vector<ASTNode*> printStatements = ASTTree.getBodyNode()->getPrintStatements();
		for (int i = 0; i < printStatements.size(); i++) {
			temp = printStatements.at(i)->getBaseExprNode()->getBaseSimpleExprNode()->getBaseTermNode()->getFactorNode()->getLiteralNode()->getLiteralValue();
			addInstruction("LDC 1, 1(0)   ; Push 1 into the temp reg R1");
			addInstruction("ADD 5,1,5   ; Incrementing Stack top by 1");
			addInstruction("LDC 1, " + temp + "(0)   ; Pushing the value of the print statement into a temp reg R1");
			addInstruction("ST 1, 0(5)   ; storing into DMEM");
			callFunction("print");
		}
	}

	// We know what the base expression node looks like So we can directly walk down it to get the literal node. Will change with project 6
	temp = ASTTree.getBodyNode()->getBaseExprNode()->getBaseSimpleExprNode()->getBaseTermNode()->getFactorNode()->getLiteralNode()->getLiteralValue();
	addInstruction("LDC 1, " + temp + "(0)   ; Pushing the return value of main into a register.");
	addInstruction("ST 1, -" + to_string(3 + CurrentFunctionParamsSize) + "(6)   ; storing return value of main into its' stack frame");
}

void CodeGenerator::GenerateFunction()
{
	addWhiteSpace();
	setRegistersInDmem();
}

void CodeGenerator::setRegistersInDmem()
{
	addInstruction("ST  2, 0(6)   ; Store register 2 into ( new status pointer) ");
	addInstruction("ST  3, 1(6)   ; Store register 3 into (new status pointer + 1) ");
	addInstruction("ST  4, 2(6)   ; Store register 4 into (new status pointer + 2) ");
	addInstruction("ST  1, 3(6)   ; Store register 1 (the old status pointer) into ( new status pointer + 3) ");

	addInstruction("LDC  1, 3(0)   ; Store constant 3 in R1 ");
	addInstruction("ADD  5,1,6   ; Adjust top of stack to be (new status pointer + 3) ");
}

void CodeGenerator::returnFromFunction()
{
	//Store R6 into R1. R1 Does not persist across function calls.
	addInstruction("ADD  1,6,0   ; Storing a temp copy of status pointer in R1");

	restoreRegistersFromDmem();
	addInstruction("LD  5,-1(1)   ; Restoring the stack top variable");
	addInstruction("LD  7,-2(1)   ; Restoring the return adress from the control link");
}

void CodeGenerator::callFunction(string functionName)
{
	int argCount = functionName == "print" ? 1 : SymbolTable.find(functionName)->second.getParameters().size();

	//set incoming args
	for (int i = 0; i < argCount; i++) {
		addInstruction("LD 1, -" + to_string(i) + "(5)   ; Moving Temp arg " + to_string(i) + " to R1");
		addInstruction("ST 1, " + to_string(i + 2) + "(5)   ; Storing Temp arg " + to_string(i) + " to Arg 1");
	}
	//set access link
	//set ctrl link
	//update status pointer
	//jump to function
	// 1 slots past the last command line arg = access link (top of stack)
	addInstruction("ST  5, " + to_string(argCount + 3) + "(5)   ; Storing Access Link, about to call " + functionName);


	//set return adress
	// control link = return adress. Store it into Dmem at 2 slots past last command line arg.
	// USING REGISTER 1 AS TEMP REGISTER.
	addInstruction("LDA  " + to_string(1) + ", " + to_string(4) + "(7) ; Saving next executed line, about to call " + functionName);
	addInstruction("ST  " + to_string(1) + ", " + to_string(argCount + 2) + "(5)   ; Storing the return adress in DMEM at the control link slot");

	//adjust status pointer but remember to keep old one alive in R1 so that the called function can store it.
	addInstruction("ADD  1, 6, 0   ; Copying current status pointer before function call, about to call " + functionName);
	addInstruction("LDA  " + to_string(6) + ", " + to_string(argCount + 4) + "(5)   ; Adjusting Status pointer, about to call " + functionName);

	//jump
	addInstruction("LDA  " + to_string(7) + ", " + to_string(FunctionLocation.find(functionName)->second) + "(0)   ; Jump to " + functionName);


}

void CodeGenerator::restoreRegistersFromDmem()
{
	addInstruction("LD  2, 0(6)   ; restore register 2 from (status pointer ) ");
	addInstruction("LD  3, 1(6)   ; restore register 3 from (status pointer + 1) ");
	addInstruction("LD  4, 2(6)   ; restore register 4 from (status pointer + 2) ");
	addInstruction("LD  6, 3(6)   ; restore register 6 from (status pointer + 3) ");
}

void CodeGenerator::writeInstructionsToFile()
{
	ofstream TMFile;
	TMFile.open("TmFile.tm", ios::out); // change file name


	// read in all cl args and put into a stack frame.
	// set control link
	// set access link



	// adjust program counter to call main (which i imagine is (runtime env) + 1?).
	// where to put print? (before or after main).
	
	// Just prints "1" to the screen right now.

	for (int i = 0; i < Instructions.size(); i++)
	{
		TMFile << Instructions.at(i) << endl;
	}

	TMFile.close();
}