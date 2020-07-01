#ifndef TINY_COMPILE_LOG_H
#define TINY_COMPILE_LOG_H

#include <iostream>
#include <string>
using namespace std;

class Log {
	private: 
	    ostream& out;
	    int errorCount = 0;
	public:
	    Log(ostream& outputStream): out(outputStream){}
	    int getErrorCount() {
	        return errorCount;
	    }
	    bool hasError() {
	        return errorCount != 0;
	    }
	
	    void error(string info, int line, int offset) {
	        out << "ERROR IN LINE " << line << ":" << offset << " " << info << endl;
	        errorCount ++;
	    }
	
	    void parseError(string info, int line, int offset) {
	        out << "SYNTAX ERROR IN LINE " << line << ":" << offset << " " << info << endl;
	        errorCount ++;
	    }
	
	    void typeError(string info, int line, int offset) {
	        out << "TYPE ERROR IN LINE " << line << ":" << offset << " " << info << endl;
	        errorCount ++;
	    }
};

#endif //TINY_COMPILE_LOG_H
