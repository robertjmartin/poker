#include<list>
#include<string>
#include "TestLogger.h"
typedef int(*TESTMETHOD)(void);

using namespace std;

static TestLogger Logger;

class TestCase
{
public:
	TestCase(string name, TESTMETHOD method);

	string TestName;
	TESTMETHOD TestMethod;
};




