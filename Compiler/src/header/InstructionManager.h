#pragma once
#include <string>;

using namespace std;

enum RegisterOnlyInstruction {
	IN,
	OUT,
	ADD,
	SUB,
	MUL,
	DIV,
	HALT
};

enum RegisterMemoryInstruction {
	LDC,
	LDA,
	LD,
	ST,
	JEQ,
	JNE,
	JLT,
	JLE,
	JGT, 
	JGE
};

class InstructionManager {
public:
	string AddInstructionRO(RegisterOnlyInstruction RO, int R1, int R2, int R3, string comment);
	string AddInstructionRO(RegisterOnlyInstruction RO, int R1, int R2, int R3);
	string AddInstructionRM(RegisterMemoryInstruction RM, int R1, int R2, int offset, string comment);
	string AddInstructionRM(RegisterMemoryInstruction RM, int R1, int R2, int offset);
	string MapROtoString(RegisterOnlyInstruction RO);
	string MapRMtoString(RegisterMemoryInstruction RM);
private:

};