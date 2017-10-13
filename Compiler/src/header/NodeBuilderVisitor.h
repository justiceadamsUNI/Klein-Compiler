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

	virtual void visitProgramNode(SemanticStack& SemanticStack) {
		// pop off every defenition node from the stack and store it inside a program node.
		// Then push that program node back onto the stack.
		
		ASTNode ProgramNodeVar(ProgramNode);
		while (!SemanticStack.isEmpty())
		{
			if (SemanticStack.top().getAstNodeType() == DefinitionsNode)
			{
				ASTNode StackTop = SemanticStack.pop();
				VectorOfASTNodes.push_back(StackTop);
				ProgramNodeVar.addDefinitionToVector(&VectorOfASTNodes.back());
				continue;
			}
			else {
				//Exit while loop
				break;
			}
		}

		cout << "adding a Program node!" << endl;
		SemanticStack.push(ProgramNodeVar);
	}

	virtual void visitDefinitionsNode(SemanticStack& SemanticStack) {
		// pop off every def node from the stack and store it inside a definitions node.
		// Then push that definitions node back onto the stack.
		
		ASTNode DefinitionsNodeVar(DefinitionsNode);
		while (!SemanticStack.isEmpty())
		{
			if (SemanticStack.top().getAstNodeType() == DefNode)
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

		cout << "adding a Definitions node!" << endl;
		SemanticStack.push(DefinitionsNodeVar);

	}

	virtual void visitIdentifierNode(SemanticStack& SemanticStack, string IdentifierName) {
		// store the identifier name inside the identifier node, then push the identifier node onto the stack.
		ASTNode IdentifierNodeVar(IdentifierNode);
		IdentifierNodeVar.setIdentifierName(IdentifierName);

		cout << "adding a Identifier node!" << endl;
		SemanticStack.push(IdentifierNode);
	}

	virtual void visitDefNode(SemanticStack& SemanticStack) {
		ASTNode DefNodeVar(DefNode);

		if (SemanticStack.top().getAstNodeType() == BodyNode)
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			DefNodeVar.setBodyNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Def Node, but didn't find Body Node on stack");
		}

		if (SemanticStack.top().getAstNodeType() == TypeNode)
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

		if (SemanticStack.top().getAstNodeType() == IdentifierNode)
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			DefNodeVar.setIdentifierNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Def Node, but didn't find Identifier Node on stack");
		}
	

		cout << "adding a Def node!" << endl;
		SemanticStack.push(DefNodeVar);

	}

	virtual void visitFormalsNode(SemanticStack& SemanticStack) {
		//represents epsilon case. still a valid formals node.
		ASTNode FormalsNodeVar(FormalsNodeType);

		cout << "adding a Formals node!" << endl;
		SemanticStack.push(FormalsNodeVar);
	}
	

	virtual void visitNonEmptyFormalsNode(SemanticStack& SemanticStack) {
		// pop off every Formal node from the stack and store it inside a Non Empty Formals Node.
		// Then push that Non Empty Formals Node node back onto the stack.

		ASTNode NonEmptyFormalsNodeVar(NonEmptyFormalsNode);
		while (!SemanticStack.isEmpty())
		{
			if (SemanticStack.top().getAstNodeType() == FormalNode)
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

		cout << "adding a Non Empty Formals node!" << endl;
		SemanticStack.push(NonEmptyFormalsNodeVar);
	}

	virtual void visitFormalNode(SemanticStack& SemanticStack) {
		// pop off identifier node and store it in Formal node.
		// pop off type node and store it in Formal node.

		// push Formal Node onto stack.
		ASTNode FormalNode(FormalNode);
		if (SemanticStack.top().getAstNodeType() == TypeNode)
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			FormalNode.setTypeNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Formal Node, but didn't find Type Node on stack");
		}

		if (SemanticStack.top().getAstNodeType() == IdentifierNode)
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			FormalNode.setIdentifierNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Formal Node, but didn't find Identifier Node on stack");
		}


		cout << "adding a Formal node!" << endl;
		SemanticStack.push(FormalNode);
	}

	virtual void visitBodyNode(SemanticStack& SemanticStack) {
		// pop off list of print statment nodes and store it in Body node.
		// pop off expr node and store it in Body node.

		ASTNode BodyNode(BodyNode);
		if (SemanticStack.top().isExpressionNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			BodyNode.setBaseExprNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Body Node, but didn't find Expr Node on stack");
		}

		while (!SemanticStack.isEmpty())
		{
			if (SemanticStack.top().getAstNodeType() == PrintStatemetNode)
			{
				ASTNode StackTop = SemanticStack.pop();
				VectorOfASTNodes.push_back(StackTop);
				BodyNode.addPrintStatementToVector(&VectorOfASTNodes.back());
				continue;
			}
			else {
				//Exit while loop
				break;
			}
		}
		cout << "adding a Body node!" << endl;
		SemanticStack.push(BodyNode);
	}

	virtual void visitTypeNode(SemanticStack& SemanticStack, string Type) {
		// store Type string inside the Type Node.
		// push Type node onto the stack
		
		ASTNode TypeNode(TypeNode);
		TypeNode.setDataType(Type);

		cout << "adding a Type node!" << endl;
		SemanticStack.push(TypeNode);
	}

	virtual void visitLessThanNode(SemanticStack& SemanticStack) {
		// pop off simple expression node and store it in Less than node.
		// pop off simple expression node and store it in Less than node.

		// push less than Node onto stack.

		ASTNode LessThanExprNode(LessThanExprNode);

		if (SemanticStack.top().isSimpleExpressionNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			LessThanExprNode.setBaseSimpleExprNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Less Than Node, but didn't find (First) Simple Expression Node on stack");
		}
		if (SemanticStack.top().isSimpleExpressionNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			LessThanExprNode.setBaseSimpleExprNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Less Than Node, but didn't find (Second) Simple Expression Node on stack");
		}

		cout << "adding a Less than node!" << endl;
		SemanticStack.push(LessThanExprNode);
	}

	virtual void visitEqualNode(SemanticStack& SemanticStack) {
		// pop off simple expression node and store it in equal than node.
		// pop off simple expression node and store it in equal than node.

		// pushequal Node onto stack.
		ASTNode EqualNode(EqualToExprNode);

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
			EqualNode.setBaseSimpleExprNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Equal Node, but didn't find (Second) Simple Expression Node on stack");
		}

		cout << "adding an Equal to node!" << endl;
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

		cout << "adding a Base Expression node!" << endl;
		SemanticStack.push(BaseExpression);
	}

	virtual void visitOrNode(SemanticStack& SemanticStack) {
		// pop off term node and store it in or node.
		// pop off term node and store it in or node.

		// push or Node onto stack.
		ASTNode OrNode(OrSimpleExprNode);

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
		if (SemanticStack.top().getAstNodeType() == OrSimpleExprNode)
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

		cout << "adding a Or Expression node!" << endl;
		SemanticStack.push(OrNode);
	}

	virtual void visitAdditionNode(SemanticStack& SemanticStack) {
		// pop off term node and store it in addition node.
		// pop off term node and store it in addition node.
		// push addition Node onto stack.
		ASTNode AdditionNode(AdditionSimpleExprNode);

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

		cout << "adding an Addition node!" << endl;
		SemanticStack.push(AdditionNode);
	}

	virtual void visitSubtractionNode(SemanticStack& SemanticStack) {
		// pop off term node and store it in subtaction node.
		// pop off term node and store it in subtraction node.

		// push subtraction Node onto stack.
		ASTNode SubtractionNode(SubtractorSimpleExprNode);

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

		cout << "adding a Subtraction node!" << endl;
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

		cout << "adding a Base Simple Expr node!" << endl;
		SemanticStack.push(BaseSimpleExprNode);

	}

	virtual void visitAndNode(SemanticStack& SemanticStack) {
		// pop off factor node and store it in and node.
		// pop off factor node and store it in and node.

		// push and Node onto stack.
		ASTNode AndNode(AndTermNode);

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
		if (SemanticStack.top().getAstNodeType() == AndTermNode)
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			AndNode.setBaseTermNode(&VectorOfASTNodes.back());
		}
		else if (SemanticStack.top().isFactorNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			AndNode.setFactorNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build And Node, but didn't find (Second) Factor/ And Node on stack");
		}

		cout << "adding an And node!" << endl;
		SemanticStack.push(AndNode);

	}

	virtual void visitMultiplicatorNode(SemanticStack& SemanticStack) {
		// pop off factor node and store it in multiplicator node.
		// pop off factor node and store it in multiplicator node.

		// push multiplicator node onto stack.
		ASTNode MultiplicatorNode(MultiplicatorTermNode);

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

		cout << "adding a Multiplication node!" << endl;
		SemanticStack.push(MultiplicatorNode);
	}

	virtual void visitDividerNode(SemanticStack& SemanticStack) {
		// pop off factor node and store it in Divider node.
		// pop off factor node and store it in Divider node.

		// push Divider note onto stack.
		ASTNode DividerNode(DividerTermNode);

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

		cout << "adding a Division node!" << endl;
		SemanticStack.push(DividerNode);

	}

	virtual void visitBaseTermNode(SemanticStack& SemanticStack) {
		// pop off factor node and store it in Base term expression node.
		// push Base term expression Node onto stack.

		//Only want to happen if we're dealing with a term that hasn't already been built.
		//This is because we account for run-on operators.

		ASTNode BaseTermNode(BaseTermNodeType);
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
		cout << "adding a Base Term node!" << endl;
		SemanticStack.push(BaseTermNode);
	}

	virtual void visitIfFactorNode(SemanticStack& SemanticStack) {
		// pop off expr node and store it in if factor node.
		// pop off expr node and store it in if factor node.
		// pop off expr node and store it in if factor node.
		// push if factor node onto stack.

		ASTNode IfFactorNode(IfFactorNode);
		if (SemanticStack.top().isExpressionNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			IfFactorNode.setBaseExprNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build If Statement Node, but didn't find (first) Expression Node on stack");
		}

		if (SemanticStack.top().isExpressionNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			IfFactorNode.setBaseExprNode2(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build If Statement Node, but didn't find (second) Expression Node on stack");
		}

		if (SemanticStack.top().isExpressionNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			IfFactorNode.setBaseExprNode3(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build If Statement Node, but didn't find (third) Expression Node on stack");
		}

		cout << "adding an If Statement node!" << endl;
		SemanticStack.push(IfFactorNode);
	}

	virtual void visitNotFactorNode(SemanticStack& SemanticStack) {
		// pop off factor node and store it in not factor node
		// push not factor node onto stack.

		ASTNode NotFactorNode(NotFactorNode);
		if (SemanticStack.top().isFactorNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			NotFactorNode.setFactorNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Not Factor Node, but didn't find Factor Node on stack");
		}
		cout << "adding a Not Factor node!" << endl;
		SemanticStack.push(NotFactorNode);
	}

	virtual void visitLiteralFactorNode(SemanticStack& SemanticStack) {
		// pop off literal node and store it in literal factor node
		// push literal factor node onto stack.

		ASTNode LiteralFactorNode(LiteralFactorNode);
		if (SemanticStack.top().isLiteralNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			LiteralFactorNode.setLiteralNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Literal Factor Node, but didn't find Literal Node on stack");
		}
		cout << "adding a Literal Factor node!" << endl;
		SemanticStack.push(LiteralFactorNode);
	}

	virtual void visitSubtractionFactorNode(SemanticStack& SemanticStack) {
		// pop off factor node and store it in subtraction factor node
		// push subtraction factor node onto stack.

		ASTNode SubtractionFactorNode(SubtractionFactorNode);
		if (SemanticStack.top().isFactorNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			SubtractionFactorNode.setFactorNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Subtraction Factor Node, but didn't find Factor Node on stack");
		}
		cout << "adding a Subtraction Factor node!" << endl;
		SemanticStack.push(SubtractionFactorNode);

	}

	virtual void visitParenthesisedExpressionNode(SemanticStack& SemanticStack) {
		// pop off expr node and store it in parenthesised expression node.

		// push parenthesised expression node onto stack.
		ASTNode ParenthesisedExpr(ParenExprFactorNode);
		if (SemanticStack.top().isExpressionNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			ParenthesisedExpr.setBaseExprNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Parenthesised Expression Node, but didn't find Factor Node on stack");
		}
		cout << "adding a Parenthesised Expression node!" << endl;
		SemanticStack.push(ParenthesisedExpr);
	}

	virtual void visitIdentifierActualsNode(SemanticStack& SemanticStack) {
		// pop off identifier node and store it in identifier actuals node.
		// pop off actuals node and store it in identifier actuals node.
		ASTNode IdentifierActualsNode(IdentifierFactorNode);
		
		if (SemanticStack.top().isActualsNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			IdentifierActualsNode.setBaseActualsNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Identifier Actuals Node, but didn't find Actuals Node on stack");
		}

		if (SemanticStack.top().getAstNodeType() == IdentifierNode)
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			IdentifierActualsNode.setIdentifierNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Identifier Actuals Node, but didn't find Identifier Node on stack");
		}
		cout << "adding a Identifier Actuals node!" << endl;
		SemanticStack.push(IdentifierActualsNode);
	}

	virtual void visitSingletonIdentifierFactorNode(SemanticStack& SemanticStack) {
		// pop off identifier node and store it in singleton identifier factor node.

		// push singleton identifier factor node onto stack.
		ASTNode SingletonIdentifierNode(SingletonIdentifierFactorNode);
		if (SemanticStack.top().getAstNodeType() == IdentifierNode)
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			SingletonIdentifierNode.setIdentifierNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Singelton Identifier Factor Node, but didn't find Identifier Node on stack");
		}

		cout << "adding a Singelton Identifier Factor node!" << endl;
		SemanticStack.push(SingletonIdentifierNode);
	}

	virtual void visitBaseActualsNode(SemanticStack& SemanticStack) {
		// represent epsilon case.
		ASTNode BaseActualsNode(BaseActualsNodeType);

		cout << "adding a Base Actuals node!" << endl;
		SemanticStack.push(BaseActualsNode);
	}

	virtual void visitNonBaseActualsNode(SemanticStack& SemanticStack) {
		// pop non empty actuals node and store it in non base actuals node.

		// push non base actuals node onto stack.
		ASTNode NonBaseActualsNode(NonBaseActualsNode);

		if (SemanticStack.top().getAstNodeType() == NonEmptyActualsNode)
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			NonBaseActualsNode.setBaseActualsNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Non Base Actuals Node, but didn't find Non empty Actuals Node on stack");
		}

		cout << "adding a Non Base Actuals node!" << endl;
		SemanticStack.push(NonBaseActualsNode);
	}

	virtual void visitNonEmptyActualsNode(SemanticStack& SemanticStack) {
		// pop off every expr node from the stack and store it inside a non empty actuals node.
		// Then push that non empty actuals node back onto the stack.

		//Only want to happen if we're dealing with the case that non empty actuals hasn't already been built.
		//This is because we account for run-on non empty actuals.
		if (SemanticStack.top().getAstNodeType() == NonEmptyActualsNode)
		{
			//We don't need to handle this case, so just return.
			return;
		}

		ASTNode NonEmptyActualsNodeVar(NonEmptyActualsNode);
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

		cout << "adding a Non Empty Actuals node!" << endl;
		SemanticStack.push(NonEmptyActualsNodeVar);
	}

	virtual void visitIntegerLiteralNode(SemanticStack& SemanticStack, int IntData) {
		// store IntData inside the integer literal node and push that integer literal node onto the stack
		ASTNode IntegerLiteralNode(IntegerLiteralNode);
		IntegerLiteralNode.setLiteralValue("test"); // convert int to string

		cout << "adding a Identifier Literal node!" << endl;
		SemanticStack.push(IntegerLiteralNode);
	}

	virtual void visitBooleanLiteralNode(SemanticStack& SemanticStack, string BooleanValue) {
		// store BooleanValue inside the boolean literal node and push that boolean literal node onto the stack
		ASTNode BooleanLiteralNode(BooleanLiteralNode);
		BooleanLiteralNode.setLiteralValue(BooleanValue); // convert int to string

		cout << "adding a Boolean Literal node!" << endl;
		SemanticStack.push(BooleanLiteralNode);
	}

	virtual void visitPrintStatementNode(SemanticStack& SemanticStack) {
		// pop off expr node from the stack and store it inside a print statment node.
		// Then push that  print statment node back onto the stack.
		ASTNode PrintStatementNode(PrintStatemetNode);

		if (SemanticStack.top().isExpressionNode())
		{
			ASTNode StackTop = SemanticStack.pop();
			VectorOfASTNodes.push_back(StackTop);
			PrintStatementNode.setBaseExprNode(&VectorOfASTNodes.back());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Print Statement Node, but didn't find Expression Node on stack");
		}

		cout << "adding a Print Statment node!" << endl;
		SemanticStack.push(PrintStatementNode);
	}

	private:
		//This is to keep alive the objects which the pointers point to.
		vector<ASTNode> VectorOfASTNodes;
};