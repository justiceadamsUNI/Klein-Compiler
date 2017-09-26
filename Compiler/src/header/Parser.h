#pragma once
#include "TokenType.h"
#include "Token.h"
#include <list>
#include <stdexcept>

using namespace std;

class Parser
{
	// Represents every terminal/non-terminal that could 
	// possibly be put onto the stack.
public:
	enum StackValues
	{
		INTEGER_LITERAL,
		INTEGER_DATA_TYPE,
		BOOLEN_DATA_TYPE,
		BOOLEAN_LITERAL,
		PLUS_OPERATOR,
		MINUS_OPERATOR,
		DIVIDES_OPERATOR,
		MULTIPLY_OPERATOR,
		FUNCTION,
		LEFT_PAREN,
		RIGHT_PAREN,
		COMMA_LITERAL,
		COLON_LITERAL,
		IDENTIFIER,
		PRINT,
		LESS_THAN_OPERATOR,
		EQUAL_SIGN,
		AND,
		OR,
		NOT,
		IF,
		THEN,
		ELSE,
		END_OF_STREAM,
		PROGRAM,
		DEFINITIONS,
		DEF,
		FORMALS,
		NON_EMPTY_FORMALS,
		NON_EMPTY_FORMALS_TAIL,
		FORMAL,
		BODY,
		TYPE,
		EXPR,
		EXPR_TAIL,
		SIMPLE_EXPR,
		SIMPLE_EXPR_TAIL,
		TERM,
		TERM_TAIL,
		FACTOR,
		FACTOR_ID_TAIL,
		ACTUALS,
		NON_EMPTY_ACTUALS,
		NON_EMPTY_ACTUALS_TAIL,
		LITERAL,
		PRINT_STATEMENT
	};

//public:
	//ToDo: plug in public methods. Probably a match() and isValid()

private:
	// All the stack values that are terminals
	list<StackValues> TerminalValues = { 
		INTEGER_LITERAL,
		INTEGER_DATA_TYPE,
		BOOLEN_DATA_TYPE,
		BOOLEAN_LITERAL,
		PLUS_OPERATOR,
		MINUS_OPERATOR,
		DIVIDES_OPERATOR,
		MULTIPLY_OPERATOR,
		FUNCTION,
		LEFT_PAREN,
		RIGHT_PAREN,
		COMMA_LITERAL,
		COLON_LITERAL,
		IDENTIFIER,
		PRINT,
		LESS_THAN_OPERATOR,
		EQUAL_SIGN,
		AND,
		OR,
		NOT,
		IF,
		THEN,
		ELSE,
		END_OF_STREAM
	};

	// All the stack values that are Non-terminals
	list<StackValues> NonTerminalValues = {
		PROGRAM,
		DEFINITIONS,
		DEF,
		FORMALS,
		NON_EMPTY_FORMALS,
		NON_EMPTY_FORMALS_TAIL,
		FORMAL,
		BODY,
		TYPE,
		EXPR,
		EXPR_TAIL,
		SIMPLE_EXPR,
		SIMPLE_EXPR_TAIL,
		TERM,
		TERM_TAIL,
		FACTOR,
		FACTOR_ID_TAIL,
		ACTUALS,
		NON_EMPTY_ACTUALS,
		NON_EMPTY_ACTUALS_TAIL,
		LITERAL,
		PRINT_STATEMENT
	};

	bool isTerminalValue(StackValues value);


	StackValues mapFromScannerTokenToStackValue(Token token);
};
