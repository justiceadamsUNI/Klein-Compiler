#pragma once
#include "../header/Scanner.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>

#define CHAR_NOT_FOUND string::npos

using namespace std;

Scanner::Scanner(string FilePath)
{	
	FileContents = readFile(FilePath);
	FilePosition = 0;
	FileSize = FileContents.size();
}

Scanner::Scanner(string TestFileContents, bool Testing)
{
	//ONLY USE THIS CONSTRUCTOR FOR TESTING PURPOSES.
	FileContents = TestFileContents;
	FilePosition = 0;
	FileSize = FileContents.size();
}

Token Scanner::peek()
{
	int tempFilePosition = FilePosition;
	Token PeekedToken = next();
	FilePosition = tempFilePosition;

	return PeekedToken;
}

Token Scanner::next()
{
	while (FilePosition < FileSize)
	{
		if (skipPastWhiteSpace())
		{
			continue;
		}

		char CharAtPosition = FileContents[FilePosition];

		switch (CharAtPosition)
		{
		case ',':
			return consumeCommaToken();
		case ':':
			return consumeColonToken();
		case '+':
			return consumeOperatorToken(CharAtPosition);
		case '*':
			return consumeOperatorToken(CharAtPosition);
		case '/':
			return consumeOperatorToken(CharAtPosition);
		case '-':
			return consumeOperatorToken(CharAtPosition);
		case '(':
			if (isCommentStart())
			{
				ignoreComment();
				continue;
			}
			else {
				return consumeParenthesisToken(CharAtPosition);
			}
		case ')':
			return consumeParenthesisToken(CharAtPosition);
		case '<':
			return consumeComparatorToken(CharAtPosition);
		case '=':
			return consumeComparatorToken(CharAtPosition);
		default:
			break;
		}

		// Handle More Complex Tokens
		if (isdigit(CharAtPosition))
		{
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
		else {
			string ErrorMessage = "ERROR: Unsupported character found while scanning for tokens at pos - " + to_string(FilePosition) + " char= " + CharAtPosition;
			throw runtime_error(ErrorMessage);
		}
	}

	return Token(END_OF_FILE, "");
}

bool Scanner::skipPastWhiteSpace()
{
	int InitialFilePosition = FilePosition;
	while (FilePosition < FileSize && isspace(FileContents[FilePosition]))
	{
		FilePosition++;
	}

	//Returns true if whitespace was skipped.
	return InitialFilePosition != FilePosition;
}

string Scanner::readFile(string FilePath)
{
	if (!isValidKleinFile(FilePath))
	{
		throw invalid_argument("The File Must be a .kln File! - " + FilePath);
	}

	string FileData;
	fstream inputFile;
	inputFile.open(FilePath.c_str());

	// Check for successful opening
	if (inputFile.fail())
	{
		throw invalid_argument("The File Cannot Be Found! - " + FilePath);
	}

	FileData.assign((istreambuf_iterator<char>(inputFile)),
		(istreambuf_iterator<char>()));

	return FileData;
}

bool Scanner::isValidKleinFile(string FilePath)
{
	//Int 4 used here since .kln is 4 characters.
	return FilePath.size() > 4 && FilePath.compare(FilePath.size() - 4, 4, ".kln") == 0;
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
	while (!ValidEndState && FilePosition < FileSize)
	{
		if (FilePosition + 1 == FileSize)
		{
			//Last character in file. Update pointer, and set End State To Valid.
			FilePosition++;
			ValidEndState = true;
			continue;
		}

		char NextChar = FileContents[FilePosition + 1];

		//If not the last character in file, check that next char.
		if (isdigit(NextChar)) 
		{
			//If next character a digit, add to accumulator and keep going.
			Accumulator += NextChar;
			FilePosition++;
			continue;
		}
		else if (SelfDelimiters.find(NextChar) != CHAR_NOT_FOUND || isspace(NextChar))
		{
			//If next character is a self delimiter, or a space, we're in a stop state.
			ValidEndState = true;
			FilePosition++;
			continue;
		}
		else
		{
			//If any other character is recognized, blow up.
			string ErrorMessage = "ERROR: Unexpected character while scanning Integer literal at pos - " + to_string(FilePosition + 1) + " char = " + NextChar;
			throw runtime_error(ErrorMessage);
		}
	}

	//If the Integer is out of bounds, tell them. Must Use Long Long Int. Regular Int isn't big enough with C++
	 if (stoll(Accumulator) > pow(2,32)) {
		string ErrorMessage = "ERROR: Value of integer entered is out of bounds - " + Accumulator;
		throw runtime_error(ErrorMessage);
	}

	return Token(INTEGER, Accumulator);
}

Token Scanner::consumeZeroToken()
{
	
	while (FilePosition + 1  < FileSize)
	{
		char NextChar = FileContents[FilePosition + 1];
		if (NextChar == '0')
		{
			FilePosition++;
			continue;

		}
		else if (SelfDelimiters.find(NextChar) != CHAR_NOT_FOUND || isspace(NextChar))
		{
			FilePosition++;
			break;
		}
		//If any other character (NOT ZERO) is recognized, blow up.
		else if (isdigit(NextChar))
		{
			string ErrorMessage = "ERROR: Integers can't have leading zeros. - " + to_string(FilePosition + 1);
			throw runtime_error(ErrorMessage);
		}
		else
		{
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
	while (!ValidEndState && FilePosition < FileSize)
	{
		if (FilePosition + 1 == FileSize)
		{
			//Last character in file. Update pointer, and set End State To Valid.
			FilePosition++;
			ValidEndState = true;
			continue;
		}

		char NextChar = FileContents[FilePosition + 1];


		//If not the last character in file, check the next char.
		if ((isalpha(NextChar) || NextChar == '_' || isdigit(NextChar))) {
			//If Next character a digit or an underscore and is of valid length, add to accumulator and keep going
			Accumulator += NextChar;
			FilePosition++;
			continue;
		}
		else if (SelfDelimiters.find(NextChar) != CHAR_NOT_FOUND || isspace(NextChar))
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

	//If the identifier is longer than 256 valid characters, throw error
	if (Accumulator.length() > 256) {
		string ErrorMessage = "ERROR: Length of Identifier is too long at pos - " + to_string(FilePosition);
		throw runtime_error(ErrorMessage);
	}
	else if (GenericKeywordTypeMap.find(Accumulator) != GenericKeywordTypeMap.end()) {
		return Token(GenericKeywordTypeMap.find(Accumulator)->second, Accumulator);
	}
	else {
		return Token(IDENTIFIER, Accumulator);
	}
}

bool Scanner::isCommentStart()
{
	return FilePosition + 1 < FileSize && FileContents[FilePosition + 1] == '*';
}

void Scanner::ignoreComment()
{
	int InitialCommentPosition = FilePosition;
	FilePosition = FilePosition + 2;

	while (FilePosition < FileSize)
	{
		if (skipPastWhiteSpace())
		{
			continue;
		}

		if (FileContents[FilePosition] == '*') {
			if (FilePosition + 1 < FileSize
				&& FileContents[FilePosition + 1] == ')')
			{
				//Valid comment end state. Return.
				FilePosition = FilePosition + 2;
				return;
			}
		}

		FilePosition++;
	}

	string ErrorMessage ="ERROR: You forgot to close you're comment that started at pos. " + to_string(InitialCommentPosition);
	throw runtime_error(ErrorMessage);
}