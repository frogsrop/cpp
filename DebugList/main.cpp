#include "debugList.h"
#include <bits/stdc++.h>
using namespace std;

debugList<int> x;
debugList<int> y;

struct A {
  int x;
  A(int a) { x = a; }
};

int main() {
  x.push_back(1);
  x.push_back(2);
  x.push_back(3);
  x.push_back(4);
  x.push_back(5);
  y.push_back(6);
  y.push_back(7);
  y.push_back(8);
  y.push_back(9);
  y.push_back(10);
  const debugList<int> any = y;
  auto it1 = y.begin();
  debugList<int>::const_iterator it2 = y.begin();
  (++++++it1);
  (++++it2);
  cout << (it1 == it1) << endl;
  for (auto rit = x.rbegin(); rit != x.rend(); rit++) {
    cout << (*rit) << ' ';
  }
  cout << endl;
  // debugList<A> a;
}
/*
iterator to const
const it
my const to non const
rbegin rend const

*/
