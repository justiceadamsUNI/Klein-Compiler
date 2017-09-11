#pragma once
#include <string>
#include <map>
#include <list>
#include <vector>
#include "../header/TokenType.h"
#include "../header/Token.h"

using namespace std;

class Scanner
{
public:
	Scanner(string FilePath);
	
	Token getNextToken();

	Token peek();

private:
	string SelfDelimiters = "+-*/,:,<=()";
	string Digits = "0123456789";
	string FileContents;
	int FilePosition;

	Token consumeParenthesisToken(char Paren);

	Token consumeCommaToken();

	Token consumeColonToken();

	Token consumeOperatorToken(char Operator);

	Token consumeComparatorToken(char Comparator);

	Token consumeIntegerToken();

	bool isCommentStart();

	void ignoreComment();

	void skipPastWhiteSpace();

	string readFile(string FilePath);
};