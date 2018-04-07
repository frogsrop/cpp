#include "debugList.h"
#include <bits/stdc++.h>
using namespace std;

template class debugList<int>;

debugList<int> x;

debugList<int> y;

int main() {
  x.push_back(1);

  x.push_back(2);

  x.push_back(3);

  x.push_back(4);

  y.push_back(5);

  y.push_back(6);

  y.push_back(7);

  y.push_back(8);
  swap(x, y);

  debugList<int>::const_iterator it = x.begin();
  debugList<int>::const_iterator it1 = y.begin();
  return 0;
}
