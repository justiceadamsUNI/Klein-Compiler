#pragma once
#include <string>
#include <vector>

using namespace std;

class ASTNode {
	// Base AST Node from which every class extends.
};

class IdentifierNode : ASTNode {
public:
	string IdentifierName;
};

class FactorNode : ASTNode {
	// Base Factor Node that the others extend from
	// Should never be on the stack.
};

class BaseActualsNode : ASTNode {
	// Contains nothing.
	// Can be on the stack.
};

class BaseTermNode : ASTNode {
public:
	FactorNode Factor1;
};

class BaseSimpleExprNode : ASTNode {
public:
	BaseTermNode  Term1;
};

class BaseExprNode : ASTNode {
public:
	BaseSimpleExprNode SimpleExpression1;
};

class FormalsNode : ASTNode {
	// Contains Nothing
	// Can be on the stack since Formals can be epsilon
};

class PrintStatementNode : ASTNode {
public:
	BaseExprNode Expression;
};

class LiteralNode : ASTNode {
public:
	string LiteralValue;
	// This is a base class
	// Can not be on the stack.
};

class IntegerLiteralNode : LiteralNode {
};

class BooleanLiteralNode : LiteralNode {
};

class NonEmptyActualsNode : ASTNode {
public:
	vector<BaseExprNode> Expressions;
};

class NonBaseActualsNode : BaseActualsNode {
public:
	NonEmptyActualsNode NonEmptyActuals;
};

class ParenExprFactorNode : FactorNode {
public:
	BaseExprNode Expression;
};

class SubtractionFactorNode : FactorNode {
public:
	FactorNode Factor;
};

class LiteralFactorNode : FactorNode {
public:
	LiteralNode Literal;
};

class IdentifierFactorNode : FactorNode {
public:
	IdentifierNode Identifier;
	BaseActualsNode Actuals;
};

class SingletonIdentifierFactorNode : FactorNode {
public:
	IdentifierNode Identifier;
};

class NotFactorNode : FactorNode {
public:
	FactorNode Factor;
};

class IfFactorNode : FactorNode {
public:
	BaseExprNode ExpressionFollowingIf;
	BaseExprNode ExpressionFollowingThen;
	BaseExprNode ExpressionFollowingElse;
};

class MultiplicatorTermNode : BaseTermNode {
public:
	FactorNode Factor2;
};

class DividerTermNode : BaseTermNode {
public:
	FactorNode Factor2;
};

class AndTermNode : BaseTermNode {
public:
	FactorNode Factor2;
};

class SubtractorSimpleExprNode : BaseSimpleExprNode {
public:
	BaseTermNode  Term2;
};

class OrSimpleExprNode : BaseSimpleExprNode {
public:
	BaseTermNode  Term2;
};

class AdditionSimpleExprNode : BaseSimpleExprNode {
public:
	BaseTermNode  Term2;
};

class EqualToExprNode : BaseExprNode {
public:
	BaseSimpleExprNode SimpleExpression2;
};

class LessThanExprNode : BaseExprNode {
public:
	BaseSimpleExprNode SimpleExpression2;
};

class TypeNode : ASTNode {
public:
	string DataType;
};

class BodyNode : ASTNode {
public:
	vector<PrintStatementNode> PrintStatements;
	BaseExprNode Expression;
};

class FormalNode : ASTNode {
public:
	IdentifierNode Identifier;
	TypeNode Type;
};

class NonEmptyFormalsNode : FormalsNode {
public:
	vector<FormalNode> FormalNodes;
};

class DefNode : ASTNode {
public:
	IdentifierNode Identifier;
	FormalsNode Formals;
	TypeNode Type;
	BodyNode Body;
};

class DefinitionsNode : ASTNode {
public:
	vector<DefNode> DefNodes;
};

class ProgramNode : ASTNode {
public:
	vector<DefinitionsNode> Definitions;
};