#include "catch.hpp"
#include "../src/header/Parser.h"

Parser createParserWithBodyContaining(string text) {
	Scanner Scanner("function main(x:integer):integer " + text, true);

	return Parser(Scanner);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps INTEGER to INTEGER_LITERAL", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(INTEGER, "123");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == INTEGER_LITERAL);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps Int DATA_TYPE to INTEGER_DATA_TYPE", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(DATA_TYPE, "integer");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == INTEGER_DATA_TYPE);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps bool DATA_TYPE to BOOLEAN_DATA_TYPE", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(DATA_TYPE, "boolean");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == BOOLEAN_DATA_TYPE);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps boolean literal to BOOLEAN_LITERAL", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(BOOLEAN, "true");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == BOOLEAN_LITERAL);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps  + ARITHMETIC_OPERATOR to PLUS_OPERATOR", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(ARITHMETIC_OPERATOR, "+");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == PLUS_OPERATOR);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps  - ARITHMETIC_OPERATOR to MINUS_OPERATOR", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(ARITHMETIC_OPERATOR, "-");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == MINUS_OPERATOR);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps  / ARITHMETIC_OPERATOR to DIVIDES_OPERATOR", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(ARITHMETIC_OPERATOR, "/");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == DIVIDES_OPERATOR);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps  * ARITHMETIC_OPERATOR to MULTIPLY_OPERATOR", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(ARITHMETIC_OPERATOR, "*");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == MULTIPLY_OPERATOR);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps PRIMITIVE_KEYWORD to FUNCTION", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(PRIMITIVE_KEYWORD, "function");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == FUNCTION);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps PARENTHESIS of ( to LEFT_PAREN", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(PARENTHESIS, "(");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == LEFT_PAREN);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps PARENTHESIS of ) to RIGHT_PAREN", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(PARENTHESIS, ")");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == RIGHT_PAREN);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps COLON to COLON_LITERAL", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(COLON, "");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == COLON_LITERAL);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps COMMA to COMMA_LITERAL", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(COMMA, "");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == COMMA_LITERAL);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps IDENTIFIER that's not print to IDENTIFIER", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(IDENTIFIER, "identifier_12");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == IDENTIFIER_LITERAL);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps IDENTIFIER of print to PRINT", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(IDENTIFIER, "print");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == PRINT);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps COMPARATOR of < to LESS_THAN_OPERATOR", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(COMPARATOR, "<");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == LESS_THAN_OPERATOR);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps COMPARATOR of = to EQUAL_SIGN", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(COMPARATOR, "=");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == EQUAL_SIGN);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps LOGICAL_OPERATOR of or to OR", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(LOGICIAL_OPERATOR, "or");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == OR);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps LOGICAL_OPERATOR of and to AND", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(LOGICIAL_OPERATOR, "and");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == AND);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps LOGICAL_OPERATOR of not to NOT", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(LOGICIAL_OPERATOR, "not");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == NOT);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps CONDITIONAL of if to IF", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(CONDITIONAL, "if");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == IF);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps CONDITIONAL of then to THEN", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(CONDITIONAL, "then");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == THEN);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps CONDITIONAL of else to ELSE", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(CONDITIONAL, "else");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == ELSE);
}

TEST_CASE("Parser mapFromScannerTokenToStackValue() Correctly maps END_OF_FILE to END_OF_STREAM", "[Parser]") {
	Scanner Scanner("", true);
	Parser Parser(Scanner);
	Token Token(END_OF_FILE, "");

	REQUIRE(Parser.mapFromScannerTokenToStackValue(Token) == END_OF_STREAM);
}

TEST_CASE("Parser doesnt find the rule from ParseTable.find()", "[Parser]") {
	Scanner Scanner("parser_failing_program_no_return_value.kln", true);
	Parser Parser(Scanner);
	
	REQUIRE_THROWS_AS(Parser.parseProgram(), runtime_error);
}

TEST_CASE("Parser doesnt throw error for program containing run on boolean operators()", "[Parser]") {
	Parser Parser = createParserWithBodyContaining("if ((diff = 7) or (diff = 0) or (diff = -7) or (diff = -14)) then true else false");

	REQUIRE(Parser.isProgramValid() == true);
}