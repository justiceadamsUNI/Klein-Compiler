#include "../header/Scanner.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

Scanner::Scanner(string FilePath)
{	
	FileContents = readFile(FilePath);
	FilePosition = 0;
}

Token Scanner::getNextToken()
{
	while (FilePosition < FileContents.size())
	{
		skipPastWhiteSpace();
		char CharAtPosition = FileContents[FilePosition];

		if (CharAtPosition == ',')
		{
			return consumeCommaToken();

		} else if (CharAtPosition == ':')
		{
			return consumeColonToken();
		}
		else if (CharAtPosition == '+' || CharAtPosition == '*' || CharAtPosition == '/' || CharAtPosition == '-')
		{
			return consumeOperatorToken(CharAtPosition);
		}
		else if (CharAtPosition == '(')
		{
			if (isCommentStart())
			{
				ignoreComment();
			}
			else {
				return consumeParenthesisToken(CharAtPosition);
			}
		}
		else if (CharAtPosition == ')')
		{
			return consumeParenthesisToken(CharAtPosition);
		}
		else if (CharAtPosition == '<' || CharAtPosition == '=')
		{
			return consumeComparatorToken(CharAtPosition);
		}
		else if (Digits.find(CharAtPosition) != string::npos) {
			if (CharAtPosition == '0')
			{
				return consumeZeroToken();
			}
			else
			{
				return consumeIntegerToken();
			}
		}
		else if (isalpha(CharAtPosition)) {
			return consumeGenericWordToken();
		}
	}

	return Token(END_OF_FILE, "");
}

void Scanner::skipPastWhiteSpace()
{
	while (FilePosition < FileContents.size() && isspace(FileContents[FilePosition]))
	{
		FilePosition++;
	}
}

string Scanner::readFile(string FilePath)
{
	string FileData;
	fstream inputFile;
	inputFile.open(FilePath.c_str());

	// Check for successful opening
	if (inputFile.fail())
	{
		throw invalid_argument("The File Cannot Be Found! - " + FilePath);
	}

	FileData.assign((std::istreambuf_iterator<char>(inputFile)),
		(std::istreambuf_iterator<char>()));

	return FileData;
}

Token Scanner::consumeParenthesisToken(char ParenChar)
{
	FilePosition++;
	return Token(PARENTHESIS, string(1, ParenChar));
}

Token Scanner::consumeCommaToken()
{
	FilePosition++;
	return Token(COMMA, "");
}

Token Scanner::consumeColonToken()
{
	FilePosition++;
	return Token(COLON, "");
}

Token Scanner::consumeOperatorToken(char OperatorChar)
{
	FilePosition++;
	return Token(ARITHMETIC_OPERATOR, string(1, OperatorChar));
}

Token Scanner::consumeComparatorToken(char ComparatorChar)
{
	FilePosition++;
	return Token(COMPARATOR, string(1, ComparatorChar));
}

Token Scanner::consumeIntegerToken()
{
	string Accumulator = string(1, FileContents[FilePosition]);

	//Set initial endState To False
	bool ValidEndState = false;
	while (!ValidEndState && FilePosition < FileContents.size())
	{
		if (FilePosition + 1 == FileContents.size())
		{
			//Last character in file. Update accumulator, and set End State To Valid.
			FilePosition++;
			ValidEndState = true;
			continue;
		}

		char NextChar = FileContents[FilePosition + 1];

		//If not the last character in file, check that next char.
		if (Digits.find(NextChar) != string::npos)
		{
			//If next character a digit, add to accumulator and keep going.
			Accumulator = Accumulator + NextChar;
			FilePosition++;
			continue;
		}
		else if (SelfDelimiters.find(NextChar) != string::npos || isspace(NextChar))
		{
			//If next character is a self delimiter, or a space, we're in a stop state.
			ValidEndState = true;
			FilePosition++;
			continue;
		}
		else
		{
			//If any other character is recognized, blow up.
			string ErrorMessage = "ERROR: Unexpected character while scanning Integer literal at pos - " + to_string(FilePosition + 1) + " char= " + NextChar;
			throw runtime_error(ErrorMessage);
		}
	}

	return Token(INTEGER, Accumulator);
}

Token Scanner::consumeZeroToken()
{
	
	while (FilePosition + 1  < FileContents.size())
	{
		char NextChar = FileContents[FilePosition + 1];
		if (NextChar == '0')
		{
			FilePosition++;
			continue;

		}
		else if (SelfDelimiters.find(NextChar) != string::npos || isspace(NextChar))
		{
			FilePosition++;
			break;
		}
		else {
			//If any other character (NOT ZERO) is recognized, blow up.
			if (Digits.find(NextChar) != string::npos)
			{
				string ErrorMessage = "ERROR: Integers can't have leading zeros. - " + to_string(FilePosition + 1);
				throw runtime_error(ErrorMessage);

			}
			string ErrorMessage = "ERROR: Unexpected character while scanning Integer. at pos - " + to_string(FilePosition + 1) + " char= " + NextChar;
			throw runtime_error(ErrorMessage);
		}
	}

	return Token(INTEGER, "0");
}

Token Scanner::consumeGenericWordToken()
{
	string Accumulator = string(1, FileContents[FilePosition]);

	//Set initial endState To False
	bool ValidEndState = false;
	while (!ValidEndState && FilePosition < FileContents.size())
	{
		if (FilePosition + 1 == FileContents.size())
		{
			//Last character in file. Update accumulator, and set End State To Valid.
			FilePosition++;
			ValidEndState = true;
			continue;
		}

		char NextChar = FileContents[FilePosition + 1];

		//If not the last character in file, check that next char.
		if (isalpha(NextChar) || NextChar == '_' || Digits.find(NextChar) != string::npos)
		{
			//If next character a digit, add to accumulator and keep going.
			Accumulator = Accumulator + NextChar;
			FilePosition++;
			continue;
		}
		else if (SelfDelimiters.find(NextChar) != string::npos || isspace(NextChar))
		{
			//If next character is a self delimiter, or a space, we're in a stop state.
			ValidEndState = true;
			FilePosition++;
			continue;
		}
		else
		{
			//If any other character is recognized, blow up.
			string ErrorMessage = "ERROR: Unexpected character while scanning Identifier at pos - " + to_string(FilePosition + 1) + " char= " + NextChar;
			throw runtime_error(ErrorMessage);
		}
	}

	return Token(IDENTIFIER, Accumulator);
}

bool Scanner::isCommentStart()
{
	return FilePosition + 1 < FileContents.size() && FileContents[FilePosition + 1] == '*';
}

void Scanner::ignoreComment()
{
	int InitialCommentPosition = FilePosition;
	FilePosition = FilePosition + 2;

	while (FilePosition < FileContents.size())
	{
		skipPastWhiteSpace();

		if (FileContents[FilePosition] = '*') {
			if (FilePosition + 1 < FileContents.size()
				&& FileContents[FilePosition + 1] == ')')
			{
				//Valid comment end state. Return.
				FilePosition = FilePosition + 2;
				return;
			}
		}

		FilePosition++;
	}

	ostringstream ErrorMessageStream;
	ErrorMessageStream << "ERROR: You forgot to close you're comment that started at pos. " << InitialCommentPosition;
	throw  std::runtime_error(ErrorMessageStream.str());
}

