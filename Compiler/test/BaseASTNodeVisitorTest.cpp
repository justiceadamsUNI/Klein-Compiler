#include "catch.hpp"
#include "fakeit.hpp"
#include "../src/header/ASTNodeVisitor.h"

using namespace fakeit;

void mockVisitorAccept(Mock <ASTNodeVisitor>& MockVisitor, StackValues SemanticAction) {
	ASTNodeVisitor& AstNodeVisitorImpl = MockVisitor.get();
	PStack SemanticStack;
	AstNodeVisitorImpl.accept(SemanticAction, SemanticStack, "", 0);
}

TEST_CASE("Visitor calls visitProgramNode() when accept() called with BUILD_PROGRAM Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitProgramNode));

	mockVisitorAccept(MockVisitor, BUILD_PROGRAM_NODE);

	Verify(Method(MockVisitor, visitProgramNode));
}

TEST_CASE("Visitor calls visitDefinitionsNode() when accept() called with BUILD_DEFINITIONS_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitDefinitionsNode));

	mockVisitorAccept(MockVisitor, BUILD_DEFINITIONS_NODE);

	Verify(Method(MockVisitor, visitDefinitionsNode));
}

TEST_CASE("Visitor calls visitIdentifierNode() when accept() called with BUILD_IDENTIFIER_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitIdentifierNode));

	mockVisitorAccept(MockVisitor, BUILD_IDENTIFIER_NODE);

	Verify(Method(MockVisitor, visitIdentifierNode));
}

TEST_CASE("Visitor calls visitDefNode() when accept() called with BUILD_DEF_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitDefNode));

	mockVisitorAccept(MockVisitor, BUILD_DEF_NODE);

	Verify(Method(MockVisitor, visitDefNode));
}

TEST_CASE("Visitor calls visitFormalsNode() when accept() called with BUILD_FORMALS_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitFormalsNode));

	mockVisitorAccept(MockVisitor, BUILD_FORMALS_NODE);

	Verify(Method(MockVisitor, visitFormalsNode));
}

TEST_CASE("Visitor calls visitNonEmptyFormalsNode() when accept() called with BUILD_NONEMPTYFORMALS_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitNonEmptyFormalsNode));

	mockVisitorAccept(MockVisitor, BUILD_NONEMPTYFORMALS_NODE);

	Verify(Method(MockVisitor, visitNonEmptyFormalsNode));
}

TEST_CASE("Visitor calls visitFormalNode() when accept() called with BUILD_FORMAL_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitFormalNode));

	mockVisitorAccept(MockVisitor, BUILD_FORMAL_NODE);

	Verify(Method(MockVisitor, visitFormalNode));
}

TEST_CASE("Visitor calls visitBodyNode() when accept() called with BUILD_BODY_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitBodyNode));

	mockVisitorAccept(MockVisitor, BUILD_BODY_NODE);

	Verify(Method(MockVisitor, visitBodyNode));
}

TEST_CASE("Visitor calls visitTypeNode() when accept() called with BUILD_TYPE_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitTypeNode));

	mockVisitorAccept(MockVisitor, BUILD_TYPE_NODE);

	Verify(Method(MockVisitor, visitTypeNode));
}

TEST_CASE("Visitor calls visitLessThanNode() when accept() called with BUILD_LESSTHAN_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitLessThanNode));

	mockVisitorAccept(MockVisitor, BUILD_LESSTHAN_NODE);

	Verify(Method(MockVisitor, visitLessThanNode));
}

TEST_CASE("Visitor calls visitEqualNode() when accept() called with BUILD_EQUAL_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitEqualNode));

	mockVisitorAccept(MockVisitor, BUILD_EQUAL_NODE);

	Verify(Method(MockVisitor, visitEqualNode));
}

TEST_CASE("Visitor calls visitBaseExpressionNode() when accept() called with BUILD_BASEEXPR_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitBaseExpressionNode));

	mockVisitorAccept(MockVisitor, BUILD_BASEEXPR_NODE);

	Verify(Method(MockVisitor, visitBaseExpressionNode));
}

TEST_CASE("Visitor calls visitOrNode() when accept() called with BUILD_OR_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitOrNode));

	mockVisitorAccept(MockVisitor, BUILD_OR_NODE);

	Verify(Method(MockVisitor, visitOrNode));
}

TEST_CASE("Visitor calls visitAdditionNode() when accept() called with BUILD_ADDITION_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitAdditionNode));

	mockVisitorAccept(MockVisitor, BUILD_ADDITION_NODE);

	Verify(Method(MockVisitor, visitAdditionNode));
}

TEST_CASE("Visitor calls visitSubtractionNode() when accept() called with BUILD_SUBTRACTION_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitSubtractionNode));

	mockVisitorAccept(MockVisitor, BUILD_SUBTRACTION_NODE);

	Verify(Method(MockVisitor, visitSubtractionNode));
}

TEST_CASE("Visitor calls visitBaseSimpleExpressionNode() when accept() called with BUILD_BASESIMPLEEXPR_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitBaseSimpleExpressionNode));

	mockVisitorAccept(MockVisitor, BUILD_BASESIMPLEEXPR_NODE);

	Verify(Method(MockVisitor, visitBaseSimpleExpressionNode));
}

TEST_CASE("Visitor calls visitAndNode() when accept() called with BUILD_AND_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitAndNode));

	mockVisitorAccept(MockVisitor, BUILD_AND_NODE);

	Verify(Method(MockVisitor, visitAndNode));
}

TEST_CASE("Visitor calls visitMultiplicatorNode() when accept() called with BUILD_MULTIPLICATOR_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitMultiplicatorNode));

	mockVisitorAccept(MockVisitor, BUILD_MULTIPLICATOR_NODE);

	Verify(Method(MockVisitor, visitMultiplicatorNode));
}

TEST_CASE("Visitor calls visitDividerNode() when accept() called with BUILD_MULTIPLICATOR_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitDividerNode));

	mockVisitorAccept(MockVisitor, BUILD_DIVIDER_NODE);

	Verify(Method(MockVisitor, visitDividerNode));
}

TEST_CASE("Visitor calls visitBaseTermNode() when accept() called with BUILD_BASETERM_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitBaseTermNode));

	mockVisitorAccept(MockVisitor, BUILD_BASETERM_NODE);

	Verify(Method(MockVisitor, visitBaseTermNode));
}

TEST_CASE("Visitor calls visitIfFactorNode() when accept() called with BUILD_IFFACTOR_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitIfFactorNode));

	mockVisitorAccept(MockVisitor, BUILD_IFFACTOR_NODE);

	Verify(Method(MockVisitor, visitIfFactorNode));
}

TEST_CASE("Visitor calls visitNotFactorNode() when accept() called with BUILD_NOTFACTOR_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitNotFactorNode));

	mockVisitorAccept(MockVisitor, BUILD_NOTFACTOR_NODE);

	Verify(Method(MockVisitor, visitNotFactorNode));
}

TEST_CASE("Visitor calls visitLiteralFactorNode() when accept() called with BUILD_LITERALFACTOR_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitLiteralFactorNode));

	mockVisitorAccept(MockVisitor, BUILD_LITERALFACTOR_NODE);

	Verify(Method(MockVisitor, visitLiteralFactorNode));
}

TEST_CASE("Visitor calls visitSubtractionFactorNode() when accept() called with BUILD_SUBTRACTIONFACTOR_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitSubtractionFactorNode));

	mockVisitorAccept(MockVisitor, BUILD_SUBTRACTIONFACTOR_NODE);

	Verify(Method(MockVisitor, visitSubtractionFactorNode));
}

TEST_CASE("Visitor calls visitParenthesisedExpressionNode() when accept() called with BUILD_PARENEXPR_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitParenthesisedExpressionNode));

	mockVisitorAccept(MockVisitor, BUILD_PARENEXPR_NODE);

	Verify(Method(MockVisitor, visitParenthesisedExpressionNode));
}

TEST_CASE("Visitor calls visitIdentifierActualsNode() when accept() called with BUILD_IDENTIFIERACTUALS_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitIdentifierActualsNode));

	mockVisitorAccept(MockVisitor, BUILD_IDENTIFIERACTUALS_NODE);

	Verify(Method(MockVisitor, visitIdentifierActualsNode));
}

TEST_CASE("Visitor calls visitSingletonIdentifierFactorNode() when accept() called with BUILD_SINGLETONIDENTIFIERFACTOR_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitSingletonIdentifierFactorNode));

	mockVisitorAccept(MockVisitor, BUILD_SINGLETONIDENTIFIERFACTOR_NODE);

	Verify(Method(MockVisitor, visitSingletonIdentifierFactorNode));
}

TEST_CASE("Visitor calls visitBaseActualsNode() when accept() called with BUILD_BASEACTUALS_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitBaseActualsNode));

	mockVisitorAccept(MockVisitor, BUILD_BASEACTUALS_NODE);

	Verify(Method(MockVisitor, visitBaseActualsNode));
}

TEST_CASE("Visitor calls visitNonBaseActualsNode() when accept() called with BUILD_NONBASEACTUALS_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitNonBaseActualsNode));

	mockVisitorAccept(MockVisitor, BUILD_NONBASEACTUALS_NODE);

	Verify(Method(MockVisitor, visitNonBaseActualsNode));
}

TEST_CASE("Visitor calls visitNonEmptyActualsNode() when accept() called with BUILD_NONEMPTYACTUALS_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitNonEmptyActualsNode));

	mockVisitorAccept(MockVisitor, BUILD_NONEMPTYACTUALS_NODE);

	Verify(Method(MockVisitor, visitNonEmptyActualsNode));
}

TEST_CASE("Visitor calls visitIntegerLiteralNode() when accept() called with BUILD_INTEGERLITERAL_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitIntegerLiteralNode));

	mockVisitorAccept(MockVisitor, BUILD_INTEGERLITERAL_NODE);

	Verify(Method(MockVisitor, visitIntegerLiteralNode));
}

TEST_CASE("Visitor calls visitBooleanLiteralNode() when accept() called with BUILD_BOOLEANLITERAL_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitBooleanLiteralNode));

	mockVisitorAccept(MockVisitor, BUILD_BOOLEANLITERAL_NODE);

	Verify(Method(MockVisitor, visitBooleanLiteralNode));
}

TEST_CASE("Visitor calls visitPrintStatementNode() when accept() called with BUILD_PRINTSTATEMENT_NODE Stack Value", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitPrintStatementNode));

	mockVisitorAccept(MockVisitor, BUILD_PRINTSTATEMENT_NODE);

	Verify(Method(MockVisitor, visitPrintStatementNode));
}

TEST_CASE("Visitor throws error when accept() called with a value that isn't a semantic action", "[Base AST Node Visitor]") {
	Mock <ASTNodeVisitor> MockVisitor;
	Fake(Method(MockVisitor, visitPrintStatementNode));

	REQUIRE_THROWS_AS(mockVisitorAccept(MockVisitor, INTEGER_DATA_TYPE), runtime_error);
}