#include "catch.hpp"
#include "tm-cli-go.hpp"
#include "../src/header/Parser.h"
#include "../src/header/SemanticChecker.h"
#include "../src/header/CodeGenerator.h"
#include <array>

void compileKleinFileToTm(string fileName) {
	// Scan, Parse, Semantic Check, Then Compile to target file
	Scanner Scanner(fileName, 1);
	Parser Parser(Scanner);
	Parser.parseProgram();

	ASTNode AstTree = Parser.getFinalASTNode();
	SemanticChecker SemanticCheckerVar = SemanticChecker(AstTree);

	CodeGenerator CodeGenerator(AstTree, SemanticCheckerVar.getSymbolTable(), "UnitTestGeneratedProgram.tm");
	CodeGenerator.writeOutTargetCode();
}

vector<int> callTmProgramWithArgumentsAndGetOutputStatements(int argc, char* argv[]) {
	// Mimic the way that TM takes in command line args.
	// remember to always have an arg list of 2 or more.
	TmCliGoTest TmCli = TmCliGoTest();
	TmCli.main(argc, argv);

	return TmCli.getOutputStatemnts();
}

TEST_CASE("Test that the generic print-one.kln prints the value 1 twice when compiled to TM", "[Integration Test]") {
	compileKleinFileToTm("programs/print-one.kln");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutputStatements(2, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(0) == 1);
	REQUIRE(OutputStatements.at(1) == 1);
}

TEST_CASE("Test that fraction addition works correctly", "[Integration Test]") {
	compileKleinFileToTm("programs/fraction-addition.kln");

	char* argv[6] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm", "1", "2", "3", "4" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutputStatements(6, argv);

	REQUIRE(OutputStatements.size() == 3);
	REQUIRE(OutputStatements.at(2) == 1);
	REQUIRE(OutputStatements.at(1) == 8);
	REQUIRE(OutputStatements.at(0) == 10);
}

TEST_CASE("Test that test_klein_program_nested_if.kln works correctly", "[Integration Test]") {
	compileKleinFileToTm("test/klein/test_klein_program_nested_if.kln");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutputStatements(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 100);
}

TEST_CASE("Test that test_klein_program_multiple_function_calls.kln works correctly", "[Integration Test]") {
	compileKleinFileToTm("test/klein/test_klein_program_multiple_function_calls.kln");

	char* argv[5] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm", "1", "20", "10" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutputStatements(5, argv);

	REQUIRE(OutputStatements.size() == 1);
	// Blaze it fam.
	REQUIRE(OutputStatements.at(0) == 420);
}

TEST_CASE("Test that isExcellent works correctly when number is excellent", "[Integration Test]") {
	compileKleinFileToTm("programs/is-excellent.kln");

	char* argv[3] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm", "3468" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutputStatements(3, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that isExcellent works correctly when number is not excellent", "[Integration Test]") {
	compileKleinFileToTm("programs/is-excellent.kln");

	char* argv[3] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm", "50" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutputStatements(3, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 0);
}

TEST_CASE("Test that divisible-by-seven.kln works correctly when number is divisible by seven", "[Integration Test]") {
	compileKleinFileToTm("programs/divisible-by-seven.kln");

	char* argv[3] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm", "-49" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutputStatements(3, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that divisible-by-seven.kln works correctly when number is not divisible by seven", "[Integration Test]") {
	compileKleinFileToTm("programs/divisible-by-seven.kln");

	char* argv[3] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm", "701" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutputStatements(3, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 0);
}

TEST_CASE("Test that modulus-by-hand.kln works correctly", "[Integration Test]") {
	compileKleinFileToTm("programs/modulus-by-hand.kln");

	char* argv[4] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm", "25", "13" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutputStatements(4, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 12);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that fibonacci.kln works correctly with valid paramater", "[Integration Test]") {
	compileKleinFileToTm("programs/fibonacci.kln");

	char* argv[3] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm", "12" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutputStatements(3, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 144);
}

TEST_CASE("Test that fibonacci.kln works correctly with invalid paramater", "[Integration Test]") {
	compileKleinFileToTm("programs/fibonacci.kln");

	char* argv[3] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm", "-12" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutputStatements(3, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 0);
}

TEST_CASE("Test that divide.kln works correctly", "[Integration Test]") {
	compileKleinFileToTm("programs/divide.kln");

	char* argv[5] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm", "89", "19", "4" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutputStatements(5, argv);

	REQUIRE(OutputStatements.size() == 5);
	REQUIRE(OutputStatements.at(4) == 2);
	REQUIRE(OutputStatements.at(3) == 2);
	REQUIRE(OutputStatements.at(2) == 4);
	REQUIRE(OutputStatements.at(1) == 8);
	REQUIRE(OutputStatements.at(0) == 46);
}

TEST_CASE("Test that sieve.kln works correctly", "[Integration Test]") {
	compileKleinFileToTm("programs/sieve.kln");

	char* argv[3] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm", "7" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutputStatements(3, argv);

	REQUIRE(OutputStatements.size() == 7);
	REQUIRE(OutputStatements.at(6) == 1);
	REQUIRE(OutputStatements.at(5) == 7);
	REQUIRE(OutputStatements.at(4) == 0);
	REQUIRE(OutputStatements.at(3) == 5);
	REQUIRE(OutputStatements.at(2) == 0);
	REQUIRE(OutputStatements.at(1) == 3);
	REQUIRE(OutputStatements.at(0) == 2);
}

TEST_CASE("Test that horner-param.kln works correctly", "[Integration Test]") {
	compileKleinFileToTm("programs/horner-param.kln");

	char* argv[7] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm", "5", "1", "-5", "-3", "10" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutputStatements(7, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 5047);
}

TEST_CASE("Test that circular-prime.kln works correctly", "[Integration Test]") {
	compileKleinFileToTm("programs/circular-prime.kln");

	char* argv[3] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm", "73" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutputStatements(3, argv);

	REQUIRE(OutputStatements.size() == 12);
	REQUIRE(OutputStatements.at(11) == 11);
	REQUIRE(OutputStatements.at(10) == 73);
	REQUIRE(OutputStatements.at(9) == 71);
	REQUIRE(OutputStatements.at(8) == 37);
	REQUIRE(OutputStatements.at(7) == 31);
	REQUIRE(OutputStatements.at(6) == 17);
	REQUIRE(OutputStatements.at(5) == 13);
	REQUIRE(OutputStatements.at(4) == 11);
	REQUIRE(OutputStatements.at(3) == 7);
	REQUIRE(OutputStatements.at(2) == 5);
	REQUIRE(OutputStatements.at(1) == 3);
	REQUIRE(OutputStatements.at(0) == 2);
}

TEST_CASE("Test that euclid.kln works correctly", "[Integration Test]") {
	compileKleinFileToTm("programs/euclid.kln");

	char* argv[4] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm", "7074", "108" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutputStatements(4, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 54);
}

TEST_CASE("Test that euclid.kln works correctly with relitively prime numbers", "[Integration Test]") {
	compileKleinFileToTm("programs/euclid.kln");

	char* argv[4] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm", "7073", "108" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutputStatements(4, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 1);
}