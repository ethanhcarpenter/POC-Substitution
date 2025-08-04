#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <regex>
#include <map>

enum TokenType {
	Lambda,
	Church,
	NumberLiteral,
	OpenParenthesis,
	OpenBraces,
	CloseBraces,
	CloseParenthesis,
	OpenBrackets,
	CloseBrackets,
	Variable,
	Expression,
	Comma,
	Dot,
	EndOfExpression,
};

class Token {
private:
	TokenType type;
	std::string stringType;
	std::string value;
public:
	Token(TokenType tokentype, std::string val, std::string stype);
	TokenType getType();
	std::string getValue();
	std::string getStringType();
};

class Var {
private:
	std::string name;
public:
	void setName(std::string n);
	std::string getName();
};

class Abstraction {
private:
	Abstraction* nextExpression = nullptr;
	std::string finalExpression="";
	Var* boundVariable;
public:
	Abstraction* getNextExpression();
	void setNextExpression(Abstraction* next);
	void addToFinalExpression(std::string c);
	void setFinalExpression(std::string expression);
	std::string getFinalExpression();
	bool hasNextExpression();
	Var* getBoundVariable();
	void setBoundVariable(Var* v);
};

class Applicaption {
private:
	Abstraction expression;
	std::string input;
public:
	Abstraction solve();
};

class Lexer {
private:
	std::string stringForm;
	std::string preLambdaReserve = "";
	Abstraction* tree;
	std::vector<std::string> inputs = {};
	std::vector<Token> tokens;
public:
	Lexer();
	void lex();
	void apply();
	std::vector<Token> getTokens();
	void tokenize(const std::string input,bool justTokens=false);
	Abstraction* getLowestAbstraction();
	Abstraction* getHighestAbstraction();
	void setHighestAbstraction(Abstraction* newHighest);
	Token* browseFromCurrentToken(int tokenIndex, int forwards);
	std::string tokenStringValue(int tokenIndex);
	bool isFutureLambda(int tokenIndex);
	std::vector<std::string>* getInputs();
	void overrideInput(std::string input,int depth);
	void createNewInput();
	void addStringToLastInput(std::string s);
	void printTokens();
	void replaceVariableNames(int depth);
	void updateStringForm(int depth=-1);
	std::string getStringForm();
	std::string getPreLambdaReserve();
	void setPreLambdaReserve(std::string reserve);
	void addStringToPreLambdaReserve(std::string reserve);
	void removeTopLevelBrackets();
};

class Parser {
private:
	Lexer* lexer=new Lexer;
	std::vector<std::vector<std::map<std::string, std::string>>> correspondingVariableNames;
	std::string finalExpression = "";
public:
	Lexer* getLexer();
    void evaluate(int inputRecurse =0,std::string expression="");
	static std::string betaReduce(std::string expression, std::string var, std::string replace);
	std::tuple<std::string, Lexer> alphaReduce(std::string input, int inputRecurse, int depth);
	void logEvents();
	std::string getFinalExpression();
	void cleanupFinalExpression();

};
