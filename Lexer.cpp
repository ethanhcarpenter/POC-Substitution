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
	std::string numberBuffer = "";
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
		else if (std::isdigit(c)) {
			numberBuffer += c;
		}
		else {
			if (variableBuffer.size() > 0) {
				tokens.push_back(Token(TokenType::Variable, variableBuffer, "Variable"));
				variableBuffer = "";
			}
			if (numberBuffer.size() > 0) {
				tokens.push_back(Token(TokenType::NumberLiteral, numberBuffer, "NumberLiteral"));
				numberBuffer = "";
			}
		}
		if (c == '(') { tokens.push_back(Token(TokenType::OpenParenthesis, std::to_string(c), "OpenParenthesis")); }
		else if (c == ')') { tokens.push_back(Token(TokenType::CloseParenthesis, std::to_string(c), "CloseParenthesis")); }
		else if (c == '[') { tokens.push_back(Token(TokenType::OpenBrackets, std::to_string(c), "OpenBrackets")); }
		else if (c == ']') { tokens.push_back(Token(TokenType::CloseBrackets, std::to_string(c), "CloseBrackets")); }
		else if (c == '{') { tokens.push_back(Token(TokenType::OpenBraces, std::to_string(c), "OpenBraces")); }
		else if (c == '}') { tokens.push_back(Token(TokenType::CloseBraces, std::to_string(c), "CloseBraces")); }
		else if (c == '.') { tokens.push_back(Token(TokenType::Dot, std::to_string(c), "Dot")); }
		else if (c == ',') { tokens.push_back(Token(TokenType::Comma, std::to_string(c), "Comma")); }

	}
	if (variableBuffer.size() > 0) {
		tokens.push_back(Token(TokenType::Variable, variableBuffer, "Variable"));
		variableBuffer = "";
	}
	if (numberBuffer.size() > 0) {
		tokens.push_back(Token(TokenType::NumberLiteral, numberBuffer, "NumberLiteral"));
		numberBuffer = "";
	}
	lex();
}
Abstraction* Abstraction::getNextExpression() {
	return nextExpression;
}
void Abstraction::setNextExpression(Abstraction* next) {
	nextExpression = next;
}
bool Abstraction::hasNextExpression() {
	return nextExpression != nullptr;
}
void Abstraction::setBoundVariable(Var* v) {
	boundVariable = v;
}
void Abstraction::setFinalExpression(std::string expression) {
	finalExpression = expression;
}
std::string Abstraction::getFinalExpression() {
	return finalExpression;
}
Var* Abstraction::getBoundVariable() {
	return boundVariable;
}
Abstraction* Lexer::getLowestAbstraction() {
	Abstraction* lowestAbstraction = tree;
	while (lowestAbstraction->hasNextExpression()) {
		lowestAbstraction = lowestAbstraction->getNextExpression();
	}
	return lowestAbstraction;
}
Abstraction* Lexer::getHighestAbstraction() { return tree; }
void Abstraction::addToFinalExpression(char c) {
	finalExpression += c;
}
Token* Lexer::browseFromCurrentToken(int tokenIndex, int forwards) {
	if (tokenIndex + forwards >= tokens.size()) {
		return new Token(TokenType::EndOfExpression, "", "EndOfExpression");
	}
	return &tokens[tokenIndex + forwards];

}
bool Lexer::isFutureLambda(int tokenIndex) {
	for (int i = tokenIndex; i < tokens.size(); i++) {
		if (browseFromCurrentToken(i, 1)->getType() == Lambda) { return true; }
		return false;
	}
}
char Lexer::tokenCharValue(int tokenIndex) {
	std::string valueStr = tokens[tokenIndex].getValue();
	char valueChar = static_cast<char>(std::stoi(valueStr));
	return valueChar;
}
std::vector<std::string>* Lexer::getInputs() {
	return &inputs;
}
void Lexer::createNewInput() {
	inputs.push_back("");
}
void Lexer::addCharToLastInput(char c) {
	inputs.back() += " ";
	inputs.back() += c;
}
void Lexer::addStringToLastInput(std::string s) {
	inputs.back() += " ";
	inputs.back() += s;
}


void Lexer::lex() {
	int tokenIndex = 0;
	for (Token t : tokens) {
		if (t.getType() == OpenBrackets) { tree = new Abstraction; }
		if (t.getType() == Lambda) {
			Abstraction* lowestAbstraction = getLowestAbstraction();
			Var* newVariable = new Var();

			newVariable->setName(tokenCharValue(tokenIndex + 1));
			lowestAbstraction->setBoundVariable(newVariable);
			lowestAbstraction->setNextExpression(new Abstraction);
		}
		if (t.getType() == Dot) {
			if (!isFutureLambda(tokenIndex)) {
				Abstraction* lowestAbstraction = getLowestAbstraction();
				int tempTokenIndex = tokenIndex;
				Token* nextToken = browseFromCurrentToken(tempTokenIndex, 1);
				while (nextToken->getType() != CloseBrackets) {
					tempTokenIndex++;
					lowestAbstraction->addToFinalExpression(tokenCharValue(tempTokenIndex));
					nextToken = browseFromCurrentToken(tempTokenIndex, 1);
				}
			}
		}
		if (t.getType() == CloseBrackets) {
			int tempTokenIndex = tokenIndex;
			Token* nextToken = browseFromCurrentToken(tempTokenIndex, 1);
			while (nextToken->getType() != EndOfExpression) {
				createNewInput();
				while (nextToken->getType() != Comma && nextToken->getType() != EndOfExpression) {
					tempTokenIndex++;
					if (nextToken->getType() == Lambda||nextToken->getType()==Variable) {
						addStringToLastInput(nextToken->getValue());
					}
					else {
						char c = tokenCharValue(tempTokenIndex);
						addCharToLastInput(c);
					}
					nextToken = browseFromCurrentToken(tempTokenIndex, 1);
				}
				tempTokenIndex++;
				nextToken = browseFromCurrentToken(tempTokenIndex, 1);
			}
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

Lexer* Parser::getLexer() {
	return lexer;
}
void Parser::logEvents() {

}
std::string Parser::substitute(std::string expression, std::string var, std::string replace) {
	int pos = expression.find(var);
	while (pos != std::string::npos) {
		expression.replace(pos, var.size(), replace);
		pos = expression.find(var, pos + replace.size());
	}
	return expression;
}

void Parser::evaluate(int depth) {
	Abstraction* expression = lexer->getHighestAbstraction();
	std::vector<std::string>* inputs = lexer->getInputs();
	if (expression->getFinalExpression() == "") {
		for (int i = 0; i < depth; i++) { expression = expression->getNextExpression(); }
	}
	Var* var = expression->getBoundVariable();
	std::string input = inputs->at(depth);
	evaluatedValue = lexer->getLowestAbstraction()->getFinalExpression();
	evaluatedValue = substitute(evaluatedValue, std::string(1, var->getName()), input);
	lexer->getLowestAbstraction()->setFinalExpression(evaluatedValue);

	if (expression->getNextExpression()->getFinalExpression() == "") {
		evaluate(depth + 1);
	}
}