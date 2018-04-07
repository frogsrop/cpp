#include "debugList.h"
#include <bits/stdc++.h>
using namespace std;

template class debugList<int>;
template class debugList<double>;

debugList<int> x;

debugList<int> y;

int main() {
  y.push_back(1);

  y.push_back(2);

  y.push_back(3);

  y.push_back(4);

  y.push_back(5);

  x.push_back(6);

  x.push_back(7);

  x.push_back(8);

  x.push_back(9);

  x.push_back(10);

  x.splice(next(x.begin(), 1),y, y.begin(),next(y.begin(), 2));
  for(auto&t:y)
    cout<<t<<' ';
  cout<<endl;
  for(auto&t:x)
    cout<<t<<' ';

  return 0;
}
