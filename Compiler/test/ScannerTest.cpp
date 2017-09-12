#include "catch.hpp"
#include"../src/header/Scanner.h"

TEST_CASE("Scanner getNextToken() returns END_OF_FILE Token for empty file", "[Scanner]") {
	string TestFileContents = "         \n\n\n";
	Scanner Scanner(TestFileContents, true);
	Token NextToken = Scanner.getNextToken();

	REQUIRE(NextToken.getTokenType() == END_OF_FILE);
	REQUIRE(NextToken.getValue() == "");
}

TEST_CASE("Scanner getNextToken() returns COMMA Token for ,", "[Scanner]") {
	string TestFileContents = ",";
	Scanner Scanner(TestFileContents, true);
	Token NextToken = Scanner.getNextToken();

	REQUIRE(NextToken.getTokenType() == COMMA);
	REQUIRE(NextToken.getValue() == "");
}

TEST_CASE("Scanner getNextToken() returns COLON Token for :", "[Scanner]") {
	string TestFileContents = ":";
	Scanner Scanner(TestFileContents, true);
	Token NextToken = Scanner.getNextToken();

	REQUIRE(NextToken.getTokenType() == COLON);
	REQUIRE(NextToken.getValue() == "");
}

TEST_CASE("Scanner getNextToken() returns ARITHMETIC_OPERATOR Token With Correct Value for +", "[Scanner]") {
	string TestFileContents = "+";
	Scanner Scanner(TestFileContents, true);
	Token NextToken = Scanner.getNextToken();

	REQUIRE(NextToken.getTokenType() == ARITHMETIC_OPERATOR);
	REQUIRE(NextToken.getValue() == "+");
}

TEST_CASE("Scanner getNextToken() returns ARITHMETIC_OPERATOR Token With Correct Value for *", "[Scanner]") {
	string TestFileContents = "*";
	Scanner Scanner(TestFileContents, true);
	Token NextToken = Scanner.getNextToken();

	REQUIRE(NextToken.getTokenType() == ARITHMETIC_OPERATOR);
	REQUIRE(NextToken.getValue() == "*");
}

TEST_CASE("Scanner getNextToken() returns ARITHMETIC_OPERATOR Token With Correct Value for -", "[Scanner]") {
	string TestFileContents = "-";
	Scanner Scanner(TestFileContents, true);
	Token NextToken = Scanner.getNextToken();

	REQUIRE(NextToken.getTokenType() == ARITHMETIC_OPERATOR);
	REQUIRE(NextToken.getValue() == "-");
}

TEST_CASE("Scanner getNextToken() returns ARITHMETIC_OPERATOR Token With Correct Value for /", "[Scanner]") {
	string TestFileContents = "/";
	Scanner Scanner(TestFileContents, true);
	Token NextToken = Scanner.getNextToken();

	REQUIRE(NextToken.getTokenType() == ARITHMETIC_OPERATOR);
	REQUIRE(NextToken.getValue() == "/");
}

TEST_CASE("Scanner getNextToken() returns PARENTHESIS Token With Correct Value for (", "[Scanner]") {
	string TestFileContents = "(";
	Scanner Scanner(TestFileContents, true);
	Token NextToken = Scanner.getNextToken();

	REQUIRE(NextToken.getTokenType() == PARENTHESIS);
	REQUIRE(NextToken.getValue() == "(");
}

TEST_CASE("Scanner getNextToken() returns PARENTHESIS Token With Correct Value for )", "[Scanner]") {
	string TestFileContents = ")";
	Scanner Scanner(TestFileContents, true);
	Token NextToken = Scanner.getNextToken();

	REQUIRE(NextToken.getTokenType() == PARENTHESIS);
	REQUIRE(NextToken.getValue() == ")");
}

TEST_CASE("Scanner getNextToken() Skips Anything Within Comments", "[Scanner]") {
	string TestFileContents = "(*SKIP THIS STUFF *)";
	Scanner Scanner(TestFileContents, true);
	Token NextToken = Scanner.getNextToken();

	REQUIRE(NextToken.getTokenType() == END_OF_FILE);
	REQUIRE(NextToken.getValue() == "");
}

TEST_CASE("Scanner getNextToken() Throws Error When Comment Isn't Closed", "[Scanner]") {
	string TestFileContents = "(*SKIP THIS STUFF ";
	Scanner Scanner(TestFileContents, true);
	REQUIRE_THROWS_AS(Scanner.getNextToken(), runtime_error);
}

TEST_CASE("Scanner getNextToken() Ignores Comment And Finds Next Token Normally", "[Scanner]") {
	string TestFileContents = "(*SKIP THIS STUFF*)+";
	Scanner Scanner(TestFileContents, true);
	Token NextToken = Scanner.getNextToken();

	REQUIRE(NextToken.getTokenType() == ARITHMETIC_OPERATOR);
	REQUIRE(NextToken.getValue() == "+");
}

TEST_CASE("Scanner getNextToken() Returns COMPARATOR Token For < With Correct Value", "[Scanner]") {
	string TestFileContents = "<";
	Scanner Scanner(TestFileContents, true);
	Token NextToken = Scanner.getNextToken();

	REQUIRE(NextToken.getTokenType() == COMPARATOR);
	REQUIRE(NextToken.getValue() == "<");
}

TEST_CASE("Scanner getNextToken() Returns COMPARATOR Token For = With Correct Value", "[Scanner]") {
	string TestFileContents = "=";
	Scanner Scanner(TestFileContents, true);
	Token NextToken = Scanner.getNextToken();

	REQUIRE(NextToken.getTokenType() == COMPARATOR);
	REQUIRE(NextToken.getValue() == "=");
}

TEST_CASE("Scanner getNextToken() Returns INTEGER Token For NonZero Integer With Correct Value", "[Scanner]") {
	string TestFileContents = "1234";
	Scanner Scanner(TestFileContents, true);
	Token NextToken = Scanner.getNextToken();

	REQUIRE(NextToken.getTokenType() == INTEGER);
	REQUIRE(NextToken.getValue() == "1234");
}

TEST_CASE("Scanner getNextToken() Returns INTEGER Token For Zero Integer With Correct Value", "[Scanner]") {
	string TestFileContents = "000";
	Scanner Scanner(TestFileContents, true);
	Token NextToken = Scanner.getNextToken();

	REQUIRE(NextToken.getTokenType() == INTEGER);
	REQUIRE(NextToken.getValue() == "0");
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
	Token NextToken = Scanner.getNextToken();

	REQUIRE(NextToken.getTokenType() == INTEGER);
	REQUIRE(NextToken.getValue() == "123");

	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == END_OF_FILE);
}

TEST_CASE("Scanner getNextToken() Returns INTEGER Token Properly when Self Delimiters Or Whitespace Is Found", "[Scanner]") {
	string TestFileContents = "1 2+3-4*5/6=7<8(9)10,11:12";
	Scanner Scanner(TestFileContents, true);
	Token NextToken = Scanner.getNextToken();

	REQUIRE(NextToken.getTokenType() == INTEGER);
	REQUIRE(NextToken.getValue() == "1");
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == INTEGER);
	REQUIRE(NextToken.getValue() == "2");
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == ARITHMETIC_OPERATOR);
	REQUIRE(NextToken.getValue() == "+");
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == INTEGER);
	REQUIRE(NextToken.getValue() == "3");
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == ARITHMETIC_OPERATOR);
	REQUIRE(NextToken.getValue() == "-");
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == INTEGER);
	REQUIRE(NextToken.getValue() == "4");
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == ARITHMETIC_OPERATOR);
	REQUIRE(NextToken.getValue() == "*");
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == INTEGER);
	REQUIRE(NextToken.getValue() == "5");
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == ARITHMETIC_OPERATOR);
	REQUIRE(NextToken.getValue() == "/");
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == INTEGER);
	REQUIRE(NextToken.getValue() == "6");
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == COMPARATOR);
	REQUIRE(NextToken.getValue() == "=");
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == INTEGER);
	REQUIRE(NextToken.getValue() == "7");
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == COMPARATOR);
	REQUIRE(NextToken.getValue() == "<");
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == INTEGER);
	REQUIRE(NextToken.getValue() == "8");
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == PARENTHESIS);
	REQUIRE(NextToken.getValue() == "(");
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == INTEGER);
	REQUIRE(NextToken.getValue() == "9");
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == PARENTHESIS);
	REQUIRE(NextToken.getValue() == ")");
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == INTEGER);
	REQUIRE(NextToken.getValue() == "10");
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == COMMA);
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == INTEGER);
	REQUIRE(NextToken.getValue() == "11");
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == COLON);
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == INTEGER);
	REQUIRE(NextToken.getValue() == "12");
}

TEST_CASE("Scanner Constructor() Throws Error When File Not Found", "[Scanner]") {
	REQUIRE_THROWS_AS(Scanner("fakeFile.kln"), invalid_argument);
}

TEST_CASE("Scanner Constructor() Throws Error When File Not Klein File", "[Scanner]") {
	REQUIRE_THROWS_AS(Scanner("fakeFile.txt"), invalid_argument);
}

TEST_CASE("Scanner Constructor() Sucessfull Reads Tokens When Program Is Valid", "[Scanner]") {
	Scanner Scanner("Compiler/test/test_klein_program_1.kln");
	Token NextToken = Scanner.getNextToken();

	REQUIRE(NextToken.getTokenType() == PARENTHESIS);
	REQUIRE(NextToken.getValue() == "(");
	
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == INTEGER);
	REQUIRE(NextToken.getValue() == "34");
	
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == ARITHMETIC_OPERATOR);
	REQUIRE(NextToken.getValue() == "+");
	
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == INTEGER);
	REQUIRE(NextToken.getValue() == "34");
	
	NextToken = Scanner.getNextToken();
	REQUIRE(NextToken.getTokenType() == END_OF_FILE);
}
