#pragma once
#include <string>
#include <vector>

using namespace std;

enum ASTNodeType {
	IdentifierNode,
	FactorNodeType,
	BaseActualsNodeType,
	BaseTermNodeType,
	BaseSimpleExprNodeType,
	BaseExprNodeType,
	FormalsNodeType,
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
	ASTNode* getIdentifierNode2(){
		return IdentifierNode2;
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

	//Validator methods
	bool isFormalsNode() {
		return NodeType == FormalsNodeType || NodeType == NonEmptyFormalsNode;
	}

	bool isExpressionNode() {
		return NodeType == BaseExprNodeType ||
			NodeType == LessThanExprNode ||
			NodeType == EqualToExprNode;
	}

	bool isSimpleExpressionNode() {
		return NodeType == BaseSimpleExprNodeType ||
			NodeType == AdditionSimpleExprNode ||
			NodeType == SubtractorSimpleExprNode ||
			NodeType == OrSimpleExprNode;
	}

	bool isTermNode() {
		return NodeType == BaseTermNodeType ||
			NodeType == MultiplicatorTermNode ||
			NodeType == DividerTermNode ||
			NodeType == AndTermNode;
	}

	bool isFactorNode() {
		return NodeType == FactorNodeType ||
			NodeType == ParenExprFactorNode ||
			NodeType == SubtractionFactorNode ||
			NodeType == LiteralFactorNode ||
			NodeType == IdentifierFactorNode ||
			NodeType == SingletonIdentifierFactorNode ||
			NodeType == NotFactorNode ||
			NodeType == IfFactorNode;
	}

	bool isLiteralNode() {
		return NodeType == BooleanLiteralNode ||
			NodeType == IntegerLiteralNode;
	}

	bool isActualsNode() {
		return NodeType == BaseActualsNodeType ||
			NodeType == NonBaseActualsNode;
	}

	bool isArithmeticTerm() {
		return NodeType == DividerTermNode ||
			NodeType == MultiplicatorTermNode;
	}

	bool isArithmeticSimpleExpression() {
		return NodeType == AdditionSimpleExprNode ||
			NodeType == SubtractorSimpleExprNode;
	}
	
	//Setters
		void setIdentifierName(string inIdentifierName) {
		IdentifierName = inIdentifierName;
	}
	void setFactorNode(ASTNode* inFactorNode) {
		FactorNode = inFactorNode;
	}
	void setBaseTermNode(ASTNode* inBaseTermNode) {
		BaseTermNode = inBaseTermNode;
	}
	void setBaseSimpleExprNode(ASTNode* inBaseSimpleExprNode) {
		BaseSimpleExprNode = inBaseSimpleExprNode;
	}
	void setBaseExprNode(ASTNode* inBaseExprNode) {
		BaseExprNode = inBaseExprNode;
	}
	void setLiteralValue(string inLiteralValue) {
		LiteralValue = inLiteralValue;
	}
	void addExpressionToVector(ASTNode* inExpressions) {
		Expressions.push_back(inExpressions);
	}
	void setNonEmptyActualsNode(ASTNode* inNoneEmptyActualsNode) {
		NonEmptyActualsNode = inNoneEmptyActualsNode;
	}
	void setLiteralNode(ASTNode* inLiteralNode) {
		LiteralNode = inLiteralNode;
	}
	void setIdentifierNode(ASTNode* inIdentifierNodeVar) {
		IdentifierNodeVar = inIdentifierNodeVar;
	}
	void setBaseActualsNode(ASTNode* inBaseActualsNode) {
		BaseActualsNode = inBaseActualsNode;
	}
	void setBaseExprNode2(ASTNode* inBaseExprNode2) {
		BaseExprNode2 = inBaseExprNode2;
	}
	void setBaseExprNode3(ASTNode* inBaseExprNode3) {
		BaseExprNode3 = inBaseExprNode3;
	}
	void setFactorNode2(ASTNode* inFactorNode2) {
		FactorNode2 = inFactorNode2;
	}
	void setBaseTermNode2(ASTNode* inBaseTermNode2) {
		BaseTermNode2 = inBaseTermNode2;
	}
	void setBaseSimpleExprNode2(ASTNode* inBaseSimpleExprNode2) {
		BaseSimpleExprNode2 = inBaseSimpleExprNode2;
	}
	void setDataType(string inDataType) {
		DataType = inDataType;
	}
	void setIdentifierNode2(ASTNode* inIdentifierNode2) {
		IdentifierNode2 = inIdentifierNode2;
	}
	void setTypeNode(ASTNode* inTypeNode) {
		TypeNode = inTypeNode;
	}
	void addPrintStatementToVector(ASTNode* inPrintStatement) {
		PrintStatements.push_back(inPrintStatement);
	}
	void addFormalNodeToVector(ASTNode* Formal) {
		FormalNodes.push_back(Formal);
	}
	void setFormalsNode(ASTNode* inFormalsNode) {
		FormalsNode = inFormalsNode;
	}
	void setBodyNode(ASTNode* inBodyNode) {
		BodyNode = inBodyNode;
	}
	void addDefinitionToVector(ASTNode* Definition) {
		Definitions.push_back(Definition);
	}
	void addDefToVector(ASTNode* Def) {
		DefNodes.push_back(Def);
	}
private :
	ASTNodeType NodeType;

	string IdentifierName = "test";
	ASTNode* FactorNode = nullptr;
	ASTNode* BaseTermNode = nullptr;
	ASTNode* BaseSimpleExprNode = nullptr;
	ASTNode* BaseExprNode = nullptr;
	string LiteralValue = "test";
	vector<ASTNode*> Expressions = {};
	ASTNode* NonEmptyActualsNode = nullptr;
	ASTNode* LiteralNode = nullptr;
	ASTNode* IdentifierNodeVar = nullptr;
	ASTNode* BaseActualsNode = nullptr;
	ASTNode* BaseExprNode2 = nullptr;
	ASTNode* BaseExprNode3 = nullptr;
	ASTNode* FactorNode2 = nullptr;
	ASTNode* BaseTermNode2 = nullptr;
	ASTNode* BaseSimpleExprNode2 = nullptr;
	string DataType = "test";
	ASTNode* IdentifierNode2 = nullptr;
	ASTNode* TypeNode = nullptr;
	vector<ASTNode*> PrintStatements = {};
	vector<ASTNode*> FormalNodes = {};
	ASTNode* FormalsNode = nullptr;
	ASTNode* BodyNode = nullptr;
	vector<ASTNode*> DefNodes = {};
	vector<ASTNode*>Definitions = {};
};
