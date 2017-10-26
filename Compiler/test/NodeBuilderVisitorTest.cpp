#include "catch.hpp"
#include "fakeit.hpp"
#include "../src/header/NodeBuilderVisitor.h"

TEST_CASE("visitProgramNode() sucessfully builds a program node out of definitions and updates the semantic stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode Definition1(DEFINITIONS_NODE_TYPE);
	Definition1.setLiteralValue("def 1");

	Stack.push(Definition1);

	Visitor.accept(BUILD_PROGRAM_NODE, Stack, "NULL", "0");

	ASTNode StackTop = Stack.pop();
	REQUIRE(Stack.isEmpty());
	REQUIRE(StackTop.getAstNodeType() == PROGRAM_NODE_TYPE);
	REQUIRE(StackTop.getDefinitions()->getLiteralValue() == "def 1");
}

TEST_CASE("visitProgramNode() throws error when defininitions node isn't on the stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;
	ASTNode ErrorNode(MULTIPLICATOR_TERM_NODE_TYPE);
	Stack.push(ErrorNode);

	REQUIRE_THROWS_AS(Visitor.accept(BUILD_PROGRAM_NODE, Stack, "NULL", "0"), runtime_error);
}

TEST_CASE("visitDefinitionsNode() sucessfully builds a definitions node out of def nodes and updates the semantic stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode Def1(DEF_NODE_TYPE);
	ASTNode Def2(DEF_NODE_TYPE);
	ASTNode Def3(DEF_NODE_TYPE);
	Def1.setLiteralValue("def 1");
	Def2.setLiteralValue("def 2");
	Def3.setLiteralValue("def 3");

	Stack.push(Def1);
	Stack.push(Def2);
	Stack.push(Def3);

	Visitor.accept(BUILD_DEFINITIONS_NODE, Stack, "NULL", "0");

	ASTNode StackTop = Stack.pop();
	REQUIRE(Stack.isEmpty());
	REQUIRE(StackTop.getAstNodeType() == DEFINITIONS_NODE_TYPE);
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
	REQUIRE(StackTop.getAstNodeType() == DEFINITIONS_NODE_TYPE);
	REQUIRE(StackTop.getDefNodes().empty());
}

TEST_CASE("visitIdentifierNode() retains it's identifier name on the semantic stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	Visitor.accept(BUILD_IDENTIFIER_NODE, Stack, "identifier", "0");
	
	ASTNode StackTop = Stack.pop();
	REQUIRE(Stack.isEmpty());
	REQUIRE(StackTop.getAstNodeType() == IDENTIFIER_NODE_TYPE);
	REQUIRE(StackTop.getIdentifierName() == "identifier");
}

TEST_CASE("visitDefNode() sucessfully builds a def node out of body, type, formals, and identiier node and updates the semantic stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode Identifier(IDENTIFIER_NODE_TYPE);
	ASTNode Formals(FORMALS_NODE_TYPE);
	ASTNode Type(TYPE_NODE_TYPE);
	ASTNode Body(BODY_NODE_TYPE);
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
	REQUIRE(StackTop.getAstNodeType() == DEF_NODE_TYPE);
	REQUIRE(StackTop.getIdentifierNode()->getLiteralValue() == "identifier");
	REQUIRE(StackTop.getFormalsNode()->getLiteralValue() == "formals");
	REQUIRE(StackTop.getTypeNode()->getLiteralValue() == "type");
	REQUIRE(StackTop.getBodyNode()->getLiteralValue() == "body");
}

TEST_CASE("visitDefNode() throws error when body node not found", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode Identifier(IDENTIFIER_NODE_TYPE);
	Stack.push(Identifier);

	REQUIRE_THROWS_AS(Visitor.accept(BUILD_DEF_NODE, Stack, "NULL", "0"), runtime_error);
}

TEST_CASE("visitDefNode() throws error when type not found", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode Identifier(IDENTIFIER_NODE_TYPE);
	ASTNode Body(BODY_NODE_TYPE);
	Stack.push(Identifier);
	Stack.push(Body);

	REQUIRE_THROWS_AS(Visitor.accept(BUILD_DEF_NODE, Stack, "NULL", "0"), runtime_error);
}

TEST_CASE("visitDefNode() throws error when formals node not found", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode Identifier(IDENTIFIER_NODE_TYPE);
	ASTNode Type(TYPE_NODE_TYPE);
	ASTNode Body(BODY_NODE_TYPE);
	Stack.push(Identifier);
	Stack.push(Type);
	Stack.push(Body);

	REQUIRE_THROWS_AS(Visitor.accept(BUILD_DEF_NODE, Stack, "NULL", "0"), runtime_error);
}

TEST_CASE("visitDefNode() throws error when identifier node not found", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode ErrorNode(MULTIPLICATOR_TERM_NODE_TYPE);
	ASTNode Formals(FORMALS_NODE_TYPE);
	ASTNode Type(TYPE_NODE_TYPE);
	ASTNode Body(BODY_NODE_TYPE);
	Stack.push(ErrorNode);
	Stack.push(Formals);
	Stack.push(Type);
	Stack.push(Body);

	REQUIRE_THROWS_AS(Visitor.accept(BUILD_DEF_NODE, Stack, "NULL", "0"), runtime_error);
}

TEST_CASE("visitFormalsNode() adds formals node to the stack without altering anything else", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode Body(BODY_NODE_TYPE);
	Stack.push(Body);

	Visitor.accept(BUILD_FORMALS_NODE, Stack, "NULL", "0");

	ASTNode StackTop = Stack.pop();
	REQUIRE(Stack.top().getAstNodeType() == BODY_NODE_TYPE);
	REQUIRE(StackTop.getAstNodeType() == FORMALS_NODE_TYPE);
}

TEST_CASE("visitNonEmptyFormalsNode() sucessfully builds a NonEmptyFormals node out of formals and updates the semantic stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode Formal1(FORMAL_NODE_TYPE);
	ASTNode Formal2(FORMAL_NODE_TYPE);
	ASTNode Formal3(FORMAL_NODE_TYPE);
	Formal1.setLiteralValue("formal 1");
	Formal2.setLiteralValue("formal 2");
	Formal3.setLiteralValue("formal 3");

	Stack.push(Formal1);
	Stack.push(Formal2);
	Stack.push(Formal3);

	Visitor.accept(BUILD_NONEMPTYFORMALS_NODE, Stack, "NULL", "0");

	ASTNode StackTop = Stack.pop();
	REQUIRE(Stack.isEmpty());
	REQUIRE(StackTop.getAstNodeType() == NON_EMPTY_FORMALS_NODE_TYPE);
	REQUIRE(StackTop.getFormalNodes().size() == 3);
	REQUIRE(StackTop.getFormalNodes()[0]->getLiteralValue() == "formal 3");
	REQUIRE(StackTop.getFormalNodes()[1]->getLiteralValue() == "formal 2");
	REQUIRE(StackTop.getFormalNodes()[2]->getLiteralValue() == "formal 1");
}

TEST_CASE("visitFormalNode() sucessfully builds a formal node out of type and identiier node and updates the semantic stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode Identifier(IDENTIFIER_NODE_TYPE);
	ASTNode Type(TYPE_NODE_TYPE);
	Identifier.setLiteralValue("identifier");
	Type.setLiteralValue("type");

	Stack.push(Identifier);
	Stack.push(Type);

	Visitor.accept(BUILD_FORMAL_NODE, Stack, "NULL", "0");

	ASTNode StackTop = Stack.pop();
	REQUIRE(Stack.isEmpty());
	REQUIRE(StackTop.getAstNodeType() == FORMAL_NODE_TYPE);
	REQUIRE(StackTop.getIdentifierNode()->getLiteralValue() == "identifier");
	REQUIRE(StackTop.getTypeNode()->getLiteralValue() == "type");
}

TEST_CASE("visitFormalNode() throws error when type node not found", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode Identifier(IDENTIFIER_NODE_TYPE);
	Identifier.setLiteralValue("identifier");
	Stack.push(Identifier);

	REQUIRE_THROWS_AS(Visitor.accept(BUILD_FORMAL_NODE, Stack, "NULL", "0"), runtime_error);
}

TEST_CASE("visitFormalNode() throws error when identifier node not found", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;
	
	ASTNode ErrorNode(PROGRAM_NODE_TYPE);
	ASTNode Type(TYPE_NODE_TYPE);

	Stack.push(ErrorNode);
	Stack.push(Type);

	REQUIRE_THROWS_AS(Visitor.accept(BUILD_FORMAL_NODE, Stack, "NULL", "0"), runtime_error);
}

TEST_CASE("visitBodyNode() sucessfully builds a body node out of expr and print nodes and updates the semantic stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode ExprNode(BASE_EXPR_NODE_TYPE);
	ASTNode PrintStatement1(PRINT_STATEMENT_NODE_TYPE);
	ASTNode PrintStatement2(PRINT_STATEMENT_NODE_TYPE);
	ASTNode PrintStatement3(PRINT_STATEMENT_NODE_TYPE);
	ExprNode.setLiteralValue("expression");
	PrintStatement1.setLiteralValue("print 1");
	PrintStatement2.setLiteralValue("print 2");
	PrintStatement3.setLiteralValue("print 3");

	Stack.push(PrintStatement1);
	Stack.push(PrintStatement2);
	Stack.push(PrintStatement3);
	Stack.push(ExprNode);

	Visitor.accept(BUILD_BODY_NODE, Stack, "NULL", "0");

	ASTNode StackTop = Stack.pop();
	REQUIRE(Stack.isEmpty());
	REQUIRE(StackTop.getAstNodeType() == BODY_NODE_TYPE);
	REQUIRE(StackTop.getPrintStatements().size() == 3);
	REQUIRE(StackTop.getPrintStatements()[0]->getLiteralValue() == "print 3");
	REQUIRE(StackTop.getPrintStatements()[1]->getLiteralValue() == "print 2");
	REQUIRE(StackTop.getPrintStatements()[2]->getLiteralValue() == "print 1");
	REQUIRE(StackTop.getBaseExprNode()->getLiteralValue() == "expression");
}

TEST_CASE("visitBodyNode() sucessfully builds a body node when no print statements are one the semantic stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode ExprNode(BASE_EXPR_NODE_TYPE);
	ExprNode.setLiteralValue("expression");
	Stack.push(ExprNode);

	Visitor.accept(BUILD_BODY_NODE, Stack, "NULL", "0");

	ASTNode StackTop = Stack.pop();
	REQUIRE(Stack.isEmpty());
	REQUIRE(StackTop.getAstNodeType() == BODY_NODE_TYPE);
	REQUIRE(StackTop.getPrintStatements().empty() == true);
	REQUIRE(StackTop.getBaseExprNode()->getLiteralValue() == "expression");
}

TEST_CASE("visitBodyNode() throws error when there isn't an expression node one the semantic stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode ErrorNode(PROGRAM_NODE_TYPE);
	Stack.push(ErrorNode);

	REQUIRE_THROWS_AS(Visitor.accept(BUILD_BODY_NODE, Stack, "NULL", "0"), runtime_error);
}


TEST_CASE("visitTypeNode() retains data type when pushed onto the sematntic stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	Visitor.accept(BUILD_TYPE_NODE, Stack, "type", "0");

	ASTNode StackTop = Stack.pop();
	REQUIRE(Stack.isEmpty());
	REQUIRE(StackTop.getAstNodeType() == TYPE_NODE_TYPE);
	REQUIRE(StackTop.getDataType() == "type");
}

TEST_CASE("visitLessThanNode() sucessfully builds a less than node out of two simpleexpr nodes and updates the semantic stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode SimpleExpr1(BASE_SIMPLE_EXPR_NODE_TYPE);
	ASTNode SimpleExpr2(BASE_SIMPLE_EXPR_NODE_TYPE);
	SimpleExpr1.setLiteralValue("simple expr 1");
	SimpleExpr2.setLiteralValue("simple expr 2");

	Stack.push(SimpleExpr1);
	Stack.push(SimpleExpr2);

	Visitor.accept(BUILD_LESSTHAN_NODE, Stack, "NULL", "0");

	ASTNode StackTop = Stack.pop();
	REQUIRE(Stack.isEmpty());
	REQUIRE(StackTop.getAstNodeType() == LESS_THAN_EXPR_NODE_TYPE);
	REQUIRE(StackTop.getBaseSimpleExprNode()->getLiteralValue()== "simple expr 2");
	REQUIRE(StackTop.getBaseSimpleExprNode2()->getLiteralValue() == "simple expr 1");
}

TEST_CASE("visitLessThanNode() throws error when no spimple expr nodes are on the semantic stack", "[Node Builder Visitor]") {
	NodeBuilderVisitor Visitor;
	SemanticStack Stack;

	ASTNode ErrorNode(PROGRAM_NODE_TYPE);
	Stack.push(ErrorNode);

	REQUIRE_THROWS_AS(Visitor.accept(BUILD_LESSTHAN_NODE, Stack, "NULL", "0"), runtime_error);
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