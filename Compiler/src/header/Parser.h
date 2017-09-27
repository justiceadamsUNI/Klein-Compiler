#pragma once
#include "TokenType.h"
#include "Scanner.h"
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

private:
	Scanner ScannerVar;

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

			// I made it to row 14 in the excell document. The rest needs to be flesched out still. This is going
			// to be a super large map, but it is by far the cleanest solution I could think of, and it represents exactly what we want. 
			// Let's talk math: Essentially we want a two variable function f(x,y) where the domain = D^2 and Codomain = D.
			// This appears to be exactly that, and we won't have any empty list, nor will we have a large matrix to maintain.
			// Ideally from our cpp file we could for example call map.find(make_pair<StackValues, StackValues>(BODY, LEFT_PAREN)) and it
			// would return {EXPR} as a list! (see last row here). Anyway, if a pair isn't a key in the table, we're in an error state. AKA
			// the function is undefined here. Note that we're exlusively dealing with our stack_values enum, so that's dope!
		}
	};
};