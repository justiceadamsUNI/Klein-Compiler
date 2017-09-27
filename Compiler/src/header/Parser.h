#pragma once
#include "TokenType.h"
#include "Scanner.h"
#include "PStack.h"
#include "Token.h"
#include <list>
#include <map>
#include <stdexcept>

using namespace std;

class Parser
{
public:
	Parser(Scanner& InScanner);

	// Represents every terminal/non-terminal that could 
	// possibly be put onto the stack.
	enum StackValues
	{
		INTEGER_LITERAL,
		INTEGER_DATA_TYPE,
		BOOLEAN_DATA_TYPE,
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
		IDENTIFIER_LITERAL,
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
		PRINT_STATEMENT,
	};

	StackValues mapFromScannerTokenToStackValue(Token token);

	void parseProgram();

	bool isProgramValid();

private:
	Scanner ScannerVar;

	PStack Stack;

	// All the stack values that are terminals
	list<StackValues> TerminalValues = { 
		INTEGER_LITERAL,
		INTEGER_DATA_TYPE,
		BOOLEAN_DATA_TYPE,
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
		IDENTIFIER_LITERAL,
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

	bool isTerminalValue(StackValues Value);

	StackValues mapArithmeticOperatorTokenToStackValue(Token InToken);

	StackValues mapLogicalOperatorTokenToStackValue(Token InToken);
	
	StackValues mapConditionalTokenToStackValue(Token InToken);


	// PARSE TABLE - Sparse 2D array representation.
	// Read below the map for a better understanding. 
	// After struggling for a while, I think this is the best data structure to use here.
	// Ideally from our cpp file we could for example call map.find(make_pair<StackValues, StackValues>(BODY, LEFT_PAREN)) and it
	// would return {EXPR} as a list! (see last row here). Anyway, if a pair isn't a key in the table, we're in an error state.

	const map<pair<StackValues, StackValues>, list<StackValues>> ParseTable {
		{
			{ make_pair<StackValues, StackValues>(PROGRAM, FUNCTION), list<StackValues>{DEFINITIONS} },
			{ make_pair<StackValues, StackValues>(PROGRAM, END_OF_STREAM), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(DEFINITIONS, END_OF_STREAM), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(DEFINITIONS, FUNCTION), list<StackValues>{DEF, DEFINITIONS} },
			{ make_pair<StackValues, StackValues>(DEF, FUNCTION), list<StackValues>{FUNCTION, IDENTIFIER_LITERAL, LEFT_PAREN, FORMALS, RIGHT_PAREN, COLON_LITERAL, TYPE, BODY} },
			{ make_pair<StackValues, StackValues>(FORMALS, RIGHT_PAREN), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(FORMALS, IDENTIFIER_LITERAL), list<StackValues>{NON_EMPTY_FORMALS} },
			{ make_pair<StackValues, StackValues>(NON_EMPTY_FORMALS, IDENTIFIER_LITERAL), list<StackValues>{FORMAL, NON_EMPTY_FORMALS_TAIL} },
			{ make_pair<StackValues, StackValues>(NON_EMPTY_FORMALS_TAIL, COMMA_LITERAL), list<StackValues>{COMMA_LITERAL, NON_EMPTY_FORMALS_TAIL} },
			{ make_pair<StackValues, StackValues>(NON_EMPTY_FORMALS_TAIL, RIGHT_PAREN), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(FORMAL, IDENTIFIER_LITERAL), list<StackValues>{IDENTIFIER_LITERAL, COLON_LITERAL, TYPE} },
			{ make_pair<StackValues, StackValues>(BODY, PRINT), list<StackValues>{PRINT_STATEMENT, BODY} },
			{ make_pair<StackValues, StackValues>(BODY, IF), list<StackValues>{EXPR} },
			{ make_pair<StackValues, StackValues>(BODY, NOT), list<StackValues>{EXPR} },
			{ make_pair<StackValues, StackValues>(BODY, IDENTIFIER_LITERAL), list<StackValues>{EXPR} },
			{ make_pair<StackValues, StackValues>(BODY, INTEGER_LITERAL), list<StackValues>{EXPR} },
			{ make_pair<StackValues, StackValues>(BODY, BOOLEAN_LITERAL), list<StackValues>{EXPR} },
			{ make_pair<StackValues, StackValues>(BODY, MINUS_OPERATOR), list<StackValues>{EXPR} },
			{ make_pair<StackValues, StackValues>(BODY, LEFT_PAREN), list<StackValues>{EXPR} },
			{ make_pair<StackValues, StackValues>(TYPE, INTEGER_DATA_TYPE), list<StackValues>{INTEGER_DATA_TYPE} },
			{ make_pair<StackValues, StackValues>(TYPE, BOOLEAN_DATA_TYPE), list<StackValues>{BOOLEAN_DATA_TYPE} },
			{ make_pair<StackValues, StackValues>(EXPR, IF), list<StackValues>{SIMPLE_EXPR, EXPR_TAIL} },
			{ make_pair<StackValues, StackValues>(EXPR, NOT), list<StackValues>{SIMPLE_EXPR, EXPR_TAIL} },
			{ make_pair<StackValues, StackValues>(EXPR, IDENTIFIER_LITERAL), list<StackValues>{SIMPLE_EXPR, EXPR_TAIL} },
			{ make_pair<StackValues, StackValues>(EXPR, INTEGER_LITERAL), list<StackValues>{SIMPLE_EXPR,EXPR_TAIL} },
			{ make_pair<StackValues, StackValues>(EXPR, BOOLEAN_LITERAL), list<StackValues>{SIMPLE_EXPR, EXPR_TAIL} },
			{ make_pair<StackValues, StackValues>(EXPR, MINUS_OPERATOR), list<StackValues>{SIMPLE_EXPR, EXPR_TAIL} },
			{ make_pair<StackValues, StackValues>(EXPR, LEFT_PAREN), list<StackValues>{SIMPLE_EXPR, EXPR_TAIL} },
			{ make_pair<StackValues, StackValues>(EXPR_TAIL, LESS_THAN_OPERATOR), list<StackValues>{LESS_THAN_OPERATOR, SIMPLE_EXPR} },
			{ make_pair<StackValues, StackValues>(EXPR_TAIL, EQUAL_SIGN), list<StackValues>{EQUAL_SIGN, SIMPLE_EXPR} },
			{ make_pair<StackValues, StackValues>(EXPR_TAIL, FUNCTION), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(EXPR_TAIL, END_OF_STREAM), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(EXPR_TAIL, RIGHT_PAREN), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(EXPR_TAIL, COMMA_LITERAL), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(EXPR_TAIL, THEN), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(EXPR_TAIL, ELSE), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(EXPR_TAIL, AND), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(EXPR_TAIL, MULTIPLY_OPERATOR), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(EXPR_TAIL, DIVIDES_OPERATOR), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(EXPR_TAIL, OR), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(EXPR_TAIL, PLUS_OPERATOR), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(EXPR_TAIL, MINUS_OPERATOR), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR, IF), list<StackValues>{TERM, SIMPLE_EXPR_TAIL} },
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR, NOT), list<StackValues>{TERM, SIMPLE_EXPR_TAIL} },
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR, IDENTIFIER_LITERAL), list<StackValues>{TERM, SIMPLE_EXPR_TAIL} },
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR, INTEGER_LITERAL), list<StackValues>{TERM, SIMPLE_EXPR_TAIL} },
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR, BOOLEAN_LITERAL), list<StackValues>{TERM, SIMPLE_EXPR_TAIL} },
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR, MINUS_OPERATOR), list<StackValues>{TERM, SIMPLE_EXPR_TAIL} },
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR, LEFT_PAREN), list<StackValues>{TERM, SIMPLE_EXPR_TAIL} },
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR_TAIL, OR), list<StackValues>{OR, TERM} },
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR_TAIL, PLUS_OPERATOR), list<StackValues>{PLUS_OPERATOR, TERM} },
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR_TAIL, MINUS_OPERATOR), list<StackValues>{MINUS_OPERATOR, TERM} },
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR_TAIL, FUNCTION), list<StackValues>{}},
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR_TAIL, END_OF_STREAM), list<StackValues>{}},
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR_TAIL, RIGHT_PAREN), list<StackValues>{}},
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR_TAIL, COMMA_LITERAL), list<StackValues>{}},
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR_TAIL, THEN), list<StackValues>{}},
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR_TAIL, ELSE), list<StackValues>{}},
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR_TAIL, AND), list<StackValues>{}},
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR_TAIL, MULTIPLY_OPERATOR), list<StackValues>{}},
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR_TAIL, DIVIDES_OPERATOR), list<StackValues>{}},
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR_TAIL, LESS_THAN_OPERATOR), list<StackValues>{}},
			{ make_pair<StackValues, StackValues>(SIMPLE_EXPR_TAIL, EQUAL_SIGN), list<StackValues>{}},
			{ make_pair<StackValues, StackValues>(TERM, IF), list<StackValues>{FACTOR, TERM_TAIL}},
			{ make_pair<StackValues, StackValues>(TERM, NOT), list<StackValues>{FACTOR, TERM_TAIL}},
			{ make_pair<StackValues, StackValues>(TERM, IDENTIFIER_LITERAL), list<StackValues>{FACTOR, TERM_TAIL}},
			{ make_pair<StackValues, StackValues>(TERM, INTEGER_LITERAL), list<StackValues>{FACTOR, TERM_TAIL}},
			{ make_pair<StackValues, StackValues>(TERM, BOOLEAN_LITERAL), list<StackValues>{FACTOR, TERM_TAIL}},
			{ make_pair<StackValues, StackValues>(TERM, MINUS_OPERATOR), list<StackValues>{FACTOR, TERM_TAIL}},
			{ make_pair<StackValues, StackValues>(TERM, LEFT_PAREN), list<StackValues>{FACTOR, TERM_TAIL}},
			{ make_pair<StackValues, StackValues>(TERM_TAIL, AND), list<StackValues>{AND, FACTOR}},
			{ make_pair<StackValues, StackValues>(TERM_TAIL, MULTIPLY_OPERATOR), list<StackValues>{MULTIPLY_OPERATOR, FACTOR}},
			{ make_pair<StackValues, StackValues>(TERM_TAIL, DIVIDES_OPERATOR), list<StackValues>{DIVIDES_OPERATOR, FACTOR}},
			{ make_pair<StackValues, StackValues>(TERM_TAIL, FUNCTION), list<StackValues>{}},
			{ make_pair<StackValues, StackValues>(TERM_TAIL, END_OF_STREAM), list<StackValues>{}},
			{ make_pair<StackValues, StackValues>(TERM_TAIL, RIGHT_PAREN), list<StackValues>{}},
			{ make_pair<StackValues, StackValues>(TERM_TAIL, COMMA_LITERAL), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(TERM_TAIL, THEN), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(TERM_TAIL, ELSE), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(TERM_TAIL, OR), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(TERM_TAIL, PLUS_OPERATOR), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(TERM_TAIL, MINUS_OPERATOR), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(TERM_TAIL, LESS_THAN_OPERATOR), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(TERM_TAIL, EQUAL_SIGN), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(FACTOR, IF), list<StackValues>{IF, EXPR, THEN, EXPR, ELSE, EXPR}},
			{ make_pair<StackValues, StackValues>(FACTOR, NOT), list<StackValues>{NOT, FACTOR}},
			{ make_pair<StackValues, StackValues>(FACTOR, IDENTIFIER_LITERAL), list<StackValues>{IDENTIFIER_LITERAL, FACTOR_ID_TAIL}},
			{ make_pair<StackValues, StackValues>(FACTOR, INTEGER_LITERAL), list<StackValues>{LITERAL}},
			{ make_pair<StackValues, StackValues>(FACTOR, BOOLEAN_LITERAL), list<StackValues>{LITERAL}},
			{ make_pair<StackValues, StackValues>(FACTOR, MINUS_OPERATOR), list<StackValues>{MINUS_OPERATOR, FACTOR} },
			{ make_pair<StackValues, StackValues>(FACTOR, LEFT_PAREN), list<StackValues>{LEFT_PAREN, EXPR, RIGHT_PAREN} },
			{ make_pair<StackValues, StackValues>(FACTOR_ID_TAIL, LEFT_PAREN), list<StackValues>{LEFT_PAREN, ACTUALS, RIGHT_PAREN} },
			{ make_pair<StackValues, StackValues>(FACTOR_ID_TAIL, FUNCTION), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(FACTOR_ID_TAIL, END_OF_STREAM), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(FACTOR_ID_TAIL, RIGHT_PAREN), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(FACTOR_ID_TAIL, COMMA_LITERAL), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(FACTOR_ID_TAIL, THEN), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(FACTOR_ID_TAIL, ELSE), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(FACTOR_ID_TAIL, AND), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(FACTOR_ID_TAIL, MULTIPLY_OPERATOR), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(FACTOR_ID_TAIL, DIVIDES_OPERATOR), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(FACTOR_ID_TAIL, OR), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(FACTOR_ID_TAIL, PLUS_OPERATOR), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(FACTOR_ID_TAIL, MINUS_OPERATOR), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(FACTOR_ID_TAIL, LESS_THAN_OPERATOR), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(FACTOR_ID_TAIL, EQUAL_SIGN), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(ACTUALS, RIGHT_PAREN), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(ACTUALS, IF), list<StackValues>{NON_EMPTY_ACTUALS} },
			{ make_pair<StackValues, StackValues>(ACTUALS, NOT), list<StackValues>{NON_EMPTY_ACTUALS} },
			{ make_pair<StackValues, StackValues>(ACTUALS, IDENTIFIER_LITERAL), list<StackValues>{NON_EMPTY_ACTUALS} },
			{ make_pair<StackValues, StackValues>(ACTUALS, INTEGER_LITERAL), list<StackValues>{NON_EMPTY_ACTUALS} },
			{ make_pair<StackValues, StackValues>(ACTUALS, BOOLEAN_LITERAL), list<StackValues>{NON_EMPTY_ACTUALS} },
			{ make_pair<StackValues, StackValues>(ACTUALS, MINUS_OPERATOR), list<StackValues>{NON_EMPTY_ACTUALS} },
			{ make_pair<StackValues, StackValues>(ACTUALS, LEFT_PAREN), list<StackValues>{NON_EMPTY_ACTUALS} },
			{ make_pair<StackValues, StackValues>(NON_EMPTY_ACTUALS, IF), list<StackValues>{EXPR, NON_EMPTY_ACTUALS_TAIL} },
			{ make_pair<StackValues, StackValues>(NON_EMPTY_ACTUALS, NOT), list<StackValues>{EXPR, NON_EMPTY_ACTUALS_TAIL} },
			{ make_pair<StackValues, StackValues>(NON_EMPTY_ACTUALS, IDENTIFIER_LITERAL), list<StackValues>{EXPR, NON_EMPTY_ACTUALS_TAIL} },
			{ make_pair<StackValues, StackValues>(NON_EMPTY_ACTUALS, INTEGER_LITERAL), list<StackValues>{EXPR, NON_EMPTY_ACTUALS_TAIL} },
			{ make_pair<StackValues, StackValues>(NON_EMPTY_ACTUALS, BOOLEAN_LITERAL), list<StackValues>{EXPR, NON_EMPTY_ACTUALS_TAIL} },
			{ make_pair<StackValues, StackValues>(NON_EMPTY_ACTUALS, MINUS_OPERATOR), list<StackValues>{EXPR, NON_EMPTY_ACTUALS_TAIL} },
			{ make_pair<StackValues, StackValues>(NON_EMPTY_ACTUALS, LEFT_PAREN), list<StackValues>{EXPR, NON_EMPTY_ACTUALS_TAIL} },
			{ make_pair<StackValues, StackValues>(NON_EMPTY_ACTUALS_TAIL, COMMA_LITERAL), list<StackValues>{COMMA_LITERAL, NON_EMPTY_ACTUALS} },
			{ make_pair<StackValues, StackValues>(NON_EMPTY_ACTUALS_TAIL, RIGHT_PAREN), list<StackValues>{} },
			{ make_pair<StackValues, StackValues>(LITERAL, INTEGER_LITERAL), list<StackValues>{INTEGER_LITERAL} },
			{ make_pair<StackValues, StackValues>(LITERAL, BOOLEAN_LITERAL), list<StackValues>{BOOLEAN_LITERAL} },
			{ make_pair<StackValues, StackValues>(PRINT_STATEMENT, PRINT), list<StackValues>{PRINT, LEFT_PAREN, EXPR, RIGHT_PAREN}}
		}
	};
};