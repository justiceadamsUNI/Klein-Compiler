#pragma once
#include <string>
#include <vector>

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

enum Register {
	REGISTER_0,
	REGISTER_1,
	REGISTER_2,
	REGISTER_3,
	REGISTER_4,
	REGISTER_5,
	REGISTER_6,
	REGISTER_7
};

class InstructionManager {
public:
	vector<string> getInstructions();
	int getInstructionCount();
	int getInstructionVectorSize();

	void addInstructionRO(RegisterOnlyInstruction RO, Register R1, Register R2, Register R3, string comment);
	void addInstructionRO(RegisterOnlyInstruction RO, Register R1, Register R2, Register R3);
	void addInstructionRM(RegisterMemoryInstruction RM, Register R1, Register R2, int offset, string comment);
	void addInstructionRM(RegisterMemoryInstruction RM, Register R1, Register R2, int offset);
	void addInstructionRMForBackPatching(RegisterMemoryInstruction RM, Register R1, Register R2, string ReplaceLabel, string comment);

	// For back-patching and comments
	void replaceInstructionAtIndex(int index, string Replacement);
	void addCommentToInstructions(string comment);

private:
	vector<string> Instructions;
	int InstructionCount = 0;

	string mapROtoString(RegisterOnlyInstruction RO);
	string mapRMtoString(RegisterMemoryInstruction RM);
	string mapRegisterToString(Register Rx);
};