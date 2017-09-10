#include "../header/Scanner.h"
#include <fstream>
#include <iostream>
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

	FileContents.assign((std::istreambuf_iterator<char>(inputFile)),
		std::istreambuf_iterator<char>());
}
