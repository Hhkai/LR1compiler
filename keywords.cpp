#include "keywords.h"
#include <iostream>
#include <fstream>

std::map<std::string, int> kwords::str2int;
std::map<int, std::string> kwords::int2str;

void kwords::init_keywords(std::string filename)
{
    std::ifstream read_file;
    read_file.open(filename);
    
    char buffer[256];
    int i = 20;
    while (!read_file.eof()) {
        read_file.getline(buffer, 256);
        if (buffer[0] == '/' && buffer[1] == '/') continue;
        if (buffer[0] == 0 || buffer[0] == ' ' || buffer[0] == '\n') continue;
        kwords::str2int[buffer] = i;
        kwords::int2str[i] = std::string(buffer);
        i++;
    }
}

std::string kwords::getStr(int x)
{
    std::map<int, std::string>::iterator itr = kwords::int2str.find(x);

    if (itr != kwords::int2str.end()) {
        return itr->second;
    }
    std::cout << "find key words error:" << x << std::endl;
    return "error";
}

int kwords::getInt(std::string s)
{
    std::map<std::string, int>::iterator itr = kwords::str2int.find(s);

    if (itr != kwords::str2int.end()) {
        return itr->second;
    }
    return kwords::getInt("id");
}