#include "../header/Scanner.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

Scanner::Scanner()
{
	//no file to scan. Use this constructor for testing.
}

Scanner::Scanner(string FilePath)
{
	std::fstream inputFile;
	inputFile.open(FilePath);

    // Check for successful opening
    if(inputFile.fail())
    {
		throw std::invalid_argument("The File Cannot Be Found! - " + FilePath);
	}

	string word;
	while (inputFile >> word) {
		FileContents.push_back(word);
	}
}

list<Token> Scanner::getTokens()
{
	list<Token> Tokens;
	
	for (int wordIndex = 0; wordIndex < FileContents.size(); wordIndex++)
	{
		string word = FileContents.at(wordIndex);

		string accum = "";
		for (int i = 0; i < word.size(); i++)
		{
			//Split the word if needed and add back into the vector.
			char letter = word[i];
			
			if (letter == ',')
			{
				Token CommaToken = Token(TokenType::COMMA, "");
				Tokens.push_back(CommaToken);
				accum = "";
				i++;
				continue;
			}

			if (letter == '+')
			{
				//create plus token
				//create token for whatever is in accum (if it's not empty);

				//add accum.
				//add plus token.
				//i+2;

			}
			if (isdigit(i))
			{
				//add to accumulator
				//i++
				//continue
			}

		}
	}
}


