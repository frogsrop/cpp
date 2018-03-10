#include<bits/stdc++.h>
#include"debugList.h"
using namespace std;

debugList<int> x;
debugList<int> y;

int main() {
    int n = 10;
    for(int i = 0;i<n;i++)
    {
        x.push_front(i);
        y.push_front(n - 1 -i);
    }
    //x.swap(y);
    swap(x,y);
    cout<<x.size_()<<' ';
    cout<<y.size_()<<endl;
    auto it = x.begin();
    auto itt = y.begin();
    for(int i = 0;i<n;i++)
    {
        cout<<(*it)<<' ';
        cout<<(*itt)<<endl;
        it++;
        itt++;
    }

    debugList<int>::iterator_imp ittt;
    ittt = it;
    ittt--;
    cout<<*ittt;
}
