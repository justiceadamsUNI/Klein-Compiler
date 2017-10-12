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
		
		ASTNode ProgramNode(ProgramNode);
		while (!SemanticStack.isEmpty())
		{
			if (SemanticStack.top().getAstNodeType() == DefinitionsNode)
			{
				ProgramNode.addDefinitionToVector(&SemanticStack.pop());
				continue;
			}
			else {
				//Exit while loop
				break;
			}
		}

		cout << "adding a Program node!" << endl;
		SemanticStack.push(ProgramNode);
	}

	virtual void visitDefinitionsNode(SemanticStack& SemanticStack) {
		// pop off every def node from the stack and store it inside a definitions node.
		// Then push that definitions node back onto the stack.
		
		ASTNode DefinitionsNode(DefinitionsNode);
		while (!SemanticStack.isEmpty())
		{
			if (SemanticStack.top().getAstNodeType() == DefNode)
			{
				DefinitionsNode.addDefToVector(&SemanticStack.pop());
				continue;
			}
			else {
				//Exit while loop
				break;
			}
		}

		cout << "adding a Definitions node!" << endl;
		SemanticStack.push(DefinitionsNode);

	}

	virtual void visitIdentifierNode(SemanticStack& SemanticStack, string IdentifierName) {
		// store the identifier name inside the identifier node, then push the identifier node onto the stack.
		ASTNode IdentifierNode(IdentifierNode);
		IdentifierNode.setIdentifierName(IdentifierName);

		cout << "adding a Identifier node!" << endl;
		SemanticStack.push(IdentifierNode);
	}

	virtual void visitDefNode(SemanticStack& SemanticStack) {
		ASTNode DefNode(DefNode);

		if (SemanticStack.top().getAstNodeType() == BodyNode)
		{
			DefNode.setBodyNode(&SemanticStack.pop());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Def Node, but didn't find Body Node on stack");
		}

		if (SemanticStack.top().getAstNodeType() == TypeNode)
		{
			DefNode.setTypeNode(&SemanticStack.pop());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Def Node, but didn't find Type Node on stack");
		}

		if (SemanticStack.top().isFormalsNode())
		{
			DefNode.setFormalsNode(&SemanticStack.pop());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Def Node, but didn't find Formals Node on stack");
		}

		if (SemanticStack.top().getAstNodeType() == IdentifierNode)
		{
			DefNode.setIdentifierNode(&SemanticStack.pop());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Def Node, but didn't find Identifier Node on stack");
		}
	

		cout << "adding a Def node!" << endl;
		SemanticStack.push(DefNode);

	}

	virtual void visitFormalsNode(SemanticStack& SemanticStack) {
		//represents epsilon case. still a valid formals node.
		ASTNode FormalsNode(FormalsNodeType);

		cout << "adding a Formals node!" << endl;
		SemanticStack.push(FormalsNode);
	}
	

	virtual void visitNonEmptyFormalsNode(SemanticStack& SemanticStack) {
		// pop off every Formal node from the stack and store it inside a Non Empty Formals Node.
		// Then push that Non Empty Formals Node node back onto the stack.

		ASTNode NonEmptyFormalsNode(NonEmptyFormalsNode);
		while (!SemanticStack.isEmpty())
		{
			if (SemanticStack.top().getAstNodeType() == FormalNode)
			{
				NonEmptyFormalsNode.addFormalNodeToVector(&SemanticStack.pop());
				continue;
			}
			else {
				//Exit while loop
				break;
			}
		}

		cout << "adding a Non Empty Formals node!" << endl;
		SemanticStack.push(NonEmptyFormalsNode);
	}

	virtual void visitFormalNode(SemanticStack& SemanticStack) {
		// pop off identifier node and store it in Formal node.
		// pop off type node and store it in Formal node.

		// push Formal Node onto stack.
		ASTNode FormalNode(FormalNode);
		if (SemanticStack.top().getAstNodeType() == TypeNode)
		{
			FormalNode.setTypeNode(&SemanticStack.pop());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Formal Node, but didn't find Type Node on stack");
		}

		if (SemanticStack.top().getAstNodeType() == IdentifierNode)
		{
			FormalNode.setIdentifierNode(&SemanticStack.pop());
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
			BodyNode.setBaseExprNode(&SemanticStack.pop());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Body Node, but didn't find Expr Node on stack");
		}

		while (!SemanticStack.isEmpty())
		{
			if (SemanticStack.top().getAstNodeType() == PrintStatemetNode)
			{
				BodyNode.addPrintStatementToVector(&SemanticStack.pop());
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
			LessThanExprNode.setBaseSimpleExprNode(&SemanticStack.pop());
		}
		else {
			throw runtime_error("ERROR: Attempted to build Less Than Node, but didn't find (First) Simple Expression Node on stack");
		}
		if (SemanticStack.top().isSimpleExpressionNode())
		{
			LessThanExprNode.setBaseSimpleExprNode(&SemanticStack.pop());
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
	}

	virtual void visitBaseExpressionNode(SemanticStack& SemanticStack) {
		// pop off simple expression node and store it in base expression node

		// push Base expression node onto the stack.
	}

	virtual void visitOrNode(SemanticStack& SemanticStack) {
		// pop off term node and store it in or node.
		// pop off term node and store it in or node.

		// push or Node onto stack.
	}

	virtual void visitAdditionNode(SemanticStack& SemanticStack) {
		// pop off term node and store it in addition node.
		// pop off term node and store it in addition node.

		// push addition Node onto stack.
	}

	virtual void visitSubtractionNode(SemanticStack& SemanticStack) {
		// pop off term node and store it in subtaction node.
		// pop off term node and store it in subtraction node.

		// push subtraction Node onto stack.

	}

	virtual void visitBaseSimpleExpressionNode(SemanticStack& SemanticStack) {
		// pop off term node and store it in Base simple expression node.

		// push Base simple expression Node onto stack.

	}

	virtual void visitAndNode(SemanticStack& SemanticStack) {
		// pop off factor node and store it in and node.
		// pop off factor node and store it in and node.

		// push and Node onto stack.

	}

	virtual void visitMultiplicatorNode(SemanticStack& SemanticStack) {
		// pop off factor node and store it in multiplicator node.
		// pop off factor node and store it in multiplicator node.

		// push multiplicator note onto stack.

	}

	virtual void visitDividerNode(SemanticStack& SemanticStack) {
		// pop off factor node and store it in Divider node.
		// pop off factor node and store it in Divider node.

		// push Divider note onto stack.

	}

	virtual void visitBaseTermNode(SemanticStack& SemanticStack) {
		// pop off factor node and store it in Base term expression node.

		// push Base term expression Node onto stack.

	}

	virtual void visitIfFactorNode(SemanticStack& SemanticStack) {
		// pop off expr node and store it in if factor node.
		// pop off expr node and store it in if factor node.
		// pop off expr node and store it in if factor node.

		// push if factor node onto stack.
	}

	virtual void visitNotFactorNode(SemanticStack& SemanticStack) {
		// pop off factor node and store it in not factor node

		// push not factor node onto stack.
	}

	virtual void visitLiteralFactorNode(SemanticStack& SemanticStack) {
		// pop off literal node and store it in literal factor node

		// push literal factor node onto stack.
	}

	virtual void visitSubtractionFactorNode(SemanticStack& SemanticStack) {
		// pop off factor node and store it in subtraction factor node

		// push subtraction factor node onto stack.

	}

	virtual void visitParenthesisedExpressionNode(SemanticStack& SemanticStack) {
		// pop off expr node and store it in parenthesised expression node.

		// push parenthesised expression node onto stack.
	}

	virtual void visitIdentifierActualsNode(SemanticStack& SemanticStack) {
		// pop off identifier node and store it in identifier actuals node.
		// pop off actuals node and store it in identifier actuals node.

		// push identifier actuals node onto stack.
	}

	virtual void visitSingletonIdentifierFactorNode(SemanticStack& SemanticStack) {
		// pop off identifier node and store it in singleton identifier factor node.

		// push singleton identifier factor node onto stack.
	}

	virtual void visitBaseActualsNode(SemanticStack& SemanticStack) {
		// Why do we want something that holds nothing? <--------
	}

	virtual void visitNonBaseActualsNode(SemanticStack& SemanticStack) {
		// pop non empty actuals node and store it in non base actuals node.

		// push non base actuals node onto stack.
	}

	virtual void visitNonEmptyActualsNode(SemanticStack& SemanticStack) {
		// pop off every expr node from the stack and store it inside a non empty actuals node.
		// Then push that non empty actuals node back onto the stack.
	}

	virtual void visitIntegerLiteralNode(SemanticStack& SemanticStack, int IntData) {
		// store IntData inside the integer literal node and push that integer literal node onto the stack
	}

	virtual void visitBooleanLiteralNode(SemanticStack& SemanticStack, string BooleanValue) {
		// store BooleanValue inside the boolean literal node and push that boolean literal node onto the stack
	}

	virtual void visitPrintStatementNode(SemanticStack& SemanticStack) {
		// pop off expr node from the stack and store it inside a print statment node.
		// Then push that  print statment node back onto the stack.
	}
};