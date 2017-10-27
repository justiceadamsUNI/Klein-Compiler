#pragma once
#include <string>
#include <vector>
#include "ReturnType.h"

using namespace std;

enum ASTNodeType {
	IDENTIFIER_NODE_TYPE,
	FACTOR_NODE_TYPE,
	BASE_ACTUALS_NODE_TYPE,
	BASE_TERM_NODE_TYPE,
	BASE_SIMPLE_EXPR_NODE_TYPE,
	BASE_EXPR_NODE_TYPE,
	FORMALS_NODE_TYPE,
	PRINT_STATEMENT_NODE_TYPE,
	LITERAL_NODE_TYPE,
	INTEGER_LITERAL_NODE_TYPE,
	BOOLEAN_LITERAL_NODE_TYPE,
	NON_EMPTY_ACTUALS_NODE_TYPE,
	NON_BASE_ACTUALS_NODE_TYPE,
	PARENTHESISED_EXPR_FACTOR_NODE_TYPE,
	SUBTRACTION_FACTOR_NODE_TYPE,
	LITERAL_FACTOR_NODE_TYPE,
	FUNCTION_CALL_TYPE,
	SINGLETON_IDENTIFIER_FACTOR_NODE_TYPE,
	NOT_FACTOR_NODE_TYPE,
	IF_FACTOR_NODE_TYPE,
	MULTIPLICATOR_TERM_NODE_TYPE,
	DIVIDER_TERM_NODE_TYPE,
	AND_TERM_NODE_TYPE,
	SUBTRACTOR_SIMPLE_EXPR_NODE_TYPE,
	OR_SIMPLE_EXPR_NODE_TYPE,
	ADDITION_SIMPLE_EXPR_NODE_TYPE,
	EQUAL_TO_EXPR_NODE_TYPE,
	LESS_THAN_EXPR_NODE_TYPE,
	TYPE_NODE_TYPE,
	BODY_NODE_TYPE,
	FORMAL_NODE_TYPE,
	NON_EMPTY_FORMALS_NODE_TYPE,
	DEF_NODE_TYPE,
	DEFINITIONS_NODE_TYPE,
	PROGRAM_NODE_TYPE
};

class ASTNode {
public:
	ASTNode(ASTNodeType InputNode){
		NodeType = InputNode;
	}

	ASTNodeType getAstNodeType() {
		return NodeType;
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
	ASTNode* getDefinitions() {
		return Definitions;
	}
	ReturnTypes getReturnType() {
		return NodeReturnType;
	}

	// Validator methods
	bool isFormalsNode() {
		return NodeType == FORMALS_NODE_TYPE || NodeType == NON_EMPTY_FORMALS_NODE_TYPE;
	}

	bool isExpressionNode() {
		return NodeType == BASE_EXPR_NODE_TYPE ||
			NodeType == LESS_THAN_EXPR_NODE_TYPE ||
			NodeType == EQUAL_TO_EXPR_NODE_TYPE;
	}

	bool isSimpleExpressionNode() {
		return NodeType == BASE_SIMPLE_EXPR_NODE_TYPE ||
			NodeType == ADDITION_SIMPLE_EXPR_NODE_TYPE ||
			NodeType == SUBTRACTOR_SIMPLE_EXPR_NODE_TYPE ||
			NodeType == OR_SIMPLE_EXPR_NODE_TYPE;
	}

	bool isTermNode() {
		return NodeType == BASE_TERM_NODE_TYPE ||
			NodeType == MULTIPLICATOR_TERM_NODE_TYPE ||
			NodeType == DIVIDER_TERM_NODE_TYPE ||
			NodeType == AND_TERM_NODE_TYPE;
	}

	// check if we need FACTOR_NODE_TYPE.
	bool isFactorNode() {
		return NodeType == FACTOR_NODE_TYPE ||
			NodeType == PARENTHESISED_EXPR_FACTOR_NODE_TYPE ||
			NodeType == SUBTRACTION_FACTOR_NODE_TYPE ||
			NodeType == LITERAL_FACTOR_NODE_TYPE ||
			NodeType == FUNCTION_CALL_TYPE ||
			NodeType == SINGLETON_IDENTIFIER_FACTOR_NODE_TYPE ||
			NodeType == NOT_FACTOR_NODE_TYPE ||
			NodeType == IF_FACTOR_NODE_TYPE;
	}

	bool isLiteralNode() {
		return NodeType == BOOLEAN_LITERAL_NODE_TYPE ||
			NodeType == INTEGER_LITERAL_NODE_TYPE;
	}

	bool isActualsNode() {
		return NodeType == BASE_ACTUALS_NODE_TYPE ||
			NodeType == NON_BASE_ACTUALS_NODE_TYPE;
	}

	bool isArithmeticTerm() {
		return NodeType == DIVIDER_TERM_NODE_TYPE ||
			NodeType == MULTIPLICATOR_TERM_NODE_TYPE;
	}

	bool isArithmeticSimpleExpression() {
		return NodeType == ADDITION_SIMPLE_EXPR_NODE_TYPE ||
			NodeType == SUBTRACTOR_SIMPLE_EXPR_NODE_TYPE;
	}
	
	// Setters
	void setIdentifierName(string InIdentifierName) {
		IdentifierName = InIdentifierName;
	}
	void setFactorNode(ASTNode* InFactorNode) {
		FactorNode = InFactorNode;
	}
	void setBaseTermNode(ASTNode* InBaseTermNode) {
		BaseTermNode = InBaseTermNode;
	}
	void setBaseSimpleExprNode(ASTNode* InBaseSimpleExprNode) {
		BaseSimpleExprNode = InBaseSimpleExprNode;
	}
	void setBaseExprNode(ASTNode* InBaseExprNode) {
		BaseExprNode = InBaseExprNode;
	}
	void setLiteralValue(string InLiteralValue) {
		LiteralValue = InLiteralValue;
	}
	void addExpressionToVector(ASTNode* InExpressions) {
		Expressions.push_back(InExpressions);
	}
	void setNonEmptyActualsNode(ASTNode* InNoneEmptyActualsNode) {
		NonEmptyActualsNode = InNoneEmptyActualsNode;
	}
	void setLiteralNode(ASTNode* InLiteralNode) {
		LiteralNode = InLiteralNode;
	}
	void setIdentifierNode(ASTNode* InIdentifierNodeVar) {
		IdentifierNodeVar = InIdentifierNodeVar;
	}
	void setBaseActualsNode(ASTNode* InBaseActualsNode) {
		BaseActualsNode = InBaseActualsNode;
	}
	void setBaseExprNode2(ASTNode* InBaseExprNode2) {
		BaseExprNode2 = InBaseExprNode2;
	}
	void setBaseExprNode3(ASTNode* InBaseExprNode3) {
		BaseExprNode3 = InBaseExprNode3;
	}
	void setFactorNode2(ASTNode* InFactorNode2) {
		FactorNode2 = InFactorNode2;
	}
	void setBaseTermNode2(ASTNode* InBaseTermNode2) {
		BaseTermNode2 = InBaseTermNode2;
	}
	void setBaseSimpleExprNode2(ASTNode* InBaseSimpleExprNode2) {
		BaseSimpleExprNode2 = InBaseSimpleExprNode2;
	}
	void setDataType(string InDataType) {
		DataType = InDataType;
	}
	void setTypeNode(ASTNode* InTypeNode) {
		TypeNode = InTypeNode;
	}
	void addPrintStatementToVector(ASTNode* InPrintStatement) {
		PrintStatements.push_back(InPrintStatement);
	}
	void addFormalNodeToVector(ASTNode* Formal) {
		FormalNodes.push_back(Formal);
	}
	void setFormalsNode(ASTNode* InFormalsNode) {
		FormalsNode = InFormalsNode;
	}
	void setBodyNode(ASTNode* InBodyNode) {
		BodyNode = InBodyNode;
	}
	void setDefinitionsNode(ASTNode* InDefinitions) {
		Definitions = InDefinitions;
	}
	void addDefToVector(ASTNode* Def) {
		DefNodes.push_back(Def);
	}
	void setReturnType(ReturnTypes type) {
		NodeReturnType = type;
	}
private :
	// everything initialized to null to represent there is no child node there in the tree.
	ASTNodeType NodeType;
	ReturnTypes NodeReturnType = NO_RETURN_TYPE;

	string IdentifierName = "NULL";
	string LiteralValue = "NULL";
	string DataType = "NULL";

	ASTNode* FactorNode = nullptr;
	ASTNode* FactorNode2 = nullptr;
	ASTNode* BaseTermNode = nullptr;
	ASTNode* BaseTermNode2 = nullptr;
	ASTNode* BaseSimpleExprNode = nullptr;
	ASTNode* BaseSimpleExprNode2 = nullptr;
	ASTNode* BaseExprNode = nullptr;
	ASTNode* BaseExprNode2 = nullptr;
	ASTNode* BaseExprNode3 = nullptr;
	ASTNode* LiteralNode = nullptr;
	ASTNode* IdentifierNodeVar = nullptr;
	ASTNode* BaseActualsNode = nullptr;
	ASTNode* NonEmptyActualsNode = nullptr;
	ASTNode* TypeNode = nullptr;
	ASTNode* FormalsNode = nullptr;
	ASTNode* BodyNode = nullptr;
	ASTNode* Definitions = nullptr;
	
	vector<ASTNode*> Expressions = {};
	vector<ASTNode*> PrintStatements = {};
	vector<ASTNode*> FormalNodes = {};
	vector<ASTNode*> DefNodes = {};
};
