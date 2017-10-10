#pragma once
#include "PStack.h"
#include <string>
#include <stdexcept>

using namespace std;

class ASTNodeVisitor {
public:
	// PStack needs to be SemanticStack! Just here to compile for right now until harsha is done.
	void accept(StackValues ParserStackValue, PStack& SemanticStack, string StringData, int IntData) {
		switch (ParserStackValue)
		{
		case BUILD_PROGRAM_NODE:
			visitProgramNode(SemanticStack);
			return;
		case BUILD_DEFINITIONS_NODE:
			visitDefinitionsNode(SemanticStack);
			return;
		case BUILD_IDENTIFIER_NODE:
			visitIdentifierNode(SemanticStack, StringData);
			return;
		case BUILD_DEF_NODE:
			visitDefNode(SemanticStack);
			return;
		case BUILD_FORMALS_NODE:
			visitFormalsNode(SemanticStack);
			return;
		case BUILD_NONEMPTYFORMALS_NODE:
			visitNonEmptyFormalsNode(SemanticStack);
			return;
		case BUILD_FORMAL_NODE:
			visitFormalNode(SemanticStack);
			return;
		case BUILD_BODY_NODE:
			visitBodyNode(SemanticStack);
			return;
		case BUILD_TYPE_NODE:
			visitTypeNode(SemanticStack);
			return;
		case BUILD_LESSTHAN_NODE:
			visitLessThanNode(SemanticStack);
			return;
		case BUILD_EQUAL_NODE:
			visitEqualNode(SemanticStack);
			return;
		case BUILD_BASEEXPR_NODE:
			visitBaseExpressionNode(SemanticStack);
			return;
		case BUILD_OR_NODE:
			visitOrNode(SemanticStack);
			return;
		case BUILD_ADDITION_NODE:
			visitAdditionNode(SemanticStack);
			return;
		case BUILD_SUBTRACTION_NODE:
			visitSubtractionNode(SemanticStack);
			return;
		case BUILD_BASESIMPLEEXPR_NODE:
			visitBaseSimpleExpressionNode(SemanticStack);
			return;
		case BUILD_AND_NODE:
			visitAndNode(SemanticStack);
			return;
		case BUILD_MULTIPLICATOR_NODE:
			visitMultiplicatorNode(SemanticStack);
			return;
		case BUILD_DIVIDER_NODE:
			visitDividerNode(SemanticStack);
			return;
		case BUILD_BASETERM_NODE:
			visitBaseTermNode(SemanticStack);
			return;
		case BUILD_IFFACTOR_NODE:
			visitIfFactorNode(SemanticStack);
			return;
		case BUILD_NOTFACTOR_NODE:
			visitNotFactorNode(SemanticStack);
			return;
		case BUILD_LITERALFACTOR_NODE:
			visitLiteralFactorNode(SemanticStack);
			return;
		case BUILD_SUBTRACTIONFACTOR_NODE:
			visitSubtractionFactorNode(SemanticStack);
			return;
		case BUILD_PARENEXPR_NODE:
			visitParenthesisedExpressionNode(SemanticStack);
			return;
		case BUILD_IDENTIFIERACTUALS_NODE:
			visitIdentifierActualsNode(SemanticStack);
			return;
		case BUILD_SINGLETONIDENTIFIERFACTOR_NODE:
			visitSingletonIdentifierFactorNode(SemanticStack);
			return;
		case BUILD_BASEACTUALS_NODE:
			visitBaseActualsNode(SemanticStack);
			return;
		case BUILD_NONBASEACTUALS_NODE:
			visitNonBaseActualsNode(SemanticStack);
			return;
		case BUILD_NONEMPTYACTUALS_NODE:
			visitNonEmptyActualsNode(SemanticStack);
			return;
		case BUILD_INTEGERLITERAL_NODE:
			visitIntegerLiteralNode(SemanticStack, IntData);
			return;
		case BUILD_BOOLEANLITERAL_NODE:
			visitBooleanLiteralNode(SemanticStack, StringData);
			return;
		case BUILD_PRINTSTATEMENT_NODE:
			visitPrintStatementNode(SemanticStack);
			return;
		default:
			break;
		}

		throw runtime_error("ERROR: Semantic action expected, but found - " + StackValuesPrintMap.find(ParserStackValue)->second);
	}

	// Visitor methods
	virtual void visitProgramNode(PStack& SemanticStack) = 0;
	virtual void visitDefinitionsNode(PStack& SemanticStack) = 0;
	virtual void visitIdentifierNode(PStack& SemanticStack, string IdentifierName) = 0;
	virtual void visitDefNode(PStack& SemanticStack) = 0;
	virtual void visitFormalsNode(PStack& SemanticStack) = 0;
	virtual void visitNonEmptyFormalsNode(PStack& SemanticStack) = 0;
	virtual void visitFormalNode(PStack& SemanticStack) = 0;
	virtual void visitBodyNode(PStack& SemanticStack) = 0;
	virtual void visitTypeNode(PStack& SemanticStack) = 0;
	virtual void visitLessThanNode(PStack& SemanticStack) = 0;
	virtual void visitEqualNode(PStack& SemanticStack) = 0;
	virtual void visitBaseExpressionNode(PStack& SemanticStack) = 0;
	virtual void visitOrNode(PStack& SemanticStack) = 0;
	virtual void visitAdditionNode(PStack& SemanticStack) = 0;
	virtual void visitSubtractionNode(PStack& SemanticStack) = 0;
	virtual void visitBaseSimpleExpressionNode(PStack& SemanticStack) = 0;
	virtual void visitAndNode(PStack& SemanticStack) = 0;
	virtual void visitMultiplicatorNode(PStack& SemanticStack) = 0;
	virtual void visitDividerNode(PStack& SemanticStack) = 0;
	virtual void visitBaseTermNode(PStack& SemanticStack) = 0;
	virtual void visitIfFactorNode(PStack& SemanticStack) = 0;
	virtual void visitNotFactorNode(PStack& SemanticStack) = 0;
	virtual void visitLiteralFactorNode(PStack& SemanticStack) = 0;
	virtual void visitSubtractionFactorNode(PStack& SemanticStack) = 0;
	virtual void visitParenthesisedExpressionNode(PStack& SemanticStack) = 0;
	virtual void visitIdentifierActualsNode(PStack& SemanticStack) = 0;
	virtual void visitSingletonIdentifierFactorNode(PStack& SemanticStack) = 0;
	virtual void visitBaseActualsNode(PStack& SemanticStack) = 0;
	virtual void visitNonBaseActualsNode(PStack& SemanticStack) = 0;
	virtual void visitNonEmptyActualsNode(PStack& SemanticStack) = 0;
	virtual void visitIntegerLiteralNode(PStack& SemanticStack, int IntData) = 0;
	virtual void visitBooleanLiteralNode(PStack& SemanticStack, string BooleanValue) = 0;
	virtual void visitPrintStatementNode(PStack& SemanticStack) = 0;
};