#include "../header/Scanner.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>

using namespace std;

Scanner::Scanner(string FilePath)
{	
	FileContents = readFile(FilePath);
	FilePosition = 0;
}

Scanner::Scanner(string TestFileContents, bool Testing)
{
	//ONLY USE THIS CONSTRUCTOR FOR TESTING PURPOSES.
	FileContents = TestFileContents;
	FilePosition = 0;
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
	while (FilePosition < FileContents.size())
	{
		if (skipPastWhiteSpace())
		{
			continue;
		}

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
		else {
			string ErrorMessage = "ERROR: Unsupported character found while scanning for tokens at pos - " + to_string(FilePosition) + " char= " + CharAtPosition;
			throw runtime_error(ErrorMessage);
		}
	}

	//ONCE THE WORD STATE MACHINE IS COMPLETED IT NEEDS TO BE TESTED INSIDE SCANNERTEST.CPP. DO.NOT.FORGET.
	return Token(END_OF_FILE, "");
}

bool Scanner::skipPastWhiteSpace()
{
	int InitialFilePosition = FilePosition;
	while (FilePosition < FileContents.size() && isspace(FileContents[FilePosition]))
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

	FileData.assign((std::istreambuf_iterator<char>(inputFile)),
		(std::istreambuf_iterator<char>()));

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
		//Also check to make sure the integer is within valid range.
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

	//If the Integer is out of bounds, tell them the same. Must Use Long Long Int. Regular Int isn't big enough with C++
	 if (stoll(Accumulator) > pow(2,32)) {
		string ErrorMessage = "ERROR: Value of integer entered is out of bounds - " + Accumulator;
		throw runtime_error(ErrorMessage);
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


		//If not the last character in file, check the next char.
		//Also keep making sure that Accumulator doesn't exceed more than 256 characters.


		if ((isalpha(NextChar) || NextChar == '_' || isdigit(NextChar)) && Accumulator.length() < 257) {
			//If Next character a digit or an underscore and is of valid length, add to accumulator and keep going
			Accumulator += NextChar;
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

	//If the identifier is longer than 256 valid characters, tell them they can't
	if (Accumulator.length() > 256) {
		string ErrorMessage = "ERROR: Length of Identifier is toooo long at pos - " + to_string(FilePosition);
		throw runtime_error(ErrorMessage);
	}
	else {
			if (Accumulator == "function" || Accumulator == "main" || Accumulator == "print") {
				FilePosition = FilePosition++;
				return Token(PRIMITIVE_KEYWORD, Accumulator);
			}
			//Check Accumulator to see if it is a Logical Operator
			else if (Accumulator == "and" || Accumulator == "or" || Accumulator == "not") {
				FilePosition = FilePosition++;
				return Token(LOGICIAL_OPERATOR, Accumulator);
			}
			//Check Accumulator to see if it is a Integer
			else if (Accumulator == "integer" || Accumulator == "boolean") {
				FilePosition = FilePosition++;
				return Token(DATA_TYPE, Accumulator);
			}
			//Check Accumulator to see if it is a Boolean
			else if (Accumulator == "true" || Accumulator == "false") {
				FilePosition = FilePosition++;
				return Token(BOOLEAN, Accumulator);
			}
			else if (Accumulator == "if" || Accumulator == "than" || Accumulator == "else") {
				FilePosition = FilePosition++;
				return Token(CONDITIONAL, Accumulator);
			}
			else {
				return Token(IDENTIFIER, Accumulator);
			}

		}
		
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
		if (skipPastWhiteSpace())
		{
			continue;
		};

		if (FileContents[FilePosition] == '*') {
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