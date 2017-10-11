#pragma once
#include "../header/ASTNodeVisitor.h"
#include <stdexcept>
#include <iostream>
#include <typeinfo>
#include <algorithm>

using namespace std;
class NodeBuilderVisitor : public ASTNodeVisitor {
public:
	list<string> Formals = { "FormalsNode", "NonEmptyFormalsNode" };
	list<string> ExprNodes = { "BaseExprNode", "EqualToExprNode", "LessThanExprNode" };
	list<string> SimpleExprNodes = {"BaseSimpleExprNode", "SubtractorSimpleExprNode", "OrSimpleExprNode", "AdditionSimpleExprNode"};
	list<string> TermNodes = { "BaseTermNode", "MultiplicatorTermNode", "DividerTermNode", "AndTermNode"};
	list<string> FactorNodes = { "FactorNode", "ParenExprFactorNode", "SubtractionFactorNode", "LiteralFactorNode", "IdentifierFactorNode", "SingletonIdentifierFactorNode", "NotFactorNode", "IfFactorNode"};
	list<string> LiteralNodes = { "LiteralNode", "IntegerLiteralNode", "BooleanLiteralNode" };
	list<string> ActualsNodes = { "BaseActualsNode", "NonBaseActualsNode" };

	virtual void visitProgramNode(SemanticStack& SemanticStack) {
		// pop off every defenition node from the stack and store it inside a program node.
		// Then push that program node back onto the stack.
		
		ProgramNode Program;
		while (!SemanticStack.isEmpty())
		{
			if (typeid(*SemanticStack.top()).name() == "DefinitionsNode")
			{
				DefinitionsNode* Definitions = (DefinitionsNode*)SemanticStack.top();
				SemanticStack.pop();
				Program.Definitions.push_back(Definitions);
				continue;
			}
			else {
				//Exit while loop
				break;
			}
		}

		//PUSH ADRESS OF PROGRAM NODE
		cout << "adding a Program node!" << endl;
		SemanticStack.push(&Program);
	}

	virtual void visitDefinitionsNode(SemanticStack& SemanticStack) {
		// pop off every def node from the stack and store it inside a definitions node.
		// Then push that definitions node back onto the stack.

		// pop off every defenition node from the stack and store it inside a program node.
		// Then push that program node back onto the stack.
		
		DefinitionsNode Definitions;
		while (!SemanticStack.isEmpty())
		{
			if (typeid(*SemanticStack.top()).name() == "DefNode")
			{
				DefNode* Def = (DefNode*)SemanticStack.top();
				SemanticStack.pop();
				Definitions.DefNodes.push_back(Def);
				continue;
			}
			else {
				//Exit while loop
				break;
			}
		}

		//PUSH ADRESS OF DEFINITIONS NODE
		cout << "adding a Definitions node!" << endl;
		SemanticStack.push(&Definitions);

	}

	virtual void visitIdentifierNode(SemanticStack& SemanticStack, string IdentifierName) {
		// store the identifier name inside the identifier node, then push the identifier node onto the stack.
		IdentifierNode Identifier;
		Identifier.IdentifierName = IdentifierName;

		//PUSH ADRESS OF IDENTIFIER NODE
		cout << "adding a Identifier node!" << endl;
		SemanticStack.push(&Identifier);
	}

	virtual void visitDefNode(SemanticStack& SemanticStack) {
		// pop off identifier node and store it in Def node.
		// pop off formals node and store it in Def node.
		// pop off type node and store it in Def node.
		// pop off body node and store it in Def node.
		// push Def node onto stack.

		DefNode Def;

		if (typeid(*SemanticStack.top()).name() == "IdentifierNode")
		{
			IdentifierNode* Identifier = (IdentifierNode*)SemanticStack.top();
			Def.Identifier = Identifier;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Def Node, but didn't find Identifier Node on stack");
		}

		list<string>::iterator foundElement = find(Formals.begin(), Formals.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != Formals.end())
		{
			FormalsNode* Formals = (FormalsNode*)SemanticStack.top();
			Def.Formals = Formals;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Def Node, but didn't find Formals Node on stack");
		}


		if (typeid(*SemanticStack.top()).name() == "TypeNode")
		{
			TypeNode* Type = (TypeNode*)SemanticStack.top();
			Def.Type = Type;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Def Node, but didn't find Type Node on stack");
		}

		
		if (typeid(*SemanticStack.top()).name() == "BodyNode")
		{
			BodyNode* Body = (BodyNode*)SemanticStack.top();
			Def.Body = Body;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Def Node, but didn't find Body Node on stack");
		}

		//PUSH ADRESS OF DEF NODE
		cout << "adding a Def node!" << endl;
		SemanticStack.push(&Def);

	}

	virtual void visitFormalsNode(SemanticStack& SemanticStack) {
		//represents epsilon case. still a valid formals node.
		FormalsNode Formals;
		cout << "adding a Formals node!" << endl;
		SemanticStack.push(&Formals);
	}
	

	virtual void visitNonEmptyFormalsNode(SemanticStack& SemanticStack) {
		// pop off every Formal node from the stack and store it inside a Non Empty Formals Node.
		// Then push that Non Empty Formals Node node back onto the stack.

		NonEmptyFormalsNode NonEmptyFormals;
		while (!SemanticStack.isEmpty())
		{
			if (typeid(*SemanticStack.top()).name() == "FormalNode")
			{
				FormalNode* Formal = (FormalNode*)SemanticStack.top();
				NonEmptyFormals.FormalNodes.push_back(Formal);
				SemanticStack.pop();
				continue;
			}
			else {
				//Exit while loop
				break;
			}
		}

		//PUSH ADRESS OF NON EMPTY FORMALS NODE
		cout << "adding a Non Empty Formals node!" << endl;
		SemanticStack.push(&NonEmptyFormals);
	}

	virtual void visitFormalNode(SemanticStack& SemanticStack) {
		// pop off identifier node and store it in Formal node.
		// pop off type node and store it in Formal node.

		// push Formal Node onto stack.
		FormalNode Formal;

		if (typeid(*SemanticStack.top()).name() == "IdentifierNode")
		{
			IdentifierNode* Identifier = (IdentifierNode*)SemanticStack.top();
			Formal.Identifier = Identifier;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Formal Node, but didn't find Identifier Node on stack");
		}

		if (typeid(*SemanticStack.top()).name() == "TypeNode")
		{
			TypeNode* Type = (TypeNode*)SemanticStack.top();
			Formal.Type = Type;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Formal Node, but didn't find Type Node on stack");
		}

		//PUSH ADRESS OF FORMAL NODE
		cout << "adding a Formal node!" << endl;
		SemanticStack.push(&Formal);
	}

	virtual void visitBodyNode(SemanticStack& SemanticStack) {
		// pop off list of print statment nodes and store it in Body node.
		// pop off expr node and store it in Body node.
		cout << "trying to build body node!" << endl;
		BodyNode Body;

		while (!SemanticStack.isEmpty())
		{
			cout << "stack not empty" << endl;
			if (typeid(*SemanticStack.top()).name() == "PrintStatementNode")
			{
				PrintStatementNode* PrintStatement = (PrintStatementNode*)SemanticStack.top();
				Body.PrintStatements.push_back(PrintStatement);
				cout << "print statement found" << endl;
				SemanticStack.pop();
				continue;
			}
			else {
				//Exit while loop
				cout << "break" << endl;
				break;
			}
		}

		list<string>::iterator foundElement = find(ExprNodes.begin(), ExprNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != ExprNodes.end())
		{
			cout << "trying to get baseExpr node!" << endl;
			BaseExprNode* ExprNode = (BaseExprNode*)SemanticStack.top();
			Body.Expression = ExprNode;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Body Node, but didn't find Expr Node on stack");
		}


		//PUSH ADRESS OF BODY NODE.
		cout << "adding a Body node!" << endl;
		SemanticStack.push(&Body);
	}

	virtual void visitTypeNode(SemanticStack& SemanticStack, string Type) {
		// store Type string inside the Type Node.
		// push Type node onto the stack
		TypeNode TypeNodeVar;
		TypeNodeVar.DataType = Type;

		cout << "adding a Type node!" << endl;
		SemanticStack.push(&TypeNodeVar);
	}

	virtual void visitLessThanNode(SemanticStack& SemanticStack) {
		// pop off simple expression node and store it in Less than node.
		// pop off simple expression node and store it in Less than node.

		// push less than Node onto stack.

		LessThanExprNode LessThanExp;

		list<string>::iterator foundElement = find(SimpleExprNodes.begin(), SimpleExprNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != SimpleExprNodes.end())
		{
			BaseSimpleExprNode* SimpleExpr1 = (BaseSimpleExprNode*)SemanticStack.top();
			LessThanExp.SimpleExpression1 = SimpleExpr1;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Less Than Node, but didn't find (First) Simple Expression Node on stack");
		}

		foundElement = find(SimpleExprNodes.begin(), SimpleExprNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != SimpleExprNodes.end())
		{
			BaseSimpleExprNode* SimpleExpr2 = (BaseSimpleExprNode*)SemanticStack.top();
			LessThanExp.SimpleExpression2 = SimpleExpr2;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Less Than Node, but didn't find (Second) Simple Expression Node on stack");
		}

		//PUSH ADRESS OF LESS THAN NODE.
		cout << "adding a Less than node!" << endl;
		SemanticStack.push(&LessThanExp);
	}

	virtual void visitEqualNode(SemanticStack& SemanticStack) {
		// pop off simple expression node and store it in equal than node.
		// pop off simple expression node and store it in equal than node.

		// pushequal Node onto stack.
		EqualToExprNode EqualToExpr;

		list<string>::iterator foundElement = find(SimpleExprNodes.begin(), SimpleExprNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != SimpleExprNodes.end())
		{
			BaseSimpleExprNode* SimpleExpr1 = (BaseSimpleExprNode*)SemanticStack.top();
			EqualToExpr.SimpleExpression1 = SimpleExpr1;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to Equal To Node, but didn't find (First) Simple Expression Node on stack");
		}

		foundElement = find(SimpleExprNodes.begin(), SimpleExprNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != SimpleExprNodes.end())
		{
			BaseSimpleExprNode* SimpleExpr2 = (BaseSimpleExprNode*)SemanticStack.top();
			EqualToExpr.SimpleExpression2 = SimpleExpr2;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Equal To Node, but didn't find (Second) Simple Expression Node on stack");
		}

		//PUSH ADRESS OF EQUAL TO NODE.
		cout << "adding a Equal To node!" << endl;
		SemanticStack.push(&EqualToExpr);
	}

	virtual void visitBaseExpressionNode(SemanticStack& SemanticStack) {
		// pop off simple expression node and store it in base expression node
		// push Base expression node onto the stack.
		BaseExprNode BaseExpr;

		list<string>::iterator foundElement = find(SimpleExprNodes.begin(), SimpleExprNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != SimpleExprNodes.end())
		{
			BaseSimpleExprNode* SimpleExpr1 = (BaseSimpleExprNode*)SemanticStack.top();
			BaseExpr.SimpleExpression1= SimpleExpr1;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Base Expression Node, but didn't find Simple Expression Node on stack");
		}

		//PUSH ADRESS OF BASE EXPRESSION NODE.
		cout << "adding a Base Expression node!" << endl;
		SemanticStack.push(&BaseExpr);

	}

	virtual void visitOrNode(SemanticStack& SemanticStack) {
		// pop off term node and store it in or node.
		// pop off term node and store it in or node.

		// push or Node onto stack.

		OrSimpleExprNode OrSimpleExpr;

		list<string>::iterator foundElement = find(TermNodes.begin(), TermNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != TermNodes.end())
		{
			BaseTermNode* Term1 = (BaseTermNode*)SemanticStack.top();
			OrSimpleExpr.Term1 = Term1;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Or Simple Expr Node, but didn't find (first) Term Node on stack");
		}

		foundElement = find(TermNodes.begin(), TermNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != TermNodes.end())
		{
			BaseTermNode* Term2 = (BaseTermNode*)SemanticStack.top();
			OrSimpleExpr.Term2 = Term2;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Or Simple Expr Node, but didn't find (second) Term Node on stack");
		}

		//PUSH ADRESS OF OR NODE.
		cout << "adding a Or Expression node!" << endl;
		SemanticStack.push(&OrSimpleExpr);
	}

	virtual void visitAdditionNode(SemanticStack& SemanticStack) {
		// pop off term node and store it in addition node.
		// pop off term node and store it in addition node.

		// push addition Node onto stack.
		AdditionSimpleExprNode AdditionSimpleExpr;


		list<string>::iterator foundElement = find(TermNodes.begin(), TermNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != TermNodes.end())
		{
			BaseTermNode* Term1 = (BaseTermNode*)SemanticStack.top();
			AdditionSimpleExpr.Term1 = Term1;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Addition Node, but didn't find (first) Term Node on stack");
		}

		foundElement = find(TermNodes.begin(), TermNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != TermNodes.end())
		{
			BaseTermNode* Term2 = (BaseTermNode*)SemanticStack.top();
			AdditionSimpleExpr.Term2 = Term2;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Addition Node, but didn't find (second) Term Node on stack");
		}

		//PUSH ADRESS OF ADDITION NODE.
		cout << "adding an Addition node!" << endl;
		SemanticStack.push(&AdditionSimpleExpr);
	}

	virtual void visitSubtractionNode(SemanticStack& SemanticStack) {
		// pop off term node and store it in subtaction node.
		// pop off term node and store it in subtraction node.

		// push subtraction Node onto stack.
		SubtractorSimpleExprNode SubtractionNode;

		list<string>::iterator foundElement = find(TermNodes.begin(), TermNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != TermNodes.end())
		{
			BaseTermNode* Term1 = (BaseTermNode*)SemanticStack.top();
			SubtractionNode.Term1 = Term1;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Subtraction Node, but didn't find (first) Term Node on stack");
		}

		foundElement = find(TermNodes.begin(), TermNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != TermNodes.end())
		{
			BaseTermNode* Term2 = (BaseTermNode*)SemanticStack.top();
			SubtractionNode.Term2 = Term2;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Subtraction Node, but didn't find (second) Term Node on stack");
		}

		//PUSH ADRESS OF SUBTRACTION NODE.
		cout << "adding an Subtraction node!" << endl;
		SemanticStack.push(&SubtractionNode);
	}

	virtual void visitBaseSimpleExpressionNode(SemanticStack& SemanticStack) {
		// pop off term node and store it in Base simple expression node.
		// push Base simple expression Node onto stack.

		BaseSimpleExprNode BaseSimpleExpr;

		list<string>::iterator foundElement = find(TermNodes.begin(), TermNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != TermNodes.end())
		{
			BaseTermNode* Term1 = (BaseTermNode*)SemanticStack.top();
			BaseSimpleExpr.Term1 = Term1;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Base Simple Expr Node, but didn't find Term Node on stack");
		}

		//PUSH ADRESS OF BASE SIMPLE EXPRESSION NODE.
		cout << "adding an Base Simple Expression node!" << endl;
		SemanticStack.push(&BaseSimpleExpr);
	}

	virtual void visitAndNode(SemanticStack& SemanticStack) {
		// pop off factor node and store it in and node.
		// pop off factor node and store it in and node.

		// push and Node onto stack.
		AndTermNode AndTerm;

		list<string>::iterator foundElement = find(FactorNodes.begin(), FactorNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != FactorNodes.end())
		{
			FactorNode* Factor1 = (FactorNode*)SemanticStack.top();
			AndTerm.Factor1 = Factor1;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build And Node, but didn't find (first) Factor Node on stack");
		}

		foundElement = find(FactorNodes.begin(), FactorNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != FactorNodes.end())
		{
			FactorNode* Factor2 = (FactorNode*)SemanticStack.top();
			AndTerm.Factor2 = Factor2;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build And Node, but didn't find (second) Factor Node on stack");
		}

		//PUSH ADRESS OF AND NODE.
		cout << "adding an And node!" << endl;
		SemanticStack.push(&AndTerm);
	}

	virtual void visitMultiplicatorNode(SemanticStack& SemanticStack) {
		// pop off factor node and store it in multiplicator node.
		// pop off factor node and store it in multiplicator node.

		// push multiplicator note onto stack.
		
		MultiplicatorTermNode MultiplicatorTerm;
		
		list<string>::iterator foundElement = find(FactorNodes.begin(), FactorNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != FactorNodes.end())
		{
			FactorNode* Factor1 = (FactorNode*)SemanticStack.top();
			MultiplicatorTerm.Factor1 = Factor1;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Multiplication Node, but didn't find (first) Factor Node on stack");
		}

		foundElement = find(FactorNodes.begin(), FactorNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != FactorNodes.end())
		{
			FactorNode* Factor2 = (FactorNode*)SemanticStack.top();
			MultiplicatorTerm.Factor2 = Factor2;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Multiplication Node, but didn't find (second) Factor Node on stack");
		}

		//PUSH ADRESS OF MULTIPLICATION NODE.
		cout << "adding an Multiplication node!" << endl;
		SemanticStack.push(&MultiplicatorTerm);
	}

	virtual void visitDividerNode(SemanticStack& SemanticStack) {
		// pop off factor node and store it in Divider node.
		// pop off factor node and store it in Divider node.

		// push Divider note onto stack.
		DividerTermNode DividerTerm;

		list<string>::iterator foundElement = find(FactorNodes.begin(), FactorNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != FactorNodes.end())
		{
			FactorNode* Factor1 = (FactorNode*)SemanticStack.top();
			DividerTerm.Factor1 = Factor1;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Divider Node, but didn't find (first) Factor Node on stack");
		}

		foundElement = find(FactorNodes.begin(), FactorNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != FactorNodes.end())
		{
			FactorNode* Factor2 = (FactorNode*)SemanticStack.top();
			DividerTerm.Factor2 = Factor2;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Divider Node, but didn't find (second) Factor Node on stack");
		}

		//PUSH ADRESS OF DIVIDER NODE.
		cout << "adding an Divider node!" << endl;
		SemanticStack.push(&DividerTerm);
	}

	virtual void visitBaseTermNode(SemanticStack& SemanticStack) {
		// pop off factor node and store it in Base term expression node.
		// push Base term expression Node onto stack.

		BaseTermNode BaseTerm;

		list<string>::iterator foundElement = find(FactorNodes.begin(), FactorNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != FactorNodes.end())
		{
			FactorNode* Factor1 = (FactorNode*)SemanticStack.top();
			BaseTerm.Factor1 = Factor1;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Base Term Node, but didn't find Factor Node on stack");
		}
		cout << "adding a Base Term node!" << endl;
		SemanticStack.push(&BaseTerm);
	}

	virtual void visitIfFactorNode(SemanticStack& SemanticStack) {
		// pop off expr node and store it in if factor node.
		// pop off expr node and store it in if factor node.
		// pop off expr node and store it in if factor node.

		// push if factor node onto stack.
		IfFactorNode IfFactor;

		list<string>::iterator foundElement = find(ExprNodes.begin(), ExprNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != ExprNodes.end())
		{
			BaseExprNode* Expression1 = (BaseExprNode*)SemanticStack.top();
			IfFactor.Expression1 = Expression1;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build If Statement Node, but didn't find (first) Expression Node on stack");
		}

		foundElement = find(ExprNodes.begin(), ExprNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != ExprNodes.end())
		{
			BaseExprNode* Expression2 = (BaseExprNode*)SemanticStack.top();
			IfFactor.Expression2 = Expression2;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build If Statement Node, but didn't find (second) Expression Node on stack");
		}

		foundElement = find(ExprNodes.begin(), ExprNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != ExprNodes.end())
		{
			BaseExprNode* Expression3 = (BaseExprNode*)SemanticStack.top();
			IfFactor.Expression3 = Expression3;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build If Statement Node, but didn't find (third) Expression Node on stack");
		}

		//PUSH ADRESS OF IF FACTOR NODE.
		cout << "adding an If Statement node!" << endl;
		SemanticStack.push(&IfFactor);
	}

	virtual void visitNotFactorNode(SemanticStack& SemanticStack) {
		// pop off factor node and store it in not factor node

		// push not factor node onto stack.
		NotFactorNode NotFactor;

		list<string>::iterator foundElement = find(FactorNodes.begin(), FactorNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != FactorNodes.end())
		{
			FactorNode* Factor1 = (FactorNode*)SemanticStack.top();
			NotFactor.Factor = Factor1;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Not Factor Node, but didn't find Factor Node on stack");
		}

		//PUSH ADRESS OF NOT FACTOR NODE.
		cout << "adding a Not Factor node!" << endl;
		SemanticStack.push(&NotFactor);
	}

	virtual void visitLiteralFactorNode(SemanticStack& SemanticStack) {
		// pop off literal node and store it in literal factor node

		// push literal factor node onto stack.
		cout << "trying to add a Literal Factor node!" << endl;
		LiteralFactorNode LiteralFactor;
		cout << "trying to find element" << endl;
		list<string>::iterator foundElement = find(LiteralNodes.begin(), LiteralNodes.end(), typeid(*SemanticStack.top()).name());
		cout << "search complete" << endl;
		if (foundElement != LiteralNodes.end())
		{
			cout << "cast" << endl;
			LiteralNode* Literal = (LiteralNode*)SemanticStack.top();
			cout << "cast done" << endl;
			LiteralFactor.Literal = Literal;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Literal Factor Node, but didn't find Literal Node on stack");
		}

		//PUSH ADRESS OF LITERAL FACTOR NODE.
		cout << "adding a Literal Factor node!" << endl;
		SemanticStack.push(&LiteralFactor);
	}

	virtual void visitSubtractionFactorNode(SemanticStack& SemanticStack) {
		// pop off factor node and store it in subtraction factor node

		// push subtraction factor node onto stack.
		SubtractionFactorNode SubtractionFactor;

		list<string>::iterator foundElement = find(FactorNodes.begin(), FactorNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != FactorNodes.end())
		{
			FactorNode* Factor1 = (FactorNode*)SemanticStack.top();
			SubtractionFactor.Factor = Factor1;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Negated Factor Node, but didn't find Factor Node on stack");
		}
		//PUSH ADRESS OF SUBTRACTED FACTOR NODE.
		cout << "adding a Negated Factor node!" << endl;
		SemanticStack.push(&SubtractionFactor);

	}

	virtual void visitParenthesisedExpressionNode(SemanticStack& SemanticStack) {
		// pop off expr node and store it in parenthesised expression node.

		// push parenthesised expression node onto stack.
		ParenExprFactorNode ParenthesisedExpr;

		list<string>::iterator foundElement = find(ExprNodes.begin(), ExprNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != ExprNodes.end())
		{
			BaseExprNode* Expression = (BaseExprNode*)SemanticStack.top();
			ParenthesisedExpr.Expression = Expression;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Parenthesised Expression Node, but didn't find Expression Node on stack");
		}

		//PUSH ADRESS OF PARENTHESISED EXPR NODE.
		cout << "adding a Parenthesised Expression Factor node!" << endl;
		SemanticStack.push(&ParenthesisedExpr);
	}

	virtual void visitIdentifierActualsNode(SemanticStack& SemanticStack) {
		// pop off identifier node and store it in identifier actuals node.
		// pop off actuals node and store it in identifier actuals node.

		// push identifier actuals node onto stack.
		IdentifierFactorNode IdentifierWithActuals;

		if (typeid(*SemanticStack.top()).name() == "IdentifierNode")
		{
			IdentifierNode* Identifier = (IdentifierNode*)SemanticStack.top();
			IdentifierWithActuals.Identifier = Identifier;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Identifier Actuals Node, but didn't find Identifier Node on stack");
		}

		list<string>::iterator foundElement = find(ActualsNodes.begin(), ActualsNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != ActualsNodes.end())
		{
			BaseActualsNode* Actuals = (BaseActualsNode*)SemanticStack.top();
			IdentifierWithActuals.Actuals = Actuals;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Identifier Actuals Node, but didn't find Actuals Node on stack");
		}

		//PUSH ADRESS OF IDENTIFIER WITH ACTUALS NODE.
		cout << "adding a Identifier Actuals node!" << endl;
		SemanticStack.push(&IdentifierWithActuals);
	}

	virtual void visitSingletonIdentifierFactorNode(SemanticStack& SemanticStack) {
		// pop off identifier node and store it in singleton identifier factor node.

		// push singleton identifier factor node onto stack.
		SingletonIdentifierFactorNode SingletonIdentifierFactor;

		if (typeid(*SemanticStack.top()).name() == "IdentifierNode")
		{
			IdentifierNode* Identifier = (IdentifierNode*)SemanticStack.top();
			SingletonIdentifierFactor.Identifier = Identifier;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Singleton Identifier Factor Node, but didn't find Identifier Node on stack");
		}
		
		//PUSH ADRESS OF SINGLETON IDENTIFIER FACTOR NODE.
		cout << "adding a Singleton Identifier Factor node!" << endl;
		SemanticStack.push(&SingletonIdentifierFactor);
	}

	virtual void visitBaseActualsNode(SemanticStack& SemanticStack) {
		// represents epsilon case. Push actuals node which contains nothing
		BaseActualsNode BaseActuals;
		cout << "adding a Base Actuals node!" << endl;
		SemanticStack.push(&BaseActuals);
	}

	virtual void visitNonBaseActualsNode(SemanticStack& SemanticStack) {
		// pop non empty actuals node and store it in non base actuals node.

		// push non base actuals node onto stack.
		NonBaseActualsNode NonBaseActuals;

		if (typeid(*SemanticStack.top()).name() == "NonEmptyActualsNode")
		{
			NonEmptyActualsNode* NonEmptyActuals = (NonEmptyActualsNode*)SemanticStack.top();
			NonBaseActuals.NonEmptyActuals = NonEmptyActuals;
			SemanticStack.pop();
		}
		else {
			throw runtime_error("ERROR: Attempted to build Non Base Actuals Node, but didn't find Non empty Actuals Node on stack");
		}

		//PUSH ADRESS OF NON BASE ACTUALS NODE.
		cout << "adding a Non Base Actuals node!" << endl;
		SemanticStack.push(&NonBaseActuals);
	}

	virtual void visitNonEmptyActualsNode(SemanticStack& SemanticStack) {
		// pop off every expr node from the stack and store it inside a non empty actuals node.
		// Then push that non empty actuals node back onto the stack.
		NonEmptyActualsNode NonEmptyActuals;
		while (!SemanticStack.isEmpty())
		{

			list<string>::iterator foundElement = find(ExprNodes.begin(), ExprNodes.end(), typeid(*SemanticStack.top()).name());
			if (foundElement != ExprNodes.end())
			{
				BaseExprNode* ExprNode = (BaseExprNode*)SemanticStack.top();
				NonEmptyActuals.Expressions.push_back(ExprNode);
				SemanticStack.pop();
				continue;
			}
			else {
				//Exit while loop
				break;
			}
		}

		//PUSH ADRESS OF NON EMPTY ACTUALS NODE.
		cout << "adding a Non Empty Actuals node!" << endl;
		SemanticStack.push(&NonEmptyActuals);
	}

	virtual void visitIntegerLiteralNode(SemanticStack& SemanticStack, int IntData) {
		// store IntData inside the integer literal node and push that integer literal node onto the stack
		IntegerLiteralNode IntegerLiteral;
		IntegerLiteral.LiteralValue = "test"; // Convert IntData to string

		cout << "adding a Integer Literal node!" << endl;
		SemanticStack.push(&IntegerLiteral);
	}

	virtual void visitBooleanLiteralNode(SemanticStack& SemanticStack, string BooleanValue) {
		// store BooleanValue inside the boolean literal node and push that boolean literal node onto the stack
		BooleanLiteralNode BooleanLiteral;
		BooleanLiteral.LiteralValue = BooleanValue;

		cout << "adding a Boolean Literal node!" << endl;
		SemanticStack.push(&BooleanLiteral);
	}

	virtual void visitPrintStatementNode(SemanticStack& SemanticStack) {
		// pop off expr node from the stack and store it inside a print statment node.
		// Then push that  print statment node back onto the stack.
		PrintStatementNode PrintStatement;

		list<string>::iterator foundElement = find(ExprNodes.begin(), ExprNodes.end(), typeid(*SemanticStack.top()).name());
		if (foundElement != ExprNodes.end())
		{
			BaseExprNode* ExprNode = (BaseExprNode*)SemanticStack.top();
			PrintStatement.Expression = ExprNode;
		}
		else {
			throw runtime_error("ERROR: Attempted to build Print Statement Node, but didn't find Expression Node on stack");
		}


		//PUSH ADRESS OF NPRINT STATMENT NODE.
		cout << "adding a Print Statment node!" << endl;
		SemanticStack.push(&PrintStatement);
	}
};