#pragma once
#ifndef __MYDICT_H__
#define __MYDICT_H__
#include <vector>
using std::vector;
#include <set>
using std::set;
#include <map>
using std::map;
#include <iostream>
using std::string;
using std::pair;
namespace wd{
class mydict
{
    static mydict * _mydict;
    vector<pair<string, int>> _dict;
    map<string, set<int>> _index_table;
public:
    void init_dict(const char * dictenpath, const char * dictcnpath);
    void read_from_file(const char * filename);
    void record_to_index(int idx);
    static mydict * createInstance();
    vector<pair<string, int>> & get_dict();
    map<string, set<int>> & get_index_table();
};
}
#endif


