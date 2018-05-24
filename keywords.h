#pragma once
#include <map>
#include <string>

namespace kwords
{
    extern std::map<std::string, int> str2int;
    extern std::map<int, std::string> int2str;
    void init_keywords(std::string);
    std::string getStr(int);
    int getInt(std::string);
}