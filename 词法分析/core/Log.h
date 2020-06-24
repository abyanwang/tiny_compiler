#ifndef TINY_COMPILE_LOG_H
#define TINY_COMPILE_LOG_H

#include <iostream>
#include <string>
using namespace std;

class Log {
	private: 
	    ostream& out;
	    int error_count = 0;
	public:
	    Log(ostream& output_stream) : out(output_stream) {}
	    int getErrorCount() {
	        return error_count;
	    }
	    bool hasError() {
	        return error_count != 0;
	    }
	
	    void error(const string &info, int line, int offset) {
	        out << "ERROR IN LINE " << line << ":" << offset << " " << info << endl;
	        error_count ++;
	    }
	
	    void parse_error(const string &info, int line, int offset) {
	        out << "SYNTAX ERROR IN LINE " << line << ":" << offset << " " << info << endl;
	        error_count ++;
	    }
	
	    void type_error(const std::string &info, int line, int offset) {
	        out << "TYPE ERROR IN LINE " << line << ":" << offset << " " << info << endl;
	        error_count ++;
	    }
};

#endif //TINY_COMPILE_LOG_H
