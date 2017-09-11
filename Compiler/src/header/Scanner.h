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
	map<string, string> GenericKeywordMap;
	string FileContents;
	int FilePosition;

	Token scanForInteger();

	Token consumeParenthesisToken(char Paren);

	Token consumeCommaToken();

	Token consumeColonToken();

	Token consumeOperatorToken(char Operator);

	bool isCommentStart();

	void ignoreComment();


	void skipPastWhiteSpace();

	string readFile(string FilePath);
};