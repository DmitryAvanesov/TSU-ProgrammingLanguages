#pragma once
#include <string>
#include <vector>
#include "Type.h"
using namespace std;

class Token {
private:
	struct position {
		int line;
		int col;
	};

public:
	Type tokenType;
	position tokenPosition;
	string tokenValue;
	vector<Token*> tokenInnerTokens;
	int nestingLevel;
	int expressionPriority;

	Token(Type newType, int newLine = 0, int newCol = 0,
		string newValue = "", vector<Token*> newInnerTokens = {});
};

