
#include "dictproducer.h"
#include "cppjieba1/Jieba.hpp"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <iostream>
using std::endl;
using std::cout;

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <sstream>
using std::istringstream;
using std::ostringstream;

namespace wd
{
void punct_replace(string & line)
{
    char * buf = (char *)calloc(1,1024);
    strcpy(buf, line.c_str());
    for(size_t i = 0; i<strlen(buf); i++){
        if(!isalpha(buf[i]))
            buf[i] = ' ';
        if(!islower(buf[i]) && buf[i] != ' ')
            buf[i] += 32;
    }
    line = buf;
}
dict_producer::dict_producer(const string & dir):
    _dir(dir)
{}
dict_producer::dict_producer(const string & filename, int num)
    :_dir(filename)
{
    _files.reserve(num);
}
void dict_producer::build_dict()
{
	DIR *dir1;
	dir1 = opendir(_dir.c_str());
	struct dirent *pd;
	while((pd = readdir(dir1)) != NULL){
	    if(pd->d_name[0] == '.')  
            continue;
        char *path = (char *)calloc(1, 128);
        sprintf(path,"%s%s%s",_dir.c_str(),"/",pd->d_name);
        ifstream ifs(path);
        if(!ifs.good()){
            cout<<"err file\n";
            break;
        }
        _files.push_back(pd->d_name);
		string line;
		while(getline(ifs,line))
    	{
       		string word;
       		wd::punct_replace(line);
       		istringstream iss(line);
            while(iss>>word){
           		push_dict(word);
       		}
    	}
   		ifs.close();
	}
}
void dict_producer::build_cn_dict()
{
    ifstream ifs(_dir);
    if(!ifs.good()){
        cout<<"err file\n";
        return;
    }
    _files.push_back(_dir);	
    string line;
    while(getline(ifs,line))
    {
        const char* const DICT_PATH = "/home/lagrange/201707/spellcorrect/test1/test5/jiebadict/jieba.dict.utf8";
        const char* const HMM_PATH = "/home/lagrange/201707/spellcorrect/test1/test5/jiebadict/hmm_model.utf8";
        const char* const USER_DICT_PATH = "/home/lagrange/201707/spellcorrect/test1/test5/jiebadict/user.dict.utf8";
        const char* const IDF_PATH = "/home/lagrange/201707/spellcorrect/test1/test5/jiebadict/idf.utf8";
        const char* const STOP_WORD_PATH = "/home/lagrange/201707/spellcorrect/test1/test5/jiebadict/stop_words.utf8";
        cppjieba::Jieba jieba(DICT_PATH,
            HMM_PATH,
            USER_DICT_PATH,
            IDF_PATH,
            STOP_WORD_PATH);
        vector<string> words;
        vector<cppjieba::Word> jiebawords;
        jieba.Cut(line, words, true);
        ostringstream oss;
        oss<<limonp::Join(words.begin(), words.end(), " ")<<endl;
        istringstream iss(oss.str());
        string sword;
        while(iss>>sword)
        {
            cout<<sword<<endl;
	    push_dict(sword);
        }
    }

}
void dict_producer::show_files()const
{
    auto it = _files.begin();
    for(;it!= _files.end(); ++it)
        cout<<*it<<endl;
}
void dict_producer::show_dict() const
{
    auto it = _dict.begin();
    for(;it != _dict.end(); ++it)
        cout<<it->first<<" "<<it->second<<endl;
}
void dict_producer::get_files()
{
    
}
void dict_producer::store_dict(const char *filepath)
{
    ofstream ofs(filepath);
    auto it = _dict.begin();
    for(;it != _dict.end(); ++it)
        ofs<<it->first<<" "<<it->second<<endl;
    ofs.close();
}
void dict_producer::push_dict(const string & word)
{
    auto it = _dict.find(word);
    if(it != _dict.end())
        it->second++;
    else
        _dict.insert(std::pair<string,int>(word,1));
}

}//end of namespace
#ifdef HOLA
class dict_producer
{
    string _dir;
    vector<string> _files;
    map<string,int> _dict;
public:
//    dict_producer(const string & dir);
    dict_producer(const string & dir, splittool * splittool);
//    void build_dict();
    void build_cn_dict();
//    void store_dict(const char * filepath);
//    void show_files()const;
//    void show_dict()const;
//    void get_files();
//    void push_dict(const string & word);
};

#endif
