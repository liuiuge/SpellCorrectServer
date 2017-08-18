#pragma once
#ifndef __CACHEMANAGER_H__
#define __CACHEMANAGER_H__

#include "cache.h"
#include <pthread.h>
#include <map>
using std::map;
namespace wd
{
class cachemanager
{
    static qcache _qcache;
    static map<pthread_t,qcache> _qcaches;
public:
    static void create_cache(int,const string &);
    static qcache & get_cache(int);
    static void updateCache();
};
}

#endif
