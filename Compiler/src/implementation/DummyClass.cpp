#include "../header/DummyClass.h"
#include <stdexcept>

DummyClass::DummyClass(int value)
{
	if (value == 0)
	{
		throw std::invalid_argument("This Dummy Class is going to explode!");
	}
}
