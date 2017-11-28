#include "../header/InstructionManager.h"

using namespace std;

string returnString = "";

string InstructionManager::AddInstructionRO(RegisterOnlyInstruction RO, int R1, int R2, int R3, string comment)
{
	returnString = MapROtoString(RO) + " " + to_string(R1) + ", " + to_string(R2) + ", " + to_string(R3);
	returnString += "\t;\t" + comment;
	return returnString;
}

string InstructionManager::AddInstructionRO(RegisterOnlyInstruction RO, int R1, int R2, int R3)
{
	returnString = MapROtoString(RO) + " " + to_string(R1) + ", " + to_string(R2) + ", " + to_string(R3);
	return returnString;
}

string InstructionManager::AddInstructionRM(RegisterMemoryInstruction RM, int R1, int R2, int offset, string comment)
{
	returnString = MapRMtoString(RM) + " " + to_string(R1) + ", " + to_string(offset) + "(" + to_string(R2) + ")";
	returnString += "\t;\t" + comment;
	return returnString;
}

string InstructionManager::AddInstructionRM(RegisterMemoryInstruction RM, int R1, int R2, int offset)
{
	returnString = MapRMtoString(RM) + " " + to_string(R1) + ", " + to_string(offset) + "(" + to_string(R2) + ")";
	return returnString;
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
		break;
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
		break;
	}
}
