#ifndef XETHRU_EXAMPLE_UTIL_HPP
#define XETHRU_EXAMPLE_UTIL_HPP

#include <iostream>
#include <ctime>
#include <iomanip>
#include <string>

struct pretty_time
{
    pretty_time() {}
    friend std::ostream& operator<<(std::ostream& os, const pretty_time& mp)
    {
        time_t rawtime;
        struct tm * timeinfo;
        char buffer[80];

        time (&rawtime);
        timeinfo = localtime(&rawtime);

        strftime(buffer,80,"%d-%m-%Y %I:%M:%S",timeinfo);
        std::string str(buffer);

        os << str;
        return os;
    }
};

#endif

