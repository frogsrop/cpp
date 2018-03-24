#include "debugList.h"
#include <bits/stdc++.h>
using namespace std;

debugList<int> x;
debugList<int> y;

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
  auto it1 = y.rbegin();
  debugList<int>::const_reverse_iterator it2 = y.rbegin();
  (++++++it1);
  (++++it2);
  cout << (it1 == it2) << endl;
  for (auto rit = x.rbegin(); rit != x.rend(); rit++) {
    cout << (*rit) << ' ';
  }
  cout << endl;
  // debugList<A> a;
}
/*
 * != it const
 * splice const it
 * rbegin
 * rend
 *
*/
