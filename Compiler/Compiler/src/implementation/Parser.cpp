#include "../header/Parser.h"
#include <algorithm>
#include <iostream>


Parser::Parser(Scanner& InScanner): ScannerVar(InScanner) {
	return;
}

void Parser::parseProgram()
{
	Stack.push(END_OF_STREAM);
	Stack.push(PROGRAM);
	StackValues StackTop = Stack.top();
	
	StackValues PeekedTokenValue = mapFromScannerTokenToStackValue(ScannerVar.peek());
	while (StackTop != END_OF_STREAM) {
		
		if (isTerminalValue(StackTop)) {
			if (StackTop == PeekedTokenValue)
			{
				Stack.pop();
				ScannerVar.next();
			}
			else {
				string StackTopString = StackValuesPrintMap.find(StackTop)->second;
				string PeekedTokenSting = StackValuesPrintMap.find(PeekedTokenValue)->second;
				throw runtime_error("ERROR: Problem matching terminal value. Token from input stream - " + PeekedTokenSting + ", Stack Top - " + StackTopString);
			}
		}
		else {
			if (ParseTable.find(make_pair(StackTop, PeekedTokenValue)) != ParseTable.end()) {
				list<StackValues> Rule = ParseTable.find(make_pair(StackTop, PeekedTokenValue))->second;

				Stack.pop();
				addRuleToStack(Rule);
			}
			else {
				string StackTopString = StackValuesPrintMap.find(StackTop)->second;
				string PeekedTokenSting = StackValuesPrintMap.find(PeekedTokenValue)->second;
				throw runtime_error("ERROR: No Rule exist in the Parse Table for (" + StackTopString + ", " + PeekedTokenSting + ")");
			}
		}

		PeekedTokenValue = mapFromScannerTokenToStackValue(ScannerVar.peek());
		StackTop = Stack.top();
	}

	if (Stack.top() == END_OF_STREAM && PeekedTokenValue == END_OF_STREAM)
	{
		Stack.pop();
	}

	if (!Stack.isEmpty())
	{
		string StackTopString = StackValuesPrintMap.find(StackTop)->second;
		throw runtime_error("ERROR: There are still values on the stack, but input stream has ended. Top of stack - " + StackTopString);
	}
}

bool Parser::isTerminalValue(StackValues value)
{
	list<StackValues>::iterator foundElement = std::find(TerminalValues.begin(), TerminalValues.end(), value);
	return foundElement != TerminalValues.end();
}

StackValues Parser::mapFromScannerTokenToStackValue(Token InToken)
{
	switch (InToken.getTokenType())
	{
	case INTEGER:
		return INTEGER_LITERAL;
	case DATA_TYPE:
		return InToken.getValue() == "integer" ? INTEGER_DATA_TYPE : BOOLEAN_DATA_TYPE;
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
		return InToken.getValue() == "print" ? PRINT : IDENTIFIER_LITERAL;
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

StackValues Parser::mapArithmeticOperatorTokenToStackValue(Token InToken)
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

StackValues Parser::mapLogicalOperatorTokenToStackValue(Token InToken)
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

StackValues Parser::mapConditionalTokenToStackValue(Token InToken)
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

void Parser::addRuleToStack(list<StackValues> Rule)
{
	// Adds values to stack in reverse order
	while (!Rule.empty())
	{
		Stack.push(Rule.back());
		Rule.pop_back();
	}
}
