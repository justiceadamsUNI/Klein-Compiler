#pragma once
#include <string>
#include <vector>

using namespace std;

class ASTNode {
	virtual void polymorphicEnabler() {}
	// Base AST Node from which every class extends.
};

class IdentifierNode : public ASTNode {
public:
	string IdentifierName;
};

class FactorNode : public ASTNode {
	// Base Factor Node that the others extend from
	// Should never be on the stack.
};

class BaseActualsNode : public ASTNode {
	// Contains nothing.
	// Can be on the stack.
};

class BaseTermNode : public ASTNode {
public:
	FactorNode* Factor1;
};

class BaseSimpleExprNode : public ASTNode {
public:
	BaseTermNode*  Term1;
};

class BaseExprNode : public ASTNode {
public:
	BaseSimpleExprNode* SimpleExpression1;
};

class FormalsNode : public ASTNode {
	// Contains Nothing
	// Can be on the stack since Formals can be epsilon
};

class PrintStatementNode : public ASTNode {
public:
	BaseExprNode* Expression;
};

class LiteralNode : public ASTNode {
public:
	string LiteralValue;
	// This is a base class
	// Can not be on the stack.
};

class IntegerLiteralNode : public LiteralNode {
};

class BooleanLiteralNode : public LiteralNode {
};

class NonEmptyActualsNode : public ASTNode {
public:
	vector<BaseExprNode*> Expressions;
};

class NonBaseActualsNode : public BaseActualsNode {
public:
	NonEmptyActualsNode* NonEmptyActuals;
};

class ParenExprFactorNode : public FactorNode {
public:
	BaseExprNode* Expression;
};

class SubtractionFactorNode : public FactorNode {
public:
	FactorNode* Factor;
};

class LiteralFactorNode : public FactorNode {
public:
	LiteralNode* Literal;
};

class IdentifierFactorNode : public FactorNode {
public:
	IdentifierNode* Identifier;
	BaseActualsNode* Actuals;
};

class SingletonIdentifierFactorNode : public FactorNode {
public:
	IdentifierNode* Identifier;
};

class NotFactorNode : public FactorNode {
public:
	FactorNode* Factor;
};

class IfFactorNode : public FactorNode {
public:
	BaseExprNode* Expression1;
	BaseExprNode* Expression2;
	BaseExprNode* Expression3;
};

class MultiplicatorTermNode : public BaseTermNode {
public:
	FactorNode* Factor2;
};

class DividerTermNode : public BaseTermNode {
public:
	FactorNode* Factor2;
};

class AndTermNode : public BaseTermNode {
public:
	FactorNode* Factor2;
};

class SubtractorSimpleExprNode : public BaseSimpleExprNode {
public:
	BaseTermNode*  Term2;
};

class OrSimpleExprNode : public BaseSimpleExprNode {
public:
	BaseTermNode*  Term2;
};

class AdditionSimpleExprNode : public BaseSimpleExprNode {
public:
	BaseTermNode*  Term2;
};

class EqualToExprNode : public BaseExprNode {
public:
	BaseSimpleExprNode* SimpleExpression2;
};

class LessThanExprNode : public BaseExprNode {
public:
	BaseSimpleExprNode* SimpleExpression2;
};

class TypeNode : public ASTNode {
public:
	string DataType;
};

class BodyNode : public ASTNode {
public:
	vector<PrintStatementNode*> PrintStatements;
	BaseExprNode* Expression;
};

class FormalNode : public ASTNode {
public:
	IdentifierNode* Identifier;
	TypeNode* Type;
};

class NonEmptyFormalsNode : public FormalsNode {
public:
	vector<FormalNode*> FormalNodes;
};

class DefNode : public ASTNode {
public:
	IdentifierNode* Identifier;
	FormalsNode* Formals;
	TypeNode* Type;
	BodyNode* Body;
};

class DefinitionsNode : public ASTNode {
public:
	vector<DefNode*> DefNodes;
};

class ProgramNode : public ASTNode {
public:
	vector<DefinitionsNode*> Definitions;
};