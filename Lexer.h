#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

enum TokenType {
	Lambda,
	OpenParenthesis,
	CloseParenthesis,
	Variable,
	Expression,
	NumberLiteral,
	Dot,
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

class Abs {
private:
	Abs* nextExpression=nullptr;
	std::string finalExpression;
	Var* boundVariable;
public:
	Abs* getNextExpression();
	void setNextExpression(Abs* next);
	bool hasNextExpression();
	void setBoundVariable(Var* v);
};

class Lexer {
private:
	Abs* tree = new Abs;
	std::vector<Token> tokens;
public:
	Lexer();
	void lex();
	void tokenize(const std::string input);
	Abs* getLowestAbstraction();
	Abs* getHighestAbstraction();
	void printTokens();
};
