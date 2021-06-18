#include <map>
#include <string>
#include <vector>
#include <regex>
#include "Token.h"
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "Interpreter.h"
using namespace std;

int main()
{
	vector<Token*> lexicalAnalyzerOutput;
	vector<Token*> syntaxAnalyzerOutput;

	LexicalAnalyzer lexicalAnalyzer;
	lexicalAnalyzerOutput = lexicalAnalyzer.Launch();

	SyntaxAnalyzer syntaxAnalyzer;
	syntaxAnalyzerOutput = syntaxAnalyzer.Launch(lexicalAnalyzerOutput);

	Interpreter interpreter;
	interpreter.Launch(syntaxAnalyzerOutput[0]);

	return 0;
}