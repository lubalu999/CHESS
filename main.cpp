#include <iostream>
#include "Game.h"
#include "Interface.h"
#include "Dialog.h"
#include "Vector.h"

using namespace std;

int main()
{
	try {
		Dialog();
	}
	catch (...)
	{
		std::cerr;
	}

	return 0;
}
