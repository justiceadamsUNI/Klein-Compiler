#pragma once
#include <string>
#include <map>
#include <list>
#include <vector>
#include "../header/TokenType.h"
#include "../header/Token.h"
#include <stdexcept>

using namespace std;

class Scanner
{
public:
	Scanner(string FilePath);

	//Constructor for testing.
	Scanner(string TestFileContents, bool Testing);
	
	Token next();

	Token peek();

private:
	string SelfDelimiters = "+-*/,:,<=()";
	string FileContents;
	int FilePosition;
	int FileSize;

	const map<string, TokenType> GenericKeywordTypeMap{
		{ "function", PRIMITIVE_KEYWORD},
		{ "and", LOGICIAL_OPERATOR},
		{ "or", LOGICIAL_OPERATOR},
		{ "not", LOGICIAL_OPERATOR},
		{ "integer", DATA_TYPE},
		{ "boolean", DATA_TYPE},
		{ "true", BOOLEAN},
		{ "false", BOOLEAN},
		{ "if", CONDITIONAL},
		{ "then", CONDITIONAL},
		{ "else", CONDITIONAL}
	};


	Token consumeParenthesisToken(char Paren);

	Token consumeCommaToken();

	Token consumeColonToken();

	Token consumeOperatorToken(char Operator);

	Token consumeComparatorToken(char Comparator);

	Token consumeIntegerToken();

	Token consumeZeroToken();

	Token consumeGenericWordToken();

	bool isCommentStart();

	void ignoreComment();

	bool skipPastWhiteSpace();

	bool isValidKleinFile(string FilePath);

	string readFile(string FilePath);
};