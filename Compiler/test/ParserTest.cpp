#include "catch.hpp"
#include "../src/header/Parser.h"

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps INTEGER to INTEGER_LITERAL", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(INTEGER, "123");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == Parser::INTEGER_LITERAL);
}