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

TEST_CASE("Parser throws error for failing program with no return value", "[Parser]") {
	Scanner Scanner("programs/parser_failing_program_no_return_value.kln");
	Parser Parser(Scanner);
	
	REQUIRE_THROWS_AS(Parser.parseProgram(), runtime_error);
}

TEST_CASE("Parser throws error for failing program with that misuses a colon", "[Parser]") {
	Scanner Scanner("programs/parser_failing_program_misuse_colon.kln");
	Parser Parser(Scanner);

	REQUIRE_THROWS_AS(Parser.parseProgram(), runtime_error);
}

TEST_CASE("Parser throws error for failing with a non-finished function declaration", "[Parser]") {
	Scanner Scanner("programs/parser_failing_program_new_function.kln");
	Parser Parser(Scanner);

	REQUIRE_THROWS_AS(Parser.parseProgram(), runtime_error);
}

TEST_CASE("Parser throws error for failing program that spells boolean wrong when defining a data type", "[Parser]") {
	Scanner Scanner("programs/parser_failing_program_spelling_error_wrong_data_type.kln");
	Parser Parser(Scanner);

	REQUIRE_THROWS_AS(Parser.parseProgram(), runtime_error);
}

TEST_CASE("Parser throws error for failing program that uses an int data type instead of integer", "[Parser]") {
	Scanner Scanner("programs/parser_failing_program_wrong_data_type.kln");
	Parser Parser(Scanner);

	REQUIRE_THROWS_AS(Parser.parseProgram(), runtime_error);
}

TEST_CASE("Parser determines program is valid for program containing run on boolean operators", "[Parser]") {
	Parser Parser = createParserWithBodyContaining("if ((diff = 7) or (diff = 0) or (diff = -7) or (diff = -14)) then true else false");

	REQUIRE(Parser.isProgramValid() == true);
}

TEST_CASE("Parser determines program is valid for program containing multiple parenthesised terms", "[Parser]") {
	Parser Parser = createParserWithBodyContaining("(x / 10 + (mod(x, 10) * pow(10, log10(x))))");

	REQUIRE(Parser.isProgramValid() == true);
}

TEST_CASE("Parser determines program is valid for program containing nested if statements", "[Parser]") {
	string nestedIfs = " if (diff = 7) then if diff < 14 then true else false else if diff < 14 then false else main(diff)";
	Parser Parser = createParserWithBodyContaining(nestedIfs);

	REQUIRE(Parser.isProgramValid() == true);
}

TEST_CASE("Parser determines program is valid for program containing run on arithmetic operators", "[Parser]") {
	Parser Parser = createParserWithBodyContaining(" 8 - 9/2 * diff * wallingford + is + cool  + 9 * 8 / 10");

	REQUIRE(Parser.isProgramValid() == true);
}

TEST_CASE("Parser determines program is valid for program containing function calls within functions", "[Parser]") {
	Parser Parser = createParserWithBodyContaining("not hasDivisorFrom(2, n)");

	REQUIRE(Parser.isProgramValid() == true);
}

TEST_CASE("Parser determines program is valid for program containing multiple print statements", "[Parser]") {
	Parser Parser = createParserWithBodyContaining("print(number) print(reverse(number)) 1");

	REQUIRE(Parser.isProgramValid() == true);
}

TEST_CASE("Parser determines program is valid for program containing comments within the body", "[Parser]") {
	Parser Parser = createParserWithBodyContaining("gcd(a,b) (*comment*) + 1445");

	REQUIRE(Parser.isProgramValid() == true);
}

TEST_CASE("Parser determines program is Invalid for program containing function with no return", "[Parser]") {
	Parser Parser = createParserWithBodyContaining("print(wheres_da_return)");

	REQUIRE(Parser.isProgramValid() == false);
}

TEST_CASE("Parser determines program is Invalid for program containing non-closing parenthesis", "[Parser]") {
	Parser Parser = createParserWithBodyContaining("((test + 98/90-(1))");

	REQUIRE(Parser.isProgramValid() == false);
}

TEST_CASE("Parser determines program is Invalid for program containing invalid paramaters", "[Parser]") {
	Scanner Scanner("function remainder(a : integer, b : )", true);
	Parser Parser(Scanner);

	REQUIRE(Parser.isProgramValid() == false);
}

TEST_CASE("Parser determines program is Invalid for program containing if statement without then", "[Parser]") {
	Parser Parser = createParserWithBodyContaining("if d > 0 45 else 66");

	REQUIRE(Parser.isProgramValid() == false);
}

TEST_CASE("Parser determines program is Invalid for program containing if statement without else", "[Parser]") {
	Parser Parser = createParserWithBodyContaining("if d > 0 45 then 6");

	REQUIRE(Parser.isProgramValid() == false);
}

TEST_CASE("Parser determines program is Invalid for program containing boolean operators on single values", "[Parser]") {
	Parser Parser = createParserWithBodyContaining("65 or ()");

	REQUIRE(Parser.isProgramValid() == false);
}

TEST_CASE("Parser determines program is Invalid for program containing arithmetic operators on single values", "[Parser]") {
	Parser Parser = createParserWithBodyContaining("65 + ()");

	REQUIRE(Parser.isProgramValid() == false);
}

TEST_CASE("Parser determines program is Invalid for function with no return type", "[Parser]") {
	Scanner Scanner("function main(x : integer) : horner(x, 3, 0)", true);
	Parser Parser(Scanner);

	REQUIRE(Parser.isProgramValid() == false);
}

TEST_CASE("Parser determines program is Invalid for ", "[Parser]") {
	Scanner Scanner("function main(x : integer) : horner(x, 3, 0)", true);
	Parser Parser(Scanner);

	REQUIRE(Parser.isProgramValid() == false);
}

TEST_CASE("Parser determines program is Invalid for function that returns two types (not possible)", "[Parser]") {
	Scanner Scanner("function main(x : integer) : integer or boolean horner(x, 3, 0)", true);
	Parser Parser(Scanner);

	REQUIRE(Parser.isProgramValid() == false);
}