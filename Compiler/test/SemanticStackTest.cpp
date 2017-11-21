#include "catch.hpp"
#include "../src/header/SemanticStack.h"

TEST_CASE("Semantic Stack pop() returns top of stack correctly and removes it from stack", "[Semantic Stack]") {
	SemanticStack MyStack;
	ASTNode Node = ASTNode(PROGRAM_NODE_TYPE);
	Node.setLiteralValue("test");

	MyStack.push(Node);

	REQUIRE(MyStack.pop().getLiteralValue() == "test");
	REQUIRE(MyStack.isEmpty() == true);
}

TEST_CASE("Semantic Stack pop() throws error when stack is empty", "[Semantic Stack]") {
	SemanticStack MyStack;

	REQUIRE_THROWS_AS(MyStack.pop(), out_of_range);
}

TEST_CASE("Semantic Stack pop() returns top of stack correctly sequentually", "[Semantic Stack]") {
	SemanticStack MyStack;
	
	ASTNode Node = ASTNode(PROGRAM_NODE_TYPE);
	Node.setLiteralValue("bottom");
	MyStack.push(Node);

	ASTNode Node2 = ASTNode(PROGRAM_NODE_TYPE);
	Node2.setLiteralValue("top");
	MyStack.push(Node2);

	REQUIRE(MyStack.pop().getLiteralValue() == "top");
	REQUIRE(MyStack.pop().getLiteralValue() == "bottom");
}

TEST_CASE("Semantic Stack push() adds value to the top of the stack correctly", "[Semantic Stack]") {
	SemanticStack MyStack;
	
	ASTNode Node = ASTNode(PROGRAM_NODE_TYPE);
	Node.setLiteralValue("bottom");
	MyStack.push(Node);

	ASTNode Node2 = ASTNode(PROGRAM_NODE_TYPE);
	Node2.setLiteralValue("top");
	MyStack.push(Node2);

	REQUIRE(MyStack.pop().getLiteralValue() == "top");
}

TEST_CASE("Semantic Stack isEmpty() returns true for an empty stack", "[Semantic Stack]") {
	SemanticStack MyStack;

	REQUIRE(MyStack.isEmpty() == true);
}

TEST_CASE("Semantic Stack isEmpty() returns false for a non-empty stack", "[Semantic Stack]") {
	SemanticStack MyStack;
	MyStack.push(ASTNode(PROGRAM_NODE_TYPE));

	REQUIRE(MyStack.isEmpty() == false);
}

TEST_CASE("Semantic Stack top() returns top of stack correctly without consuming value", "[Semantic Stack]") {
	SemanticStack MyStack;
	
	ASTNode Node = ASTNode(PROGRAM_NODE_TYPE);
	Node.setLiteralValue("bottom");
	MyStack.push(Node);

	ASTNode Node2 = ASTNode(PROGRAM_NODE_TYPE);
	Node2.setLiteralValue("top");
	MyStack.push(Node2);

	REQUIRE(MyStack.top().getLiteralValue() == "top");
	REQUIRE(MyStack.top().getLiteralValue() == "top");
}

TEST_CASE("Semantic Stack top() returns top of stack correctly and we can still consume it", "[Semantic Stack]") {
	SemanticStack MyStack;
	
	ASTNode Node = ASTNode(PROGRAM_NODE_TYPE);
	Node.setLiteralValue("bottom");
	MyStack.push(Node);

	ASTNode Node2 = ASTNode(PROGRAM_NODE_TYPE);
	Node2.setLiteralValue("top");
	MyStack.push(Node2);

	REQUIRE(MyStack.top().getLiteralValue() == "top");
	REQUIRE(MyStack.pop().getLiteralValue() == "top");
	REQUIRE(MyStack.top().getLiteralValue() == "bottom");
}

TEST_CASE("Semantic Stack top() throws error when stack is empty", "[Semantic Stack]") {
	SemanticStack MyStack;

	REQUIRE_THROWS_AS(MyStack.top(), out_of_range);
}