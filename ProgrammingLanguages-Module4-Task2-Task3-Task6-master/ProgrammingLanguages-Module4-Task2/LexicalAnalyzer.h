#pragma once
#include <string>
#include <map>
#include <vector>
#include <regex>
#include <iostream>
#include <fstream>
#include "Token.h"
#include "Type.h"
using namespace std;

class LexicalAnalyzer {
private:
	map<Type, string> typeNames = {
		{Type::IntegerKeyword, "Integer keyword"},
		{Type::BooleanKeyword, "Boolean keyword"},
		{Type::StringKeyword, "String keyword"},
		{Type::ArrayKeyword, "Array keyword"},
		{Type::IfKeyword, "If keyword"},
		{Type::WhileKeyword, "While keyword"},
		{Type::AndKeyword, "And keyword"},
		{Type::OrKeyword, "Or keyword"},
		{Type::TrueKeyword, "True keyword"},
		{Type::FalseKeyword, "False keyword"},
		{Type::AndOrKeyword, "And/or keyword"},
		{Type::TrueFalseKeyword, "True/false keyword"},

		{Type::AssignmentOperation, "Assignment operation"},
		{Type::EqualityOperation, "Equality operation"},
		{Type::InequalityOperation, "Inequality operation"},
		{Type::MoreOperation, "More operation"},
		{Type::LessOperation, "Less operation"},
		{Type::MoreOrEqualOperation, "More or equal operation"},
		{Type::LessOrEqualOperation, "Less or equal operation"},
		{Type::AdditionOperation, "Addition operation"},
		{Type::SubtractionOperation, "Subtraction operation"},
		{Type::MultiplicationOperation, "Multiplication operation"},
		{Type::DivisionOperation, "Division operation"},
		{Type::ExpressionOperation, "Expression operation"},
		{Type::ComparisonOperation, "Comparison operation"},

		{Type::NewLineSymbol, "New line symbol"},
		{Type::OpeningBraceSymbol, "Opening brace symbol"},
		{Type::ClosingBraceSymbol, "Closing brace symbol"},
		{Type::OpeningParenthesisSymbol, "Opening parenthesis symbol"},
		{Type::ClosingParenthesisSymbol, "Closing parenthesis symbol"},
		{Type::OpeningSquareBracketSymbol, "Opening square bracket symbol"},
		{Type::ClosingSquareBracketSymbol, "Closing square bracket symbol"},

		{Type::Program, "Program"},
		{Type::Element, "Element"},
		{Type::IfBlock, "If block"},
		{Type::WhileBlock, "While block"},
		{Type::Line, "Line"},
		{Type::Condition, "Condition"},
		{Type::Assign, "Assign"},
		{Type::Expression, "Expression"},
		{Type::Comparison, "Comparison"},
		{Type::Name, "Name"},
		{Type::VariableDeclaration, "Variable declaration"},
		{Type::Number, "Number"},
		{Type::Word, "Word"},
		{Type::Array, "Array"},
		{Type::ArrayElement, "Array element"},
		{Type::String, "String"}
	};

	map<string, Type> keywords = {
		{"int", Type::IntegerKeyword},
		{"bool", Type::BooleanKeyword},
		{"string", Type::StringKeyword},
		{"array", Type::ArrayKeyword},
		{"if", Type::IfKeyword},
		{"while", Type::WhileKeyword},
		{"and", Type::AndKeyword},
		{"or", Type::OrKeyword},
		{"true", Type::TrueKeyword},
		{"false", Type::FalseKeyword}
	};

	map<string, Type> operations = {
		{"=", Type::AssignmentOperation},
		{"==", Type::EqualityOperation},
		{"<>", Type::InequalityOperation},
		{">", Type::MoreOperation},
		{"<", Type::LessOperation},
		{">=", Type::MoreOrEqualOperation},
		{"<=", Type::LessOrEqualOperation},
		{"+", Type::AdditionOperation},
		{"-", Type::SubtractionOperation},
		{"*", Type::MultiplicationOperation},
		{"/", Type::DivisionOperation}
	};

	map<string, Type> symbols = {
		{"\n", Type::NewLineSymbol},
		{"{", Type::OpeningBraceSymbol},
		{"}", Type::ClosingBraceSymbol},
		{"(", Type::OpeningParenthesisSymbol},
		{")", Type::ClosingParenthesisSymbol},
		{"[", Type::OpeningSquareBracketSymbol},
		{"]", Type::ClosingSquareBracketSymbol}
	};

	regex arrayElementRegex;
	regex numberRegex;
	regex wordRegex;
	regex stringRegex;

	ifstream inputFile;
	ofstream outputFile;
	vector<Token*> tokens;
	string inputLine;
	string buffer;
	int line;
	int col;

public:
	LexicalAnalyzer();
	vector<Token*> Launch();
	void RecognizeBuffer();
};
