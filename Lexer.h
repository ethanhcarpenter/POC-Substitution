#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

enum TokenType {
	Lambda,
	NumberLiteral,
	OpenParenthesis,
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
	void setName(char n);
	char getName();
};

class Abstraction {
private:
	Abstraction* nextExpression = nullptr;
	std::string finalExpression="";
	Var* boundVariable;
public:
	Abstraction* getNextExpression();
	void setNextExpression(Abstraction* next);
	void addToFinalExpression(char c);
	bool hasNextExpression();
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
	Abstraction* tree;
	std::vector<std::string> inputs = {};
	std::vector<Token> tokens;
public:
	Lexer();
	void lex();
	void apply();
	void tokenize(const std::string input);
	Abstraction* getLowestAbstraction();
	Abstraction* getHighestAbstraction();
	Token* browseFromCurrentToken(int tokenIndex, int forwards);
	char tokenCharValue(int tokenIndex);
	bool isFutureLambda(int tokenIndex);
	std::vector<std::string>* getInputs();
	void createNewInput();
	void addtoLastInput(char c);
	void printTokens();
};

class Parser {
private:
	Lexer* lexer = new Lexer;
	std::string evaluatedValue = "";
public:
	Lexer* getLexer();
	void evaluate();

};
