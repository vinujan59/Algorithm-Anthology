/*

3.3.3 - 1D Segment Tree with Range Updates

Description: A segment tree is a data structure used for
solving the dynamic range query problem, which asks to
determine the minimum (or maximum) value in any given
range in an array that is constantly being updated.
Lazy propagation is a technique applied to segment trees that
allows range updates to be carried out in O(log(N)) time.

Time Complexity: Assuming merge() is O(1), query(), update(),
at() are O(log(N)). If merge() is not constant time, then all
running times are multiplied by whatever complexity the merge
function runs in.

Space Complexity: O(N) on the size of the array. A segment
tree for an array of size N needs 2^(log2(N)-1) = 4N nodes.

Note: This implementation is 0-based, meaning that all
indices from 0 to size() - 1, inclusive, are accessible.

*/

#include <limits>

template<class T> class segment_tree {
  int len, x, y;
  T *tree, *lazy, *init, val;

  //define the following yourself. merge(x, nullv) must return x for all valid x
  static inline const T nullv() { return std::numeric_limits<T>::min(); }
  static inline const T merge(const T & a, const T & b) { return a > b ? a : b; }
  
 public:
  segment_tree(int N, T * array = 0) {
    len = N;
    tree = new T[4*len];
    lazy = new T[4*len];
    for (int i = 0; i < (len << 2); i++) lazy[i] = nullv();
    if (array != 0) {
      init = array;
      build(0, 0, len - 1);
    }
  }

  ~segment_tree() {
    delete[] tree;
    delete[] lazy;
  }
  
  int size() { return len; }
  T at(int idx) { return query(idx, idx); }
  
  T query(int lo, int hi) {
    x = lo; y = hi;
    return internal_query(0, 0, len - 1);
  }

  void update(int idx, const T & v) {
    x = y = idx; val = v;
    internal_update(0, 0, len - 1);
  }

  void update(int lo, int hi, const T & v) {
    x = lo; y = hi; val = v;
    internal_update(0, 0, len - 1);
  }

 private:
  void build(int node, int lo, int hi) {
    if (lo == hi) {
      tree[node] = init[lo];
      return;
    }
    build(node*2 + 1, lo, (lo + hi)/2);
    build(node*2 + 2, (lo + hi)/2 + 1, hi);
    tree[node] = merge(tree[node*2], tree[node*2 + 1]);
  }

  T internal_query(int node, int lo, int hi) {
    if (x > hi || y < lo) return nullv();
    if (x <= lo && hi <= y) {
      if (lazy[node] == nullv()) return tree[node];
      return tree[node] = lazy[node];
    }
    int Lchild = node*2 + 1, Rchild = node*2 + 2;
    if (lazy[node] != nullv()) {
      lazy[Lchild] = lazy[Rchild] = lazy[node];
      lazy[node] = nullv();
    }
    return merge(internal_query(Lchild, lo, (lo + hi)/2),
                 internal_query(Rchild, (lo + hi)/2 + 1, hi));
  }

  void internal_update(int node, int lo, int hi) {
    if (x > hi || y < lo) return;
    if (lo == hi) {
      tree[node] = val;
      return;
    }
    if (x <= lo && hi <= y) {
      tree[node] = lazy[node] = merge(lazy[node], val);
      return;
    }
    int Lchild = node*2 + 1, Rchild = node*2 + 2;
    if (lazy[node] != nullv()) {
      lazy[Lchild] = lazy[Rchild] = lazy[node];
      lazy[node] = nullv();    
    }
    internal_update(Lchild, lo, (lo + hi)/2);
    internal_update(Rchild, (lo + hi)/2 + 1, hi);
    tree[node] = merge(tree[Lchild], tree[Rchild]);
  }
};

/*** Example Usage ***/

#include <iostream>
using namespace std;

int main() {
  int arr[5] = {6, 4, 1, 8, 10}; 
  segment_tree<int> T(5, arr);
  cout << "Array contains:";
  for (int i = 0; i < T.size(); i++) cout << " " << T.at(i);
  cout << "\n";
  
  T.update(2, 4, 12);
  
  cout << "Array contains:";
  for (int i = 0; i < T.size(); i++) cout << " " << T.at(i);
  cout << "\nThe max value in the range [0, 3] is ";
  cout << T.query(0, 3) << ".\n"; //12
  return 0;
}