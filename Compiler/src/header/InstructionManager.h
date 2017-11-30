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
	void AddInstructionRO(RegisterOnlyInstruction RO, Register R1, Register R2, Register R3, string comment);
	void AddInstructionRO(RegisterOnlyInstruction RO, Register R1, Register R2, Register R3);
	
	void AddInstructionRM(RegisterMemoryInstruction RM, Register R1, Register R2, int offset, string comment);
	void AddInstructionRM(RegisterMemoryInstruction RM, Register R1, Register R2, int offset);
	void AddInstructionRMForBackPatching(RegisterMemoryInstruction RM, Register R1, Register R2, string ReplaceLabel, string comment);

	void replaceInstructionAtIndex(int index, string Replacement);

	void AddCommentToInstructions(string comment);
	
	int GetInstructionCount();
	int GetInstructionVectorSize();
	vector<string> getInstructions();

private:
	string MapROtoString(RegisterOnlyInstruction RO);
	string MapRMtoString(RegisterMemoryInstruction RM);
	string MapRegisterToString(Register Rx);
	
	vector<string> Instructions;
	int InstructionCount = 0;

};