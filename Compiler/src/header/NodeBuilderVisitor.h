#pragma once
#include "../header/ASTNodeVisitor.h"
#include "../header/ASTNodes.h"
#include <stdexcept>
#include <iostream>
#include <typeinfo>
#include <algorithm>

using namespace std;
class NodeBuilderVisitor : public ASTNodeVisitor {
public:
	vector<ASTNode> VectorOfASTNodes;

	NodeBuilderVisitor() {
		// We need this so that pointers don't get destroyed within the AST Nodes
		// (AST Nodes hold pointers to other AST Nodes)
		VectorOfASTNodes.reserve(10000);
	}

	virtual void visitProgramNode(SemanticStack& SemanticStack) {
		// Program node consist of a definitions node

		ASTNode ProgramNodeVar(PROGRAM_NODE_TYPE);
		if (SemanticStack.top().getAstNodeType() == DEFINITIONS_NODE_TYPE)
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			ProgramNodeVar.setDefinitionsNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Program Node, but didn't find Definitions Node on stack");
		}

		SemanticStack.push(ProgramNodeVar);
	}

	virtual void visitDefinitionsNode(SemanticStack& SemanticStack) {
		// Definitions node consist of def nodes
		
		ASTNode DefinitionsNodeVar(DEFINITIONS_NODE_TYPE);
		while (!SemanticStack.isEmpty())
		{
			if (SemanticStack.top().getAstNodeType() == DEF_NODE_TYPE)
			{
				ASTNode StackTop = SemanticStack.pop();
				VectorOfASTNodes.push_back(StackTop);
				DefinitionsNodeVar.addDefToVector(&VectorOfASTNodes.back());
				continue;
			}
			else {
				// We can have 0 def nodes
				break;
			}
		}
	
		SemanticStack.push(DefinitionsNodeVar);
	}

	virtual void visitIdentifierNode(SemanticStack& SemanticStack, string IdentifierName) {
		// Identifier node consist of string literal value
		ASTNode IdentifierNodeVar(IDENTIFIER_NODE_TYPE);
		IdentifierNodeVar.setIdentifierName(IdentifierName);

		SemanticStack.push(IdentifierNodeVar);
	}

	virtual void visitDefNode(SemanticStack& SemanticStack) {
		// Def node consist of Body, Type, Formals, and Type nodes
		ASTNode DefNodeVar(DEF_NODE_TYPE);

		if (SemanticStack.top().getAstNodeType() == BODY_NODE_TYPE)
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			DefNodeVar.setBodyNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Def Node, but didn't find Body Node on stack");
		}
		if (SemanticStack.top().getAstNodeType() == TYPE_NODE_TYPE)
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			DefNodeVar.setTypeNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Def Node, but didn't find Type Node on stack");
		}
		if (SemanticStack.top().isFormalsNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			DefNodeVar.setFormalsNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Def Node, but didn't find Formals Node on stack");
		}
		if (SemanticStack.top().getAstNodeType() == IDENTIFIER_NODE_TYPE)
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			DefNodeVar.setIdentifierNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Def Node, but didn't find Identifier Node on stack");
		}
	
		SemanticStack.push(DefNodeVar);
	}

	virtual void visitFormalsNode(SemanticStack& SemanticStack) {
		// Represents epsilon case. Still a valid formals node although it contains nothing
		ASTNode FormalsNodeVar(FORMALS_NODE_TYPE);
		SemanticStack.push(FormalsNodeVar);
	}
	

	virtual void visitNonEmptyFormalsNode(SemanticStack& SemanticStack) {
		// Non empty formals node consist of Formal nodes.
		ASTNode NonEmptyFormalsNodeVar(NON_EMPTY_FORMALS_NODE_TYPE);
		while (!SemanticStack.isEmpty())
		{
			if (SemanticStack.top().getAstNodeType() == FORMAL_NODE_TYPE)
			{
				ASTNode StackTop = SemanticStack.pop();
				VectorOfASTNodes.push_back(StackTop);
				NonEmptyFormalsNodeVar.addFormalNodeToVector(&VectorOfASTNodes.back());
				continue;
			}
			else {
				// Exit while loop
				break;
			}
		}

		SemanticStack.push(NonEmptyFormalsNodeVar);
	}

	virtual void visitFormalNode(SemanticStack& SemanticStack) {
		// Formal Node consist of type node and identifier node
		ASTNode FormalNodeVar(FORMAL_NODE_TYPE);
		if (SemanticStack.top().getAstNodeType() == TYPE_NODE_TYPE)
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			FormalNodeVar.setTypeNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Formal Node, but didn't find Type Node on stack");
		}
		if (SemanticStack.top().getAstNodeType() == IDENTIFIER_NODE_TYPE)
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			FormalNodeVar.setIdentifierNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Formal Node, but didn't find Identifier Node on stack");
		}

		SemanticStack.push(FormalNodeVar);
	}

	virtual void visitBodyNode(SemanticStack& SemanticStack) {
		// Body node consist of 0 or more print statements and an expresison

		ASTNode BodyNodeVar(BODY_NODE_TYPE);
		if (SemanticStack.top().isExpressionNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			BodyNodeVar.setBaseExprNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Body Node, but didn't find Expr Node on stack");
		}

		while (!SemanticStack.isEmpty())
		{
			if (SemanticStack.top().getAstNodeType() == PRINT_STATEMENT_NODE_TYPE)
			{
				ASTNode StackTop = SemanticStack.pop();
				VectorOfASTNodes.push_back(StackTop);
				BodyNodeVar.addPrintStatementToVector(&VectorOfASTNodes.back());
				continue;
			}
			else {
				// Exit while loop (Can have 0 print statments)
				break;
			}
		}

		SemanticStack.push(BodyNodeVar);
	}

	virtual void visitTypeNode(SemanticStack& SemanticStack, string Type) {
		// Type node consist of string literal data type
		ASTNode TypeNodeVar(TYPE_NODE_TYPE);
		TypeNodeVar.setDataType(Type);

		SemanticStack.push(TypeNodeVar);
	}

	virtual void visitLessThanNode(SemanticStack& SemanticStack) {
		// Less than node consist of two simple expression nodes
		ASTNode LessThanExprNodeVar(LESS_THAN_EXPR_NODE_TYPE);

		if (SemanticStack.top().isSimpleExpressionNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			LessThanExprNodeVar.setBaseSimpleExprNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Less Than Node, but didn't find (First) Simple Expression Node on stack");
		}
		if (SemanticStack.top().isSimpleExpressionNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			LessThanExprNodeVar.setBaseSimpleExprNode2(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Less Than Node, but didn't find (Second) Simple Expression Node on stack");
		}

		SemanticStack.push(LessThanExprNodeVar);
	}

	virtual void visitEqualNode(SemanticStack& SemanticStack) {
		// Equak node consist of two simple expression nodes
		ASTNode EqualNode(EQUAL_TO_EXPR_NODE_TYPE);

		if (SemanticStack.top().isSimpleExpressionNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			EqualNode.setBaseSimpleExprNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Equal Node, but didn't find (First) Simple Expression Node on stack");
		}
		if (SemanticStack.top().isSimpleExpressionNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			EqualNode.setBaseSimpleExprNode2(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Equal Node, but didn't find (Second) Simple Expression Node on stack");
		}

		SemanticStack.push(EqualNode);
	}

	virtual void visitBaseExpressionNode(SemanticStack& SemanticStack) {
		// Base Expression node consist of simple expression node

		// For run on scenarios
		if (SemanticStack.top().isExpressionNode())
		{
			return;
		}

		ASTNode BaseExpression(BASE_EXPR_NODE_TYPE);
		if (SemanticStack.top().isSimpleExpressionNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			BaseExpression.setBaseSimpleExprNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Base Expression Node, but didn't find Simple Expression Node on stack");
		}

		SemanticStack.push(BaseExpression);
	}

	virtual void visitOrNode(SemanticStack& SemanticStack) {
		// Or node consist of a term and one of {term, simple expression}
		ASTNode OrNode(OR_SIMPLE_EXPR_NODE_TYPE);

		if (SemanticStack.top().isTermNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			OrNode.setBaseTermNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Or Simple Expr Node, but didn't find (first) Term Node on stack");
		}
		//  What follows could be another Or node if it's a run on Or scenario
		if (SemanticStack.top().getAstNodeType() == OR_SIMPLE_EXPR_NODE_TYPE)
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			OrNode.setBaseSimpleExprNode(&VectorOfASTNodes.back());
		}
		else if (SemanticStack.top().isTermNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			OrNode.setBaseTermNode2(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Or Simple Expr Node, but didn't find (second) Term Node on stack");
		}

		SemanticStack.push(OrNode);
	}

	virtual void visitAdditionNode(SemanticStack& SemanticStack) {
		// Addition node consist of a term node and one of {term, simple expression}
		ASTNode AdditionNode(ADDITION_SIMPLE_EXPR_NODE_TYPE);

		if (SemanticStack.top().isTermNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			AdditionNode.setBaseTermNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Addition Node, but didn't find (first) Term Node on stack");
		}

		// What follows could be another arithmetic based node if it's a run on arithmetic operator scenario
		if (SemanticStack.top().isArithmeticSimpleExpression())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			AdditionNode.setBaseSimpleExprNode(&VectorOfASTNodes.back());
		}
		else if (SemanticStack.top().isTermNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			AdditionNode.setBaseTermNode2(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Addition Node, but didn't find (second) Term Node/arithmetic Node on stack");
		}

		SemanticStack.push(AdditionNode);
	}

	virtual void visitSubtractionNode(SemanticStack& SemanticStack) {
		// Subtraction node consist of a term node and one of {term, simple expression}
		ASTNode SubtractionNode(SUBTRACTOR_SIMPLE_EXPR_NODE_TYPE);

		if (SemanticStack.top().isTermNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			SubtractionNode.setBaseTermNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Subtraction Node, but didn't find (first) Term Node on stack");
		}

		// What follows could be another arithmetic based node if it's a run on arithmetic operator scenario
		if (SemanticStack.top().isArithmeticSimpleExpression())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			SubtractionNode.setBaseSimpleExprNode(&VectorOfASTNodes.back());
		}
		else if (SemanticStack.top().isTermNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			SubtractionNode.setBaseTermNode2(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Subtraction Node, but didn't find (second) Term Node/arithmetic Node on stack");
		}

		SemanticStack.push(SubtractionNode);

	}

	virtual void visitBaseSimpleExpressionNode(SemanticStack& SemanticStack) {
		// Base Simple Expression consist of term node
		ASTNode BaseSimpleExprNode(BASE_SIMPLE_EXPR_NODE_TYPE);

		// For run on scenarios.
		if (SemanticStack.top().isSimpleExpressionNode())
		{
			return;
		}

		if (SemanticStack.top().isTermNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			BaseSimpleExprNode.setBaseTermNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Base Simple Expr Node, but didn't find Term Node on stack");
		}

		SemanticStack.push(BaseSimpleExprNode);
	}

	virtual void visitAndNode(SemanticStack& SemanticStack) {
		// And node consist of a factor node and one of {factor, term}
		ASTNode AndNode(AND_TERM_NODE_TYPE);

		if (SemanticStack.top().isFactorNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			AndNode.setFactorNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build And Node, but didn't find (first) Factor Node on stack");
		}

		// What follows could be another and node if it's a run on and scenario
		if (SemanticStack.top().getAstNodeType() == AND_TERM_NODE_TYPE)
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			AndNode.setBaseTermNode(&VectorOfASTNodes.back());
		}
		else if (SemanticStack.top().isFactorNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			AndNode.setFactorNode2(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build And Node, but didn't find (Second) Factor/ And Node on stack");
		}

		SemanticStack.push(AndNode);

	}

	virtual void visitMultiplicatorNode(SemanticStack& SemanticStack) {
		// Multiplication node consist of a factor node and one of {factor, simple expression, term}
		ASTNode MultiplicatorNode(MULTIPLICATOR_TERM_NODE_TYPE);

		if (SemanticStack.top().isFactorNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			MultiplicatorNode.setFactorNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Multiplication Node, but didn't find (first) Factor Node on stack");
		}

		// What follows could be another arithmetic based node if it's a run on arithmetic operator scenario
		if (SemanticStack.top().isArithmeticSimpleExpression())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			MultiplicatorNode.setBaseSimpleExprNode(&VectorOfASTNodes.back());
		}
		else if (SemanticStack.top().isArithmeticTerm())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			MultiplicatorNode.setBaseTermNode(&VectorOfASTNodes.back());
		}
		else if (SemanticStack.top().isFactorNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			MultiplicatorNode.setFactorNode2(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Multiplication Node, but didn't find (Second) Factor Node/arithmetic Node on stack");
		}

		SemanticStack.push(MultiplicatorNode);
	}

	virtual void visitDividerNode(SemanticStack& SemanticStack) {
		// Divider node consist of a factor node and one of {factor, simple expression, term}
		ASTNode DividerNode(DIVIDER_TERM_NODE_TYPE);

		if (SemanticStack.top().isFactorNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			DividerNode.setFactorNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Division Node, but didn't find (first) Factor Node on stack");
		}

		// What follows could be another arithmetic based node if it's a run on arithmetic operator scenario
		if (SemanticStack.top().isArithmeticSimpleExpression())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			DividerNode.setBaseSimpleExprNode(&VectorOfASTNodes.back());
		}
		else if (SemanticStack.top().isArithmeticTerm())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			DividerNode.setBaseTermNode(&VectorOfASTNodes.back());
		}
		else if (SemanticStack.top().isFactorNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			DividerNode.setFactorNode2(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Division Node, but didn't find (Second) Factor Node/arithmetic Node on stack");
		}

		SemanticStack.push(DividerNode);
	}

	virtual void visitBaseTermNode(SemanticStack& SemanticStack) {
		// Base Term node consist of factor node

		// Only want to happen if we're dealing with a term that hasn't already been built.
		// This is because we account for run-on operators.
		ASTNode BaseTermNode(BASE_TERM_NODE_TYPE);
		if (SemanticStack.top().isTermNode())
		{
			return;
		}
		if (SemanticStack.top().isFactorNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			BaseTermNode.setFactorNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Base Term Node, but didn't find Factor Node on stack");
		}

		SemanticStack.push(BaseTermNode);
	}

	virtual void visitIfFactorNode(SemanticStack& SemanticStack) {
		// If factor consist of three expression nodes
		ASTNode IfFactorNodeVar(IF_FACTOR_NODE_TYPE);
		if (SemanticStack.top().isExpressionNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			IfFactorNodeVar.setBaseExprNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build If Statement Node, but didn't find (first) Expression Node on stack");
		}
		if (SemanticStack.top().isExpressionNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			IfFactorNodeVar.setBaseExprNode2(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build If Statement Node, but didn't find (second) Expression Node on stack");
		}
		if (SemanticStack.top().isExpressionNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			IfFactorNodeVar.setBaseExprNode3(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build If Statement Node, but didn't find (third) Expression Node on stack");
		}

		SemanticStack.push(IfFactorNodeVar);
	}

	virtual void visitNotFactorNode(SemanticStack& SemanticStack) {
		// Not factor consist of a factor node

		ASTNode NotFactorNodeVar(NOT_FACTOR_NODE_TYPE);
		if (SemanticStack.top().isFactorNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			NotFactorNodeVar.setFactorNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Not Factor Node, but didn't find Factor Node on stack");
		}

		SemanticStack.push(NotFactorNodeVar);
	}

	virtual void visitLiteralFactorNode(SemanticStack& SemanticStack) {
		// Literal factor node consist of literal node
		ASTNode LiteralFactorNodeVar(LITERAL_FACTOR_NODE_TYPE);
		if (SemanticStack.top().isLiteralNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			LiteralFactorNodeVar.setLiteralNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Literal Factor Node, but didn't find Literal Node on stack");
		}

		SemanticStack.push(LiteralFactorNodeVar);
	}

	virtual void visitNegatedFactorNode(SemanticStack& SemanticStack) {
		// Negated Factor node consist of factor node
		ASTNode SubtractionFactorNode(SUBTRACTION_FACTOR_NODE_TYPE);
		if (SemanticStack.top().isFactorNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			SubtractionFactorNode.setFactorNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Subtraction Factor Node, but didn't find Factor Node on stack");
		}
	
		SemanticStack.push(SubtractionFactorNode);
	}

	virtual void visitParenthesisedExpressionNode(SemanticStack& SemanticStack) {
		// Parenthesised Expression node consist of expression node
		ASTNode ParenthesisedExpr(PARENTHESISED_EXPR_FACTOR_NODE_TYPE);
		if (SemanticStack.top().isExpressionNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			ParenthesisedExpr.setBaseExprNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Parenthesised Expression Node, but didn't find Factor Node on stack");
		}

		SemanticStack.push(ParenthesisedExpr);
	}

	virtual void visitFunctionCallNode(SemanticStack& SemanticStack) {
		// Function call node consist of actuals and an identifier
		ASTNode IdentifierActualsNode(FUNCTION_CALL_TYPE);
		if (SemanticStack.top().isActualsNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			IdentifierActualsNode.setBaseActualsNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Identifier Actuals Node, but didn't find Actuals Node on stack");
		}
		if (SemanticStack.top().getAstNodeType() == IDENTIFIER_NODE_TYPE)
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			IdentifierActualsNode.setIdentifierNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Identifier Actuals Node, but didn't find Identifier Node on stack");
		}

		SemanticStack.push(IdentifierActualsNode);
	}

	virtual void visitSingletonIdentifierFactorNode(SemanticStack& SemanticStack) {
		// Singleton Identifier Factor Node consist of identifier node
		ASTNode SingletonIdentifierNode(SINGLETON_IDENTIFIER_FACTOR_NODE_TYPE);
		if (SemanticStack.top().getAstNodeType() == IDENTIFIER_NODE_TYPE)
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			SingletonIdentifierNode.setIdentifierNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Singelton Identifier Factor Node, but didn't find Identifier Node on stack");
		}

		SemanticStack.push(SingletonIdentifierNode);
	}

	virtual void visitBaseActualsNode(SemanticStack& SemanticStack) {
		// Represent epsilon case. Contains Nothing
		ASTNode BaseActualsNode(BASE_ACTUALS_NODE_TYPE);

		SemanticStack.push(BaseActualsNode);
	}

	virtual void visitNonBaseActualsNode(SemanticStack& SemanticStack) {
		// Non Base Actuals node consist of non empty actuals node
		ASTNode NonBaseActualsNodeVar(NON_BASE_ACTUALS_NODE_TYPE);
		if (SemanticStack.top().getAstNodeType() == NON_EMPTY_ACTUALS_NODE_TYPE)
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			NonBaseActualsNodeVar.setNonEmptyActualsNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Non Base Actuals Node, but didn't find Non empty Actuals Node on stack");
		}

		SemanticStack.push(NonBaseActualsNodeVar);
	}

	virtual void visitNonEmptyActualsNode(SemanticStack& SemanticStack) {
		// Non empty actuals node consist of list of expression nodes

		//Only want to happen if we're dealing with the case that non empty actuals hasn't already been built.
		//This is because we account for run-on non empty actuals.
		if (SemanticStack.top().getAstNodeType() == NON_EMPTY_ACTUALS_NODE_TYPE)
		{
			return;
		}
		ASTNode NonEmptyActualsNodeVar(NON_EMPTY_ACTUALS_NODE_TYPE);
		while (!SemanticStack.isEmpty())
		{
			if (SemanticStack.top().isExpressionNode())
			{
				ASTNode StackTop = SemanticStack.pop();
				VectorOfASTNodes.push_back(StackTop);
				NonEmptyActualsNodeVar.addExpressionToVector(&VectorOfASTNodes.back());
				continue;
			}
			else {
				//Exit while loop
				break;
			}
		}

		SemanticStack.push(NonEmptyActualsNodeVar);
	}

	virtual void visitIntegerLiteralNode(SemanticStack& SemanticStack, string IntData) {
		// Integer literal node consist of string literal value
		ASTNode IntegerLiteralNode(INTEGER_LITERAL_NODE_TYPE);
		IntegerLiteralNode.setLiteralValue(IntData);

		SemanticStack.push(IntegerLiteralNode);
	}

	virtual void visitBooleanLiteralNode(SemanticStack& SemanticStack, string BooleanValue) {
		// Boolean literal node consist of string literal value
		ASTNode BooleanLiteralNode(BOOLEAN_LITERAL_NODE_TYPE);
		BooleanLiteralNode.setLiteralValue(BooleanValue);

		SemanticStack.push(BooleanLiteralNode);
	}

	virtual void visitPrintStatementNode(SemanticStack& SemanticStack) {
		// Print statement node consist of expression node
		ASTNode PrintStatementNode(PRINT_STATEMENT_NODE_TYPE);
		if (SemanticStack.top().isExpressionNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			PrintStatementNode.setBaseExprNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Print Statement Node, but didn't find Expression Node on stack");
		}

		SemanticStack.push(PrintStatementNode);
	}
};