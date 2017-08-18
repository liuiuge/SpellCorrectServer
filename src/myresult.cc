

#include "myresult.h"
namespace wd{

bool operator <(const wd::Myresult & lhs, const wd::Myresult & rhs)
{
    if(lhs._idist != rhs._idist)
        return lhs._idist < rhs._idist;
    if(lhs._ifreq != rhs._ifreq)
        return lhs._ifreq < rhs._ifreq;
    return lhs._word > rhs._word;
}

bool operator >(const wd::Myresult & lhs, const wd::Myresult & rhs)
{
    if(lhs._idist != rhs._idist)
        return lhs._idist > rhs._idist;
    if(lhs._ifreq != rhs._ifreq)
        return lhs._ifreq > rhs._ifreq;
    return lhs._word < rhs._word;
}

}