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

