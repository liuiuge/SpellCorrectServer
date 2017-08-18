

//Author:liuiuge(1220595883@qq.com
#include <math.h>
#include <iostream>
#include <vector>
using std::vector;
#include <string>
using std::string;
#define min(a,b) ((a<b) ?a:b)

int ldistance(const string & sleft, const string & sright)
{
    int cntl = sleft.length();
    int cntr = sright.length();
    if( cntl == 0 ) return cntr;
    if( cntr == 0 ) return cntl;
    typedef vector<vector<int>> Tmatrix;
    Tmatrix matrix(cntl+1);
    for( int i = 0; i <= cntl; ++i )
        matrix[i].resize(cntr+1);
    for( int i = 1; i <= cntl; ++i)
        matrix[i][0] = i;
    for( int i = 1; i <= cntr; ++i)
        matrix[0][i] = i;
    for( int i = 1; i <= cntl; ++i )
    {
        const char si = sleft[i-1];
        for(int j = 1; j <= cntr; ++j)
        {
            const char dj = sright[j - 1];
            int cost;
            if(si == dj)
                cost = 0;
            else 
                cost = 1;
            const int above = matrix[i-1][j] + 1;
            const int left = matrix[i][j-1] + 1;
            const int diag = matrix[i-1][j-1] + cost;
            int min1 = min(left,diag);
            matrix[i][j] = min(above, min1);
        }
    }
    return matrix[cntl][cntr];
}
