#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer() {
	tmpToken = NULL;
	tokenRecognized = true;
	currentPosition = 0;
}

vector<Token*> SyntaxAnalyzer::Launch(vector<Token*> tokens) {
	outputFile.open("outputSyntaxAnalyzer.txt");

	tokens = StructurizeArrays(tokens);
	tokens = StructurizeTokensWrapper(tokens, nonTerminals[0]);
	tokens = StructurizeExpressions(tokens);
	tokens = StructurizeTokensWrapper(tokens, nonTerminals[1]);
	tokens = StructurizeConditions(tokens);
	tokens = StructurizeTokensWrapper(tokens, nonTerminals[2]);
	tokens = RemoveNewLines(tokens);
	tokens = StructurizeIfWhileBlocks(tokens);
	tokens = StructurizeTokensWrapper(tokens, nonTerminals[3]);
	tokens = StructurizeProgram(tokens);

	if (tokens.size() > 1 || tokens[0]->tokenType != Type::Program) {
		outputFile << "Error Wrong program structure";
	}
	else {
		for (Token* token : tokens) {
			PrintToken(token);
		}
	}

	outputFile.close();
	return tokens;
}

vector<Token*> SyntaxAnalyzer::StructurizeTokensWrapper(vector<Token*> tokens,
	map<vector<Type>, Type> nonTerminalsLevel)
{
	tokenRecognized = true;

	for (Token* token : tokens) {
		types.push_back(token->tokenType);
	}

	if (find(types.begin(), types.end(), Type::Line) != types.end()) {
		tokens = RemoveNewLines(tokens);
	}

	while (tokenRecognized) {
		tokenRecognized = false;
		currentPosition = 0;
		tokens = StructurizeTokens(tokens, nonTerminalsLevel);
	}

	return tokens;
}

vector<Token*> SyntaxAnalyzer::StructurizeTokens(vector<Token*> tokens,
	map<vector<Type>, Type> nonTerminalsLevel)
{
	while (currentPosition < types.size()) {
		for (auto nonTerminal : nonTerminalsLevel) {
			vector<Type> nonTerminalsKey = nonTerminal.first;
			Type nonTerminalsValue = nonTerminal.second;

			if (currentPosition + nonTerminalsKey.size() <= types.size() &&
				nonTerminalsKey == vector<Type>(types.begin() + currentPosition,
					types.begin() + currentPosition + nonTerminalsKey.size())) {

				tmpToken = new Token(
					nonTerminalsValue,
					tokens[currentPosition]->tokenPosition.line,
					tokens[currentPosition]->tokenPosition.col,
					"",
					vector<Token*>(tokens.begin() + currentPosition,
						tokens.begin() + currentPosition + nonTerminalsKey.size())
				);

				for (Token* token : tmpToken->tokenInnerTokens) {
					SetNestingLevel(token);
				}

				tokens.erase(tokens.begin() + currentPosition,
					tokens.begin() + currentPosition + nonTerminalsKey.size());
				tokens.insert(tokens.begin() + currentPosition,
					tmpToken);

				tokenRecognized = true;
				break;
			}
		}

		currentPosition++;
		types.clear();

		for (Token* token : tokens) {
			types.push_back(token->tokenType);
		}
	}

	return tokens;
}

vector<Token*> SyntaxAnalyzer::StructurizeArrays(vector<Token*> tokens) {
	tokenRecognized = false;
	currentPosition = 0;

	while (currentPosition < tokens.size()) {
		if ((tokens[currentPosition]->tokenType == Type::OpeningSquareBracketSymbol &&
			buffer.empty())
			||
			(!buffer.empty() &&
			((tokens[currentPosition]->tokenType == Type::Number &&
				(buffer.back()->tokenType == Type::OpeningSquareBracketSymbol ||
					buffer.back()->tokenType == Type::Number))
				||
				(tokens[currentPosition]->tokenType == Type::String &&
					(buffer.back()->tokenType == Type::OpeningSquareBracketSymbol ||
						buffer.back()->tokenType == Type::String))
				||
				(tokens[currentPosition]->tokenType == Type::ClosingSquareBracketSymbol &&
					(buffer.back()->tokenType == Type::Number ||
						buffer.back()->tokenType == Type::String)))))
		{
			buffer.push_back(tokens[currentPosition]);
		}
		else {
			tokens = CheckArray(tokens);
		}

		currentPosition++;
	}

	tokens = CheckArray(tokens);

	types.clear();

	for (Token* token : tokens) {
		types.push_back(token->tokenType);
	}

	return tokens;
}

vector<Token*> SyntaxAnalyzer::CheckArray(vector<Token*> tokens) {
	if (!buffer.empty()) {
		if (buffer.back()->tokenType == Type::ClosingSquareBracketSymbol)
		{
			Token* tmpToken = new Token(
				Type::Array,
				buffer.front()->tokenPosition.line,
				buffer.front()->tokenPosition.col,
				"",
				vector<Token*>(tokens.begin() + currentPosition - buffer.size(),
					tokens.begin() + currentPosition)
			);

			for (Token* token : tmpToken->tokenInnerTokens) {
				SetNestingLevel(token);
			}

			tokens.insert(tokens.begin() + currentPosition - buffer.size(),
				tmpToken);
			tokens.erase(tokens.begin() + currentPosition + 1 - buffer.size(),
				tokens.begin() + currentPosition + 1);

			currentPosition -= buffer.size();
		}
		else {
			outputFile << "Error at " << tokens[currentPosition]->tokenPosition.line <<
				":" << tokens[currentPosition]->tokenPosition.col <<
				" Incorrect array delcaration\n";
		}

		buffer.clear();
	}

	return tokens;
}

vector<Token*> SyntaxAnalyzer::StructurizeExpressions(vector<Token*> tokens) {
	tokenRecognized = false;
	currentPosition = 0;

	while (currentPosition < tokens.size()) {
		if (((tokens[currentPosition]->tokenType == Type::Name ||
				tokens[currentPosition]->tokenType == Type::TrueFalseKeyword) &&
					(buffer.empty() || buffer.back()->tokenType == Type::ExpressionOperation ||
						buffer.back()->tokenType == Type::OpeningParenthesisSymbol))
			||
			(tokens[currentPosition]->tokenType == Type::ExpressionOperation &&
				!buffer.empty() && (buffer.back()->tokenType == Type::Name ||
					buffer.back()->tokenType == Type::TrueFalseKeyword ||
						buffer.back()->tokenType == Type::ClosingParenthesisSymbol))
			||
			(tokens[currentPosition]->tokenType == Type::OpeningParenthesisSymbol &&
				(buffer.empty() || buffer.back()->tokenType == Type::ExpressionOperation ||
					buffer.back()->tokenType == Type::OpeningParenthesisSymbol))
			||
			(tokens[currentPosition]->tokenType == Type::ClosingParenthesisSymbol &&
				!buffer.empty() &&
					(buffer.back()->tokenType == Type::Name ||
						buffer.back()->tokenType == Type::TrueFalseKeyword ||
							buffer.back()->tokenType == Type::ClosingParenthesisSymbol)))
		{
			buffer.push_back(tokens[currentPosition]);
		}
		else {
			tokens = CheckExpression(tokens);
		}

		currentPosition++;
	}

	tokens = CheckExpression(tokens);

	types.clear();

	for (Token* token : tokens) {
		types.push_back(token->tokenType);
	}

	return tokens;
}

vector<Token*> SyntaxAnalyzer::CheckExpression(vector<Token*> tokens) {
	if (!buffer.empty()) {
		if (buffer.back()->tokenType == Type::Name ||
			buffer.back()->tokenType == Type::TrueFalseKeyword ||
			buffer.back()->tokenType == Type::ClosingParenthesisSymbol)
		{
			if (BracketSequenceIsCorrect()) {
				Token* tmpToken = new Token(
					Type::Expression,
					buffer.front()->tokenPosition.line,
					buffer.front()->tokenPosition.col,
					"",
					vector<Token*>(tokens.begin() + currentPosition - buffer.size(),
						tokens.begin() + currentPosition)
				);

				for (Token* token : tmpToken->tokenInnerTokens) {
					SetNestingLevel(token);
				}

				tokens.insert(tokens.begin() + currentPosition - buffer.size(),
					tmpToken);
				tokens.erase(tokens.begin() + currentPosition + 1 - buffer.size(),
					tokens.begin() + currentPosition + 1);

				currentPosition -= buffer.size();
			}
			else {
				outputFile << "Error at " << tokens[currentPosition]->tokenPosition.line <<
					":" << tokens[currentPosition]->tokenPosition.col <<
					" Incorrect parenthesis sequence\n";
			}
		}
		else {
			outputFile << "Error at " << tokens[currentPosition]->tokenPosition.line <<
				":" << tokens[currentPosition]->tokenPosition.col <<
				" Expression must not end with an operator or opening parethesis\n";
		}

		buffer.clear();
	}
	
	return tokens;
}

bool SyntaxAnalyzer::BracketSequenceIsCorrect() {
	int counter = 0;
	
	for (int i = 0; i < buffer.size(); i++) {
		if (buffer[i]->tokenType == Type::OpeningParenthesisSymbol) {
			counter++;
		}
		else if (buffer[i]->tokenType == Type::ClosingParenthesisSymbol) {
			counter--;
		}

		if (counter < 0) {
			return false;
		}
	}

	return counter == 0;
}

vector<Token*> SyntaxAnalyzer::StructurizeConditions(vector<Token*> tokens) {
	tokenRecognized = false;
	currentPosition = 0;
	
	while (currentPosition < tokens.size()) {
		if ((tokens[currentPosition]->tokenType == Type::Comparison &&
			(buffer.empty() || buffer.back()->tokenType == Type::AndOrKeyword))
			||
			(tokens[currentPosition]->tokenType == Type::AndOrKeyword &&
				!buffer.empty() && buffer.back()->tokenType == Type::Comparison) ||
				(tokens[currentPosition]->tokenType == Type::Expression &&
					tokens[currentPosition]->tokenInnerTokens.size() == 1 &&
					tokens[currentPosition]->tokenInnerTokens.front()->tokenType == Type::TrueFalseKeyword))
		{
			buffer.push_back(tokens[currentPosition]);
		}
		else {
			tokens = CheckCondition(tokens);
		}

		currentPosition++;
	}

	tokens = CheckCondition(tokens);

	types.clear();

	for (Token* token : tokens) {
		types.push_back(token->tokenType);
	}

	return tokens;
}

vector<Token*> SyntaxAnalyzer::CheckCondition(vector<Token*> tokens) {
	if (!buffer.empty()) {
		if (buffer.back()->tokenType == Type::Comparison ||
			buffer.back()->tokenType == Type::Expression)
		{
			Token* tmpToken = new Token(
				Type::Condition,
				buffer.front()->tokenPosition.line,
				buffer.front()->tokenPosition.col,
				"",
				vector<Token*>(tokens.begin() + currentPosition - buffer.size(),
					tokens.begin() + currentPosition)
			);

			for (Token* token : tmpToken->tokenInnerTokens) {
				SetNestingLevel(token);
			}

			tokens.insert(tokens.begin() + currentPosition - buffer.size(),
				tmpToken);
			tokens.erase(tokens.begin() + currentPosition + 1 - buffer.size(),
				tokens.begin() + currentPosition + 1);

			currentPosition -= buffer.size();
		}
		else {
			outputFile << "Error at " << tokens[currentPosition]->tokenPosition.line <<
				":" << tokens[currentPosition]->tokenPosition.col <<
				" Condition must not end with an operator\n";
		}

		buffer.clear();
	}

	return tokens;
}

vector<Token*> SyntaxAnalyzer::StructurizeIfWhileBlocks(vector<Token*> tokens) {
	tokenRecognized = false;
	currentPosition = 0;

	while (currentPosition < tokens.size()) {
		if (((tokens[currentPosition]->tokenType == Type::IfKeyword ||
			tokens[currentPosition]->tokenType == Type::WhileKeyword) &&
			buffer.empty())
			||
			(tokens[currentPosition]->tokenType == Type::Condition &&
				!buffer.empty() &&
				(buffer.back()->tokenType == Type::IfKeyword ||
					buffer.back()->tokenType == Type::WhileKeyword))
			||
			(tokens[currentPosition]->tokenType == Type::OpeningBraceSymbol &&
				!buffer.empty() && buffer.back()->tokenType == Type::Condition)
			||
			((tokens[currentPosition]->tokenType == Type::Line ||
				tokens[currentPosition]->tokenType == Type::ClosingBraceSymbol)
				&&
				!buffer.empty() && (buffer.back()->tokenType == Type::OpeningBraceSymbol ||
					buffer.back()->tokenType == Type::Line)))
		{
			buffer.push_back(tokens[currentPosition]);
		}
		else {
			tokens = CheckIfWhileBlock(tokens);
		}

		currentPosition++;
	}

	tokens = CheckIfWhileBlock(tokens);

	types.clear();

	for (Token* token : tokens) {
		types.push_back(token->tokenType);
	}

	return tokens;
}

vector<Token*> SyntaxAnalyzer::CheckIfWhileBlock(vector<Token*> tokens) {
	if (!buffer.empty()) {
		types.clear();

		for (Token* token : buffer) {
			types.push_back(token->tokenType);
		}

		if ((buffer[0]->tokenType == Type::IfKeyword ||
			buffer[0]->tokenType == Type::WhileKeyword) &&
			buffer[1]->tokenType == Type::Condition &&
			buffer[2]->tokenType == Type::OpeningBraceSymbol &&
			buffer[buffer.size() - 1]->tokenType == Type::ClosingBraceSymbol &&
			count(types.begin(), types.end(), Type::Line) == buffer.size() - 4)
		{
			Type tmpType;

			if (buffer.front()->tokenType == Type::IfKeyword) {
				tmpType = Type::IfBlock;
			}
			else {
				tmpType = Type::WhileBlock;
			}

			Token* tmpToken = new Token(
				tmpType,
				buffer.front()->tokenPosition.line,
				buffer.front()->tokenPosition.col,
				"",
				vector<Token*>(tokens.begin() + currentPosition - buffer.size(),
					tokens.begin() + currentPosition)
			);

			for (Token* token : tmpToken->tokenInnerTokens) {
				SetNestingLevel(token);
			}

			tokens.insert(tokens.begin() + currentPosition - buffer.size(),
				tmpToken);
			tokens.erase(tokens.begin() + currentPosition + 1 - buffer.size(),
				tokens.begin() + currentPosition + 1);

			currentPosition -= buffer.size();
		}
		else {
			outputFile << "Error at " << tokens[currentPosition]->tokenPosition.line <<
				":" << tokens[currentPosition]->tokenPosition.col <<
				" Incorrect " << typeNames[buffer.front()->tokenType] <<
				" block declaration\n";
		}

		buffer.clear();
	}

	return tokens;
}

vector<Token*> SyntaxAnalyzer::StructurizeProgram(vector<Token*> tokens) {
	tokenRecognized = false;
	currentPosition = 0;

	while (currentPosition < tokens.size()) {
		if (tokens[currentPosition]->tokenType == Type::Element)
		{
			buffer.push_back(tokens[currentPosition]);
		}

		currentPosition++;
	}

	tokens = CheckProgram(tokens);

	types.clear();

	for (Token* token : tokens) {
		types.push_back(token->tokenType);
	}

	return tokens;
}

vector<Token*> SyntaxAnalyzer::CheckProgram(vector<Token*> tokens) {
	if (!buffer.empty()) {
		types.clear();

		for (Token* token : buffer) {
			types.push_back(token->tokenType);
		}

		if (count(types.begin(), types.end(), Type::Element) == buffer.size()) {
			Token* tmpToken = new Token(
				Type::Program,
				buffer.front()->tokenPosition.line,
				buffer.front()->tokenPosition.col,
				"",
				vector<Token*>(tokens.begin() + currentPosition - buffer.size(),
					tokens.begin() + currentPosition)
			);

			for (Token* token : tmpToken->tokenInnerTokens) {
				SetNestingLevel(token);
			}

			tokens.insert(tokens.begin() + currentPosition - buffer.size(),
				tmpToken);
			tokens.erase(tokens.begin() + currentPosition + 1 - buffer.size(),
				tokens.begin() + currentPosition + 1);
		}
		else {
			outputFile << "Error at " << tokens[currentPosition]->tokenPosition.line <<
				":" << tokens[currentPosition]->tokenPosition.col <<
				" Incorrect program structure\n";
		}

		buffer.clear();
	}

	return tokens;
}

vector<Token*> SyntaxAnalyzer::RemoveNewLines(vector<Token*> tokens) {
	int pos = 0;

	while (pos < tokens.size()) {
		if (tokens[pos]->tokenType == Type::NewLineSymbol) {
			tokens.erase(tokens.begin() + pos);
		}
		else {
			pos++;
		}
	}

	return tokens;
}

void SyntaxAnalyzer::SetNestingLevel(Token* token) {
	token->nestingLevel++;

	for (Token* innerToken : token->tokenInnerTokens) {
		SetNestingLevel(innerToken);
	}
}

void SyntaxAnalyzer::PrintToken(Token* token) {
	for (int i = 0; i < token->nestingLevel; i++) {
		outputFile << "\t";
	}

	outputFile << token->tokenPosition.line << ":"
		<< token->tokenPosition.col << " \t" <<
		typeNames[token->tokenType] << "  " <<
		token->tokenValue << "\n";

	for (Token* innerToken : token->tokenInnerTokens) {
		PrintToken(innerToken);
	}
}