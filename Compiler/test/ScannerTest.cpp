#include "catch.hpp"
#include"../src/header/Scanner.h"

void assertScannerHasNextTokenOfType(Scanner &Scanner, TokenType TokenType) {
	Token NextToken = Scanner.next();

	REQUIRE(NextToken.getTokenType() == TokenType);
}

void assertScannerHasNextTokenOfTypeWithValue(Scanner &Scanner, TokenType TokenType, string TokenValue) {
	Token NextToken = Scanner.next();

	REQUIRE(NextToken.getTokenType() == TokenType);
	REQUIRE(NextToken.getValue() == TokenValue);
}

void assertScannerPeeksNextTokenOfTypeWithValue(Scanner &Scanner, TokenType TokenType, string TokenValue) {
	Token NextToken = Scanner.peek();

	REQUIRE(NextToken.getTokenType() == TokenType);
	REQUIRE(NextToken.getValue() == TokenValue);
}

TEST_CASE("Scanner next() returns END_OF_FILE Token for empty file", "[Scanner]") {
	string TestFileContents = "         \n\n\n";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, END_OF_FILE, "");
}

TEST_CASE("Scanner next() returns COMMA Token for ,", "[Scanner]") {
	string TestFileContents = ",";
	Scanner Scanner(TestFileContents, true);
	
	assertScannerHasNextTokenOfTypeWithValue(Scanner, COMMA, "");
}

TEST_CASE("Scanner next() returns COLON Token for :", "[Scanner]") {
	string TestFileContents = ":";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, COLON, "");
}

TEST_CASE("Scanner next() returns ARITHMETIC_OPERATOR Token With Correct Value for +", "[Scanner]") {
	string TestFileContents = "+";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, ARITHMETIC_OPERATOR, "+");
}

TEST_CASE("Scanner next() returns ARITHMETIC_OPERATOR Token With Correct Value for *", "[Scanner]") {
	string TestFileContents = "*";
	Scanner Scanner(TestFileContents, true);
	
	assertScannerHasNextTokenOfTypeWithValue(Scanner, ARITHMETIC_OPERATOR, "*");
}

TEST_CASE("Scanner next() returns ARITHMETIC_OPERATOR Token With Correct Value for -", "[Scanner]") {
	string TestFileContents = "-";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, ARITHMETIC_OPERATOR, "-");
}

TEST_CASE("Scanner next() returns ARITHMETIC_OPERATOR Token With Correct Value for /", "[Scanner]") {
	string TestFileContents = "/";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, ARITHMETIC_OPERATOR, "/");
}

TEST_CASE("Scanner next() returns PARENTHESIS Token With Correct Value for (", "[Scanner]") {
	string TestFileContents = "(";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, PARENTHESIS, "(");
}

TEST_CASE("Scanner next() returns PARENTHESIS Tokens correctly directly after keyword.", "[Scanner]") {
	string TestFileContents = "function()";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, PRIMITIVE_KEYWORD, "function");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, PARENTHESIS, "(");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, PARENTHESIS, ")");
	assertScannerHasNextTokenOfType(Scanner, END_OF_FILE);
}

TEST_CASE("Scanner next() returns PARENTHESIS Tokens correctly directly after identifier.", "[Scanner]") {
	string TestFileContents = "variable()";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "variable");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, PARENTHESIS, "(");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, PARENTHESIS, ")");
	assertScannerHasNextTokenOfType(Scanner, END_OF_FILE);
}

TEST_CASE("Scanner next() returns PARENTHESIS Token With Correct Value for )", "[Scanner]") {
	string TestFileContents = ")";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, PARENTHESIS, ")");
}

TEST_CASE("Scanner next() Skips Anything Within Comments", "[Scanner]") {
	string TestFileContents = "(*SKIP THIS STUFF *)";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfType(Scanner, END_OF_FILE);
}

TEST_CASE("Scanner next() Throws Error When Comment Isn't Closed", "[Scanner]") {
	string TestFileContents = "(*SKIP THIS STUFF ";
	Scanner Scanner(TestFileContents, true);
	REQUIRE_THROWS_AS(Scanner.next(), runtime_error);
}

TEST_CASE("Scanner next() Ignores Comment And Finds Next Token Normally", "[Scanner]") {
	string TestFileContents = "(*SKIP THIS STUFF*)+";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, ARITHMETIC_OPERATOR, "+");
}

TEST_CASE("Scanner next() Returns COMPARATOR Token For < With Correct Value", "[Scanner]") {
	string TestFileContents = "<";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, COMPARATOR, "<");
}

TEST_CASE("Scanner next() Returns COMPARATOR Token For = With Correct Value", "[Scanner]") {
	string TestFileContents = "=";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, COMPARATOR, "=");
}

TEST_CASE("Scanner next() Returns INTEGER Token For NonZero Integer With Correct Value", "[Scanner]") {
	string TestFileContents = "1234";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "1234");
}

TEST_CASE("Scanner next() Returns INTEGER Token For Zero Integer With Correct Value", "[Scanner]") {
	string TestFileContents = "000";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "0");
}

TEST_CASE("Scanner next() Throws Error When NonZero Integer Leads With 0's", "[Scanner]") {
	string TestFileContents = "0001";
	Scanner Scanner(TestFileContents, true);
	REQUIRE_THROWS_AS(Scanner.next(), runtime_error);
}

TEST_CASE("Scanner next() Throws Error When Integer Has Invalid Character", "[Scanner]") {
	string TestFileContents = "1234a";
	Scanner Scanner(TestFileContents, true);
	REQUIRE_THROWS_AS(Scanner.next(), runtime_error);
}

TEST_CASE("Scanner next() Returns END_OF_FILE When End Of File Token Is Reached After Other Token", "[Scanner]") {
	string TestFileContents = "123";
	Scanner Scanner(TestFileContents, true);
	
	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "123");
	assertScannerHasNextTokenOfType(Scanner,END_OF_FILE);
}

TEST_CASE("Scanner next() Returns INTEGER Token Properly when Self Delimiters Or Whitespace Is Found", "[Scanner]") {
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

TEST_CASE("Scanner next() Returns PRIMITIVE_KEYWORD for 'function' with the Correct Value", "[Scanner]") {
	string TestFileContents = "function";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, PRIMITIVE_KEYWORD, "function");
	assertScannerHasNextTokenOfType(Scanner, END_OF_FILE);
}

TEST_CASE("Scanner next() Returns IDENTIFIER for 'Function' with the Correct Value (AND NOT A PRIMITIVE KEYWORD)", "[Scanner]") {
	string TestFileContents = "Function";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "Function");
}

TEST_CASE("Scanner next() Returns IDENTIFIER for 'main' with the Correct Value", "[Scanner]") {
	string TestFileContents = "main";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "main");
}

TEST_CASE("Scanner next() Returns IDENTIFIER for 'Main' with the Correct Value (AND NOT A PRIMITIVE KEYWORD)", "[Scanner]") {
	string TestFileContents = "Main";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "Main");
}

TEST_CASE("Scanner next() Returns IDENTIFIER for 'print' with the Correct Value", "[Scanner]") {
	string TestFileContents = "print";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "print");
}

TEST_CASE("Scanner next() Returns IDENTIFIER for 'Print' with the Correct Value (AND NOT A PRIMITIVE KEYWORD)", "[Scanner]") {
	string TestFileContents = "Print";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "Print");
}

TEST_CASE("Scanner next() Returns LOGICIAL_OPERATOR for 'and' with the Correct Value", "[Scanner]") {
	string TestFileContents = "and";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, LOGICIAL_OPERATOR, "and");
}

TEST_CASE("Scanner next() Returns IDENTIFIER for 'And' with the Correct Value (LOGICIAL_OPERATORD)", "[Scanner]") {
	string TestFileContents = "And";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "And");
}

TEST_CASE("Scanner next() Returns LOGICIAL_OPERATOR for 'or' with the Correct Value", "[Scanner]") {
	string TestFileContents = "or";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, LOGICIAL_OPERATOR, "or");
}

TEST_CASE("Scanner next() Returns IDENTIFIER for 'Or' with the Correct Value (LOGICIAL_OPERATOR)", "[Scanner]") {
	string TestFileContents = "Or";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "Or");
}

TEST_CASE("Scanner next() Returns LOGICIAL_OPERATOR for 'not' with the Correct Value", "[Scanner]") {
	string TestFileContents = "not";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, LOGICIAL_OPERATOR, "not");
}

TEST_CASE("Scanner next() Returns IDENTIFIER for 'Not' with the Correct Value (LOGICIAL_OPERATORD)", "[Scanner]") {
	string TestFileContents = "Not";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "Not");
}

TEST_CASE("Scanner next() Returns DATA_TYPE for 'integer' with the Correct Value", "[Scanner]") {
	string TestFileContents = "integer";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, DATA_TYPE, "integer");
}

TEST_CASE("Scanner next() Returns IDENTIFIER for 'Integer' with the Correct Value (AND NOT A DATA_TYPE)", "[Scanner]") {
	string TestFileContents = "Integer";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "Integer");
}

TEST_CASE("Scanner next() Returns DATA_TYPE for 'boolean' with the Correct Value", "[Scanner]") {
	string TestFileContents = "boolean";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, DATA_TYPE, "boolean");
}

TEST_CASE("Scanner next() Returns IDENTIFIER for 'Boolean' with the Correct Value (AND NOT A DATA_TYPE)", "[Scanner]") {
	string TestFileContents = "Boolean";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "Boolean");
}

TEST_CASE("Scanner next() Returns BOOLEAN for 'true' with the Correct Value", "[Scanner]") {
	string TestFileContents = "true";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, BOOLEAN, "true");
}

TEST_CASE("Scanner next() Returns IDENTIFIER for 'True' with the Correct Value (AND NOT A BOOLEAN)", "[Scanner]") {
	string TestFileContents = "True";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "True");
}

TEST_CASE("Scanner next() Returns BOOLEAN for 'false' with the Correct Value", "[Scanner]") {
	string TestFileContents = "false";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, BOOLEAN, "false");
}

TEST_CASE("Scanner next() Returns IDENTIFIER for 'False' with the Correct Value (AND NOT A BOOLEAN)", "[Scanner]") {
	string TestFileContents = "False";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "False");
}

TEST_CASE("Scanner next() Returns CONDITIONAL for 'if' with the Correct Value", "[Scanner]") {
	string TestFileContents = "if";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, CONDITIONAL, "if");
}

TEST_CASE("Scanner next() Returns IDENTIFIER for 'If' with the Correct Value (AND NOT A CONDITIONAL)", "[Scanner]") {
	string TestFileContents = "If";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "If");
}

TEST_CASE("Scanner next() Returns CONDITIONAL for 'then' with the Correct Value", "[Scanner]") {
	string TestFileContents = "then";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, CONDITIONAL, "then");
}

TEST_CASE("Scanner next() Returns IDENTIFIER for 'Then' with the Correct Value (AND NOT A CONDITIONAL)", "[Scanner]") {
	string TestFileContents = "Then";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "Then");
}

TEST_CASE("Scanner next() Returns CONDITIONAL for 'else' with the Correct Value", "[Scanner]") {
	string TestFileContents = "else";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, CONDITIONAL, "else");
}

TEST_CASE("Scanner next() Returns IDENTIFIER for 'Else' with the Correct Value (AND NOT A CONDITIONAL)", "[Scanner]") {
	string TestFileContents = "Else";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "Else");
}

TEST_CASE("Scanner next() Returns Valid Identifier When Underscores And Numbers Are Involved") {
	string TestFileContents = "identifIER_23_44";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "identifIER_23_44");
}

TEST_CASE("Scanner next() Finds Token After Identifier And Returns End Of File At The End") {
	string TestFileContents = "213 identifIER_23_44";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "213");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "identifIER_23_44");
	assertScannerHasNextTokenOfType(Scanner, END_OF_FILE);
}

TEST_CASE("Scanner next() Finds IDENTIFIER Token With Correct Value When Keyword And Int Smashed Together") {
	string TestFileContents = "function7 if8 or_55";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "function7");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "if8");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "or_55");
	assertScannerHasNextTokenOfType(Scanner, END_OF_FILE);
}

TEST_CASE("Scanner next() Throws Error When Unsported Character Found in identifier", "[Scanner]") {
	string TestFileContents = "abcd!";
	Scanner Scanner(TestFileContents, true);
	REQUIRE_THROWS_AS(Scanner.next(), runtime_error);
}

TEST_CASE("Scanner next() Returns IDENTIFIER Token Properly when Self Delimiters Or Whitespace Is Found", "[Scanner]") {
	string TestFileContents = "a b+c-d*e/f=g<h(i)j,k:l";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "a");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "b");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, ARITHMETIC_OPERATOR, "+");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "c");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, ARITHMETIC_OPERATOR, "-");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "d");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, ARITHMETIC_OPERATOR, "*");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "e");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, ARITHMETIC_OPERATOR, "/");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "f");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, COMPARATOR, "=");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "g");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, COMPARATOR, "<");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "h");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, PARENTHESIS, "(");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "i");;
	assertScannerHasNextTokenOfTypeWithValue(Scanner, PARENTHESIS, ")");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "j");
	assertScannerHasNextTokenOfType(Scanner, COMMA);
	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "k");
	assertScannerHasNextTokenOfType(Scanner, COLON);
	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "l");
}

TEST_CASE("Scanner peek() Returns Token But Doesn't Consume It") {
	string TestFileContents = "213 test";
	Scanner Scanner(TestFileContents, true);

	assertScannerPeeksNextTokenOfTypeWithValue(Scanner, INTEGER, "213");
	assertScannerPeeksNextTokenOfTypeWithValue(Scanner, INTEGER, "213");
}

TEST_CASE("Scanner peek() Returns Correct Token After Consuming A") {
	string TestFileContents = "213 test";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "213");
	assertScannerPeeksNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "test");
}

TEST_CASE("Scanner next() Works After peek() ") {
	string TestFileContents = "213 test";
	Scanner Scanner(TestFileContents, true);

	assertScannerPeeksNextTokenOfTypeWithValue(Scanner, INTEGER, "213");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "213");
}

TEST_CASE("Scanner peek() Detects End Of File Correctly") {
	string TestFileContents = "";
	Scanner Scanner(TestFileContents, true);

	assertScannerPeeksNextTokenOfTypeWithValue(Scanner, END_OF_FILE, "");
}

TEST_CASE("Scanner next() Throws Error When Unsported Character Found", "[Scanner]") {
	string TestFileContents = "123 #";
	Scanner Scanner(TestFileContents, true);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "123");
	REQUIRE_THROWS_AS(Scanner.next(), runtime_error);
}

TEST_CASE("Scanner Constructor() Throws Error When File Not Found", "[Scanner]") {
	REQUIRE_THROWS_AS(Scanner("fakeFile.kln", 1), invalid_argument);
}

TEST_CASE("Scanner Constructor() Throws Error When File Not Klein File", "[Scanner]") {
	REQUIRE_THROWS_AS(Scanner("fakeFile.txt", 1), invalid_argument);
}

TEST_CASE("Scanner Constructor() Sucessfull Opens File And Reads Tokens When Tokens Are Valid", "[Scanner]") {
	Scanner Scanner("test/klein/test_klein_program_1.kln", 1);

	assertScannerHasNextTokenOfTypeWithValue(Scanner, PARENTHESIS, "(");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "34");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, ARITHMETIC_OPERATOR, "+");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, INTEGER, "34");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, CONDITIONAL, "if");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, IDENTIFIER, "THAN");
	assertScannerHasNextTokenOfTypeWithValue(Scanner, CONDITIONAL, "else");
	assertScannerHasNextTokenOfType(Scanner, END_OF_FILE);
}

TEST_CASE("Scanner next() Sucessfull Reads Token of 256 Characters But Blows Up When There's 257 Characters", "[Scanner]") {
	Scanner Scanner("test/klein/test_klein_program_2_long_identifiers.kln", 1);

	assertScannerHasNextTokenOfType(Scanner, IDENTIFIER);
	REQUIRE_THROWS_AS(Scanner.next(), runtime_error);
}

TEST_CASE("Scanner next() Sucessfull Reads Integer == 2^32 But Blows Up When Integer == 2^32 + 1", "[Scanner]") {
	Scanner Scanner("test/klein/test_klein_program_3_int_range.kln", 1);

	assertScannerHasNextTokenOfType(Scanner, INTEGER);
	REQUIRE_THROWS_AS(Scanner.next(), runtime_error);
}
