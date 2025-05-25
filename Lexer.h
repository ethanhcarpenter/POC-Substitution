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

class Lexer{
private:
	std::vector<Token> tokens;
public:
	Lexer();
	void tokenize(const std::string input);
	void printTokens();
};
