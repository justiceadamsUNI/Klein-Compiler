#include "../header/InstructionManager.h"

using namespace std;
#define NO_COMMENT ""

void InstructionManager::AddInstructionRO(RegisterOnlyInstruction RO, Register R1, Register R2, Register R3, string comment)
{
	string instruction = to_string(InstructionCount) + ": " 
		+ MapROtoString(RO) + " "
		+ MapRegisterToString(R1) + ", "
		+ MapRegisterToString(R2) + ", "
		+ MapRegisterToString(R3)
		+ ";   " + comment;
	
	Instructions.push_back(instruction);
	InstructionCount++;
}

void InstructionManager::AddInstructionRO(RegisterOnlyInstruction RO, Register R1, Register R2, Register R3)
{
	AddInstructionRO(RO, R1, R2, R3, NO_COMMENT);
}

void InstructionManager::AddInstructionRM(RegisterMemoryInstruction RM, Register R1, Register R2, int offset, string comment)
{
	string instruction = to_string(InstructionCount) + ": "
		+ MapRMtoString(RM) + " "
		+ MapRegisterToString(R1) + ", "
		+ to_string(offset) + "("
		+ MapRegisterToString(R2) + ")"
		+ ";   " + comment;

	Instructions.push_back(instruction);
	InstructionCount++;
}

void InstructionManager::AddInstructionRM(RegisterMemoryInstruction RM, Register R1, Register R2, int offset)
{
	AddInstructionRM(RM, R1, R2, offset, NO_COMMENT);
}

void InstructionManager::AddInstructionRMForBackPatching(RegisterMemoryInstruction RM, Register R1, Register R2, string ReplaceLabel, string comment)
{
	string instruction = to_string(InstructionCount) + ": " 
		+ MapRMtoString(RM) + " "
		+ MapRegisterToString(R1) + ", "
		+ ReplaceLabel + "("
		+ MapRegisterToString(R2) + ")"
		+ ";   " + comment;

	Instructions.push_back(instruction);
	InstructionCount++;
}

void InstructionManager::replaceInstructionAtIndex(int index, string Replacement)
{
	Instructions.at(index) = Replacement;
}

void InstructionManager::AddCommentToInstructions(string comment)
{
	Instructions.push_back(comment);
}

int InstructionManager::GetInstructionCount()
{
	return InstructionCount;
}

int InstructionManager::GetInstructionVectorSize()
{
	return Instructions.size();
}

vector<string> InstructionManager::getInstructions()
{
	return Instructions;
}

string InstructionManager::MapROtoString(RegisterOnlyInstruction RO)
{
	switch (RO)
	{
	case IN:
		return "IN";
	case OUT:
		return "OUT";
	case ADD:
		return "ADD";
	case SUB:
		return "SUB";
	case MUL:
		return "MUL";
	case DIV:
		return "DIV";
	case HALT:
		return "HALT";
	default:
		return "ERROR";
	}

}

string InstructionManager::MapRMtoString(RegisterMemoryInstruction RM)
{
	switch (RM)
	{
	case LDC:
		return "LDC";
	case LDA:
		return "LDA";
	case LD:
		return "LD";
	case ST:
		return "ST";
	case JEQ:
		return "JEQ";
	case JNE:
		return "JNE";
	case JLT:
		return "JLT";
	case JLE:
		return "JLE";
	case JGT:
		return "JGT";
	case JGE:
		return "JGE";
	default:
		return "ERROR";
	}
}

string InstructionManager::MapRegisterToString(Register Rx)
{
	switch (Rx) {
	case REGISTER_0:
		return "0";
	case REGISTER_1:
		return "1";
	case REGISTER_2:
		return "2";
	case REGISTER_3:
		return "3";
	case REGISTER_4:
		return "4";
	case REGISTER_5:
		return "5";
	case REGISTER_6:
		return "6";
	case REGISTER_7:
		return "7";
	default:
		return "ERROR";
	}
}
