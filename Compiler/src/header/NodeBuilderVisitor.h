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
	NodeBuilderVisitor() {
		// We need this so that pointers don't get destroyed within the AST Nodes
		// (AST Nodes hold pointers to other AST Nodes). It's sorta ugly, but it works :)
		VectorOfASTNodes.reserve(10000);
	}
	vector<ASTNode> VectorOfASTNodes;

	virtual void visitProgramNode(SemanticStack& SemanticStack) {
		// pop off definitions node from the stack and store it inside a program node.
		// Then push that program node back onto the stack.

		ASTNode ProgramNodeVar(ProgramNodeTYPE);
		if (SemanticStack.top().getAstNodeType() == DefinitionsNodeTYPE)
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
		// pop off every def node from the stack and store it inside a definitions node.
		// Then push that definitions node back onto the stack.
		
		ASTNode DefinitionsNodeVar(DefinitionsNodeTYPE);
		while (!SemanticStack.isEmpty())
		{
			if (SemanticStack.top().getAstNodeType() == DefNodeTYPE)
			{
				ASTNode StackTop = SemanticStack.pop();
				VectorOfASTNodes.push_back(StackTop);
				DefinitionsNodeVar.addDefToVector(&VectorOfASTNodes.back());
				continue;
			}
			else {
				//Exit while loop
				break;
			}
		}

	
		SemanticStack.push(DefinitionsNodeVar);

	}

	virtual void visitIdentifierNode(SemanticStack& SemanticStack, string IdentifierName) {
		// store the identifier name inside the identifier node, then push the identifier node onto the stack.
		ASTNode IdentifierNodeVar(IDENTIFIERNODETYPE);
		IdentifierNodeVar.setIdentifierName(IdentifierName);

		SemanticStack.push(IdentifierNodeVar);
	}

	virtual void visitDefNode(SemanticStack& SemanticStack) {
		ASTNode DefNodeVar(DefNodeTYPE);

		if (SemanticStack.top().getAstNodeType() == BodyNodeTYPE)
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			DefNodeVar.setBodyNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Def Node, but didn't find Body Node on stack");
		}

		if (SemanticStack.top().getAstNodeType() == TypeNodeTYPE)
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

		if (SemanticStack.top().getAstNodeType() == IDENTIFIERNODETYPE)
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
		//represents epsilon case. still a valid formals node.
		ASTNode FormalsNodeVar(FormalsNodeType);

		SemanticStack.push(FormalsNodeVar);
	}
	

	virtual void visitNonEmptyFormalsNode(SemanticStack& SemanticStack) {
		// pop off every Formal node from the stack and store it inside a Non Empty Formals Node.
		// Then push that Non Empty Formals Node node back onto the stack.

		ASTNode NonEmptyFormalsNodeVar(NonEmptyFormalsNodeTYPE);
		while (!SemanticStack.isEmpty())
		{
			if (SemanticStack.top().getAstNodeType() == FormalNodeTYPE)
			{
				ASTNode StackTop = SemanticStack.pop();
				VectorOfASTNodes.push_back(StackTop);
				NonEmptyFormalsNodeVar.addFormalNodeToVector(&VectorOfASTNodes.back());
				continue;
			}
			else {
				//Exit while loop
				break;
			}
		}

		SemanticStack.push(NonEmptyFormalsNodeVar);
	}

	virtual void visitFormalNode(SemanticStack& SemanticStack) {
		// pop off identifier node and store it in Formal node.
		// pop off type node and store it in Formal node.

		// push Formal Node onto stack.
		ASTNode FormalNodeVar(FormalNodeTYPE);
		ASTNode Type(TypeNodeTYPE);
		ASTNode Identifier(IDENTIFIERNODETYPE);
		if (SemanticStack.top().getAstNodeType() == TypeNodeTYPE)
		{
			ASTNode StackTop = SemanticStack.pop();
			Type = StackTop;
			VectorOfASTNodes.push_back(Type);
			FormalNodeVar.setTypeNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Formal Node, but didn't find Type Node on stack");
		}

		if (SemanticStack.top().getAstNodeType() == IDENTIFIERNODETYPE)
		{
			ASTNode StackTop = SemanticStack.pop();
			Identifier = StackTop;
			VectorOfASTNodes.push_back(Identifier);
			FormalNodeVar.setIdentifierNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Formal Node, but didn't find Identifier Node on stack");
		}

		SemanticStack.push(FormalNodeVar);
	}

	virtual void visitBodyNode(SemanticStack& SemanticStack) {
		// pop off list of print statment nodes and store it in Body node.
		// pop off expr node and store it in Body node.

		ASTNode BodyNodeVar(BodyNodeTYPE);
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
			if (SemanticStack.top().getAstNodeType() == PrintStatemetNodeTYPE)
			{
				ASTNode StackTop = SemanticStack.pop();
				VectorOfASTNodes.push_back(StackTop);
				BodyNodeVar.addPrintStatementToVector(&VectorOfASTNodes.back());
				continue;
			}
			else {
				//Exit while loop
				break;
			}
		}

		SemanticStack.push(BodyNodeVar);
	}

	virtual void visitTypeNode(SemanticStack& SemanticStack, string Type) {
		// store Type string inside the Type Node.
		// push Type node onto the stack
		
		ASTNode TypeNodeVar(TypeNodeTYPE);
		TypeNodeVar.setDataType(Type);

		SemanticStack.push(TypeNodeVar);
	}

	virtual void visitLessThanNode(SemanticStack& SemanticStack) {
		// pop off simple expression node and store it in Less than node.
		// pop off simple expression node and store it in Less than node.

		// push less than Node onto stack.

		ASTNode LessThanExprNodeVar(LessThanExprNodeTYPE);

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
		// pop off simple expression node and store it in equal than node.
		// pop off simple expression node and store it in equal than node.

		// pushequal Node onto stack.
		ASTNode EqualNode(EqualToExprNodeTYPE);

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
		// pop off simple expression node and store it in base expression node
		// push Base expression node onto the stack.

		// For run on scenarios
		if (SemanticStack.top().isExpressionNode())
		{
			//We don't need to handle this case, so just return.
			return;
		}

		ASTNode BaseExpression(BaseExprNodeType);

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
		// pop off term node and store it in or node.
		// pop off term node and store it in or node.

		// push or Node onto stack.
		ASTNode OrNode(OrSimpleExprNodeTYPE);

		if (SemanticStack.top().isTermNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			OrNode.setBaseTermNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Or Simple Expr Node, but didn't find (first) Term Node on stack");
		}
		// -------------- What follows could be another Or node if it's a run on Or scenario ------------- //
		if (SemanticStack.top().getAstNodeType() == OrSimpleExprNodeTYPE)
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
		// pop off term node and store it in addition node.
		// pop off term node and store it in addition node.
		// push addition Node onto stack.
		ASTNode AdditionNode(AdditionSimpleExprNodeTYPE);

		if (SemanticStack.top().isTermNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			AdditionNode.setBaseTermNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Addition Node, but didn't find (first) Term Node on stack");
		}

		// -------------- What follows could be another arithmetic based node if it's a run on arithmetic operator scenario ------------- //
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
		// pop off term node and store it in subtaction node.
		// pop off term node and store it in subtraction node.

		// push subtraction Node onto stack.
		ASTNode SubtractionNode(SubtractorSimpleExprNodeTYPE);

		if (SemanticStack.top().isTermNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			SubtractionNode.setBaseTermNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Subtraction Node, but didn't find (first) Term Node on stack");
		}

		// -------------- What follows could be another arithmetic based node if it's a run on arithmetic operator scenario ------------- //
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
		// pop off term node and store it in Base simple expression node.

		// push Base simple expression Node onto stack.
		ASTNode BaseSimpleExprNode(BaseSimpleExprNodeType);

		// For run on scenarios.
		if (SemanticStack.top().isSimpleExpressionNode())
		{
			//We don't need to handle this case, so just return.
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
		// pop off factor node and store it in and node.
		// pop off factor node and store it in and node.

		// push and Node onto stack.
		ASTNode AndNode(AndTermNodeType);

		if (SemanticStack.top().isFactorNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			AndNode.setFactorNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build And Node, but didn't find (first) Factor Node on stack");
		}

		// -------------- What follows could be another and node if it's a run on and scenario ------------- //
		if (SemanticStack.top().getAstNodeType() == AndTermNodeType)
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
		// pop off factor node and store it in multiplicator node.
		// pop off factor node and store it in multiplicator node.

		// push multiplicator node onto stack.
		ASTNode MultiplicatorNode(MultiplicatorTermNodeTYPE);

		if (SemanticStack.top().isFactorNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			MultiplicatorNode.setFactorNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Multiplication Node, but didn't find (first) Factor Node on stack");
		}

		// -------------- What follows could be another arithmetic based node if it's a run on arithmetic operator scenario ------------- //
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
		// pop off factor node and store it in Divider node.
		// pop off factor node and store it in Divider node.

		// push Divider note onto stack.
		ASTNode DividerNode(DividerTermNodeTYPE);

		if (SemanticStack.top().isFactorNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			DividerNode.setFactorNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Division Node, but didn't find (first) Factor Node on stack");
		}

		// -------------- What follows could be another arithmetic based node if it's a run on arithmetic operator scenario ------------- //
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
		// pop off factor node and store it in Base term expression node.
		// push Base term expression Node onto stack.

		//Only want to happen if we're dealing with a term that hasn't already been built.
		//This is because we account for run-on operators.

		ASTNode BaseTermNode(BaseTermNodeTYPE);
		if (SemanticStack.top().isTermNode())
		{
			//We don't need to handle this case, so just return.
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
		// pop off expr node and store it in if factor node.
		// pop off expr node and store it in if factor node.
		// pop off expr node and store it in if factor node.
		// push if factor node onto stack.

		ASTNode IfFactorNodeVar(IfFactorNodeTYPE);
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
		// pop off factor node and store it in not factor node
		// push not factor node onto stack.

		ASTNode NotFactorNodeVar(NotFactorNodeTYPE);
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
		// pop off literal node and store it in literal factor node
		// push literal factor node onto stack.

		ASTNode LiteralFactorNodeVar(LiteralFactorNodeTYPE);
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

	virtual void visitSubtractionFactorNode(SemanticStack& SemanticStack) {
		// pop off factor node and store it in subtraction factor node
		// push subtraction factor node onto stack.

		ASTNode SubtractionFactorNode(SubtractionFactorNodeTYPE);
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
		// pop off expr node and store it in parenthesised expression node.

		// push parenthesised expression node onto stack.
		ASTNode ParenthesisedExpr(ParenExprFactorNodeTYPE);
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

	virtual void visitIdentifierActualsNode(SemanticStack& SemanticStack) {
		// pop off identifier node and store it in identifier actuals node.
		// pop off actuals node and store it in identifier actuals node.
		ASTNode IdentifierActualsNode(FunctionCallType);
		
		if (SemanticStack.top().isActualsNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			IdentifierActualsNode.setBaseActualsNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Identifier Actuals Node, but didn't find Actuals Node on stack");
		}

		if (SemanticStack.top().getAstNodeType() == IDENTIFIERNODETYPE)
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
		// pop off identifier node and store it in singleton identifier factor node.

		// push singleton identifier factor node onto stack.
		ASTNode SingletonIdentifierNode(SingletonIdentifierFactorNodeTYPE);
		if (SemanticStack.top().getAstNodeType() == IDENTIFIERNODETYPE)
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
		// represent epsilon case.
		ASTNode BaseActualsNode(BaseActualsNodeTYPE);

		SemanticStack.push(BaseActualsNode);
	}

	virtual void visitNonBaseActualsNode(SemanticStack& SemanticStack) {
		// pop non empty actuals node and store it in non base actuals node.

		// push non base actuals node onto stack.
		ASTNode NonBaseActualsNodeVar(NonBaseActualsNodeTYPE);

		if (SemanticStack.top().getAstNodeType() == NonEmptyActualsNodeTYPE)
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
		// pop off every expr node from the stack and store it inside a non empty actuals node.
		// Then push that non empty actuals node back onto the stack.

		//Only want to happen if we're dealing with the case that non empty actuals hasn't already been built.
		//This is because we account for run-on non empty actuals.
		if (SemanticStack.top().getAstNodeType() == NonEmptyActualsNodeTYPE)
		{
			//We don't need to handle this case, so just return.
			return;
		}

		ASTNode NonEmptyActualsNodeVar(NonEmptyActualsNodeTYPE);
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
		// store IntData inside the integer literal node and push that integer literal node onto the stack
		ASTNode IntegerLiteralNode(IntegerLiteralNodeTYPE);
		IntegerLiteralNode.setLiteralValue(IntData);

		SemanticStack.push(IntegerLiteralNode);
	}

	virtual void visitBooleanLiteralNode(SemanticStack& SemanticStack, string BooleanValue) {
		// store BooleanValue inside the boolean literal node and push that boolean literal node onto the stack
		ASTNode BooleanLiteralNode(BooleanLiteralNodeTYPE);
		BooleanLiteralNode.setLiteralValue(BooleanValue);

		SemanticStack.push(BooleanLiteralNode);
	}

	virtual void visitPrintStatementNode(SemanticStack& SemanticStack) {
		// pop off expr node from the stack and store it inside a print statment node.
		// Then push that  print statment node back onto the stack.
		ASTNode PrintStatementNode(PrintStatemetNodeTYPE);

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