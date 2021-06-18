#pragma once
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include "Type.h"
#include "Token.h"
using namespace std;

class SyntaxAnalyzer {
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

	vector<map<vector<Type>, Type>> nonTerminals{
		{
			{{Type::IntegerKeyword, Type::Word}, Type::VariableDeclaration},
			{{Type::BooleanKeyword, Type::Word}, Type::VariableDeclaration},
			{{Type::StringKeyword, Type::Word}, Type::VariableDeclaration},
			{{Type::ArrayKeyword, Type::Word}, Type::VariableDeclaration},

			{{Type::Word}, Type::Name},
			{{Type::Number}, Type::Name},
			{{Type::ArrayElement}, Type::Name},
			{{Type::String}, Type::Name},

			{{Type::AdditionOperation}, Type::ExpressionOperation},
			{{Type::SubtractionOperation}, Type::ExpressionOperation},
			{{Type::MultiplicationOperation}, Type::ExpressionOperation},
			{{Type::DivisionOperation}, Type::ExpressionOperation},

			{{Type::EqualityOperation}, Type::ComparisonOperation},
			{{Type::InequalityOperation}, Type::ComparisonOperation},
			{{Type::MoreOperation}, Type::ComparisonOperation},
			{{Type::LessOperation}, Type::ComparisonOperation},
			{{Type::MoreOrEqualOperation}, Type::ComparisonOperation},
			{{Type::LessOrEqualOperation}, Type::ComparisonOperation},

			{{Type::AndKeyword}, Type::AndOrKeyword},
			{{Type::OrKeyword}, Type::AndOrKeyword},

			{{Type::TrueKeyword}, Type::TrueFalseKeyword},
			{{Type::FalseKeyword}, Type::TrueFalseKeyword}
		},

		{
			{{Type::AssignmentOperation, Type::Expression}, Type::Assign},
			{{Type::AssignmentOperation, Type::Array}, Type::Assign},

			{{Type::Expression, Type::ComparisonOperation, Type::Expression}, Type::Comparison}
		},

		{
			{{Type::VariableDeclaration, Type::Assign, Type::NewLineSymbol}, Type::Line},
			{{Type::VariableDeclaration, Type::NewLineSymbol}, Type::Line},
			{{Type::Expression, Type::Assign, Type::NewLineSymbol}, Type::Line}
		},

		{
			{{Type::Line}, Type::Element},
			{{Type::IfBlock}, Type::Element},
			{{Type::WhileBlock}, Type::Element}
		}
	};

	ofstream outputFile;
	vector<Type> types;
	Token* tmpToken;
	bool tokenRecognized;
	int currentPosition;
	vector<Token*> buffer;

public:
	SyntaxAnalyzer();
	vector<Token*> Launch(vector<Token*> tokens);
	vector<Token*> StructurizeTokensWrapper(vector<Token*> tokens,
		map<vector<Type>, Type> nonTerminalsLevel);
	vector<Token*> StructurizeTokens(vector<Token*> tokens,
		map<vector<Type>, Type> nonTerminalsLevel);
	vector<Token*> StructurizeArrays(vector<Token*> tokens);
	vector<Token*> CheckArray(vector<Token*> tokens);
	vector<Token*> StructurizeExpressions(vector<Token*> tokens);
	vector<Token*> CheckExpression(vector<Token*> tokens);
	bool BracketSequenceIsCorrect();
	vector<Token*> StructurizeConditions(vector<Token*> tokens);
	vector<Token*> CheckCondition(vector<Token*> tokens);
	vector<Token*> StructurizeIfWhileBlocks(vector<Token*> tokens);
	vector<Token*> CheckIfWhileBlock(vector<Token*> tokens);
	vector<Token*> StructurizeProgram(vector<Token*> tokens);
	vector<Token*> CheckProgram(vector<Token*> tokens);
	vector<Token*> RemoveNewLines(vector<Token*> tokens);
	void SetNestingLevel(Token* token);
	void PrintToken(Token* token);
};

