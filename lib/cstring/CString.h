#ifndef CSTRING_H
#define CSTRING_H

#include <Arduino.h>

/*
 * CString.h
 *
 * Created on: 2024-08-10 by Vincent Palmerio
 * Maintained by Vincent Palmerio
 * Description: This file defines the CString and CStringPtr classes, as well as the cstring 
 * namespace for helper functions. Its goal is to make it super easy to concatenate strings and
 * doubles with char arrays. This is much better than using Arduino's String class, which is known to cause 
 * heap fragmentation after extended use. 
 */

namespace cstring {
    inline void print(const char* str) {
        Serial.println(str);
    }

    int append(char* str, size_t bufferLen, const char* src);

    int append(char* str, size_t bufferLen, double value, int precision);

    void trim(char* str);

    inline bool equals(const char *str1, const char *str2) {
        return strcmp(str1, str2) == 0;
    }

}

template <size_t N>
class CString {
public:
    char str[N];
    int precision;
    int leftover;

    //Constructor to initialize the CString with an empty string and default precision
    CString() : precision(2) {
        clear();
    }

    //Set the precision for adding double values to the string
    void setPrecision(int p) {
        precision = p;
    }

    void clear() {
        str[0] = '\0';
    }

    //Overload the << operator to concatenate a char *
    CString& operator<<(const char* src) {
        leftover = cstring::append(str, N, src);
        return *this;
    }

    //Overload the << operator to concatenate a double with specific precision (given by `precision` attribute)
    CString& operator<<(double value) {
        leftover = cstring::append(str, N, value, precision);
        return *this;
    }

    //trims leading and trialing whitespace and new lines, and null terminates the end of the trimmed string
    void trim() {
        cstring::trim(str);
    }

    bool equals(const char *src) {
        return cstring::equals(str, src);
    }

    void print() const {
        cstring::print(str);
    }
};

//Non-template class for CString that takes a pointer to a CString
class CStringPtr {
public:

    //The buffer passed in to the constructor
    char* str;

    //size of the passed in char buffer
    size_t size;

    //Precision for double values when concatenated to string
    int precision;

    int leftover;

    //Constructor to initialize the CString and size
    CStringPtr(char* s, size_t n) : str(s), size(n), precision(2) {
        clear();
    }

    //Method to set the precision for double values
    void setPrecision(int p) {
        precision = p;
    }

    //Method to clear the C-string
    void clear() {
        str[0] = '\0';
    }

    //Overload the << operator to concatenate another C-string
    CStringPtr& operator<<(const char* src) {
        leftover = cstring::append(str, size, src);
        return *this;
    }

    //Overload the << operator to concatenate a double with specific precision (precision changed by `setPrecision()`)
    CStringPtr& operator<<(double value)  {
        leftover = cstring::append(str, size, value, precision);
        return *this;
    }

    //trims leading and trialing whitespace and new lines
    void trim() {
        cstring::trim(str);
    }

    bool equals(const char *src) {
        return cstring::equals(str, src);
    }

    void print() const {
        cstring::print(str);
    }
};


#endif //CSTRING_H