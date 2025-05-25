#pragma once
#include "Lexer.h"


Token::Token(TokenType tokentype, std::string val,std::string stype) :type(tokentype), value(val),stringType(stype) {}

TokenType Token::getType() { return type; }
std::string Token::getStringType() { return stringType; }
std::string Token::getValue() { return value; }

Lexer::Lexer() {};

void Lexer::tokenize(const std::string input) {
	std::string lambdaBuffer = "LAMBDA";
	std::string variableBuffer = "";
	for (char c : input) {
		if (std::isalpha(c)) {
			variableBuffer += std::to_string(c);
			if (c == lambdaBuffer[0]) {
				lambdaBuffer = lambdaBuffer.erase(0, 1);
				if (lambdaBuffer == "") {
					tokens.push_back(Token(TokenType::Lambda, "LAMBDA","Lambda"));
					lambdaBuffer = "LAMBDA";
					variableBuffer = "";
				}
			}
			else{ lambdaBuffer = "LAMBDA"; }
			
		}
		else {
			if(variableBuffer.size()>0){ 
				tokens.push_back(Token(TokenType::Variable, variableBuffer,"Variable"));
				variableBuffer = "";
			}
		}
		if (c == '(') { tokens.push_back(Token(TokenType::OpenParenthesis, std::to_string(c), "OpenParenthesis")); }
		else if (c == ')') { tokens.push_back(Token(TokenType::CloseParenthesis, std::to_string(c), "CloseParenthesis")); }
		else if (c == '.') { tokens.push_back(Token(TokenType::Dot, std::to_string(c), "Dot")); }

	}
}

void Lexer::printTokens() {
	for (Token t : tokens) {
		std::cout << t.getStringType() << "\n";
	}
}