#include "mytask.h"
#include "thread.h"
#include <stdlib.h>
#include <string.h>
#define triple_min(a,b,c) a<b?(a<c?a:c):(b<c?b:c)
size_t nBytesCode(const char ch)
{
	if(ch & (1 << 7))
	{
		int nBytes = 1;
		for(int idx = 0; idx != 6; ++idx)
		{
			if(ch & (1 << (6 - idx)))
			{
				++nBytes;
			}else
				break;
		}
		return nBytes;
	}
	return 1;
}
size_t strlength(const std::string & str)
{
    size_t ilen = 0;
    for(size_t idx = 0;idx != str.size();)
    {
        int nBytes = nBytesCode(str[idx]);
        idx += nBytes;
        ++ilen;
    }
    return ilen;
}
namespace wd{
mytask::mytask(const string & query, const tcpconnectionptr & conn)
    :_queryword(query),
    _conn(conn)
{}
void mytask::execute()
{
    qcache cache = cachemanager::get_cache(wd::str2int(wd::current_thread::threadName));
    if(_queryword[_queryword.size()-1] == '\n')
        _queryword[_queryword.size()-1] = 0;
    std::string s1 = cache.query(_queryword);
    if(s1 != string()){
        printf("result: %s\n",s1.c_str());
        Myresult m1;
        m1._word = s1;
        m1._idist = m1._ifreq = 0;
	_resultque.push(m1);
    }
    else{
        printf("result: %s\n",s1.c_str());
        query_index_table();
    }
    responce();
}
void mytask::query_index_table()
{
    size_t idx = 0;
    wd::mydict *md1 = mydict::createInstance();
    map<string,set<int>> indexmap =md1->get_index_table();
    string s1;
    for(;idx<_queryword.size();)
    {
        size_t nBytes = nBytesCode(_queryword[idx]);
        s1 = _queryword.substr(idx, nBytes);
        idx += nBytes;
        if(indexmap.count(s1)){
            std::cout<<"searching index "<<s1<<std::endl;
            statistic(indexmap[s1]); 
        }
    }
}
void mytask::statistic(set<int> & iset)
{
    wd::mydict *md1 = mydict::createInstance();
    auto v1 = md1->get_dict();
    struct wd::Myresult mr1;
    for(auto & it:iset)
    {
        mr1._word = v1[it].first;
        mr1._ifreq = v1[it].second;
        mr1._idist = 100 - ldistance(mr1._word, _queryword);
        _resultque.push(mr1);
    }
}
int mytask::ldistance(const string & lhs, const string & rhs)
{
    size_t lhs_len = strlength(lhs);
	size_t rhs_len = strlength(rhs);
	int editDist[lhs_len + 1][rhs_len + 1];
	for(size_t idx = 0; idx <= lhs_len; ++idx)
	{
		editDist[idx][0] = idx;
	}
	for(size_t idx = 0; idx <= rhs_len; ++idx)
	{
		editDist[0][idx] = idx;
	}
	std::string sublhs, subrhs;
	for(std::size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i, ++lhs_idx)
	{
		size_t nBytes = nBytesCode(lhs[lhs_idx]);
		sublhs = lhs.substr(lhs_idx, nBytes);
		lhs_idx += (nBytes - 1);
		for(std::size_t dist_j = 1, rhs_idx = 0; dist_j <= rhs_len; ++dist_j, ++rhs_idx)
		{
			nBytes = nBytesCode(rhs[rhs_idx]);
			subrhs = rhs.substr(rhs_idx, nBytes);
			rhs_idx += (nBytes - 1);
			if(sublhs == subrhs)
			{
				editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j - 1];
			}
			else
			{
				editDist[dist_i][dist_j] = triple_min(
				editDist[dist_i][dist_j - 1] + 1,
				editDist[dist_i - 1][dist_j] + 1,
				editDist[dist_i - 1][dist_j - 1] + 1);
			}
		}
	}
	return editDist[lhs_len][rhs_len];
}

void mytask::responce(){
    string s1= _resultque.top()._word;
    char result[128]= {0};
    char c = ' ';
    sprintf(result, "%s%c%d%c%d"
            ,_resultque.top()._word.c_str(), c
            ,_resultque.top()._ifreq, c
            ,100-_resultque.top()._idist);
    string msg = result;
    printf("%s\n",result);
    _conn->sendinLoop(msg);
    qcache & cache = cachemanager::get_cache(wd::str2int(wd::current_thread::threadName));
	cache.add_element(_queryword, s1);
    std::cout << "> respone(): add Cache" << "\n";
}

}
#ifdef HELLO
    string _queryword;
    wd::tcpconnectionptr _conn;
    priority_queue<struct Myresult,vector<struct Myresult>,std::less<Myresult>> _resultque;
public:
    mytask(const string & query, const tcpconnectionptr &);
    void execute();
private:
    void query_index_table();
    void ldistance();
    void statistic(set<int> & iset);

};

#endif
