#include "debugList.h"
#include <bits/stdc++.h>
using namespace std;

template class debugList<int>;

debugList<int> x;

debugList<int> y;

int main() {

  for (int i = 0; i < 10000; i++) {
    x.push_back(i);
  }

  for (int i = 0; i < 20000; i++) {
    y.push_back(i);
  }
  auto it = x.begin();
  it = x.erase(it);
  x.erase(it++);
  cout<<(*x.begin())<<(*it);
  return 0;
}
