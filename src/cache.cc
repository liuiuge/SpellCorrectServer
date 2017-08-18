#include "cache.h"
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>
using std::cout;

namespace wd{
qcache::qcache(int num)
{
    _hashmap.reserve(num);
}
qcache::qcache(const qcache & cache)
    :_hashmap(cache._hashmap)
{}
void qcache::add_element(const string & key,const string & value)
{
    _hashmap.insert({key,value});
}
void qcache::read_from_file(const string & filename)
{
    std::ifstream ifs(filename);
    string line;
    while(getline(ifs,line))
    {
        std::istringstream iss(line);
        string key,value;
        iss>>key>>value;
        add_element(key,value);
    }
}
bool qcache::full()
{
    return _hashmap.size() == 10;
}
void qcache::write_to_file(const string & filename)
{
    std::ofstream ofs(filename);//c++98: void open(const char *, ios_base::openmode mode = ios_base::in);
				//c++11: void open (const string &, ios_base::openmode mode = ios_base::in);
    for(auto & it: _hashmap)
    {
        ofs<<it.first<<" "<<it.second<<"\n";
    }
    ofs.close();//不关闭会无法保存
}
string qcache::query(const string & qword)
{
    for(auto & its : _hashmap){
	if(!strcmp(its.first.c_str(),qword.c_str()))
	    return its.second;
    }
    return string();
}
void qcache::update(const  qcache & rhs)
{
    _hashmap.clear();
    _hashmap = rhs._hashmap;
}
}

#ifdef HELLO
#include <unordered_map>
using std::unordered_map;
#include <string>
using std::string;
namespace wd{
class qcache
{
    unordered_map<string,string> _hashmap;
public:
    qcache(int num = 10);
    qcache(const qcache & cache);
    void add_element(const string & key, const & value);
    void read_from_file(const string & filename);
};
}
#endif
