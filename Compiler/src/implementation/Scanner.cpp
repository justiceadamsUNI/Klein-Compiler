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
		else if (Digits.find(CharAtPosition) != string::npos)
		{
			return consumeIntegerToken();
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
		throw std::invalid_argument("The File Cannot Be Found! - " + FilePath);
	}

	FileData.assign((std::istreambuf_iterator<char>(inputFile)),
		(std::istreambuf_iterator<char>()));

	return FileData;
}

Token Scanner::consumeParenthesisToken(char ParenChar)
{
	FilePosition++;
	stringstream StringStream;
	string ParenString;

	StringStream << ParenChar;
	StringStream >> ParenString;
	return Token(PARENTHESIS, ParenString);
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
	stringstream StringStream;
	string OperatorString;
	
	StringStream << OperatorChar;
	StringStream >> OperatorString;

	return Token(ARITHMETIC_OPERATOR, OperatorString);
}

Token Scanner::consumeComparatorToken(char ComparatorChar)
{
	FilePosition++;
	stringstream StringStream;
	string ComparatorString;

	StringStream << ComparatorChar;
	StringStream >> ComparatorString;
	return Token(COMPARATOR, ComparatorString);
}

Token Scanner::consumeIntegerToken()
{
	stringstream StringStream;
	string Accumulator;

	StringStream << FileContents[FilePosition];
	StringStream >> Accumulator;


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
		}
		else if (SelfDelimiters.find(NextChar != string::npos) || isspace(NextChar))
		{
			//If next character is a self delimiter, or a space, we're in a stop state.
			ValidEndState = true;
			FilePosition++;
		}
		else
		{
			//If any other character is recognized, blow up.
			ostringstream ErrorMessageStream;
			ErrorMessageStream << "ERROR: Unexpected character while scanning Iteger literal at pos -  " << FilePosition + 1 << " char= " << NextChar;
			throw  std::runtime_error(ErrorMessageStream.str());
		}
	}

	return Token(INTEGER, Accumulator);
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

