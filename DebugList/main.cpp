#include <iostream>
#include <vector>
#include <list>
#include <memory>
#include "debugList.h"

using namespace std;


int main()
{
    debugList<int> ls;

    for(int i = 0;i<50000;i++)
    {
        int p = abs(rand()) % 4;
        if(p == 0)
        {
            ls.push_back(1);
        }
        if(p == 1)
        {
            ls.push_front(1);
        }
        if(ls.size_()==0)
            continue;
        /*if(p == 3)
        {
            ls.pop_back();
        }if(p == 4)
        {
            ls.pop_front();
        }*/
    }
    return 0;
}

