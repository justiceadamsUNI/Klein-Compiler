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

TEST_CASE("Test that simple subtraction works and computes the correct value", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1) 3-1");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 2);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that run on subtraction works and computes the correct value", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1) 3-3 -10- 40");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == -50);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that subtraction nodes work and are printed correctly when passed as a paramater to print()", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(20 - 10) 1");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 1);
	REQUIRE(OutputStatements.at(0) == 10);
}

TEST_CASE("Test that simple multiplication works and computes the correct value", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1) 3*9");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 27);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that run on multiplication works and computes the correct value", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1) 3 * 3 * 10*2");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 180);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that multiplication nodes work and are printed correctly when passed as a paramater to print()", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(10*3) 1");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 1);
	REQUIRE(OutputStatements.at(0) == 30);
}

TEST_CASE("Test that simple division works and computes the correct value", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1) 120/10");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 12);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that run on division works and computes the correct value", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1) 120/10/2/2");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 3);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that division nodes work and are printed correctly when passed as a paramater to print()", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(100/4) 1");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 1);
	REQUIRE(OutputStatements.at(0) == 25);
}

TEST_CASE("Test arithmetic expression and ensure that the order of operations are done correctly", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer 10*100+30-60/20*3");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 1021);
}

TEST_CASE("Test arithmetic expression and ensure that the order of operations are done correctly with parenthesis", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer (10*100+30-60/(20*3))*2");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 2058);
}

TEST_CASE("Test simple addition which contains negated element", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer 10 +-4");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 6);
}

TEST_CASE("Test simple subtraction which contains negated element", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer 10 --4");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 14);
}

TEST_CASE("Test simple multiplication which contains negated element", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer 10*-4");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == -40);
}

TEST_CASE("Test simple division which contains negated element", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer 10/-2");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == -5);
}

TEST_CASE("Test arithmetic expression with negated elements", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer (10*-100+30-60/(20*3))*-2");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 1942);
}

TEST_CASE("Test boolean literal prints one for the value 'true'", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer true");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test boolean literal prints zero for the value 'false'", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer false");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 0);
}

TEST_CASE("Test that we can pass boolean literals to print as arguments", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(true) print((false)) false");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 3);
	REQUIRE(OutputStatements.at(2) == 0);
	REQUIRE(OutputStatements.at(1) == 0);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that simple AND operation works and computes the correct value when doing 'true and true'", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean true and true");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that simple AND operation works and computes the correct value when doing 'true and false'", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean true and false");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 0);
}

TEST_CASE("Test that simple AND operation works and computes the correct value when doing 'false and false'", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean false and false");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 0);
}

TEST_CASE("Test that run on AND operations work and are computed correctly", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean print((true and true and true and true and true) and false) true and true and true and true and true");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 1);
	REQUIRE(OutputStatements.at(0) == 0);
}
