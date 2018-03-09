#include <iostream>
#include <vector>
#include <list>
#include <memory>
#include "debugList.h"

using namespace std;

int main()
{
    debugList<int> x;
    for (int i = 0; i < 10; i++)
    {
        x.push_back(i);
    }
    auto it = x.begin();
    auto iter = x.begin();
//    ++++iter;
//    ++++it;
    x.erase(x.end());
    cout << *x.begin() << endl;
    return 0;
}

