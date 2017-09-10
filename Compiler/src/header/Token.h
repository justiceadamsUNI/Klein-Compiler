#pragma once
#include "TokenType.h"
#include <string>

using namespace std;

class Token
{
public:
	Token(TokenType InTokenType, string InValue);

	TokenType getTokenType();

	string getValue();

private:
	TokenType ThisTokenType;
	string ThisValue;
};
