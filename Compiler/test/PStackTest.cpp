#include "catch.hpp"
#include "../src/header/PStack.h"

TEST_CASE("PStack pop() returns top of stack correctly and removes it from stack", "[PStack]") {
	PStack myStack;
	myStack.push(Parser::PROGRAM);

	REQUIRE(myStack.pop() == Parser::PROGRAM);
	REQUIRE(myStack.isEmpty() == true);
}

TEST_CASE("PStack pop() throws error when stack is empty", "[PStack]") {
	PStack myStack;

	REQUIRE_THROWS_AS(myStack.pop(), out_of_range)
}

TEST_CASE("PStack pop() returns top of stack correctly sequentually", "[PStack]") {
	PStack myStack;
	myStack.push(Parser::END_OF_STREAM);
	myStack.push(Parser::PROGRAM);

	REQUIRE(myStack.pop() == Parser::PROGRAM);
	REQUIRE(myStack.pop() == Parser::END_OF_STREAM);
}

TEST_CASE("PStack push() adds value to the top of the stack correctly", "[PStack]") {
	PStack myStack;
	myStack.push(Parser::END_OF_STREAM);
	myStack.push(Parser::PROGRAM);

	REQUIRE(myStack.pop() == Parser::PROGRAM);
}

TEST_CASE("PStack isEmpty() returns true for an empty stack", "[PStack]") {
	PStack myStack;

	REQUIRE(myStack.isEmpty() == true);
}

TEST_CASE("PStack isEmpty() returns false for a non-empty stack", "[PStack]") {
	PStack myStack;
	myStack.push(Parser::PROGRAM);

	REQUIRE(myStack.isEmpty() == false);
}