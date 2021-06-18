#pragma once
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "Token.h"
using namespace std;

class Interpreter
{
private:
	map<string, int> intVariables = {};
	map<string, bool> boolVariables = {};
	map<string, string> stringVariables = {};
	map<string, vector<int>> arrayIntVariables = {};
	map<string, vector<string>> arrayStringVariables = {};

	ofstream outputFile;

public:
	Interpreter();
	void Launch(Token* programToken);
	void InterpretLine(Token* lineToken);
	bool CheckCondition(Token* conditionToken);
	bool CheckComparison(Token* comparisonToken);
	bool CompareIntInt(Token* comparisonToken);
	bool CompareStringInt(Token* comparisonToken);
	bool CompareIntString(Token* comparisonToken);
	bool CompareStringString(Token* comparisonToken);
	int ComputeExpressionInt(Token* expressionToken);
	string ComputeExpressionString(Token* expressionToken);
	Token* PrepareExpression(Token* expressionToken);
	Token* HandleHighestPriorityOperation(Token* expressionToken);
	void PrintVariableTable();
};

