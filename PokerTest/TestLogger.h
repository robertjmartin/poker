
#ifndef _TESTLOGGER_H_
#define _TESTLOGGER_H_

class TestLogger
{
public:
	void AttachConsole();
	
	void Log( int level, char* msg);

private:
	bool _consoleenabled;

};

#endif