#include "catch.hpp"
#include "fakeit.hpp"
#include "../src/header/NodeBuilderVisitor.h"

TEST_CASE("visitProgramNode() sucessfully builds a program node out of definitions and updates the semantic stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode Definition1(DefinitionsNodeTYPE);
	ASTNode Definition2(DefinitionsNodeTYPE);
	ASTNode Definition3(DefinitionsNodeTYPE);
	Definition1.setLiteralValue("def 1");
	Definition2.setLiteralValue("def 2");
	Definition3.setLiteralValue("def 3");

	Stack.push(Definition1);
	Stack.push(Definition2);
	Stack.push(Definition3);

	Visitor.accept(BUILD_PROGRAM_NODE, Stack, "NULL", "0");

	ASTNode StackTop = Stack.pop();
	REQUIRE(Stack.isEmpty());
	REQUIRE(StackTop.getAstNodeType() == ProgramNodeTYPE);
	REQUIRE(StackTop.getDefinitions().size() == 3);
	REQUIRE(StackTop.getDefinitions()[0]->getLiteralValue() == "def 3");
	REQUIRE(StackTop.getDefinitions()[1]->getLiteralValue() == "def 2");
	REQUIRE(StackTop.getDefinitions()[2]->getLiteralValue() == "def 1");
}

TEST_CASE("visitProgramNode() sucessfully builds a program node when no definitions are on the stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	Visitor.accept(BUILD_PROGRAM_NODE, Stack, "NULL", "0");

	ASTNode StackTop = Stack.pop();
	REQUIRE(Stack.isEmpty());
	REQUIRE(StackTop.getAstNodeType() == ProgramNodeTYPE);
	REQUIRE(StackTop.getDefinitions().empty());
}

TEST_CASE("visitDefinitionsNode() sucessfully builds a definitions node out of def nodes and updates the semantic stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode Def1(DefNodeTYPE);
	ASTNode Def2(DefNodeTYPE);
	ASTNode Def3(DefNodeTYPE);
	Def1.setLiteralValue("def 1");
	Def2.setLiteralValue("def 2");
	Def3.setLiteralValue("def 3");

	Stack.push(Def1);
	Stack.push(Def2);
	Stack.push(Def3);

	Visitor.accept(BUILD_DEFINITIONS_NODE, Stack, "NULL", "0");

	ASTNode StackTop = Stack.pop();
	REQUIRE(Stack.isEmpty());
	REQUIRE(StackTop.getAstNodeType() == DefinitionsNodeTYPE);
	REQUIRE(StackTop.getDefNodes().size() == 3);
	REQUIRE(StackTop.getDefNodes()[0]->getLiteralValue() == "def 3");
	REQUIRE(StackTop.getDefNodes()[1]->getLiteralValue() == "def 2");
	REQUIRE(StackTop.getDefNodes()[2]->getLiteralValue() == "def 1");
}

TEST_CASE("visitDefinitionsNode() sucessfully builds a program node when no def nodes are on the stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	Visitor.accept(BUILD_DEFINITIONS_NODE, Stack, "NULL", "0");

	ASTNode StackTop = Stack.pop();
	REQUIRE(Stack.isEmpty());
	REQUIRE(StackTop.getAstNodeType() == DefinitionsNodeTYPE);
	REQUIRE(StackTop.getDefinitions().empty());
}

TEST_CASE("visitIdentifierNode() retains it's identifier name on the semantic stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	Visitor.accept(BUILD_IDENTIFIER_NODE, Stack, "identifier", "0");
	
	ASTNode StackTop = Stack.pop();
	REQUIRE(Stack.isEmpty());
	REQUIRE(StackTop.getAstNodeType() == IDENTIFIERNODETYPE);
	REQUIRE(StackTop.getIdentifierName() == "identifier");
}

TEST_CASE("visitDefNode() sucessfully builds a def node out of body, type, formals, and identiier node and updates the semantic stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode Identifier(IDENTIFIERNODETYPE);
	ASTNode Formals(FormalsNodeType);
	ASTNode Type(TypeNodeTYPE);
	ASTNode Body(BodyNodeTYPE);
	Identifier.setLiteralValue("identifier");
	Formals.setLiteralValue("formals");
	Type.setLiteralValue("type");
	Body.setLiteralValue("body");

	Stack.push(Identifier);
	Stack.push(Formals);
	Stack.push(Type);
	Stack.push(Body);

	Visitor.accept(BUILD_DEF_NODE, Stack, "NULL", "0");

	ASTNode StackTop = Stack.pop();
	REQUIRE(Stack.isEmpty());
	REQUIRE(StackTop.getAstNodeType() == DefNodeTYPE);
	REQUIRE(StackTop.getIdentifierNode()->getLiteralValue() == "identifier");
	REQUIRE(StackTop.getFormalsNode()->getLiteralValue() == "formals");
	REQUIRE(StackTop.getTypeNode()->getLiteralValue() == "type");
	REQUIRE(StackTop.getBodyNode()->getLiteralValue() == "body");
}

TEST_CASE("visitDefNode() throws error when body node not found", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode Identifier(IDENTIFIERNODETYPE);
	Stack.push(Identifier);

	REQUIRE_THROWS_AS(Visitor.accept(BUILD_DEF_NODE, Stack, "NULL", "0"), runtime_error);
}

TEST_CASE("visitDefNode() throws error when type not found", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode Identifier(IDENTIFIERNODETYPE);
	ASTNode Body(BodyNodeTYPE);
	Stack.push(Identifier);
	Stack.push(Body);

	REQUIRE_THROWS_AS(Visitor.accept(BUILD_DEF_NODE, Stack, "NULL", "0"), runtime_error);
}

TEST_CASE("visitDefNode() throws error when formals node not found", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode Identifier(IDENTIFIERNODETYPE);
	ASTNode Type(TypeNodeTYPE);
	ASTNode Body(BodyNodeTYPE);
	Stack.push(Identifier);
	Stack.push(Type);
	Stack.push(Body);

	REQUIRE_THROWS_AS(Visitor.accept(BUILD_DEF_NODE, Stack, "NULL", "0"), runtime_error);
}

TEST_CASE("visitDefNode() throws error when identifier node not found", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode ErrorNode(MultiplicatorTermNodeTYPE);
	ASTNode Formals(FormalsNodeType);
	ASTNode Type(TypeNodeTYPE);
	ASTNode Body(BodyNodeTYPE);
	Stack.push(ErrorNode);
	Stack.push(Formals);
	Stack.push(Type);
	Stack.push(Body);

	REQUIRE_THROWS_AS(Visitor.accept(BUILD_DEF_NODE, Stack, "NULL", "0"), runtime_error);
}

TEST_CASE("visitFormalsNode() adds formals node to the stack without altering anything else", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode Body(BodyNodeTYPE);
	Stack.push(Body);

	Visitor.accept(BUILD_FORMALS_NODE, Stack, "NULL", "0");

	ASTNode StackTop = Stack.pop();
	REQUIRE(Stack.top().getAstNodeType() == BodyNodeTYPE);
	REQUIRE(StackTop.getAstNodeType() == FormalsNodeType);
}

TEST_CASE("visitNonEmptyFormalsNode() sucessfully builds a NonEmptyFormals node out of formals and updates the semantic stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode Formal1(FormalNodeTYPE);
	ASTNode Formal2(FormalNodeTYPE);
	ASTNode Formal3(FormalNodeTYPE);
	Formal1.setLiteralValue("formal 1");
	Formal2.setLiteralValue("formal 2");
	Formal3.setLiteralValue("formal 3");

	Stack.push(Formal1);
	Stack.push(Formal2);
	Stack.push(Formal3);

	Visitor.accept(BUILD_NONEMPTYFORMALS_NODE, Stack, "NULL", "0");

	ASTNode StackTop = Stack.pop();
	REQUIRE(Stack.isEmpty());
	REQUIRE(StackTop.getAstNodeType() == NonEmptyFormalsNodeTYPE);
	REQUIRE(StackTop.getFormalNodes().size() == 3);
	REQUIRE(StackTop.getFormalNodes()[0]->getLiteralValue() == "formal 3");
	REQUIRE(StackTop.getFormalNodes()[1]->getLiteralValue() == "formal 2");
	REQUIRE(StackTop.getFormalNodes()[2]->getLiteralValue() == "formal 1");
}

TEST_CASE("visitFormalNode() sucessfully builds a formal node out of type and identiier node and updates the semantic stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode Identifier(IDENTIFIERNODETYPE);
	ASTNode Type(TypeNodeTYPE);
	Identifier.setLiteralValue("identifier");
	Type.setLiteralValue("type");

	Stack.push(Identifier);
	Stack.push(Type);

	Visitor.accept(BUILD_FORMAL_NODE, Stack, "NULL", "0");

	ASTNode StackTop = Stack.pop();
	REQUIRE(Stack.isEmpty());
	REQUIRE(StackTop.getAstNodeType() == FormalNodeTYPE);
	REQUIRE(StackTop.getIdentifierNode()->getLiteralValue() == "identifier");
	REQUIRE(StackTop.getTypeNode()->getLiteralValue() == "type");
}

TEST_CASE("visitFormalNode() throws error when type node not found", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode Identifier(IDENTIFIERNODETYPE);
	Identifier.setLiteralValue("identifier");
	Stack.push(Identifier);

	REQUIRE_THROWS_AS(Visitor.accept(BUILD_FORMAL_NODE, Stack, "NULL", "0"), runtime_error);
}

TEST_CASE("visitFormalNode() throws error when identifier node not found", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;
	
	ASTNode ErrorNode(ProgramNodeTYPE);
	ASTNode Type(TypeNodeTYPE);

	Stack.push(ErrorNode);
	Stack.push(Type);

	REQUIRE_THROWS_AS(Visitor.accept(BUILD_FORMAL_NODE, Stack, "NULL", "0"), runtime_error);
}

TEST_CASE("Visitor callzs visitBodyNode() when accept() called with BUILD_BODY_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitTypeNode() when accept() called with BUILD_TYPE_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitLessThanNode() when accept() called with BUILD_LESSTHAN_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitEqualNode() when accept() called with BUILD_EQUAL_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitBaseExpressionNode() when accept() called with BUILD_BASEEXPR_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitOrNode() when accept() called with BUILD_OR_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitAdditionNode() when accept() called with BUILD_ADDITION_NODE Stack Value", "[Node Builder Visitor]") {
}


TEST_CASE("Visitor callzs visitSubtractionNode() when accept() called with BUILD_SUBTRACTION_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitBaseSimpleExpressionNode() when accept() called with BUILD_BASESIMPLEEXPR_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitAndNode() when accept() called with BUILD_AND_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitMultiplicatorNode() when accept() called with BUILD_MULTIPLICATOR_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitDividerNode() when accept() called with BUILD_MULTIPLICATOR_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitBaseTermNode() when accept() called with BUILD_BASETERM_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitIfFactorNode() when accept() called with BUILD_IFFACTOR_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitNotFactorNode() when accept() called with BUILD_NOTFACTOR_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitLiteralFactorNode() when accept() called with BUILD_LITERALFACTOR_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitSubtractionFactorNode() when accept() called with BUILD_SUBTRACTIONFACTOR_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitParenthesisedExpressionNode() when accept() called with BUILD_PARENEXPR_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitIdentifierActualsNode() when accept() called with BUILD_IDENTIFIERACTUALS_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitSingletonIdentifierFactorNode() when accept() called with BUILD_SINGLETONIDENTIFIERFACTOR_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitBaseActualsNode() when accept() called with BUILD_BASEACTUALS_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitNonBaseActualsNode() when accept() called with BUILD_NONBASEACTUALS_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitNonEmptyActualsNode() when accept() called with BUILD_NONEMPTYACTUALS_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitIntegerLiteralNode() when accept() called with BUILD_INTEGERLITERAL_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitBooleanLiteralNode() when accept() called with BUILD_BOOLEANLITERAL_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor callzs visitPrintStatementNode() when accept() called with BUILD_PRINTSTATEMENT_NODE Stack Value", "[Node Builder Visitor]") {

}

TEST_CASE("Visitor throws errosr when accept() called with a value that isn't a semantic action", "[Node Builder Visitor]") {

}