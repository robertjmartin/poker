
#include <iostream>
#include "TestLogger.h"

using namespace std;

void TestLogger::AttachConsole()
{
	_consoleenabled = true;
}

void TestLogger::Log(int level, char* msg)
{
	if( _consoleenabled )
	{
		cout << msg << endl;
	}
}