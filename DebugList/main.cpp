#include "debugList.h"
#include <bits/stdc++.h>
using namespace std;

template class debugList<int>;
template class debugList<double>;

debugList<int> x;

debugList<int> y;

int main() {
  x.push_back(1);

  x.push_back(2);

  x.push_back(3);

  x.push_back(4);

  x.splice(next(x.begin(), 2), x, next(x.begin()), prev(x.end()));
  return 0;
}
