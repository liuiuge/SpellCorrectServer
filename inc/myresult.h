#pragma once
#ifndef __MYRESULT_H__
#define __MYRESULT_H__

#include <iostream>
using std::string;
namespace wd
{
struct Myresult
{
    string _word;
    int _ifreq;
    int _idist;
    friend bool operator <(const Myresult &, const Myresult &);
    friend bool operator >(const Myresult &, const Myresult &);
};
}
#endif
