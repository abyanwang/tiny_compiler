#ifndef TINY_COMPILE_LOG_H
#define TINY_COMPILE_LOG_H

#include <iostream>
#include <string>

class Log {
    std::ostream& out;
    int error_count = 0;
public:
    inline explicit Log(std::ostream& output_stream) : out(output_stream) {}
    inline int getErrorCount() {
        return error_count;
    }
    inline bool hasError() {
        return error_count != 0;
    }

    inline void error(const std::string &info, int line, int offset) {
        out << "ERROR IN LINE " << line << ":" << offset << " " << info << std::endl;
        error_count ++;
    }

    inline void parse_error(const std::string &info, int line, int offset) {
        out << "SYNTAX ERROR IN LINE " << line << ":" << offset << " " << info << std::endl;
        error_count ++;
    }

    inline void type_error(const std::string &info, int line, int offset) {
        out << "TYPE ERROR IN LINE " << line << ":" << offset << " " << info << std::endl;
        error_count ++;
    }
};

#endif //TINY_COMPILE_LOG_H
