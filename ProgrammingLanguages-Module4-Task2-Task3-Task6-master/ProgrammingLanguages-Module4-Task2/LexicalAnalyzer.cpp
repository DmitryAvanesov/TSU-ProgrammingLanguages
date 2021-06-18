#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer() {
	arrayElementRegex = regex("[a-zA-Z]([a-zA-Z]|\\d)*\\[(\\d+|[a-zA-Z]([a-zA-Z]|\\d)*)\\]");
	numberRegex = regex("-?\\d+");
	wordRegex = regex("[a-zA-Z]([a-zA-Z]|\\d)*");
	stringRegex = regex("'([a-zA-Z]|\\d|-)*'");

	buffer = "";
	line = 1;
	col = 1;
}

vector<Token*> LexicalAnalyzer::Launch() {
	inputFile.open("input.txt");
	outputFile.open("outputLexicalAnalyzer.txt");

	while (getline(inputFile, inputLine))
	{
		for (char symbol : inputLine) {
			if (symbol == ' ' || symbol == '\t') {
				RecognizeBuffer();
			}
			else {
				buffer.push_back(symbol);
			}

			if (symbol == '\t') {
				col += 4;
			}
			else {
				col++;
			}
		}

		RecognizeBuffer();
		buffer = "\n";
		col++;
		RecognizeBuffer();
		line++;
		col = 1;
	}

	for (Token* token : tokens) {
		outputFile << token->tokenPosition.line << ":"
			<< token->tokenPosition.col << " \t" <<
			typeNames[token->tokenType] << "  " <<
			token->tokenValue << "\n";
	}

	inputFile.close();
	outputFile.close();

	return tokens;
}

void LexicalAnalyzer::RecognizeBuffer() {
	if (keywords.count(buffer) == 1) {
		tokens.push_back(new Token(keywords[buffer], line, col - buffer.size()));
	}
	else if (operations.count(buffer) == 1) {
		tokens.push_back(new Token(operations[buffer], line, col - buffer.size()));
	}
	else if (symbols.count(buffer) == 1) {
		tokens.push_back(new Token(symbols[buffer], line, col - buffer.size()));
	}
	else if (regex_match(buffer, arrayElementRegex)) {
		tokens.push_back(new Token(Type::ArrayElement, line, col - buffer.size(), buffer));
	}
	else if (regex_match(buffer, numberRegex)) {
		tokens.push_back(new Token(Type::Number, line, col - buffer.size(), buffer));
	}
	else if (regex_match(buffer, wordRegex)) {
		tokens.push_back(new Token(Type::Word, line, col - buffer.size(), buffer));
	}
	else if (regex_match(buffer, stringRegex)) {
		tokens.push_back(new Token(Type::String, line, col - buffer.size(), buffer.substr(1, buffer.size() - 2)));
	}
	else if (!buffer.empty()) {
		outputFile << "Error at " << line << ":" << col - buffer.size();

		if (buffer.back() == ';') {
			outputFile << " Line must not end with semicolon\n";
		}
		else {
			outputFile << "\n";
		}
	}

	buffer.clear();
}