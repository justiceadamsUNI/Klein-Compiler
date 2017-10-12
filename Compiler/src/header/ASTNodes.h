#pragma once
#include <string>
#include <vector>

using namespace std;

class ASTNode {
public:
	ASTNode(ASTNodeType InputNode){
		NodeType = InputNode;
	}
	string getIdentifierName() {
		return IdentifierName;
	}
	ASTNode* getFactorNode() {
		return FactorNode;
	}
	ASTNode* getBaseTermNode() {
		return BaseTermNode;
	}
		ASTNode* getBaseSimpleExprNode() {
		return BaseSimpleExprNode;
	}
	ASTNode* getBaseExprNode(){
		return BaseExprNode;
	}
	string getLiteralValue(){
		return LiteralValue;
	}
	vector<ASTNode*> getExpressions(){
		return Expressions;
	}
	ASTNode* getNonEmptyActualsNode(){
		return NonEmptyActualsNode;
	}
	ASTNode* getLiteralNode(){
		return LiteralNode;
	}
	ASTNode* getIdentifierNode(){
		return IdentifierNodeVar;
	}
	ASTNode* getBaseActualsNode(){
		return BaseActualsNode;

	}
	ASTNode* getBaseExprNode2(){
		return BaseExprNode2;
	}
	ASTNode* getBaseExprNode3(){
		return BaseExprNode3;
	}
	ASTNode* getFactorNode2(){
		return FactorNode2;
	}
	ASTNode* getBaseTermNode2(){
		return BaseTermNode2;
	}
	ASTNode* getBaseSimpleExprNode2(){
		return BaseSimpleExprNode2;
	}
	string getDataType(){
		return DataType;
	}
	ASTNode* getIdentifierNode(){
		return IdentifierNode;
	}
	ASTNode* getTypeNode(){
		return TypeNode;
	}
	vector<ASTNode*> getPrintStatements(){
		return PrintStatements;
	}
	vector<ASTNode*> getFormalNodes(){
		return FormalNodes;
	}
	ASTNode* getFormalsNode(){
		return FormalsNode;
	}
	ASTNode* getBodyNode(){
		return BodyNode;
	}
	vector<ASTNode*> getDefNodes(){
		return DefNodes;
	}
	vector<ASTNode*> getDefinitions() {
		return Definitions;
	}
private :
	ASTNodeType NodeType;
	string IdentifierName;
	ASTNode* FactorNode;
	ASTNode* BaseTermNode;
	ASTNode* BaseSimpleExprNode;
	ASTNode* BaseExprNode;
	string LiteralValue;
	vector<ASTNode*> Expressions;
	ASTNode* NonEmptyActualsNode;
	ASTNode* LiteralNode;
	ASTNode* IdentifierNodeVar;
	ASTNode* BaseActualsNode;
	ASTNode* BaseExprNode2;
	ASTNode* BaseExprNode3;
	ASTNode* FactorNode2;
	ASTNode* BaseTermNode2;
	ASTNode* BaseSimpleExprNode2;
	string DataType;
	ASTNode* IdentifierNode2;
	ASTNode* TypeNode;
	vector<ASTNode*> PrintStatements;
	vector<ASTNode*> FormalNodes;
	ASTNode* FormalsNode;
	ASTNode* BodyNode;
	vector<ASTNode*> DefNodes;
	vector<ASTNode*>Definitions;
};

enum ASTNodeType {
	IdentifierNode,
	FactorNode,
	BaseActualsNode,
	BaseTermNode,
	BaseSimpleExprNode,
	BaseExprNode,
	FormalsNode,
	PrintStatemetNode,
	LiteralNode,
	IntegerLiteralNode,
	BooleanLiteralNode,
	NonEmptyActualsNode,
	NonBaseActualsNode,
	ParenExprFactorNode,
	SubtractionFactorNode,
	LiteralFactorNode,
	IdentifierFactorNode,
	SingletonIdentifierFactorNode,
	NotFactorNode,
	IfFactorNode,
	MultiplicatorTermNode,
	DividerTermNode,
	AndTermNode,
	SubtractorSimpleExprNode,
	OrSimpleExprNode,
	AdditionSimpleExprNode,
	EqualToExprNode,
	LessThanExprNode,
	TypeNode,
	BodyNode,
	FormalNode,
	NonEmptyFormalsNode,
	DefNode,
	DefinitionsNode,
	ProgramNode
};

