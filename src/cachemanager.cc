#include "configuration.h"
#include "cachemanager.h"
#include "stdlib.h"
#include "stdio.h"
#include <iostream>

namespace wd
{
qcache cachemanager::_qcache;
map<pthread_t, qcache> cachemanager::_qcaches;
void cachemanager::create_cache(int idx,const string & filename)
{
    qcache cache;
    cache.read_from_file(filename);
    for(int i = 0;i != idx ; ++i){
    	_qcaches.insert(std::make_pair(i,cache));
    }
    printf("create cache\n");}
qcache & cachemanager::get_cache(int id)
{
    auto it  = _qcaches.find(id);
    printf("get cache\n");
    return it->second;  
}
void cachemanager::updateCache()
{
	for(auto & it:_qcaches)
   	{
       	for(auto & hashit : it.second._hashmap)
    	{
  		    _qcache._hashmap.insert(hashit);
    	}
	}
	for(auto & it : _qcache._hashmap)
	{
		std::cout<<it.first<<" "<<it.second<<"\n";
	}
   	for(auto & it:_qcaches)
	{
   		it.second = _qcache;    		
	}
    configuration _conf("conf.txt");
    _qcache.write_to_file(_conf.getconfigmap()["cache"]);
}    
}
