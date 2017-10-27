#include "catch.hpp"
#include "../src/header/Parser.h"
#include "../src/header/SemanticChecker.h"

// This test file needs to be expanded
TEST_CASE("Semantic Checker Finds No Errors Or Warnings For Valid Program", "[Scanner]") {
	Scanner Scanner("programs/fractionAddition.kln");
	Parser Parser(Scanner);

	Parser.parseProgram();
	ASTNode AST = Parser.getFinalASTNode();
	SemanticChecker SemanticCheckerVar = SemanticChecker(AST);

	REQUIRE(SemanticCheckerVar.getErrors().empty());
	REQUIRE(SemanticCheckerVar.getWarnings().empty());
}