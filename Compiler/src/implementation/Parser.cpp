#include "../header/Parser.h"
#include <algorithm>


Parser::Parser(Scanner& InScanner): ScannerVar(InScanner) {
	return;
}

bool Parser::isTerminalValue(StackValues value)
{
	list<StackValues>::iterator foundElement = std::find(TerminalValues.begin(), TerminalValues.end(), value);
	return foundElement != TerminalValues.end();
}

Parser::StackValues Parser::mapFromScannerTokenToStackValue(Token InToken)
{
	switch (InToken.getTokenType())
	{
	case INTEGER:
		return INTEGER_LITERAL;
	case DATA_TYPE:
		return InToken.getValue() == "integer" ? INTEGER_DATA_TYPE : BOOLEN_DATA_TYPE;
	case BOOLEAN:
		return BOOLEAN_LITERAL;
	case ARITHMETIC_OPERATOR:
		return mapArithmeticOperatorTokenToStackValue(InToken);
	case PRIMITIVE_KEYWORD:
		return FUNCTION;
	case PARENTHESIS:
		return InToken.getValue() == "(" ? LEFT_PAREN : RIGHT_PAREN;
	case COMMA:
		return COMMA_LITERAL;
	case COLON:
		return COLON_LITERAL;
	case IDENTIFIER:
		return InToken.getValue() == "print" ? PRINT : IDENTIFIER;
	case COMPARATOR:
		return InToken.getValue() == "<" ? LESS_THAN_OPERATOR : EQUAL_SIGN;
	case LOGICIAL_OPERATOR:
		return mapLogicalOperatorTokenToStackValue(InToken);
	case CONDITIONAL:
		return mapConditionalTokenToStackValue(InToken);
	case END_OF_FILE:
		return END_OF_STREAM;
	}

	throw runtime_error("ERROR: Parser got in a bad state attmpting to map TokenType to StackValue. Token value - " + InToken.getValue());
}

Parser::StackValues Parser::mapArithmeticOperatorTokenToStackValue(Token InToken)
{
	if (InToken.getValue() == "+")
	{
		return PLUS_OPERATOR;
	}
	else if (InToken.getValue() == "-")
	{
		return MINUS_OPERATOR;
	}
	else if (InToken.getValue() == "/")
	{
		return DIVIDES_OPERATOR;
	}
	else
	{
		return MULTIPLY_OPERATOR;
	};
}

Parser::StackValues Parser::mapLogicalOperatorTokenToStackValue(Token InToken)
{
	if (InToken.getValue() == "and")
	{
		return AND;
	}
	else if (InToken.getValue() == "not")
	{
		return NOT;
	}
	else
	{
		return OR;
	};
}

Parser::StackValues Parser::mapConditionalTokenToStackValue(Token InToken)
{
	if (InToken.getValue() == "if")
	{
		return IF;
	}
	else if (InToken.getValue() == "then")
	{
		return THEN;
	}
	else
	{
		return ELSE;
	};
}
