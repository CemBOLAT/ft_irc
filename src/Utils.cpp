#include "../includes/Utils.hpp"
#include <iostream>
#include <cstring>
#include <string>

vector<string> Utils::split(string &str, char delim)
{
    vector<string> res;
    string  tmp;
    size_t  i = 0;
    size_t  size = str.size();
    for (size = str.size(); size > 0 && (str[size - 1] == '\r' || str[size - 1] == '\n'); size--);
    for (; i < size && str[i] == delim; i++);
    for (; i < size; i++)
    {
        if (str[i] == delim)
        {
            res.push_back(tmp);
            tmp.clear();
            for (; i < size && str[i] == delim; i++);
        }
        if (i < size)
            tmp += str[i];
    }
    if (!tmp.empty())
        res.push_back(tmp);
    return (res);
}

//int main ()
//{
//    string str = "PASS 555 ";
//    vector<string> res = Utils::split(str, ' ');
//    for (size_t i = 0; i < res.size(); i++)
//    {
//        cout << res[i] << endl;
//    }
//    return (0);
//}