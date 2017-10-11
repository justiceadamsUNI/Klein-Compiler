#include "../header/ASTNodeVisitor.h"

class NodeBuilderVisitor : public ASTNodeVisitor {
public:
	virtual void visitProgramNode(PStack& SemanticStack) {
		// pop off every defenition node from the stack and store it inside a program node.
		// Then push that program node back onto the stack.
	}

	virtual void visitDefinitionsNode(PStack& SemanticStack) {
		// pop off every def node from the stack and store it inside a definitions node.
		// Then push that definitions node back onto the stack.
	}

	virtual void visitIdentifierNode(PStack& SemanticStack, string IdentifierName) {
		// store the identifier name inside the identifier node, then push the identifier node onto the stack.
	}

	virtual void visitDefNode(PStack& SemanticStack) {
		// pop off identifier node and store it in Def node.
		// pop off formals node and store it in Def node.
		// pop off type node and store it in Def node.
		// pop off vody node and store it in Def node.

		// push Def node onto stack.
	}

	virtual void visitFormalsNode(PStack& SemanticStack) {
		// Not sure why we need this? <-------------
	}

	virtual void visitNonEmptyFormalsNode(PStack& SemanticStack) {
		// pop off every Formal node from the stack and store it inside a Non Empty Formals Node.
		// Then push that Non Empty Formals Node node back onto the stack.
	}

	virtual void visitFormalNode(PStack& SemanticStack) {
		// pop off identifier node and store it in Formal node.
		// pop off type node and store it in Formal node.

		// push Formal Node onto stack.
	}

	virtual void visitBodyNode(PStack& SemanticStack) {
		// pop off list of print statment nodes and store it in Body node.
		// pop off expr node and store it in Body node.

		// push Body node onto stack..
	}

	virtual void visitTypeNode(PStack& SemanticStack, string Type) {
		// store Type string inside the Type Node.
		// push Type node onto the stack
	}

	virtual void visitLessThanNode(PStack& SemanticStack) {
		// pop off simple expression node and store it in Less than node.
		// pop off simple expression node and store it in Less than node.

		// push less than Node onto stack.
	}

	virtual void visitEqualNode(PStack& SemanticStack) {
		// pop off simple expression node and store it in equal than node.
		// pop off simple expression node and store it in equal than node.

		// pushequal Node onto stack.
	}

	virtual void visitBaseExpressionNode(PStack& SemanticStack) {
		// pop off simple expression node and store it in base expression node

		// push Base expression node onto the stack.
	}

	virtual void visitOrNode(PStack& SemanticStack) {
		// pop off term node and store it in or node.
		// pop off term node and store it in or node.

		// push or Node onto stack.
	}

	virtual void visitAdditionNode(PStack& SemanticStack) {
		// pop off term node and store it in addition node.
		// pop off term node and store it in addition node.

		// push addition Node onto stack.
	}

	virtual void visitSubtractionNode(PStack& SemanticStack) {
		// pop off term node and store it in subtaction node.
		// pop off term node and store it in subtraction node.

		// push subtraction Node onto stack.

	}

	virtual void visitBaseSimpleExpressionNode(PStack& SemanticStack) {
		// pop off term node and store it in Base simple expression node.

		// push Base simple expression Node onto stack.

	}

	virtual void visitAndNode(PStack& SemanticStack) {
		// pop off factor node and store it in and node.
		// pop off factor node and store it in and node.

		// push and Node onto stack.

	}

	virtual void visitMultiplicatorNode(PStack& SemanticStack) {
		// pop off factor node and store it in multiplicator node.
		// pop off factor node and store it in multiplicator node.

		// push multiplicator note onto stack.

	}

	virtual void visitDividerNode(PStack& SemanticStack) {
		// pop off factor node and store it in Divider node.
		// pop off factor node and store it in Divider node.

		// push Divider note onto stack.
	
	}

	virtual void visitBaseTermNode(PStack& SemanticStack) {
		// pop off factor node and store it in Base term expression node.

		// push Base term expression Node onto stack.

	}

	virtual void visitIfFactorNode(PStack& SemanticStack) {
		// pop off expr node and store it in if factor node.
		// pop off expr node and store it in if factor node.
		// pop off expr node and store it in if factor node.

		// push if factor node onto stack.
	}

	virtual void visitNotFactorNode(PStack& SemanticStack) {
		// pop off factor node and store it in not factor node

		// push not factor node onto stack.
	}

	virtual void visitLiteralFactorNode(PStack& SemanticStack) {
		// pop off literal node and store it in literal factor node

		// push literal factor node onto stack.
	}

	virtual void visitSubtractionFactorNode(PStack& SemanticStack) {
		// pop off factor node and store it in subtraction factor node

		// push subtraction factor node onto stack.

	}

	virtual void visitParenthesisedExpressionNode(PStack& SemanticStack) {
		// pop off expr node and store it in parenthesised expression node.

		// push parenthesised expression node onto stack.
	}

	virtual void visitIdentifierActualsNode(PStack& SemanticStack) {
		// pop off identifier node and store it in identifier actuals node.
		// pop off actuals node and store it in identifier actuals node.

		// push identifier actuals node onto stack.
	}

	virtual void visitSingletonIdentifierFactorNode(PStack& SemanticStack) {
		// pop off identifier node and store it in singleton identifier factor node.

		// push singleton identifier factor node onto stack.
	}

	virtual void visitBaseActualsNode(PStack& SemanticStack) {
		// Why do we want something that holds nothing? <--------
	}

	virtual void visitNonBaseActualsNode(PStack& SemanticStack) {
		// pop non empty actuals node and store it in non base actuals node.
		
		// push non base actuals node onto stack.
	}

	virtual void visitNonEmptyActualsNode(PStack& SemanticStack) {
		// pop off every expr node from the stack and store it inside a non empty actuals node.
		// Then push that non empty actuals node back onto the stack.
	}

	virtual void visitIntegerLiteralNode(PStack& SemanticStack, int IntData) {
		// store IntData inside the integer literal node and push that integer literal node onto the stack
	}

	virtual void visitBooleanLiteralNode(PStack& SemanticStack, string BooleanValue) {
		// store BooleanValue inside the boolean literal node and push that boolean literal node onto the stack
	}

	virtual void visitPrintStatementNode(PStack& SemanticStack) {
		// pop off expr node from the stack and store it inside a print statment node.
		// Then push that  print statment node back onto the stack.
	}
};