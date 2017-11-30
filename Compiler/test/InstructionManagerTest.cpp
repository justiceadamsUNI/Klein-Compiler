#include "catch.hpp"
#include "../src/header/InstructionManager.h"

TEST_CASE("Test that R0 correctly prints 0 when used as paramater and ADD instuction works", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRO(ADD, REGISTER_0, REGISTER_0, REGISTER_0);

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "0: ADD 0, 0, 0;   ");
}

TEST_CASE("Test that R1 correctly prints 1 when used as paramater and SUB instuction works", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRO(SUB, REGISTER_1, REGISTER_1, REGISTER_1);

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "0: SUB 1, 1, 1;   ");
}

TEST_CASE("Test that R2 correctly prints 2 when used as paramater and MUL instuction works", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRO(MUL, REGISTER_2, REGISTER_2, REGISTER_2);

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "0: MUL 2, 2, 2;   ");
}

TEST_CASE("Test that R3 correctly prints 3 when used as paramater and DIV instuction works", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRO(DIV, REGISTER_3, REGISTER_3, REGISTER_3);

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "0: DIV 3, 3, 3;   ");
}

TEST_CASE("Test that R4 correctly prints 4 when used as paramater and HALT instuction works", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRO(HALT, REGISTER_4, REGISTER_4, REGISTER_4);

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "0: HALT 4, 4, 4;   ");
}

TEST_CASE("Test that R5 correctly prints 5 when used as paramater and IN instuction works", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRO(IN, REGISTER_5, REGISTER_5, REGISTER_5);

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "0: IN 5, 5, 5;   ");
}

TEST_CASE("Test that R6 correctly prints 6 when used as paramater and OUT instuction works", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRO(OUT, REGISTER_6, REGISTER_6, REGISTER_6);

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "0: OUT 6, 6, 6;   ");
}

TEST_CASE("Test that AddInstuctionRo() works when adding a comment to instruction", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRO(ADD, REGISTER_0, REGISTER_0, REGISTER_0, "Test Comment");

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "0: ADD 0, 0, 0;   Test Comment");
}

TEST_CASE("Test that R7 correctly prints 7 when used as paramater and LDC instuction works", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRM(LDC, REGISTER_7, REGISTER_7, -10);

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "0: LDC 7, -10(7);   ");
}

TEST_CASE("Test that LDA instruction works correctly", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRM(LDA, REGISTER_0, REGISTER_0, -10);

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "0: LDA 0, -10(0);   ");
}

TEST_CASE("Test that LD instruction works correctly", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRM(LD, REGISTER_0, REGISTER_0, -10);

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "0: LD 0, -10(0);   ");
}

TEST_CASE("Test that ST instruction works correctly", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRM(ST, REGISTER_0, REGISTER_0, -10);

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "0: ST 0, -10(0);   ");
}

TEST_CASE("Test that JEQ instruction works correctly", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRM(JEQ, REGISTER_0, REGISTER_0, -10);

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "0: JEQ 0, -10(0);   ");
}

TEST_CASE("Test that JNE instruction works correctly", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRM(JNE, REGISTER_0, REGISTER_0, -10);

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "0: JNE 0, -10(0);   ");
}

TEST_CASE("Test that JLT instruction works correctly", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRM(JLT, REGISTER_0, REGISTER_0, -10);

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "0: JLT 0, -10(0);   ");
}

TEST_CASE("Test that JLE instruction works correctly", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRM(JLE, REGISTER_0, REGISTER_0, -10);

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "0: JLE 0, -10(0);   ");
}

TEST_CASE("Test that JGT instruction works correctly", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRM(JGT, REGISTER_0, REGISTER_0, -10);

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "0: JGT 0, -10(0);   ");
}

TEST_CASE("Test that JGE instruction works correctly", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRM(JGE, REGISTER_0, REGISTER_0, -10);

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "0: JGE 0, -10(0);   ");
}

TEST_CASE("Test that AddInstuctionRM() works when adding a comment to instruction", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRM(LD, REGISTER_0, REGISTER_0, -10, "Test Comment");

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "0: LD 0, -10(0);   Test Comment");
}

TEST_CASE("Test that AddInstuctionRMForBackPatching() works correctly", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRMForBackPatching(JEQ, REGISTER_0, REGISTER_0, "PlaceHolder", "Test Comment");

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "0: JEQ 0, PlaceHolder(0);   Test Comment");
}

TEST_CASE("Test that ReplaceInstructionAtIndex() works correctly", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRO(ADD, REGISTER_0, REGISTER_0, REGISTER_0);
	InstructionManagerInst.replaceInstructionAtIndex(0, "Replacement");

	REQUIRE(InstructionManagerInst.getInstructions().at(0) == "Replacement");
}

TEST_CASE("Test that addCommentToInstructions() works correctly and doesn't increment InstructionCount", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRO(ADD, REGISTER_0, REGISTER_0, REGISTER_0);
	InstructionManagerInst.addCommentToInstructions("COMMENT");

	REQUIRE(InstructionManagerInst.getInstructions().at(1) == "COMMENT");
	REQUIRE(InstructionManagerInst.getInstructionCount() == 1);
}

TEST_CASE("Test that getInstructionCount() works correctly and ignores comments", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRO(ADD, REGISTER_0, REGISTER_0, REGISTER_0);
	InstructionManagerInst.addInstructionRO(ADD, REGISTER_0, REGISTER_0, REGISTER_0);
	InstructionManagerInst.addCommentToInstructions("COMMENT");

	REQUIRE(InstructionManagerInst.getInstructionCount() == 2);
}

TEST_CASE("Test that getInstructionVectorSize() works correctly and includes comments", "[Instruction Manager Test]") {
	InstructionManager InstructionManagerInst;
	InstructionManagerInst.addInstructionRO(ADD, REGISTER_0, REGISTER_0, REGISTER_0);
	InstructionManagerInst.addInstructionRO(ADD, REGISTER_0, REGISTER_0, REGISTER_0);
	InstructionManagerInst.addCommentToInstructions("COMMENT");

	REQUIRE(InstructionManagerInst.getInstructionVectorSize() == 3);
}