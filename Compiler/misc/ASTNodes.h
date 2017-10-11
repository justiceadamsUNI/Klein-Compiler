#include <string>
#include <vector>

using namespace std;

class ASTNode {

};

class IdentifierNode : ASTNode {
	string identifierLiteral;
};

class PrintStatementNode : ASTNode {
	BaseExprNode expression;
};

class LiteralNode : ASTNode {
	string literalWord;
};

class IntegerLiteralNode : ASTNode {
	string integerLiteral;
};

class BooleanLiteralNode : ASTNode {
	string booleanLiteral;
};

class NonEmptyActualsNode : ASTNode {
	vector<BaseExprNode> expressions;
};

class NonBaseActualsNode : ASTNode {
	NonEmptyActualsNode nonEmptyActuals;
};

class BaseActualsNode : ASTNode {

};

class FactorNode : ASTNode {
	//Check this
};

class ParenExprFactorNode : ASTNode {
	BaseExprNode expression;
};

class SubtractionFactorNode : ASTNode {
	FactorNode factor;
};

class LiteralFactorNode : ASTNode {
	LiteralNode literals;
};

class IdentifierFactorNode : ASTNode {
	IdentifierNode identifier;
	BaseActualsNode actuals;
};

class SingletonIdentifierFactorNode : ASTNode {
	IdentifierNode identifier;
};

class NotFactorNode : ASTNode {
	FactorNode factor;
};

class IfFactorNode : ASTNode {
	vector<BaseExprNode> expressions;
};

class MultiplicatorTermNode : ASTNode {
	vector<FactorNode> factors;
};

class DividerTermNode : ASTNode {
	vector<FactorNode> factors;
};

class AndTermNode : ASTNode {
	vector<FactorNode> factors;
};

class BaseTermNode : ASTNode {
	FactorNode factor;
};

class SubtractorSimpleExprNode : ASTNode {
	vector<BaseTermNode> terms;
};

class OrSimpleExprNode : ASTNode {
	vector<BaseTermNode> terms;
};

class AdditionSimpleExprNode : ASTNode {
	vector<BaseTermNode> terms;
};

class BaseSimpleExprNode : ASTNode {
	BaseTermNode  term;
};

class BaseExprNode : ASTNode {
	BaseSimpleExprNode simpleExpression;
};

class EqualToExprNode : ASTNode {
	vector<BaseSimpleExprNode> simpleExpressions;
};

class LessThanExprNode : ASTNode {
	vector<BaseSimpleExprNode> simpleExpressions;
};

class TypeNode : ASTNode {
	string dataType;
};

class BodyNode : ASTNode {
	vector<PrintStatementNode> printStatements;
	BaseExprNode expression;
};

class FormalNode : ASTNode {
	IdentifierNode identifier;
	TypeNode type;
};

class NonEmptyFormalsNode : ASTNode {
	vector<FormalsNode> formals;
};

class FormalsNode : ASTNode {

};

class DefNode : ASTNode {
	IdentifierNode identifier;
	FormalsNode formals;
	TypeNode type;
	BodyNode body;
};

class DefinitionsNode : ASTNode {
	vector<DefNode> defs;
};

class ProgramNode : ASTNode {
	vector<DefinitionsNode> definitions;
};