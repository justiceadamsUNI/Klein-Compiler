#include "../header/Token.h"

Token::Token(TokenType InTokenType, string InValue)
{
	ThisTokenType = InTokenType;
	ThisValue = InValue;
}

TokenType Token::getTokenType()
{
	return ThisTokenType;
}

string Token::getValue()
{
	return ThisValue;
}