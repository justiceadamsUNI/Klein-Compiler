#include "catch.hpp"
#include "tm-cli-go.hpp"
#include "../src/header/Parser.h"
#include "../src/header/SemanticChecker.h"
#include "../src/header/CodeGenerator.h"

vector<int> callTmProgramWithArgumentsAndGetOutput(char* argv[]) {
	// Mimic the way that TM takes in command line args.
	// remember to always have an arg list of 2 or more.
	TmCliGoTest TmCli = TmCliGoTest();
	TmCli.main(2, argv);

	return TmCli.getOutputStatemnts();
}

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

void compileKleinFileToTmWithoutOpeningKleinFile(string fileContents) {
	// Scan, Parse, Semantic Check, Then Compile to target file
	Scanner Scanner(fileContents, true);
	Parser Parser(Scanner);
	Parser.parseProgram();

	ASTNode AstTree = Parser.getFinalASTNode();
	SemanticChecker SemanticCheckerVar = SemanticChecker(AstTree);

	CodeGenerator CodeGenerator(AstTree, SemanticCheckerVar.getSymbolTable(), "UnitTestGeneratedProgram.tm");
	CodeGenerator.writeOutTargetCode();
}

TEST_CASE("Test that the generic print-one.kln prints the value 1 twice when compiled to TM", "[Code Generator]") {
	compileKleinFileToTm("programs/print-one.kln");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(0) == 1);
	REQUIRE(OutputStatements.at(1) == 1);
}

TEST_CASE("Test that print statements reflect their argument when compiled to TM", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(20) 1");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 1);
	REQUIRE(OutputStatements.at(0) == 20);
}

TEST_CASE("Test that the runtime env prints mains return value correctly for any number", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1) 100");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 100);
	REQUIRE(OutputStatements.at(0) == 1);
}


TEST_CASE("Test that print can be called more than once and succesfully prints each time", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1) print(2) 3");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 3);
	REQUIRE(OutputStatements.at(2) == 3);
	REQUIRE(OutputStatements.at(1) == 2);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that simple addition works and computes the correct value", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1) 3 + 3");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 6);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that run on addition works and computes the correct value", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1) 3 + 3 + 10 + 40");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 56);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that addition nodes work and are printed correctly when passed as a paramater to print()", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1 + 40 + 20) 1");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 1);
	REQUIRE(OutputStatements.at(0) == 61);
}
