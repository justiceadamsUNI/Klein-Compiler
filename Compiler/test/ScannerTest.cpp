#include "catch.hpp"
#include"../src/header/Scanner.h"

void assertScannerHasNextTokenOfType(Scanner &Scanner, TokenType TokenType) {
	Token NextToken = Scanner.getNextToken();

	REQUIRE(NextToken.getTokenType() == TokenType);
}

void assertScannerHasNextTokenOfTypeWithValue(Scanner &Scanner, TokenType TokenType, string TokenValue) {
	Token NextToken = Scanner.getNextToken();

	REQUIRE(NextToken.getTokenType() == TokenType);
	REQUIRE(NextToken.getValue() == TokenValue);
}

TEST_CASE("Scanner getNextToken() returns END_OF_FILE Token for empty file", "[Scanner]") {
	string TestFileContents = "         \n\n\n";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, END_OF_FILE, "");
}

TEST_CASE("Scanner getNextToken() returns COMMA Token for ,", "[Scanner]") {
	string TestFileContents = ",";
	Scanner Scanner(TestFileContents, true);
	
	assertScannerHasNextTokenOfTypeWithValue(Scanner, COMMA, "");
}

TEST_CASE("Scanner getNextToken() returns COLON Token for :", "[Scanner]") {
	string TestFileContents = ":";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, COLON, "");
}

TEST_CASE("Scanner getNextToken() returns ARITHMETIC_OPERATOR Token With Correct Value for +", "[Scanner]") {
	string TestFileContents = "+";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, ARITHMETIC_OPERATOR, "+");
}

TEST_CASE("Scanner getNextToken() returns ARITHMETIC_OPERATOR Token With Correct Value for *", "[Scanner]") {
	string TestFileContents = "*";
	Scanner Scanner(TestFileContents, true);
	
	assertScannerHasNextTokenOfTypeWithValue(Scanner, ARITHMETIC_OPERATOR, "*");
}

TEST_CASE("Scanner getNextToken() returns ARITHMETIC_OPERATOR Token With Correct Value for -", "[Scanner]") {
	string TestFileContents = "-";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, ARITHMETIC_OPERATOR, "-");
}

TEST_CASE("Scanner getNextToken() returns ARITHMETIC_OPERATOR Token With Correct Value for /", "[Scanner]") {
	string TestFileContents = "/";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, ARITHMETIC_OPERATOR, "/");
}

TEST_CASE("Scanner getNextToken() returns PARENTHESIS Token With Correct Value for (", "[Scanner]") {
	string TestFileContents = "(";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, PARENTHESIS, "(");
}

TEST_CASE("Scanner getNextToken() returns PARENTHESIS Token With Correct Value for )", "[Scanner]") {
	string TestFileContents = ")";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, PARENTHESIS, ")");
}

TEST_CASE("Scanner getNextToken() Skips Anything Within Comments", "[Scanner]") {
	string TestFileContents = "(*SKIP THIS STUFF *)";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfType(Scanner, END_OF_FILE);
}

TEST_CASE("Scanner getNextToken() Throws Error When Comment Isn't Closed", "[Scanner]") {
	string TestFileContents = "(*SKIP THIS STUFF ";
	Scanner Scanner(TestFileContents, true);
	REQUIRE_THROWS_AS(Scanner.getNextToken(), runtime_error);
}

TEST_CASE("Scanner getNextToken() Ignores Comment And Finds Next Token Normally", "[Scanner]") {
	string TestFileContents = "(*SKIP THIS STUFF*)+";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, ARITHMETIC_OPERATOR, "+");
}

TEST_CASE("Scanner getNextToken() Returns COMPARATOR Token For < With Correct Value", "[Scanner]") {
	string TestFileContents = "<";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, COMPARATOR, "<");
}

TEST_CASE("Scanner getNextToken() Returns COMPARATOR Token For = With Correct Value", "[Scanner]") {
	string TestFileContents = "=";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, COMPARATOR, "=");
}

TEST_CASE("Scanner getNextToken() Returns INTEGER Token For NonZero Integer With Correct Value", "[Scanner]") {
	string TestFileContents = "1234";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "1234");
}

TEST_CASE("Scanner getNextToken() Returns INTEGER Token For Zero Integer With Correct Value", "[Scanner]") {
	string TestFileContents = "000";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "0");
}

TEST_CASE("Scanner getNextToken() Throws Error When NonZero Integer Leads With 0's", "[Scanner]") {
	string TestFileContents = "0001";
	Scanner Scanner(TestFileContents, true);
	REQUIRE_THROWS_AS(Scanner.getNextToken(), runtime_error);
}

TEST_CASE("Scanner getNextToken() Throws Error When Integer Has Invalid Character", "[Scanner]") {
	string TestFileContents = "1234a";
	Scanner Scanner(TestFileContents, true);
	REQUIRE_THROWS_AS(Scanner.getNextToken(), runtime_error);
}

TEST_CASE("Scanner getNextToken() Reurns END_OF_FILE When End Of File Token Is Reached After Other Token", "[Scanner]") {
	string TestFileContents = "123";
	Scanner Scanner(TestFileContents, true);
	
	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "123");
	assertScannerHasNextTokenOfType(Scanner,END_OF_FILE);
}

TEST_CASE("Scanner getNextToken() Returns INTEGER Token Properly when Self Delimiters Or Whitespace Is Found", "[Scanner]") {
	string TestFileContents = "1 2+3-4*5/6=7<8(9)10,11:12";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "1");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "2");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, ARITHMETIC_OPERATOR, "+");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "3");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, ARITHMETIC_OPERATOR, "-");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "4");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, ARITHMETIC_OPERATOR, "*");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "5");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, ARITHMETIC_OPERATOR, "/");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "6");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, COMPARATOR, "=");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "7");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, COMPARATOR, "<");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "8");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, PARENTHESIS, "(");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "9");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, PARENTHESIS, ")");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "10");
	assertScannerHasNextTokenOfType(Scanner, COMMA);
	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "11");
	assertScannerHasNextTokenOfType(Scanner, COLON);
	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "12");
}

TEST_CASE("Scanner Constructor() Throws Error When File Not Found", "[Scanner]") {
	REQUIRE_THROWS_AS(Scanner("fakeFile.kln"), invalid_argument);
}

TEST_CASE("Scanner Constructor() Throws Error When File Not Klein File", "[Scanner]") {
	REQUIRE_THROWS_AS(Scanner("fakeFile.txt"), invalid_argument);
}

TEST_CASE("Scanner Constructor() Sucessfull Opens File And Reads Tokens When Program Is Valid", "[Scanner]") {
	Scanner Scanner("Compiler/test/test_klein_program_1.kln");

	assertScannerHasNextTokenOfTypeWithValue(Scanner, PARENTHESIS, "(");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "34");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, ARITHMETIC_OPERATOR, "+");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "34");
	assertScannerHasNextTokenOfType(Scanner, END_OF_FILE);
}
