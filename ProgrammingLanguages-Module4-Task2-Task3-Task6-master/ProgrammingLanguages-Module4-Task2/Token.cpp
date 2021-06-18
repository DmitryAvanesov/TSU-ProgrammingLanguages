#include "Token.h"

Token::Token(Type newType, int newLine, int newCol,
	string newValue, vector<Token*> newInnerTokens) {
	tokenType = newType;
	tokenPosition.line = newLine;
	tokenPosition.col = newCol;
	tokenValue = newValue;
	tokenInnerTokens = newInnerTokens;
	nestingLevel = 0;
	expressionPriority = 0;
}
