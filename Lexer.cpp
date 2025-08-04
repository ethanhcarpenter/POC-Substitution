#pragma once
#include "Lexer.h"


Token::Token(TokenType tokentype, std::string val, std::string stype) :type(tokentype), value(val), stringType(stype) {}

TokenType Token::getType() { return type; }
std::string Token::getStringType() { return stringType; }
std::string Token::getValue() { return value; }

void Var::setName(std::string n) { name = n; }
std::string Var::getName() { return name; }

Lexer::Lexer() {};

void Lexer::tokenize(const std::string input,bool justTokens) {
	std::string lambdaBuffer = "LAMBDA";
	std::string churchBuffer = "CHURCH";
	std::string variableBuffer = "";
	std::string numberBuffer = "";
	for (char c : input) {
		if (std::isdigit(c) && variableBuffer.size() == 0) {
			numberBuffer += c;
		}
		else if (std::isalpha(c) || std::isdigit(c)) {
			variableBuffer += c;
			if (c == lambdaBuffer[0]) {
				lambdaBuffer = lambdaBuffer.erase(0, 1);
				if (lambdaBuffer == "") {
					tokens.push_back(Token(TokenType::Lambda, "LAMBDA", "Lambda"));
					lambdaBuffer = "LAMBDA";
					variableBuffer = "";
				}
			}
			else if (c == churchBuffer[0]) {
				churchBuffer = churchBuffer.erase(0, 1);
				if (churchBuffer == "") {
					tokens.push_back(Token(TokenType::Church, "CHURCH", "Church"));
					churchBuffer = "CHURCH";
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
			if (numberBuffer.size() > 0) {
				tokens.push_back(Token(TokenType::NumberLiteral, numberBuffer, "NumberLiteral"));
				numberBuffer = "";
			}
		}
		if (c == '(') { tokens.push_back(Token(TokenType::OpenParenthesis, std::string(1, c), "OpenParenthesis")); }
		else if (c == ')') { tokens.push_back(Token(TokenType::CloseParenthesis, std::string(1, c), "CloseParenthesis")); }
		else if (c == '[') { tokens.push_back(Token(TokenType::OpenBrackets, std::string(1, c), "OpenBrackets")); }
		else if (c == ']') { tokens.push_back(Token(TokenType::CloseBrackets, std::string(1, c), "CloseBrackets")); }
		else if (c == '{') { tokens.push_back(Token(TokenType::OpenBraces, std::string(1, c), "OpenBraces")); }
		else if (c == '}') { tokens.push_back(Token(TokenType::CloseBraces, std::string(1, c), "CloseBraces")); }
		else if (c == '.') { tokens.push_back(Token(TokenType::Dot, std::string(1, c), "Dot")); }
		else if (c == ',') { tokens.push_back(Token(TokenType::Comma, std::string(1, c), "Comma")); }

	}
	if (variableBuffer.size() > 0) {
		tokens.push_back(Token(TokenType::Variable, variableBuffer, "Variable"));
		variableBuffer = "";
	}
	if (numberBuffer.size() > 0) {
		tokens.push_back(Token(TokenType::NumberLiteral, numberBuffer, "NumberLiteral"));
		numberBuffer = "";
	}
	if (!justTokens) { lex(); }
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
void Abstraction::addToFinalExpression(std::string s) {
	finalExpression += s;
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
		if (browseFromCurrentToken(i, 1)->getType() == CloseBrackets) { return true; }
		return false;
	}
}
std::string Lexer::tokenStringValue(int tokenIndex) {
	std::string valueString = tokens[tokenIndex].getValue();
	return valueString;
}
void Lexer::setHighestAbstraction(Abstraction* newHighest) {
	tree = newHighest;
}
std::vector<std::string>* Lexer::getInputs() {
	return &inputs;
}
void Lexer::createNewInput() {
	inputs.push_back("");
}

void Lexer::addStringToLastInput(std::string s) {
	inputs.back() += " ";
	inputs.back() += s;
}


void Lexer::lex() {
	int tokenIndex = 0;
	bool finalExpressionflag = false;
	int parenthesisFound = 0;
	bool reserveFlage = true;
	for (Token t : tokens) {
		//I genuinley got pushied for about 3 minuts for writing brackets not parentheses here
		//if (nextToken->getType() == OpenBrackets) {
		if (t.getType() == OpenBrackets) {
			tree = new Abstraction;
			reserveFlage = false;
		}
		if (t.getType() == Lambda && tree != nullptr) {
			Abstraction* lowestAbstraction = getLowestAbstraction();
			Var* newVariable = new Var();

			newVariable->setName(tokenStringValue(tokenIndex + 1));
			lowestAbstraction->setBoundVariable(newVariable);
			lowestAbstraction->setNextExpression(new Abstraction);
		}
		if (t.getType() == Dot && !finalExpressionflag && tree != nullptr) {
			if (!isFutureLambda(tokenIndex)) {
				Abstraction* lowestAbstraction = getLowestAbstraction();
				int tempTokenIndex = tokenIndex;
				Token* nextToken = browseFromCurrentToken(tempTokenIndex, 1);
				while (nextToken->getType() != CloseBrackets) {
					tempTokenIndex++;
					lowestAbstraction->addToFinalExpression(tokenStringValue(tempTokenIndex));
					nextToken = browseFromCurrentToken(tempTokenIndex, 1);
				}
				finalExpressionflag = true;
			}
		}
		if (t.getType() == CloseBrackets) {
			int tempTokenIndex = tokenIndex;
			Token* nextToken = browseFromCurrentToken(tempTokenIndex, 1);
			
			while (nextToken->getType() != EndOfExpression) {
				createNewInput();
				while ((nextToken->getType() != Comma && nextToken->getType() != EndOfExpression) || parenthesisFound != 0) {
					tempTokenIndex++;
					if (nextToken->getType() == OpenParenthesis) {
						parenthesisFound += 1;
					}
					//else if (nextToken->getType() == CloseBrackets) {
					if (nextToken->getType() == CloseParenthesis) {
						
						for (char c : getPreLambdaReserve()) {
							if (c == '(') { parenthesisFound += 1; }
						}
						Token tempNextToken = *nextToken;
						tempNextToken = *browseFromCurrentToken(tempTokenIndex, parenthesisFound);
						
						if (browseFromCurrentToken(tempTokenIndex, 1)->getType() == EndOfExpression|| tempNextToken.getType() == EndOfExpression) {
							if (browseFromCurrentToken(tempTokenIndex, 1)->getType() == EndOfExpression) {
								std::string s = tokenStringValue(tempTokenIndex);
								addStringToLastInput(s);
							}
							tempTokenIndex += parenthesisFound;
							parenthesisFound = 0;
							break;
						}
						parenthesisFound  -=1;
					}
					if (nextToken->getType() == Lambda || nextToken->getType() == Variable) {
						addStringToLastInput(nextToken->getValue());
					}
					else {
						std::string s = tokenStringValue(tempTokenIndex);
						addStringToLastInput(s);
					}
					nextToken = browseFromCurrentToken(tempTokenIndex, 1);
				}
				tempTokenIndex++;
				nextToken = browseFromCurrentToken(tempTokenIndex, 1);
			}
			break;
		}
		if (reserveFlage) {
			std::string s = t.getValue();
			addStringToPreLambdaReserve(s);
		}
		tokenIndex++;
	}
	updateStringForm();
}
void Lexer::printTokens() {

	for (Token t : tokens) {
		std::cout << t.getStringType() << "\n";
	}
}
Lexer* Parser::getLexer() {
	return lexer;
}
void Lexer::replaceVariableNames(int depth) {
	updateStringForm(depth);
}
std::string Lexer::getStringForm() {
	return stringForm;
}
void Lexer::removeTopLevelBrackets() {
	updateStringForm();
	if (stringForm.size() > 0 && stringForm[0] == '[' && stringForm[stringForm.size() - 1] == ']') {
		stringForm = stringForm.substr(1, stringForm.size() - 2);
	}
}
void Lexer::overrideInput(std::string input, int depth) {
	inputs[depth] = input;
}
void Lexer::updateStringForm(int depth) {

	std::string newStringForm = "[";
	std::string finalExpression = "";
	std::vector<std::string>* boundVars = new std::vector<std::string>;
	Abstraction* current = tree;
	while (current != nullptr) {
		if (current->getFinalExpression() == "") {
			newStringForm += "LAMBDA ";
			boundVars->push_back(current->getBoundVariable()->getName());
			std::string var = current->getBoundVariable()->getName() + (depth != -1 ? std::to_string(depth + 1) : "");
			current->getBoundVariable()->setName(var);
			newStringForm += var;
			newStringForm += ". ";
		}
		else {
			finalExpression = current->getFinalExpression();
		}
		current = current->getNextExpression();
	}
	if (depth != -1) {
		for (std::string& boundVar : *boundVars) {
			finalExpression = Parser::betaReduce(finalExpression, boundVar, boundVar + std::to_string(depth + 1) + " ");
		}
	}
	newStringForm += finalExpression;
	getLowestAbstraction()->setFinalExpression(finalExpression);
	newStringForm += "]";
	//depth==-1 not really necessary but im paranoid and the logic is ambiguous in my mind
	if (inputs.size() > 0 && depth == -1) {
		for (const std::string& input : inputs) {
			newStringForm += input + ",";
		}
		newStringForm.pop_back();
	}
	for (size_t i = 0; i + 1 < newStringForm.size(); ) {
		if (newStringForm[i] == ']' && newStringForm[i + 1] == ',') {
			newStringForm.erase(i + 1, 1);
		}
		else { ++i; }
	}
	stringForm = newStringForm;
}
void Lexer::setPreLambdaReserve(std::string reserve) {
	preLambdaReserve = reserve;
}
void Lexer::addStringToPreLambdaReserve(std::string s) {
	preLambdaReserve += s;
	preLambdaReserve += " ";
}
std::string Lexer::getPreLambdaReserve() {
	return preLambdaReserve;
}
std::vector<Token> Lexer::getTokens() {
	return tokens;
}


void Parser::logEvents() {

}

std::tuple<std::string, Lexer> Parser::alphaReduce(std::string input, int inputRecurse, int depth) {
	std::string newInput = "";
	Lexer inputLexer;

	// Ensure correspondingVariableNames has 'depth + 1' vectors
	while (correspondingVariableNames.size() <= depth) {
		correspondingVariableNames.push_back(std::vector<std::map<std::string, std::string>>());
	}
	// Ensure the vector at 'depth' has 'inputRecurse + 1' maps
	while (correspondingVariableNames[depth].size() <= inputRecurse) {
		correspondingVariableNames[depth].push_back(std::map<std::string, std::string>());
	}

	if (input[1] == '{') {
		std::string replaced = input;
		for (char& ch : replaced) {
			if (ch == '{') ch = '[';
			else if (ch == '}') ch = ']';
		}
		size_t pos = 0;
		const std::string from = "CHURCH";
		const std::string to = "LAMBDA";
		while ((pos = replaced.find(from, pos)) != std::string::npos) {
			replaced.replace(pos, from.length(), to);
			pos += to.length();
		}
		inputLexer.tokenize(replaced);
		inputLexer.replaceVariableNames(depth);
		correspondingVariableNames[depth][inputRecurse].insert({ input, inputLexer.getStringForm() });
	}
	if (input.at(1) != '(' && input.back() != ')') { correspondingVariableNames[depth][inputRecurse].insert({ input, input + std::to_string(depth + 1) }); }
	else { correspondingVariableNames[depth][inputRecurse].insert({ input, input }); }
	for (const auto& kvp : correspondingVariableNames[depth][inputRecurse]) {
		newInput = kvp.second;
	}
	return { newInput, inputLexer };
}
std::string Parser::betaReduce(std::string expression, std::string var, std::string replace) {
	var.erase(std::remove_if(var.begin(), var.end(), ::isspace), var.end());
	int pos = expression.find(var);
	while (pos != std::string::npos) {
		expression.replace(pos, var.size(), replace);
		pos = expression.find(var, pos + replace.size());
	}
	return expression;
}


void Parser::evaluate(int inputRecurse, std::string expression) {
	int currentInputRecurse = inputRecurse;
	int currentDepth = 0;
	bool fullyEvaluated = false;
	std::string betaValue = "";
	std::string currentExpression = expression;
	lexer->tokenize(currentExpression);
	while (!fullyEvaluated) {
		Abstraction* expression = lexer->getHighestAbstraction();
		std::vector<std::string>* inputs = lexer->getInputs();
		Var* var = expression->getBoundVariable();
		std::string input = inputs->at(0);
		std::string newInput;
		Lexer inputLexer;
		std::tie(newInput, inputLexer) = alphaReduce(input, currentInputRecurse, currentDepth);
		lexer->overrideInput(newInput, 0);
		lexer->updateStringForm();
		betaValue = lexer->getLowestAbstraction()->getFinalExpression();
		input = inputs->at(0);
		betaValue = betaReduce(betaValue, var->getName(), input);
		lexer->getLowestAbstraction()->setFinalExpression(betaValue);

		lexer->setHighestAbstraction(lexer->getHighestAbstraction()->getNextExpression());
		inputs->erase(inputs->begin());
		if (expression->getNextExpression()->getFinalExpression() == "") { currentInputRecurse += 1; }
		else if (expression->getNextExpression()->getFinalExpression().find("LAMBDA") != std::string::npos) {
			currentDepth += 1;
			currentInputRecurse = 0;
			lexer->removeTopLevelBrackets();
			std::cout << "Was: " << lexer->getStringForm() << std::endl;
			Lexer* newLexer = new Lexer();
			newLexer->tokenize(lexer->getStringForm());
			currentExpression = newLexer->getStringForm();
			std::cout << "Is: " << currentExpression << std::endl;
			delete lexer;         
			lexer = newLexer;       
		}
		else {
			lexer->removeTopLevelBrackets();
			std::string finalExpr = lexer->getStringForm();
			std::string preLR = lexer->getPreLambdaReserve();

			finalExpr = preLR + finalExpr;

			for (int i = static_cast<int>(correspondingVariableNames.size()) - 1; i >= 0; i--) {
				for (int j = 0; j < static_cast<int>(correspondingVariableNames[i].size()); j++) {
					for (const auto& kvp : correspondingVariableNames[i][j]) {
						finalExpr = betaReduce(finalExpr, kvp.second, kvp.first);
						int a = 12;
					}
				}
			}
			finalExpression = finalExpr;
			cleanupFinalExpression();
			fullyEvaluated = true;
		}
	}
}
std::string Parser::getFinalExpression() {
	return finalExpression;
}
void Parser::cleanupFinalExpression() {
	Lexer* l = new Lexer;
	l->tokenize(finalExpression, true);  // true = skip lexing pass, just tokenise

	std::string rebuilt;
	std::vector<Token> tokens = l->getTokens();  // add a getTokens() method if not available

	for (size_t i = 0; i < tokens.size(); ++i) {
		Token& t = tokens[i];

		// Add space between identifiers and keywords
		if (t.getType() == TokenType::Variable ||
			t.getType() == TokenType::Church ||
			t.getType() == TokenType::Lambda ||
			t.getType() == TokenType::NumberLiteral) {
			rebuilt += t.getValue() + " ";
		}
		// No space before or after punctuation tokens
		else if (t.getType() == TokenType::Dot ||
			t.getType() == TokenType::Comma ||
			t.getType() == TokenType::OpenParenthesis ||
			t.getType() == TokenType::CloseParenthesis ||
			t.getType() == TokenType::OpenBrackets ||
			t.getType() == TokenType::CloseBrackets) {
			rebuilt += t.getValue();
		}
	}

	// Remove trailing space if any
	if (!rebuilt.empty() && rebuilt.back() == ' ') {
		rebuilt.pop_back();
	}

	// Ensure the prefix exists
	const std::string prefix = "CHURCH f. CHURCH x.";
	if (rebuilt.substr(0, prefix.length()) != prefix) {
		finalExpression = prefix + " " + rebuilt;
	}
	else {
		finalExpression = rebuilt;
	}

	delete l;
}

