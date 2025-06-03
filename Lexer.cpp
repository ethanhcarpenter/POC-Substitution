#pragma once
#include "Lexer.h"


Token::Token(TokenType tokentype, std::string val, std::string stype) :type(tokentype), value(val), stringType(stype) {}

TokenType Token::getType() { return type; }
std::string Token::getStringType() { return stringType; }
std::string Token::getValue() { return value; }

void Var::setName(char n) { name = n; }
char Var::getName() { return name[0]; }

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
					tokens.push_back(Token(TokenType::Lambda, "LAMBDA", "Lambda"));
					lambdaBuffer = "LAMBDA";
					variableBuffer = "";
				}
			}
			else { lambdaBuffer = "LAMBDA"; }

		}
		else {
			if (variableBuffer.size() > 0) {
				tokens.push_back(Token(TokenType::Variable, variableBuffer, "Variable"));
				variableBuffer = "";
			}
		}
		if (c == '(') { tokens.push_back(Token(TokenType::OpenParenthesis, std::to_string(c), "OpenParenthesis")); }
		else if (c == ')') { tokens.push_back(Token(TokenType::CloseParenthesis, std::to_string(c), "CloseParenthesis")); }
		else if (c == '.') { tokens.push_back(Token(TokenType::Dot, std::to_string(c), "Dot")); }

	}
}
Abs* Abs::getNextExpression() {
	return nextExpression;
}
void Abs::setNextExpression(Abs* next) {
	nextExpression = next;
}
bool Abs::hasNextExpression() {
	return nextExpression != nullptr;
}
void Abs::setBoundVariable(Var* v) {
	boundVariable = v;
}
Abs* Lexer::getLowestAbstraction() {
	Abs* lowestAbstraction = tree;
	while (lowestAbstraction->hasNextExpression()) {
		lowestAbstraction = lowestAbstraction->getNextExpression();
	}
	return lowestAbstraction;
}

void Lexer::lex() {
	int tokenIndex = 0;
	for (Token t : tokens) {
		if (t.getType() == Lambda) {
			int tempTokenIndex = tokenIndex + 1;
			Abs* lowestAbstraction = getLowestAbstraction();
			Var* newVariable = new Var();
			std::string valueStr = tokens[tempTokenIndex].getValue();
			char valueChar = static_cast<char>(std::stoi(valueStr));
			newVariable->setName(valueChar);
			lowestAbstraction->setBoundVariable(newVariable);
			lowestAbstraction->setNextExpression(new Abs);
		}
		tokenIndex++;
	}
	int a = 12;
}



void Lexer::printTokens() {

	for (Token t : tokens) {
		std::cout << t.getStringType() << "\n";
	}
}