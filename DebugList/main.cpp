#include <iostream>
#include <vector>
#include <list>
#include <memory>
#include "debugList.h"

using namespace std;

int main()
{
    debugList<int> x;
    x.push_back(1);
    x.push_back(2);
    x.push_back(3);
    x.push_back(4);
    x.push_back(5);
    x.push_front(-3);
    x.push_front(-2);
    x.push_front(-1);
    x.pop_front();
    debugList<int> y;
    y.push_back(11);
    y.push_back(12);
    y.push_back(13);
    y.push_back(14);
    y.push_back(15);
    (*x.begin())++;

    auto it1 = x.begin();
    it1 = y.begin();
    it1++;
    x.splice(x.begin(), y,y.begin(),y.end());
    for(auto it = x.begin(); it!=x.end();it++)
        cout<<(*it)<<endl;
    //x.pop_back();
    return 0;
}

