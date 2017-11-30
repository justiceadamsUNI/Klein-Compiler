#include "catch.hpp"
#include "tm-cli-go.hpp"
#include "../src/header/Parser.h"
#include "../src/header/SemanticChecker.h"
#include "../src/header/CodeGenerator.h"
#include <array>

vector<int> callTmProgramWithArgumentsAndGetOutput(int argc, char* argv[]) {
	// Mimic the way that TM takes in command line args.
	// remember to always have an arg list of 2 or more.
	TmCliGoTest TmCli = TmCliGoTest();
	TmCli.main(argc, argv);

	return TmCli.getOutputStatemnts();
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

TEST_CASE("Test that print statements reflect their argument when compiled to TM", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(20) 1");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 1);
	REQUIRE(OutputStatements.at(0) == 20);
}

TEST_CASE("Test that the runtime env prints mains return value correctly for any number", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1) 100");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 100);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that print can be called more than once and succesfully prints each time", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1) print(2) 3");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 3);
	REQUIRE(OutputStatements.at(2) == 3);
	REQUIRE(OutputStatements.at(1) == 2);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that simple addition works and computes the correct value", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1) 3 + 3");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 6);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that run on addition works and computes the correct value", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1) 3 + 3 + 10 + 40");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 56);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that addition nodes work and are printed correctly when passed as a paramater to print()", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1 + 40 + 20) 1");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 1);
	REQUIRE(OutputStatements.at(0) == 61);
}

TEST_CASE("Test that simple subtraction works and computes the correct value", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1) 3-1");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 2);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that run on subtraction works and computes the correct value", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1) 3-3 -10- 40");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == -50);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that subtraction nodes work and are printed correctly when passed as a paramater to print()", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(20 - 10) 1");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 1);
	REQUIRE(OutputStatements.at(0) == 10);
}

TEST_CASE("Test that simple multiplication works and computes the correct value", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1) 3*9");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 27);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that run on multiplication works and computes the correct value", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1) 3 * 3 * 10*2");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 180);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that multiplication nodes work and are printed correctly when passed as a paramater to print()", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(10*3) 1");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 1);
	REQUIRE(OutputStatements.at(0) == 30);
}

TEST_CASE("Test that simple division works and computes the correct value", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1) 120/10");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 12);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that run on division works and computes the correct value", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(1) 120/10/2/2");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 3);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that division nodes work and are printed correctly when passed as a paramater to print()", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(100/4) 1");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 1);
	REQUIRE(OutputStatements.at(0) == 25);
}

TEST_CASE("Test arithmetic expression and ensure that the order of operations are done correctly", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer 10*100+30-60/20*3");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 1021);
}

TEST_CASE("Test arithmetic expression and ensure that the order of operations are done correctly with parenthesis", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer (10*100+30-60/(20*3))*2");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 2058);
}

TEST_CASE("Test simple addition which contains negated element", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer 10 +-4");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 6);
}

TEST_CASE("Test simple subtraction which contains negated element", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer 10 --4");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 14);
}

TEST_CASE("Test simple multiplication which contains negated element", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer 10*-4");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == -40);
}

TEST_CASE("Test simple division which contains negated element", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer 10/-2");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == -5);
}

TEST_CASE("Test arithmetic expression with negated elements", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer (10*-100+30-60/(20*3))*-2");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 1942);
}

TEST_CASE("Test boolean literal prints one for the value 'true'", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer true");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test boolean literal prints zero for the value 'false'", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer false");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 0);
}

TEST_CASE("Test that we can pass boolean literals to print as arguments", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : integer print(true) print((false)) false");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 3);
	REQUIRE(OutputStatements.at(2) == 0);
	REQUIRE(OutputStatements.at(1) == 0);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that simple AND operation works and computes the correct value when doing 'true and true'", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean true and true");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that simple AND operation works and computes the correct value when doing 'true and false'", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean true and false");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 0);
}

TEST_CASE("Test that simple AND operation works and computes the correct value when doing 'false and false'", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean false and false");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 0);
}

TEST_CASE("Test that run on AND operations work and are computed correctly", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean print((true and true and true and true and true) and false) true and true and true and true and true");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 1);
	REQUIRE(OutputStatements.at(0) == 0);
}

TEST_CASE("Test that simple OR operation works and computes the correct value when doing 'true or true'", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean true or true");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that simple OR operation works and computes the correct value when doing 'true or false'", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean true or false");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that simple OR operation works and computes the correct value when doing 'false or false'", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean false or false");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 0);
}

TEST_CASE("Test that run on OR operations work and are computed correctly", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean print((true or true or true or true or true) or false) false or false or false or false or false");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 0);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that boolean negation works for true", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean not true");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 0);
}

TEST_CASE("Test that boolean negation works for false", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean not false");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that boolean negation works with run on scenarios and respects order of operations (negations first)", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean print(not true and false or false) not false or not true");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 1);
	REQUIRE(OutputStatements.at(0) == 0);
}

TEST_CASE("Test that boolean negation works with parenthesised expressions", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean not (false or not true)");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that < works with correctly when expression is true", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean 2 < 3");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that < works with correctly when expression is false", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean 20 < 3");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 0);
}

TEST_CASE("Test that < works with correctly when used in conjunction with boolean statements", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean print((20 < 3) or (50<100)) false");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 0);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that single command line variable is stored and accessed correctly", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main(a: integer) : integer print(a) 1");

	char* argv[3] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm", "10" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(3, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 1);
	REQUIRE(OutputStatements.at(0) == 10);
}

TEST_CASE("Test that multiple command line variables are stored and accessed correctly", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main(a: integer, b: integer, c: integer, d: integer) : integer print(a) print(b) print(c) d");

	char* argv[6] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm", "10", "20", "30", "40"};
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(6, argv);

	REQUIRE(OutputStatements.size() == 4);
	REQUIRE(OutputStatements.at(3) == 40);
	REQUIRE(OutputStatements.at(2) == 30);
	REQUIRE(OutputStatements.at(1) == 20);
	REQUIRE(OutputStatements.at(0) == 10);
}

TEST_CASE("Test that = works correctly when expression is true", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean print(false = false) true = true");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 2);
	REQUIRE(OutputStatements.at(1) == 1);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that = works with correctly when expression is false", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean false = true");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 0);
}

TEST_CASE("Test that = works with correctly when used in conjunction with boolean statements", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main() : boolean ((5 < 10) or (20 < 10)) = true");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 1);
}

TEST_CASE("Test that a simple function call works ", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main(): integer foo() + 1 function foo(): integer 10 + 20");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 31);
}

TEST_CASE("Test that a simple function call works when passing paramaters ", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main():integer foo(30, 10) + 100 function foo(a: integer, b: integer): integer a - b");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 120);
}

TEST_CASE("Test that a simple function call works when passing paramaters and keeps paramater order", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main():integer foo(10, 30) + 100 function foo(a: integer, b: integer): integer a - b");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 80);
}


TEST_CASE("Test that nested function calls operate correctly", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main():integer foo(100, foo(30, 20)) + 5 function foo(a: integer, b: integer): integer a - b");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 95);
}

TEST_CASE("Test that simple if statement works correctly when the if evaluation is true", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main():integer if 10 < 20 then 30 else -100");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm"};
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == 30);
}

TEST_CASE("Test that simple if statement works correctly when the if evaluation is false", "[Code Generator]") {
	compileKleinFileToTmWithoutOpeningKleinFile("function main():integer if 10 < -20 then 30 else -100");

	char* argv[2] = { "tm-cli-go.exe", "UnitTestGeneratedProgram.tm" };
	vector<int> OutputStatements = callTmProgramWithArgumentsAndGetOutput(2, argv);

	REQUIRE(OutputStatements.size() == 1);
	REQUIRE(OutputStatements.at(0) == -100);
}