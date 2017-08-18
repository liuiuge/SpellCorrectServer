#pragma once
#ifndef __MYTASK_H__
#define __MYTASK_H__
#include <queue>
using std::priority_queue;
#include "mydict.h"
#include "myresult.h"
#include "cachemanager.h"
#include "tcpconnection.h"
#include <vector>
using std::vector;
#include <set>
using std::set;
namespace wd{
class mytask
{
    string _queryword;
    wd::tcpconnectionptr _conn;
    priority_queue<struct wd::Myresult,vector<struct wd::Myresult>> _resultque;
public:
    mytask(const string & query, const tcpconnectionptr &);
    void execute();
    void query_index_table();
    void statistic(set<int> & iset);
	int ldistance(const string & sleft, const string & sright);
    void responce();
};
}
#endif
