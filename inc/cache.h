#pragma once
#ifndef __CACHE_H__
#define __CACHE_H__

#include <unordered_map>
using std::unordered_map;
#include <string>
using std::string;
namespace wd{
class cachemanager;
class qcache
{
    unordered_map<string,string> _hashmap;
    bool full();
    friend class cachemanager;
public:
    qcache(int num = 10);
    qcache(const qcache & cache);
    void add_element(const string & key, const string & value);
    void read_from_file(const string & filename);
    void write_to_file(const string & filename);
    string query(const string & qword);
    void update(const qcache & rhs);
};
}
#endif
