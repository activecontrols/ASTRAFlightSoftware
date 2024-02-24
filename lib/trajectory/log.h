//
// Created by Ishan Goel on 2/23/24.
//

#ifndef LOG_H
#define LOG_H

namespace logger {
    typedef struct {
        char c;
    } Data;

    extern int FILE_WRITE_ERR, FILE_OPEN_ERR;

    extern int write_count;

    int open(const char* filename);
    void write(Data* data);
    void close();
} // log

#endif //LOG_H
