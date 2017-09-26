#include "../header/Parser.h"
#include <algorithm>

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
	case PRIMITIVE_KEYWORD:
		return FUNCTION;
	case PARENTHESIS:
		return InToken.getValue() == "(" ? LEFT_PAREN : RIGHT_PAREN;
	case COMMA:
		return COMMA_LITERAL;
	case COLON:
		return COLON_LITERAL;
	case IDENTIFIER:
		if (InToken.getValue() == "print")
		{
			return PRINT;
		}
		else
		{
			return IDENTIFIER;
		};
	case COMPARATOR:
		if (InToken.getValue() == "<")
		{
			return LESS_THAN_OPERATOR;
		}
		else
		{
			return EQUAL_SIGN;
		};
	case LOGICIAL_OPERATOR:
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
	case CONDITIONAL:
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
	case END_OF_FILE:
		return END_OF_STREAM;
	default:
		break;
	}

}