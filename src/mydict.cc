#include "mydict.h"
#include <fstream>
#include <sstream>
using std::istringstream;
using std::ifstream;
using std::cout;

namespace wd
{
mydict * mydict::_mydict = NULL;
mydict * mydict::createInstance()
{
    if(_mydict == NULL)
        _mydict = new mydict;
    return _mydict;
}
void mydict::read_from_file(const char * filename)
{
    ifstream ifs(filename);
      string line;
      while(getline(ifs, line))
      {
          istringstream iss(line);
          string word;
          int freq;
          iss>>word>>freq;
          _dict.push_back(pair<string, int>(word,freq));
      }
      ifs.close();
}
void mydict::init_dict(const char * dictenpath, const char * dictcnpath)
{
	read_from_file(dictenpath);
    read_from_file(dictcnpath);
    for(size_t idx = 0; idx != _dict.size(); ++idx)
	{
		record_to_index(idx);
	}
    cout<<"init done\n";
}
void mydict::record_to_index(int idx)
{
    string key;
    string word = _dict[idx].first;
    for(size_t iidx = 0; iidx != word.size(); )
    {
        char ch = word[iidx];
        if(ch & (1<<7))
        {
            size_t cnt = 1;
            for(size_t iiidx = 0; iiidx<6;++iiidx)
            {
                if(ch & (1<<(6-iiidx))){
                    ++cnt;
                }else{
                    break;
                }//ch & (1<<(6-iiidx)
            }//for iiidx
            key = word.substr(iidx,cnt);
            iidx += cnt;
        }else{
            key = word.substr(iidx, 1);
            ++iidx;
        }//if ch & (1<<7)
        _index_table[key].insert(idx);
    }//for iidx
}
vector<pair<string,int>> & mydict::get_dict()
{
    return _dict;
}
map<string,set<int>> & mydict::get_index_table()
{
    return _index_table;
}
};

#ifdef HELLO
    static mydict * _mydict;
    vector<pair<string, int>> _dict;
    map<string, set<int>> _index_table;
    void init_dict(const char * dictenpath, char * dictcnpath);
    static mydict * createInstance();
    vector<pair<string, int>> & get_dict();
    map<string, set<int>> & get_index_table();
};
#endif


