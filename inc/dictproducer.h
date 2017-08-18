#pragma once
#ifndef __DICTPRODUCER_H__
#define __DICTPRODUCER_H__

#include "configuration.h"
#include <vector>
using std::vector;
#include <map>
using std::map;
namespace wd{
class dict_producer
{
    string _dir;
    vector<string> _files;
    map<string,int> _dict;
public:
    dict_producer(const string & dir);
    dict_producer(const string & filename, int num);
    void build_dict();
    void build_cn_dict();
    void store_dict(const char * filepath);
    void show_files()const;
    void show_dict()const;
    void get_files();
    void push_dict(const string & word);
};
}
#endif
