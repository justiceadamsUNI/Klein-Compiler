#pragma once
#include <string>
#include <stdexcept>
#include "SemanticStack.h"
#include "StackValues.h"

using namespace std;

class ASTNodeVisitor {
public:
	void accept(StackValues ParserStackValue, SemanticStack& SemanticStack, string StringData, string IntData) {
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
			visitTypeNode(SemanticStack, StringData);
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
	virtual void visitProgramNode(SemanticStack& SemanticStack) = 0;
	virtual void visitDefinitionsNode(SemanticStack& SemanticStack) = 0;
	virtual void visitIdentifierNode(SemanticStack& SemanticStack, string IdentifierName) = 0;
	virtual void visitDefNode(SemanticStack& SemanticStack) = 0;
	virtual void visitFormalsNode(SemanticStack& SemanticStack) = 0;
	virtual void visitNonEmptyFormalsNode(SemanticStack& SemanticStack) = 0;
	virtual void visitFormalNode(SemanticStack& SemanticStack) = 0;
	virtual void visitBodyNode(SemanticStack& SemanticStack) = 0;
	virtual void visitTypeNode(SemanticStack& SemanticStack, string IdentifierName) = 0;
	virtual void visitLessThanNode(SemanticStack& SemanticStack) = 0;
	virtual void visitEqualNode(SemanticStack& SemanticStack) = 0;
	virtual void visitBaseExpressionNode(SemanticStack& SemanticStack) = 0;
	virtual void visitOrNode(SemanticStack& SemanticStack) = 0;
	virtual void visitAdditionNode(SemanticStack& SemanticStack) = 0;
	virtual void visitSubtractionNode(SemanticStack& SemanticStack) = 0;
	virtual void visitBaseSimpleExpressionNode(SemanticStack& SemanticStack) = 0;
	virtual void visitAndNode(SemanticStack& SemanticStack) = 0;
	virtual void visitMultiplicatorNode(SemanticStack& SemanticStack) = 0;
	virtual void visitDividerNode(SemanticStack& SemanticStack) = 0;
	virtual void visitBaseTermNode(SemanticStack& SemanticStack) = 0;
	virtual void visitIfFactorNode(SemanticStack& SemanticStack) = 0;
	virtual void visitNotFactorNode(SemanticStack& SemanticStack) = 0;
	virtual void visitLiteralFactorNode(SemanticStack& SemanticStack) = 0;
	virtual void visitSubtractionFactorNode(SemanticStack& SemanticStack) = 0;
	virtual void visitParenthesisedExpressionNode(SemanticStack& SemanticStack) = 0;
	virtual void visitIdentifierActualsNode(SemanticStack& SemanticStack) = 0;
	virtual void visitSingletonIdentifierFactorNode(SemanticStack& SemanticStack) = 0;
	virtual void visitBaseActualsNode(SemanticStack& SemanticStack) = 0;
	virtual void visitNonBaseActualsNode(SemanticStack& SemanticStack) = 0;
	virtual void visitNonEmptyActualsNode(SemanticStack& SemanticStack) = 0;
	virtual void visitIntegerLiteralNode(SemanticStack& SemanticStack, string IntData) = 0;
	virtual void visitBooleanLiteralNode(SemanticStack& SemanticStack, string BooleanValue) = 0;
	virtual void visitPrintStatementNode(SemanticStack& SemanticStack) = 0;
};