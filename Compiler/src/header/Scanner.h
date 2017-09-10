#pragma once
#include <string>
#include <map>
#include <list>
#include "../header/TokenType.h"
#include "../header/Token.h"

using namespace std;

class Scanner
{
public:
	Scanner();

	Scanner(string FilePath);
	
	list<Token> getTokens();

private:
	map<string, string> GenericKeywordMap;
	string FileContents;
	int FilePosition;
	TokenType LastReadTokenType;

	Token scanForTokenOfType(TokenType TokenTypeToScan);

	void seperatePotentialTokens();

	Token mutateWordTokenType();
};