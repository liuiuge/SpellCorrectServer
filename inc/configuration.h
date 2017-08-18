#pragma once
#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__
#include <map>
using std::map;
#include <string>
using std::string;
#include <fstream>
using std::ifstream;
#include <sstream>
using std::istringstream;
namespace wd{
class configuration
{
    string _filepath;
    map<string,string> _configMap;
public:
    configuration(const string & filepath)
        :_filepath(filepath)
    {}
    map<string,string> & getconfigmap()
    {
        ifstream ifs(_filepath);
        string line;
        while(getline(ifs, line))
        {
            istringstream iss(line);
            string column,value;
            iss>>column>>value;
            _configMap.insert(std::pair<string, string>(column, value));
        }
        ifs.close();
        return _configMap;
    }
};
}
#endif
