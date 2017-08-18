

//Author:liuiuge(1220595883@qq.com

#include "spellcorrectserver.h"
#include "cachemanager.h"

int main()
{
    wd::spellcorrectserver scserver("conf.txt");
    wd::configuration cf1("conf.txt");
    string filename = cf1.getconfigmap()["cache"];
    wd::cachemanager::create_cache(4,filename);
    scserver.start();
    return 0;
}
