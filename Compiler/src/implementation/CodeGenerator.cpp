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
	addInstruction("LDA  " + to_string(7) + ", " + to_string(16) + "(7)   ; Jump to main");  //16 slots to skip Print function

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
		if (DefNodes.at(i)->getIdentifierNode()->getIdentifierName() == "main"){
			walkTree(*DefNodes.at(i));
			break;
		}
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
		if (ASTTree.getAstNodeType() == DEF_NODE_TYPE) {
			vector<ASTNode*> printStatements = ASTTree.getBodyNode()->getPrintStatements();
			for (int i = 0; i < printStatements.size(); i++) {
				/*
				In the future we will have to pass temp to the function for evaluation purposes
				addInstruction("")*/
				string temp = printStatements.at(i)->getBaseExprNode()->getBaseSimpleExprNode()->getBaseTermNode()->getFactorNode()->getLiteralNode()->getLiteralValue();
				addInstruction("LDC 1, 1(0)   ; Push 1 into the temp reg R1");
				addInstruction("ADD 5,1,5   ; Incrementing Stack top by 1");
				addInstruction("LDC 1, " + temp + "(0)   ; Pushing the value of the print statement into a temp reg R1");
				addInstruction("ST 1, 0(5)   ; storing into DMEM");
				callFunction("print");
				//goto Print
		}
	}
	//Handle code for the integer literal value of 1
}

void CodeGenerator::GenerateFunction()
{
	addWhiteSpace();
	setRegistersInDmem();
}

void CodeGenerator::setRegistersInDmem()
{
	addInstruction("ST  2, 1(6)   ; Store register 2 into (status pointer + 1) ");
	addInstruction("ST  3, 2(6)   ; Store register 3 into (status pointer + 2) ");
	addInstruction("ST  4, 3(6)   ; Store register 4 into (status pointer + 3) ");
	addInstruction("ST  6, 4(6)   ; Store register 6 into (status pointer + 4) ");

	addInstruction("LDC  1, 5(0)   ; Store constant 5 in R1 ");
	addInstruction("ADD  5,1,6   ; Adjust top of stack to be (status pointer + 5) ");
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
	int argCount = SymbolTable.find(functionName)->second.getParameters().size();
	//set incoming args
	for (int i = 0; i < argCount; i++) {
		addInstruction("LD 1, -" + to_string(i) + "(5)   ; Moving Temp " + to_string(i) + " to R1");
		addInstruction("ST 1, " + to_string(i + 2) + "(5)   ; Storing Temp " + to_string(i) + " to Arg 1");
	}
	//set access link
	//set ctrl link
	//update status pointer
	//jump to function
	// 1 slots past the last command line arg = access link (top of stack)
	addInstruction("ST  5, " + to_string(argCount + 3) + "(5)   ; Storing Access Link, about to call " + functionName);


	//adjust status pointer by loading constant into R6.
	addInstruction("LDC  " + to_string(6) + ", " + to_string(argCount + 4) + "(5)   ; Adjusting Status pointer, about to call " + functionName);


	//set return adress
	// control link = return adress. Store it into Dmem at 2 slots past last command line arg.
	// USING REGISTER 1 AS TEMP REGISTER.
	addInstruction("LDA  " + to_string(1) + ", " + to_string(2) + "(7) ; Saving next executed line, about to call " + functionName);
	addInstruction("ST  " + to_string(1) + ", " + to_string(argCount + 2) + "(5)   ; Storing the return adress in DMEM at the control link slot");

	//jump
	addInstruction("LDA  " + to_string(7) + ", " + to_string(FunctionLocation.find(functionName)->second) + "(0)   ; Jump to " + functionName);


}

void CodeGenerator::restoreRegistersFromDmem()
{
	addInstruction("LD  2, 1(6)   ; restore register 2 from (status pointer + 1) ");
	addInstruction("LD  3, 2(6)   ; restore register 3 from (status pointer + 2) ");
	addInstruction("LD  4, 3(6)   ; restore register 4 from (status pointer + 3) ");
	addInstruction("LD  6, 4(6)   ; restore register 6 from (status pointer + 4) ");
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