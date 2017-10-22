#include "catch.hpp"
#include "../src/header/ASTNodes.h"

TEST_CASE("ASTNode default values represent empty state correctly", "[ASTNodes]") {
	ASTNode Node(ProgramNodeTYPE);

	REQUIRE(Node.getIdentifierName() == "NULL");
	REQUIRE(Node.getLiteralValue() == "NULL");
	REQUIRE(Node.getDataType() == "NULL");
	REQUIRE(Node.getFactorNode() == nullptr);
	REQUIRE(Node.getFactorNode2() == nullptr);
	REQUIRE(Node.getBaseTermNode() == nullptr);
	REQUIRE(Node.getBaseTermNode2() == nullptr);
	REQUIRE(Node.getBaseSimpleExprNode() == nullptr);
	REQUIRE(Node.getBaseSimpleExprNode2() == nullptr);
	REQUIRE(Node.getBaseExprNode() == nullptr);
	REQUIRE(Node.getBaseExprNode2() == nullptr);
	REQUIRE(Node.getBaseExprNode3() == nullptr);
	REQUIRE(Node.getLiteralNode() == nullptr);
	REQUIRE(Node.getIdentifierNode() == nullptr);
	REQUIRE(Node.getBaseActualsNode() == nullptr);
	REQUIRE(Node.getNonEmptyActualsNode() == nullptr);
	REQUIRE(Node.getTypeNode() == nullptr);
	REQUIRE(Node.getFormalsNode() == nullptr);
	REQUIRE(Node.getBodyNode() == nullptr);
	REQUIRE(Node.getExpressions().empty() == true);
	REQUIRE(Node.getPrintStatements().empty() == true);
	REQUIRE(Node.getFormalNodes().empty() == true);
	REQUIRE(Node.getDefNodes().empty() == true);
	REQUIRE(Node.getDefinitions().empty() == true);
}

TEST_CASE("ASTNode getAstNodeType() returns Type correctly", "[ASTNodes]") {
	ASTNode Node(ProgramNodeTYPE);

	REQUIRE(Node.getAstNodeType() == ProgramNodeTYPE);
}

TEST_CASE("ASTNode setIdentifierName() and getIdentifierName() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode Node(ProgramNodeTYPE);
	Node.setIdentifierName("test");

	REQUIRE(Node.getIdentifierName() == "test");
}

TEST_CASE("ASTNode setFactorNode() and getFactorNode() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(AndTermNodeType);
	ASTNode FactorNode(FactorNodeTYPE);
	FactorNode.setIdentifierName("test");
	TestNode.setFactorNode(&FactorNode);

	REQUIRE(TestNode.getFactorNode()->getIdentifierName() == "test");
}

TEST_CASE("ASTNode setFactorNode2() and getFactorNode2() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(AndTermNodeType);
	ASTNode FactorNode(FactorNodeTYPE);
	FactorNode.setIdentifierName("test");
	TestNode.setFactorNode2(&FactorNode);

	REQUIRE(TestNode.getFactorNode2()->getIdentifierName() == "test");
}

TEST_CASE("ASTNode setBaseTermNode() and getBaseTermNode() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(MultiplicatorTermNodeTYPE);
	ASTNode TermNode(BaseTermNodeTYPE);
	TermNode.setIdentifierName("test");
	TestNode.setBaseTermNode(&TermNode);

	REQUIRE(TestNode.getBaseTermNode()->getIdentifierName() == "test");
}

TEST_CASE("ASTNode setBaseSimpleExprNode() and getBaseSimpleExprNode() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(LessThanExprNodeTYPE);
	ASTNode SimpleExprNode(BaseSimpleExprNodeType);
	SimpleExprNode.setIdentifierName("test");
	TestNode.setBaseSimpleExprNode(&SimpleExprNode);

	REQUIRE(TestNode.getBaseSimpleExprNode()->getIdentifierName() == "test");
}

TEST_CASE("ASTNode setBaseExprNode() and getBaseExprNode() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(IfFactorNodeTYPE);
	ASTNode BaseExprNode(BaseExprNodeType);
	BaseExprNode.setIdentifierName("test");
	TestNode.setBaseExprNode(&BaseExprNode);

	REQUIRE(TestNode.getBaseExprNode()->getIdentifierName() == "test");
}

TEST_CASE("ASTNode setLiteralVale() and getLiteralValue() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(LiteralNodeTYPE);
	TestNode.setLiteralValue("test");

	REQUIRE(TestNode.getLiteralValue() == "test");
}

TEST_CASE("ASTNode addExpressionToVector() and getExpressions() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(NonEmptyActualsNodeTYPE);
	ASTNode BaseExprNode(BaseExprNodeType);
	BaseExprNode.setLiteralValue("test");
	TestNode.addExpressionToVector(&BaseExprNode);

	REQUIRE(TestNode.getExpressions().size() == 1);
	REQUIRE(TestNode.getExpressions()[0]->getLiteralValue()== "test");
}

TEST_CASE("ASTNode setNonEmptyActualsNode() and getNonEmptyActualsNode() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(NonBaseActualsNodeTYPE);
	ASTNode NonEmptyActuals(NonEmptyActualsNodeTYPE);
	NonEmptyActuals.setLiteralValue("test");
	TestNode.setNonEmptyActualsNode(&NonEmptyActuals);

	REQUIRE(TestNode.getNonEmptyActualsNode()->getLiteralValue() == "test");
}

TEST_CASE("ASTNode setLiteralNode() and getLiteralNode() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(LiteralFactorNodeTYPE);
	ASTNode LiteralNode(LiteralNodeTYPE);
	LiteralNode.setLiteralValue("test");
	TestNode.setLiteralNode(&LiteralNode);

	REQUIRE(TestNode.getLiteralNode()->getLiteralValue() == "test");
}

TEST_CASE("ASTNode setIdentifierNode() and getIdentifierNode() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(DefNodeTYPE);
	ASTNode IdentifierFactor(IdentifierFactorNodeTYPE);
	IdentifierFactor.setLiteralValue("test");
	TestNode.setIdentifierNode(&IdentifierFactor);

	REQUIRE(TestNode.getIdentifierNode()->getLiteralValue() == "test");
}

TEST_CASE("ASTNode setBaseActualsNode() and getBaseActualsNode() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(IdentifierFactorNodeTYPE);
	ASTNode BaseActuals(BaseActualsNodeTYPE);
	BaseActuals.setLiteralValue("test");
	TestNode.setBaseActualsNode(&BaseActuals);

	REQUIRE(TestNode.getBaseActualsNode()->getLiteralValue() == "test");
}

TEST_CASE("ASTNode setBaseExprNode2() and getBaseExprNode2() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(IfFactorNodeTYPE);
	ASTNode BaseExpr(BaseExprNodeType);
	BaseExpr.setLiteralValue("test");
	TestNode.setBaseExprNode2(&BaseExpr);

	REQUIRE(TestNode.getBaseExprNode2()->getLiteralValue() == "test");
}

TEST_CASE("ASTNode setBaseExprNode3() and getBaseExprNode3() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(IfFactorNodeTYPE);
	ASTNode BaseExpr(BaseExprNodeType);
	BaseExpr.setLiteralValue("test");
	TestNode.setBaseExprNode3(&BaseExpr);

	REQUIRE(TestNode.getBaseExprNode3()->getLiteralValue() == "test");
}

TEST_CASE("ASTNode setBaseTermNode2() and getBaseTermNode2() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(MultiplicatorTermNodeTYPE);
	ASTNode TermNode(BaseTermNodeTYPE);
	TermNode.setIdentifierName("test");
	TestNode.setBaseTermNode2(&TermNode);

	REQUIRE(TestNode.getBaseTermNode2()->getIdentifierName() == "test");
}

TEST_CASE("ASTNode setBaseSimpleExprNode2() and getBaseSimpleExprNode2() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(LessThanExprNodeTYPE);
	ASTNode SimpleExprNode(BaseSimpleExprNodeType);
	SimpleExprNode.setIdentifierName("test");
	TestNode.setBaseSimpleExprNode2(&SimpleExprNode);

	REQUIRE(TestNode.getBaseSimpleExprNode2()->getIdentifierName() == "test");
}

TEST_CASE("ASTNode setDataType() and getDataType() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode Node(TypeNodeTYPE);
	Node.setDataType("test");

	REQUIRE(Node.getDataType() == "test");
}

TEST_CASE("ASTNode setTypeNode() and geTypeNode() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(DefNodeTYPE);
	ASTNode TypeNode(TypeNodeTYPE);
	TypeNode.setIdentifierName("test");
	TestNode.setTypeNode(&TypeNode);

	REQUIRE(TestNode.getTypeNode()->getIdentifierName() == "test");
}

TEST_CASE("ASTNode getPrintStatements() and addPrintStatementToVector() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(BodyNodeTYPE);
	ASTNode PrintStatement(PrintStatemetNodeTYPE);
	PrintStatement.setLiteralValue("test");
	TestNode.addPrintStatementToVector(&PrintStatement);

	REQUIRE(TestNode.getPrintStatements().size() == 1);
	REQUIRE(TestNode.getPrintStatements()[0]->getLiteralValue() == "test");
}

TEST_CASE("ASTNode getFormalNodes() and addFormalNodeToVector() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(DefNodeTYPE);
	ASTNode Formal(FormalNodeTYPE);
	Formal.setLiteralValue("test");
	TestNode.addFormalNodeToVector(&Formal);

	REQUIRE(TestNode.getFormalNodes().size() == 1);
	REQUIRE(TestNode.getFormalNodes()[0]->getLiteralValue() == "test");
}

TEST_CASE("ASTNode getDefNodes() and addDefToVector() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(DefinitionsNodeTYPE);
	ASTNode DefNode(DefNodeTYPE);
	DefNode.setLiteralValue("test");
	TestNode.addDefToVector(&DefNode);

	REQUIRE(TestNode.getDefNodes().size() == 1);
	REQUIRE(TestNode.getDefNodes()[0]->getLiteralValue() == "test");
}

TEST_CASE("ASTNode getDefinitions() and addDefinitionToVector() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(ProgramNodeTYPE);
	ASTNode Definition(DefinitionsNodeTYPE);
	Definition.setLiteralValue("test");
	TestNode.addDefinitionToVector(&Definition);

	REQUIRE(TestNode.getDefinitions().size() == 1);
	REQUIRE(TestNode.getDefinitions()[0]->getLiteralValue() == "test");
}

TEST_CASE("ASTNode setFormalsNode() and getFormalsNode() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(DefNodeTYPE);
	ASTNode Formals(FormalsNodeType);
	Formals.setLiteralValue("test");
	TestNode.setFormalsNode(&Formals);

	REQUIRE(TestNode.getFormalsNode()->getLiteralValue() == "test");
}

TEST_CASE("ASTNode setBodyNode() and getBodyNode() updates/represents internal node state correctly", "[ASTNodes]") {
	ASTNode TestNode(DefNodeTYPE);
	ASTNode Body(BodyNodeTYPE);
	Body.setLiteralValue("test");
	TestNode.setBodyNode(&Body);

	REQUIRE(TestNode.getBodyNode()->getLiteralValue() == "test");
}