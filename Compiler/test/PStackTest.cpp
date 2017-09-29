#include "catch.hpp"
#include "../src/header/PStack.h"

TEST_CASE("PStack pop() returns top of stack correctly and removes it from stack", "[PStack]") {
	PStack myStack;
	myStack.push(PROGRAM);

	REQUIRE(myStack.pop() == PROGRAM);
	REQUIRE(myStack.isEmpty() == true);
}

TEST_CASE("PStack pop() throws error when stack is empty", "[PStack]") {
	PStack myStack;

	REQUIRE_THROWS_AS(myStack.pop(), out_of_range);
}

TEST_CASE("PStack pop() returns top of stack correctly sequentually", "[PStack]") {
	PStack myStack;
	myStack.push(END_OF_STREAM);
	myStack.push(PROGRAM);

	REQUIRE(myStack.pop() == PROGRAM);
	REQUIRE(myStack.pop() == END_OF_STREAM);
}

TEST_CASE("PStack push() adds value to the top of the stack correctly", "[PStack]") {
	PStack myStack;
	myStack.push(END_OF_STREAM);
	myStack.push(PROGRAM);

	REQUIRE(myStack.pop() == PROGRAM);
}

TEST_CASE("PStack isEmpty() returns true for an empty stack", "[PStack]") {
	PStack myStack;

	REQUIRE(myStack.isEmpty() == true);
}

TEST_CASE("PStack isEmpty() returns false for a non-empty stack", "[PStack]") {
	PStack myStack;
	myStack.push(PROGRAM);

	REQUIRE(myStack.isEmpty() == false);
}

TEST_CASE("PStack top() returns top of stack correctly without consuming value", "[PStack]") {
	PStack myStack;
	myStack.push(PROGRAM);
	myStack.push(LEFT_PAREN);

	REQUIRE(myStack.top() == LEFT_PAREN);
	REQUIRE(myStack.top() == LEFT_PAREN);
}

TEST_CASE("PStack top() returns top of stack correctly and we can still consume it", "[PStack]") {
	PStack myStack;
	myStack.push(PROGRAM);
	myStack.push(LEFT_PAREN);

	REQUIRE(myStack.top() == LEFT_PAREN);
	REQUIRE(myStack.pop() == LEFT_PAREN);
	REQUIRE(myStack.top() == PROGRAM);
}

TEST_CASE("PStack top() throws error when stack is empty", "[PStack]") {
	PStack myStack;

	REQUIRE_THROWS_AS(myStack.top(), out_of_range);
}