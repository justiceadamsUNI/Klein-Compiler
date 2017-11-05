#include "../header/CodeGenerator.h"
#include <iostream>
#include <fstream>

using namespace std;

void CodeGenerator::writeOutTargetCode()
{
	writeInstructionsToFile();
}

void CodeGenerator::writeInstructionsToFile()
{
	ofstream TMFile;
	TMFile.open("TmFile.tm"); // change file name

	// Just prints "1" to the screen right now.
	TMFile << "0:  LDC  0, 1(0)" << endl;
	TMFile << "1:  OUT  0, 0, 0" << endl;
	TMFile << "2:  HALT  0, 0, 0" << endl;
	TMFile.close();
}