#include <iostream>
#include <string>
#include "src.hpp"
using namespace std;

int main(){
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  sjtu::list<long long> lst;
  string op;
  while (cin >> op){
    if (op == "push_front"){
      long long x; cin >> x; lst.push_front(x);
    } else if (op == "push_back"){
      long long x; cin >> x; lst.push_back(x);
    } else if (op == "pop_front"){
      try { lst.pop_front(); } catch(...) { cout << "error\n"; }
    } else if (op == "pop_back"){
      try { lst.pop_back(); } catch(...) { cout << "error\n"; }
    } else if (op == "front"){
      try { cout << lst.front() << "\n"; } catch(...) { cout << "error\n"; }
    } else if (op == "back"){
      try { cout << lst.back() << "\n"; } catch(...) { cout << "error\n"; }
    } else if (op == "size"){
      cout << lst.size() << "\n";
    } else if (op == "clear"){
      lst.clear();
    } else if (op == "empty"){
      cout << (lst.empty() ? 1 : 0) << "\n";
    } else if (op == "insert"){
      size_t k; long long x; cin >> k >> x;
      if (k > lst.size()) k = lst.size();
      auto it = lst.begin();
      for (size_t i = 0; i < k; ++i) ++it;
      try { lst.insert(it, x); } catch(...) { cout << "error\n"; }
    } else if (op == "erase"){
      size_t k; cin >> k; if (k >= lst.size()) { cout << "error\n"; continue; }
      auto it = lst.begin();
      for (size_t i = 0; i < k; ++i) ++it;
      try { lst.erase(it); } catch(...) { cout << "error\n"; }
    }
  }
  return 0;
}
