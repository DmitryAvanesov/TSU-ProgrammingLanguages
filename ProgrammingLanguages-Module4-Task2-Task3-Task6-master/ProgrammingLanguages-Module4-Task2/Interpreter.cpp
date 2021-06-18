#include "Interpreter.h"

Interpreter::Interpreter() {
	outputFile.open("outputInterpreter.txt");
}

void Interpreter::Launch(Token* programToken) {
	for (Token* elementToken : programToken->tokenInnerTokens) {
		Token* elementChildToken = elementToken->tokenInnerTokens[0];

		if (elementChildToken->tokenType == Type::Line) {
			InterpretLine(elementChildToken);
		}
		else if (elementChildToken->tokenType == Type::IfBlock) {
			if (CheckCondition(elementChildToken->tokenInnerTokens[1])) {
				for (Token* blockChildToken : elementChildToken->tokenInnerTokens) {
					if (blockChildToken->tokenType == Type::Line) {
						InterpretLine(blockChildToken);
					}
				}
			}
		}
		else if (elementChildToken->tokenType == Type::WhileBlock) {
			vector<Token*> backupExpressionLeft = vector<Token*>(elementChildToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenInnerTokens[0]->tokenInnerTokens.begin(), elementChildToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenInnerTokens[0]->tokenInnerTokens.end());
			vector<Token*> backupExpressionRight = vector<Token*>(elementChildToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenInnerTokens[2]->tokenInnerTokens.begin(), elementChildToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenInnerTokens[2]->tokenInnerTokens.end());


			while (CheckCondition(elementChildToken->tokenInnerTokens[1])) {
				for (Token* blockChildToken : elementChildToken->tokenInnerTokens) {
					if (blockChildToken->tokenType == Type::Line) {
						vector<Token*> backupExpression = vector<Token*>(blockChildToken->tokenInnerTokens[1]->tokenInnerTokens[1]->tokenInnerTokens.begin(), blockChildToken->tokenInnerTokens[1]->tokenInnerTokens[1]->tokenInnerTokens.end());
						InterpretLine(blockChildToken);
						blockChildToken->tokenInnerTokens[1]->tokenInnerTokens[1]->tokenInnerTokens = backupExpression;
					}

					elementChildToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenInnerTokens[0]->tokenInnerTokens = backupExpressionLeft;
					elementChildToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenInnerTokens[2]->tokenInnerTokens = backupExpressionRight;
				}
			}
		}
	}

	outputFile.close();
}

bool Interpreter::CheckCondition(Token* conditionToken) {
	while (conditionToken->tokenInnerTokens.size() > 1) {
		if (conditionToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
			Type::AndKeyword)
		{
			if (!CheckComparison(conditionToken->tokenInnerTokens[0]) ||
				!CheckComparison(conditionToken->tokenInnerTokens[2]))
			{
				return false;
			}
		}
		else {
			if (!CheckComparison(conditionToken->tokenInnerTokens[0]) &&
				!CheckComparison(conditionToken->tokenInnerTokens[2]))
			{
				return false;
			}
		}
	}

	if (!CheckComparison(conditionToken->tokenInnerTokens[0]))
	{
		return false;
	}

	return true;
}

bool Interpreter::CheckComparison(Token* comparisonToken) {
	try {
		ComputeExpressionInt(comparisonToken->tokenInnerTokens[0]);
		ComputeExpressionInt(comparisonToken->tokenInnerTokens[2]);
		return CompareIntInt(comparisonToken);
	}
	catch (exception e) {
		try {
			ComputeExpressionString(comparisonToken->tokenInnerTokens[0]);
			ComputeExpressionInt(comparisonToken->tokenInnerTokens[2]);
			return CompareStringInt(comparisonToken);
		}
		catch (exception e) {
			try {
				ComputeExpressionInt(comparisonToken->tokenInnerTokens[0]);
				ComputeExpressionString(comparisonToken->tokenInnerTokens[2]);
				return CompareIntString(comparisonToken);
			}
			catch (exception e) {
				ComputeExpressionString(comparisonToken->tokenInnerTokens[0]);
				ComputeExpressionString(comparisonToken->tokenInnerTokens[2]);
				return CompareStringString(comparisonToken);
			}
		}
	}
}

bool Interpreter::CompareIntInt(Token* comparisonToken) {
	if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::MoreOperation)
	{
		if (ComputeExpressionInt(comparisonToken->tokenInnerTokens[0]) >
			ComputeExpressionInt(comparisonToken->tokenInnerTokens[2]))
		{
			return true;
		}
	}
	else if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::LessOperation)
	{
		if (ComputeExpressionInt(comparisonToken->tokenInnerTokens[0]) <
			ComputeExpressionInt(comparisonToken->tokenInnerTokens[2]))
		{
			return true;
		}
	}
	else if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::MoreOrEqualOperation)
	{
		if (ComputeExpressionInt(comparisonToken->tokenInnerTokens[0]) >=
			ComputeExpressionInt(comparisonToken->tokenInnerTokens[2]))
		{
			return true;
		}
	}
	else if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::LessOrEqualOperation)
	{
		if (ComputeExpressionInt(comparisonToken->tokenInnerTokens[0]) <=
			ComputeExpressionInt(comparisonToken->tokenInnerTokens[2]))
		{
			return true;
		}
	}
	else if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::EqualityOperation)
	{
		if (ComputeExpressionInt(comparisonToken->tokenInnerTokens[0]) ==
			ComputeExpressionInt(comparisonToken->tokenInnerTokens[2]))
		{
			return true;
		}
	}
	else if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::InequalityOperation)
	{
		if (ComputeExpressionInt(comparisonToken->tokenInnerTokens[0]) !=
			ComputeExpressionInt(comparisonToken->tokenInnerTokens[2]))
		{
			return true;
		}
	}

	return false;
}

bool Interpreter::CompareIntString(Token* comparisonToken) {
	if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::MoreOperation)
	{
		if (ComputeExpressionInt(comparisonToken->tokenInnerTokens[0]) >
			ComputeExpressionString(comparisonToken->tokenInnerTokens[2]).length())
		{
			return true;
		}
	}
	else if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::LessOperation)
	{
		if (ComputeExpressionInt(comparisonToken->tokenInnerTokens[0]) <
			ComputeExpressionString(comparisonToken->tokenInnerTokens[2]).length())
		{
			return true;
		}
	}
	else if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::MoreOrEqualOperation)
	{
		if (ComputeExpressionInt(comparisonToken->tokenInnerTokens[0]) >=
			ComputeExpressionString(comparisonToken->tokenInnerTokens[2]).length())
		{
			return true;
		}
	}
	else if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::LessOrEqualOperation)
	{
		if (ComputeExpressionInt(comparisonToken->tokenInnerTokens[0]) <=
			ComputeExpressionString(comparisonToken->tokenInnerTokens[2]).length())
		{
			return true;
		}
	}
	else if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::EqualityOperation)
	{
		if (ComputeExpressionInt(comparisonToken->tokenInnerTokens[0]) ==
			ComputeExpressionString(comparisonToken->tokenInnerTokens[2]).length())
		{
			return true;
		}
	}
	else if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::InequalityOperation)
	{
		if (ComputeExpressionInt(comparisonToken->tokenInnerTokens[0]) !=
			ComputeExpressionString(comparisonToken->tokenInnerTokens[2]).length())
		{
			return true;
		}
	}

	return false;
}

bool Interpreter::CompareStringInt(Token* comparisonToken) {
	if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::MoreOperation)
	{
		if (ComputeExpressionString(comparisonToken->tokenInnerTokens[0]).length() >
			ComputeExpressionInt(comparisonToken->tokenInnerTokens[2]))
		{
			return true;
		}
	}
	else if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::LessOperation)
	{
		if (ComputeExpressionString(comparisonToken->tokenInnerTokens[0]).length() <
			ComputeExpressionInt(comparisonToken->tokenInnerTokens[2]))
		{
			return true;
		}
	}
	else if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::MoreOrEqualOperation)
	{
		if (ComputeExpressionString(comparisonToken->tokenInnerTokens[0]).length() >=
			ComputeExpressionInt(comparisonToken->tokenInnerTokens[2]))
		{
			return true;
		}
	}
	else if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::LessOrEqualOperation)
	{
		if (ComputeExpressionString(comparisonToken->tokenInnerTokens[0]).length() <=
			ComputeExpressionInt(comparisonToken->tokenInnerTokens[2]))
		{
			return true;
		}
	}
	else if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::EqualityOperation)
	{
		if (ComputeExpressionString(comparisonToken->tokenInnerTokens[0]).length() ==
			ComputeExpressionInt(comparisonToken->tokenInnerTokens[2]))
		{
			return true;
		}
	}
	else if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::InequalityOperation)
	{
		if (ComputeExpressionString(comparisonToken->tokenInnerTokens[0]).length() !=
			ComputeExpressionInt(comparisonToken->tokenInnerTokens[2]))
		{
			return true;
		}
	}

	return false;
}

bool Interpreter::CompareStringString(Token* comparisonToken) {
	if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::MoreOperation)
	{
		if (ComputeExpressionString(comparisonToken->tokenInnerTokens[0]).length() >
			ComputeExpressionString(comparisonToken->tokenInnerTokens[2]).length())
		{
			return true;
		}
	}
	else if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::LessOperation)
	{
		if (ComputeExpressionString(comparisonToken->tokenInnerTokens[0]).length() <
			ComputeExpressionString(comparisonToken->tokenInnerTokens[2]).length())
		{
			return true;
		}
	}
	else if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::MoreOrEqualOperation)
	{
		if (ComputeExpressionString(comparisonToken->tokenInnerTokens[0]).length() >=
			ComputeExpressionString(comparisonToken->tokenInnerTokens[2]).length())
		{
			return true;
		}
	}
	else if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::LessOrEqualOperation)
	{
		if (ComputeExpressionString(comparisonToken->tokenInnerTokens[0]).length() <=
			ComputeExpressionString(comparisonToken->tokenInnerTokens[2]).length())
		{
			return true;
		}
	}
	else if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::EqualityOperation)
	{
		if (ComputeExpressionString(comparisonToken->tokenInnerTokens[0]).length() ==
			ComputeExpressionString(comparisonToken->tokenInnerTokens[2]).length())
		{
			return true;
		}
	}
	else if (comparisonToken->tokenInnerTokens[1]->tokenInnerTokens[0]->tokenType ==
		Type::InequalityOperation)
	{
		if (ComputeExpressionString(comparisonToken->tokenInnerTokens[0]).length() !=
			ComputeExpressionString(comparisonToken->tokenInnerTokens[2]).length())
		{
			return true;
		}
	}

	return false;
}

void Interpreter::InterpretLine(Token* lineToken) {
	// if line is variable declaration
	if (lineToken->tokenInnerTokens[0]->tokenType == Type::VariableDeclaration) {
		Token* variableDeclarationToken = lineToken->tokenInnerTokens[0];

		// if it's only declaration without assigning
		if (lineToken->tokenInnerTokens[1]->tokenType == Type::NewLineSymbol) {
			// if we declare an int
			if (variableDeclarationToken->tokenInnerTokens[0]->tokenType ==
				Type::IntegerKeyword)
			{
				intVariables.insert({ variableDeclarationToken->tokenInnerTokens[1]->tokenValue, 0 });
			}
			// if we declare a bool
			else if (variableDeclarationToken->tokenInnerTokens[0]->tokenType ==
				Type::BooleanKeyword)
			{
				boolVariables.insert({ variableDeclarationToken->tokenInnerTokens[1]->tokenValue, false });
			}
			// if we declare a string
			else if (variableDeclarationToken->tokenInnerTokens[0]->tokenType ==
				Type::StringKeyword)
			{
				stringVariables.insert({ variableDeclarationToken->tokenInnerTokens[1]->tokenValue, "" });
			}
			// if we declare an array (we can't do that without assigning)
			else if (variableDeclarationToken->tokenInnerTokens[0]->tokenType ==
				Type::ArrayKeyword)
			{
				outputFile << "Error at " << variableDeclarationToken->tokenPosition.line <<
					":" << variableDeclarationToken->tokenPosition.col <<
					" You must initialize an array straight away\n";
			}
		}
		// if there's declaration with assigning
		else if (lineToken->tokenInnerTokens[1]->tokenType == Type::Assign) {
			Token* assignableToken = lineToken->tokenInnerTokens[1]->tokenInnerTokens[1];

			// if we assign an int
			if (variableDeclarationToken->tokenInnerTokens[0]->tokenType ==
				Type::IntegerKeyword && assignableToken->tokenType == Type::Expression)
			{
				try {
					intVariables.insert({ variableDeclarationToken->tokenInnerTokens[1]->tokenValue,
					ComputeExpressionInt(assignableToken) });
				}
				catch(exception e) {
					outputFile << "Error at " << assignableToken->tokenPosition.line <<
						":" << assignableToken->tokenPosition.col <<
						" You cannot write string into int variable\n";
				}
			}
			// if we assign a bool
			else if (variableDeclarationToken->tokenInnerTokens[0]->tokenType ==
				Type::BooleanKeyword && assignableToken->tokenType == Type::Expression)
			{
				// if there's only "true" or "false"
				if (assignableToken->tokenInnerTokens.size() == 1) {
					bool boolValue;

					if (assignableToken->tokenInnerTokens[0]->tokenInnerTokens[0]->tokenType == Type::TrueKeyword) {
						boolValue = true;
					}
					else {
						boolValue = false;
					}

					if (boolVariables.find(variableDeclarationToken->tokenInnerTokens[1]->tokenValue) ==
						boolVariables.end())
					{
						boolVariables.insert(
							{ variableDeclarationToken->tokenInnerTokens[1]->tokenValue, boolValue }
						);
					}
					else {
						outputFile << "Error at " << variableDeclarationToken->tokenPosition.line <<
							":" << variableDeclarationToken->tokenPosition.col <<
							" The variable was already declared\n";
					}
				}
				// if there's something else (that's an error)
				else {
					outputFile << "Error at " << assignableToken->tokenPosition.line <<
						":" << assignableToken->tokenPosition.col <<
						" Bool can be assigned with true or false, nothing else\n";
				}
			}
			// if we assign a string
			else if (variableDeclarationToken->tokenInnerTokens[0]->tokenType ==
				Type::StringKeyword && assignableToken->tokenType == Type::Expression)
			{
				try {
					stringVariables.insert({ variableDeclarationToken->tokenInnerTokens[1]->tokenValue,
					ComputeExpressionString(assignableToken) });
				}
				catch (exception e) {
					outputFile << "Error at " << assignableToken->tokenPosition.line <<
						":" << assignableToken->tokenPosition.col <<
						" You cannot write int into string variable\n";
				}
			}
			// if we assign an array
			else if (variableDeclarationToken->tokenInnerTokens[0]->tokenType ==
				Type::ArrayKeyword && assignableToken->tokenType == Type::Array)
			{
				vector<int> newIntArray;
				vector<string> newStringArray;
				bool arrayIsInt = false;

				for (Token* expressionElementToken : assignableToken->tokenInnerTokens) {
					if (expressionElementToken->tokenType == Type::Number) {
						arrayIsInt = true;
						newIntArray.push_back(stoi(expressionElementToken->tokenValue));
					}
					else if (expressionElementToken->tokenType == Type::String) {
						newStringArray.push_back(expressionElementToken->tokenValue);
					}
				}

				if (arrayIsInt) {
					if (arrayIntVariables.find(variableDeclarationToken->tokenInnerTokens[1]->tokenValue) ==
						arrayIntVariables.end())
					{
						arrayIntVariables.insert(
							{ variableDeclarationToken->tokenInnerTokens[1]->tokenValue, newIntArray }
						);
					}
					else {
						outputFile << "Error at " << variableDeclarationToken->tokenPosition.line <<
							":" << variableDeclarationToken->tokenPosition.col <<
							" The variable was already declared\n";
					}
				}
				else {
					if (arrayStringVariables.find(variableDeclarationToken->tokenInnerTokens[1]->tokenValue) ==
						arrayStringVariables.end())
					{
						arrayStringVariables.insert(
							{ variableDeclarationToken->tokenInnerTokens[1]->tokenValue, newStringArray }
						);
					}
					else {
						outputFile << "Error at " << variableDeclarationToken->tokenPosition.line <<
							":" << variableDeclarationToken->tokenPosition.col <<
							" The variable was already declared\n";
					}
				}
			}
		}
	}
	// if line is assigning to the previously declared variable
	else {
		Token* varToken = lineToken->tokenInnerTokens[0];
		Token* wordToken = varToken->tokenInnerTokens[0]->tokenInnerTokens[0];
		Token* expressionToken = lineToken->tokenInnerTokens[1]->tokenInnerTokens[1];
		
		// if we assign something to a variable
		if (varToken->tokenInnerTokens.size() == 1 &&
			wordToken->tokenType == Type::Word)
		{
			if (intVariables.find(wordToken->tokenValue) != intVariables.end())
			{
				try {
					intVariables[wordToken->tokenValue] = ComputeExpressionInt(expressionToken);
				}
				catch (exception e) {
					outputFile << "Error at " << expressionToken->tokenPosition.line <<
						":" << expressionToken->tokenPosition.col <<
						" You cannot write string into int variable\n";
				}
			}
			else if (boolVariables.find(wordToken->tokenValue) != boolVariables.end()) {
				if (expressionToken->tokenInnerTokens.size() == 1 &&
					expressionToken->tokenInnerTokens[0]->tokenType ==
					Type::TrueFalseKeyword)
				{
					if (expressionToken->tokenInnerTokens[0]->tokenInnerTokens[0]->tokenType ==
						Type::TrueKeyword)
					{
						boolVariables[wordToken->tokenValue] = true;
					}
					else {
						boolVariables[wordToken->tokenValue] = false;
					}
				}
			}
			else if (stringVariables.find(wordToken->tokenValue) != stringVariables.end())
			{
				try {
					stringVariables[wordToken->tokenValue] = ComputeExpressionString(expressionToken);
				}
				catch (exception e) {
					outputFile << "Error at " << expressionToken->tokenPosition.line <<
						":" << expressionToken->tokenPosition.col <<
						" You cannot write int into string variable\n";
				}
			}
			else if (arrayIntVariables.find(wordToken->tokenValue) != arrayIntVariables.end() ||
				arrayStringVariables.find(wordToken->tokenValue) != arrayStringVariables.end())
			{
				outputFile << "Error at " << expressionToken->tokenPosition.line <<
					":" << expressionToken->tokenPosition.col <<
					" You cannot rewrite an array\n";
			}
			else {
				outputFile << "Error at " << expressionToken->tokenPosition.line <<
					":" << expressionToken->tokenPosition.col <<
					" Variable needs to be declared\n";
			}
		}
		else {
			outputFile << "Error at " << varToken->tokenPosition.line <<
				":" << varToken->tokenPosition.col <<
				" You can only assign to a variable\n";
		}
	}

	PrintVariableTable();
}

int Interpreter::ComputeExpressionInt(Token* expressionToken) {
	expressionToken = PrepareExpression(expressionToken);

	while (expressionToken->tokenInnerTokens.size() > 1) {
		expressionToken = HandleHighestPriorityOperation(expressionToken);
	}

	if (expressionToken->tokenInnerTokens[0]->tokenInnerTokens[0]->tokenType ==
		Type::String)
	{
		throw logic_error("");
	}

	return stoi(expressionToken->tokenInnerTokens[0]->tokenInnerTokens[0]->tokenValue);
}

string Interpreter::ComputeExpressionString(Token* expressionToken) {
	expressionToken = PrepareExpression(expressionToken);

	while (expressionToken->tokenInnerTokens.size() > 1) {
		expressionToken = HandleHighestPriorityOperation(expressionToken);
	}

	if (expressionToken->tokenInnerTokens[0]->tokenInnerTokens[0]->tokenType ==
		Type::Number)
	{
		throw logic_error("");
	}
	
	return expressionToken->tokenInnerTokens[0]->tokenInnerTokens[0]->tokenValue;
}

Token* Interpreter::PrepareExpression(Token* expressionToken) {
	int numberOfParenthesis = 0;
	int currentChildToken = 0;
	bool variableDefined = false;

	/* changing variables (including array elements) for their values +
	changing BOOLs for INTs + defining priorities + removing parenthesis*/
	while (currentChildToken < expressionToken->tokenInnerTokens.size()) {
		Token* expressionChildToken = expressionToken->tokenInnerTokens[currentChildToken];
		Token* tmpToken;

		if (expressionChildToken->tokenInnerTokens.size() > 0 &&
			expressionChildToken->tokenInnerTokens[0]->tokenType == Type::Word)
		{
			if (intVariables.find(expressionChildToken->tokenInnerTokens[0]->tokenValue) !=
				intVariables.end())
			{
				variableDefined = true;

				tmpToken = new Token(
					Type::Name,
					expressionChildToken->tokenPosition.line,
					expressionChildToken->tokenPosition.col,
					"",
					vector<Token*>(
						1,
						new Token(
							Type::Number,
							expressionChildToken->tokenPosition.line,
							expressionChildToken->tokenPosition.col,
							to_string(intVariables[expressionChildToken->tokenInnerTokens[0]->tokenValue]),
							vector<Token*>()
						)
					)
				);
			}
			else if (boolVariables.find(expressionChildToken->tokenInnerTokens[0]->tokenValue) !=
				boolVariables.end())
			{
				variableDefined = true;

				if (boolVariables[expressionChildToken->tokenInnerTokens[0]->tokenValue] == true) {
					tmpToken = new Token(
						Type::TrueFalseKeyword,
						expressionChildToken->tokenPosition.line,
						expressionChildToken->tokenPosition.col,
						"",
						vector<Token*>(
							1,
							new Token(
								Type::TrueKeyword,
								expressionChildToken->tokenPosition.line,
								expressionChildToken->tokenPosition.col,
								"",
								vector<Token*>()
							)
						)
					);
				}
				else {
					tmpToken = new Token(
						Type::TrueFalseKeyword,
						expressionChildToken->tokenPosition.line,
						expressionChildToken->tokenPosition.col,
						"",
						vector<Token*>(
							1,
							new Token(
								Type::FalseKeyword,
								expressionChildToken->tokenPosition.line,
								expressionChildToken->tokenPosition.col,
								"",
								vector<Token*>()
							)
							)
					);
				}
			}
			else if (stringVariables.find(expressionChildToken->tokenInnerTokens[0]->tokenValue) !=
				stringVariables.end())
			{
				variableDefined = true;

				tmpToken = new Token(
					Type::Name,
					expressionChildToken->tokenPosition.line,
					expressionChildToken->tokenPosition.col,
					"",
					vector<Token*>(
						1,
						new Token(
							Type::String,
							expressionChildToken->tokenPosition.line,
							expressionChildToken->tokenPosition.col,
							stringVariables[expressionChildToken->tokenInnerTokens[0]->tokenValue],
							vector<Token*>()
						)
					)
				);
			}
			else {
				outputFile << "Error at " << expressionChildToken->tokenPosition.line <<
					":" << expressionChildToken->tokenPosition.col <<
					" Variable must be declared before participating in an expression\n";
			}

			if (variableDefined) {
				expressionToken->tokenInnerTokens.insert(
					expressionToken->tokenInnerTokens.begin() + currentChildToken,
					tmpToken
				);

				expressionToken->tokenInnerTokens.erase(
					expressionToken->tokenInnerTokens.begin() + currentChildToken + 1,
					expressionToken->tokenInnerTokens.begin() + currentChildToken + 2
				);
			}
		}

		if (expressionChildToken->tokenInnerTokens.size() > 0 &&
			expressionChildToken->tokenInnerTokens[0]->tokenType == Type::ArrayElement)
		{
			variableDefined = false;

			string arrayName =
				expressionChildToken->tokenInnerTokens[0]->tokenValue.substr(0, expressionChildToken->tokenInnerTokens[0]->tokenValue.find_first_of('['));
			string arrayIndex = 
				expressionChildToken->tokenInnerTokens[0]->tokenValue.substr(expressionChildToken->tokenInnerTokens[0]->tokenValue.find_first_of('[') + 1, expressionChildToken->tokenInnerTokens[0]->tokenValue.size() - expressionChildToken->tokenInnerTokens[0]->tokenValue.find_first_of('[') - 2);

			if (intVariables.find(arrayIndex) != intVariables.end())
			{
				arrayIndex = to_string(intVariables[arrayIndex]);
			}
			
			if (arrayIntVariables.find(arrayName) != arrayIntVariables.end()) {
				variableDefined = true;

				tmpToken = new Token(
					Type::Name,
					expressionChildToken->tokenPosition.line,
					expressionChildToken->tokenPosition.col,
					"",
					vector<Token*>(
						1,
						new Token(
							Type::Number,
							expressionChildToken->tokenPosition.line,
							expressionChildToken->tokenPosition.col,
							to_string(arrayIntVariables[arrayName][stoi(arrayIndex)]),
							vector<Token*>()
						)
					)
				);
			}
			else if (arrayStringVariables.find(arrayName) != arrayStringVariables.end())
			{
				variableDefined = true;

				tmpToken = new Token(
					Type::Name,
					expressionChildToken->tokenPosition.line,
					expressionChildToken->tokenPosition.col,
					"",
					vector<Token*>(
						1,
						new Token(
							Type::String,
							expressionChildToken->tokenPosition.line,
							expressionChildToken->tokenPosition.col,
							arrayStringVariables[arrayName][stoi(arrayIndex)],
							vector<Token*>()
						)
					)
				);
			}

			if (variableDefined) {
				expressionToken->tokenInnerTokens.insert(
					expressionToken->tokenInnerTokens.begin() + currentChildToken,
					tmpToken
				);

				expressionToken->tokenInnerTokens.erase(
					expressionToken->tokenInnerTokens.begin() + currentChildToken + 1,
					expressionToken->tokenInnerTokens.begin() + currentChildToken + 2
				);
			}
		}

		if (expressionChildToken->tokenType == Type::TrueFalseKeyword) {
			if (expressionChildToken->tokenInnerTokens[0]->tokenType == Type::TrueKeyword) {
				tmpToken = new Token(
					Type::Name,
					expressionChildToken->tokenPosition.line,
					expressionChildToken->tokenPosition.col,
					"",
					vector<Token*>(
						1,
						new Token(
							Type::Number,
							expressionChildToken->tokenPosition.line,
							expressionChildToken->tokenPosition.col,
							"1",
							vector<Token*>()
						)
					)
				);
			}
			else {
				tmpToken = new Token(
					Type::Name,
					expressionChildToken->tokenPosition.line,
					expressionChildToken->tokenPosition.col,
					"",
					vector<Token*>(
						1,
						new Token(
							Type::Number,
							expressionChildToken->tokenPosition.line,
							expressionChildToken->tokenPosition.col,
							"0",
							vector<Token*>()
						)
					)
				);
			}

			expressionToken->tokenInnerTokens.insert(
				expressionToken->tokenInnerTokens.begin() + currentChildToken,
				tmpToken
			);

			expressionToken->tokenInnerTokens.erase(
				expressionToken->tokenInnerTokens.begin() + currentChildToken + 1,
				expressionToken->tokenInnerTokens.begin() + currentChildToken + 2
			);
		}

		if (expressionChildToken->tokenType == Type::ExpressionOperation) {
			expressionChildToken->expressionPriority += numberOfParenthesis * 2;

			if (expressionChildToken->tokenInnerTokens[0]->tokenType ==
				Type::MultiplicationOperation
				||
				expressionChildToken->tokenInnerTokens[0]->tokenType ==
				Type::DivisionOperation)
			{
				expressionChildToken->expressionPriority++;
			}
		}

		if (expressionChildToken->tokenType == Type::OpeningParenthesisSymbol) {
			numberOfParenthesis++;

			expressionToken->tokenInnerTokens.erase(
				expressionToken->tokenInnerTokens.begin() + currentChildToken,
				expressionToken->tokenInnerTokens.begin() + currentChildToken + 1
			);
		}
		else if (expressionChildToken->tokenType == Type::ClosingParenthesisSymbol) {
			numberOfParenthesis--;

			expressionToken->tokenInnerTokens.erase(
				expressionToken->tokenInnerTokens.begin() + currentChildToken,
				expressionToken->tokenInnerTokens.begin() + currentChildToken + 1
			);
		}
		else {
			currentChildToken++;
		}
	}

	return expressionToken;
}

Token* Interpreter::HandleHighestPriorityOperation(Token* expressionToken) {
	int maxPriority = -1;
	int maxPriorityPosition = 0;
	Token* tmpToken;

	for (int i = 0; i < expressionToken->tokenInnerTokens.size(); i++) {
		if (expressionToken->tokenInnerTokens[i]->tokenType ==
			Type::ExpressionOperation)
		{
			int curPriority = expressionToken->tokenInnerTokens[i]->expressionPriority;

			if (curPriority > maxPriority) {
				maxPriority = curPriority;
				maxPriorityPosition = i;
			}
		}
	}

	if (expressionToken->tokenInnerTokens[maxPriorityPosition]->tokenInnerTokens[0]->tokenType ==
		Type::AdditionOperation)
	{
		if (expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenInnerTokens[0]->tokenType ==
			Type::Number)
		{
			tmpToken = new Token(
				Type::Name,
				expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenPosition.line,
				expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenPosition.col,
				"",
				vector<Token*>(
					1,
					new Token(
						Type::Number,
						expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenPosition.line,
						expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenPosition.col,
						to_string(
							stoi(expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenInnerTokens[0]->tokenValue) +
							stoi(expressionToken->tokenInnerTokens[maxPriorityPosition + 1]->tokenInnerTokens[0]->tokenValue)
						),
						vector<Token*>()
					)
				)
			);
		}
		else if (expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenInnerTokens[0]->tokenType ==
			Type::String)
		{
			tmpToken = new Token(
				Type::Name,
				expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenPosition.line,
				expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenPosition.col,
				"",
				vector<Token*>(
					1,
					new Token(
						Type::String,
						expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenPosition.line,
						expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenPosition.col,
						expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenInnerTokens[0]->tokenValue +
						expressionToken->tokenInnerTokens[maxPriorityPosition + 1]->tokenInnerTokens[0]->tokenValue,
						vector<Token*>()
					)
				)
			);
		}
	}
	else {
		if (expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenInnerTokens[0]->tokenType !=
			Type::Number ||
			expressionToken->tokenInnerTokens[maxPriorityPosition + 1]->tokenInnerTokens[0]->tokenType !=
			Type::Number)
		{
			outputFile << "Error at " << expressionToken->tokenPosition.line <<
				":" << expressionToken->tokenPosition.col <<
				" The only allowable operation between numbers and strings is addition\n";
		}
		
		if (expressionToken->tokenInnerTokens[maxPriorityPosition]->tokenInnerTokens[0]->tokenType ==
			Type::SubtractionOperation)
		{
			tmpToken = new Token(
				Type::Name,
				expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenPosition.line,
				expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenPosition.col,
				"",
				vector<Token*>(
					1,
					new Token(
						Type::Number,
						expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenPosition.line,
						expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenPosition.col,
						to_string(
							stoi(expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenInnerTokens[0]->tokenValue) -
							stoi(expressionToken->tokenInnerTokens[maxPriorityPosition + 1]->tokenInnerTokens[0]->tokenValue)
						),
						vector<Token*>()
					)
					)
			);
		}
		else if (expressionToken->tokenInnerTokens[maxPriorityPosition]->tokenInnerTokens[0]->tokenType ==
			Type::MultiplicationOperation)
		{
			tmpToken = new Token(
				Type::Name,
				expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenPosition.line,
				expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenPosition.col,
				"",
				vector<Token*>(
					1,
					new Token(
						Type::Number,
						expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenPosition.line,
						expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenPosition.col,
						to_string(
							stoi(expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenInnerTokens[0]->tokenValue) *
							stoi(expressionToken->tokenInnerTokens[maxPriorityPosition + 1]->tokenInnerTokens[0]->tokenValue)
						),
						vector<Token*>()
					)
					)
			);
		}
		else if (expressionToken->tokenInnerTokens[maxPriorityPosition]->tokenInnerTokens[0]->tokenType ==
			Type::DivisionOperation)
		{
			tmpToken = new Token(
				Type::Name,
				expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenPosition.line,
				expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenPosition.col,
				"",
				vector<Token*>(
					1,
					new Token(
						Type::Number,
						expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenPosition.line,
						expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenPosition.col,
						to_string(
							stoi(expressionToken->tokenInnerTokens[maxPriorityPosition - 1]->tokenInnerTokens[0]->tokenValue) /
							stoi(expressionToken->tokenInnerTokens[maxPriorityPosition + 1]->tokenInnerTokens[0]->tokenValue)
						),
						vector<Token*>()
					)
					)
			);
		}
	}

	expressionToken->tokenInnerTokens.erase(
		expressionToken->tokenInnerTokens.begin() + maxPriorityPosition - 1,
		expressionToken->tokenInnerTokens.begin() + maxPriorityPosition + 2
	);

	if (maxPriorityPosition - 1 < expressionToken->tokenInnerTokens.size()) {
		expressionToken->tokenInnerTokens.insert(
			expressionToken->tokenInnerTokens.begin() + maxPriorityPosition - 1,
			tmpToken
		);
	}
	else {
		expressionToken->tokenInnerTokens.push_back(tmpToken);
	}

	return expressionToken;
}

void Interpreter::PrintVariableTable() {
	outputFile << "Int variables:\n";

	for (auto var : intVariables) {
		outputFile << var.first << " " << var.second << "\n";
	}

	outputFile << "Bool variables:\n";

	for (auto var : boolVariables) {
		outputFile << boolalpha << var.first << " " << var.second << "\n";
	}

	outputFile << "String variables:\n";

	for (auto var : stringVariables) {
		outputFile << var.first << " '" << var.second << "'\n";
	}

	outputFile << "Array<int> variables:\n";

	for (auto var : arrayIntVariables) {
		outputFile << var.first << " [ ";

		for (int arrayElem : var.second) {
			outputFile << arrayElem << " ";
		}

		outputFile << "]\n";
	}

	outputFile << "Array<string> variables:\n";

	for (auto var : arrayStringVariables) {
		outputFile << var.first << " [ ";

		for (string arrayElem : var.second) {
			outputFile << "'" << arrayElem << "' ";
		}

		outputFile << "]\n";
	}

	outputFile << "\n\n";
}
