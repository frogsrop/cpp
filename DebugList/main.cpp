#include "debugList.h"
#include <bits/stdc++.h>
using namespace std;

template class debugList<int>;
template class debugList<double>;

debugList<int> x;


int main() {
  x.push_back(1);

  x.push_back(2);

  x.push_back(3);

  x.push_back(4);

  const debugList<int> y(x);

  //y.back() = 10;
  int* qwe = new int();
  for (auto &t : y) {
    cout << t << ' ';
  }
}
