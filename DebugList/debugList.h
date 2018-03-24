#ifndef DEBUGLIST
#define DEBUGLIST

#include <cassert>
#include <memory>

template <typename T> class debugList {
private:
  class node;

public:
  class iterator_imp_const;
  class iterator_imp;

  using iterator = iterator_imp;
  using const_iterator = iterator_imp_const;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  class iterator_imp {
  public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::bidirectional_iterator_tag;

    iterator_imp();

    T &operator*() const;

    iterator_imp &operator++();

    iterator_imp operator++(int);

    iterator_imp &operator--();

    iterator_imp operator--(int);

    iterator_imp &operator=(iterator_imp const &);

    bool operator!=(iterator_imp const &) const;

    friend bool operator==(iterator_imp const &x, iterator_imp const &y) {
      assert(!x.val.expired());
      assert(!y.val.expired());
      assert(x.my == y.my);
      return (x.val.lock() == y.val.lock());
    }

    friend bool operator!=(iterator_imp const &x, iterator_imp const &y) {
      assert(!x.val.expired());
      assert(!y.val.expired());
      assert(x.my == y.my);
      return !(x == y);
    }

  private:
    friend class debugList;

    iterator_imp(std::weak_ptr<node> const &, debugList<T> *);

    std::weak_ptr<node> val;
    debugList *my;
  };

  class iterator_imp_const {
  public:
    using difference_type = std::ptrdiff_t;
    using value_type = const T;
    using pointer = const T *;
    using reference = const T &;
    using iterator_category = std::bidirectional_iterator_tag;

    iterator_imp_const();
    iterator_imp_const(const iterator_imp &);

    const T &operator*() const;

    iterator_imp_const &operator++();

    iterator_imp_const operator++(int);

    iterator_imp_const &operator--();

    iterator_imp_const operator--(int);

    iterator_imp_const &operator=(iterator_imp_const const &);

    friend bool operator==(iterator_imp_const const &x,
                           iterator_imp_const const &y) {
      assert(!x.val.expired());
      assert(!y.val.expired());
      assert(x.my == y.my);
      return (x.val.lock() == y.val.lock());
    }

    friend bool operator!=(iterator_imp_const const &x, iterator_imp_const const &y)
    {
        assert(!x.val.expired());
        assert(!y.val.expired());
        assert(x.my == y.my);
        return !(x == y);
    }

  private:
    friend class debugList;

    iterator_imp_const(std::weak_ptr<node> const &, const debugList<T> *);

    std::weak_ptr<node> val;
    debugList const *my;
  };

  debugList();
  debugList(debugList const &);

  debugList &operator=(debugList const &other);

  void push_back(T const &x);
  void push_front(T const &x);
  void pop_back();
  void pop_front();
  void clear();
  bool empty();
  void swap(debugList &);

  friend void swap(debugList &a, debugList &b) {
    using std::swap;
    a.swap(b);
  }

  T front() const;
  T back() const;

  iterator_imp begin();
  const iterator_imp_const begin() const;

  iterator_imp end();
  const iterator_imp_const end() const;

  reverse_iterator rbegin();
  const const_reverse_iterator rbegin() const;

  reverse_iterator rend();
  const const_reverse_iterator rend() const;

  void insert(iterator_imp const &it, T const &val);

  iterator_imp erase(iterator_imp const &it);

  void splice(iterator_imp_const const &before, debugList<T> &list2, iterator_imp_const it1,
              iterator_imp_const const &it2);

  size_t size_() { return size; }

private:
  class node {
  public:
    node() {
      prev = std::weak_ptr<node>();
      next = nullptr;
    }
    node(T const &x, std::weak_ptr<node> const &prv,
         std::shared_ptr<node> const &nxt) {
      obj = x;
      prev = prv;
      next = nxt;
    }

    T obj;
    std::shared_ptr<node> next;
    std::weak_ptr<node> prev;
  };

  int size;
  std::shared_ptr<node> head;
  std::shared_ptr<node> tail;
};

template <typename T> debugList<T>::debugList() {
  head = std::make_shared<node>(node());
  tail = head;
  size = 0;
}

template <typename T>
debugList<T> &debugList<T>::operator=(const debugList<T> &x) {
  clear();
  for (auto it = x.begin(); it != x.end(); it++) {
    push_back((*it));
  }
  return *this;
}

template <typename T> debugList<T>::debugList(debugList<T> const &x) {
  head = std::make_shared<node>(node());
  tail = head;
  size = 0;
  if (!x.size) {
    return;
  }
  T val = x.front();
  std::shared_ptr<node> nhead(new node(val, std::weak_ptr<node>(), head));
  fflush(stdout);
  head->prev = nhead;
  fflush(stdout);
  head = nhead;
  size = 0;
  size++;
  auto it = x.begin();
  it++;
  while (it != x.end()) {
    nhead->next = std::make_shared<node>(node(*it, nhead, nullptr));
    nhead = nhead->next;
    it++;
    size++;
  }
  nhead->next = tail;
  tail->prev = nhead;
}

template <typename T> void debugList<T>::push_back(const T &x) {
  insert(end(), x);
}

template <typename T> void debugList<T>::push_front(const T &x) {
  insert(begin(), x);
}

template <typename T> void debugList<T>::pop_back() {
  assert(size >= 1);
  iterator_imp temp = end();
  temp--;
  erase(temp);
}

template <typename T> void debugList<T>::pop_front() { erase(begin()); }

template <typename T> void debugList<T>::clear() {
  while (tail != head) {
    pop_back();
  }
}
template <typename T> bool debugList<T>::empty() { return size == 0; }
template <typename T>
typename debugList<T>::reverse_iterator debugList<T>::rbegin() {
  return reverse_iterator(debugList<T>::end());
}

template <typename T>
const typename debugList<T>::const_reverse_iterator debugList<T>::rbegin() const {
  return const_reverse_iterator(debugList<T>::end());
}

template <typename T>
typename debugList<T>::reverse_iterator debugList<T>::rend() {
  return reverse_iterator(debugList<T>::begin());
}

template <typename T>
const typename debugList<T>::const_reverse_iterator debugList<T>::rend() const {
  return const_reverse_iterator((iterator_imp_const) debugList<T>::begin());
}
template <typename T> void debugList<T>::swap(debugList<T> &x) {
  if (size == 0) {
    *this = x;
    x.clear();
    return;
  }
  if (x.size == 0) {
    x = *this;
    clear();
    return;
  }
  auto cur = end();
  cur--;
  splice(end(), x, x.begin(), x.end());
  cur++;
  x.splice(x.end(), *this, begin(), cur);
}

template <typename T> T debugList<T>::front() const {
  assert(size > 0);
  return (*begin());
}

template <typename T> T debugList<T>::back() const {
  assert(size > 0);
  iterator_imp_const temp = end();
  temp--;
  return (*temp);
}

template <typename T>
typename debugList<T>::iterator_imp debugList<T>::begin() {
  return iterator_imp(head, this);
}
template <typename T>
const typename debugList<T>::iterator_imp_const debugList<T>::begin() const {
  return iterator_imp_const(head, this);
}

template <typename T> typename debugList<T>::iterator_imp debugList<T>::end() {
  return iterator_imp(tail, this);
}

template <typename T>
const typename debugList<T>::iterator_imp_const debugList<T>::end() const {
  return iterator_imp_const(tail, this);
}
template <typename T>
void debugList<T>::insert(const iterator_imp &it, const T &val) {
  assert(it.my == this);
  assert(!it.val.expired());

  if (it.val.lock() == head) {
    std::shared_ptr<node> nhead(new node(val, std::weak_ptr<node>(), head));
    head->prev = nhead;
    head = nhead;
    size++;
    return;
  }
  std::shared_ptr<node> next = it.val.lock();
  std::weak_ptr<node> prev = next->prev;
  std::shared_ptr<node> nnode(new node(val, prev, next));
  prev.lock()->next = nnode;
  next->prev = nnode;
  size++;
}
template <typename T>
typename debugList<T>::iterator_imp
debugList<T>::erase(const iterator_imp &it) {
  assert(it.my == this);
  assert(!it.val.expired());
  assert(size > 0);
  assert(it.val.lock() != tail);
  std::shared_ptr<node> next = it.val.lock()->next;
  if (next == tail) {
    tail = tail->prev.lock();
    tail->next = nullptr;
    size--;
    return end();
  }

  if (it.val.lock() == head) {
    head = next;
    head->prev = std::weak_ptr<node>();
    size--;
    return begin();
  }
  std::shared_ptr<node> nxt = it.val.lock()->next;
  std::shared_ptr<node> prv = it.val.lock()->prev.lock();
  nxt->prev = prv;
  prv->next = nxt;
  iterator_imp ret(nxt, this);
  size--;
  return ret;
}

template <typename T>
void debugList<T>::splice(const iterator_imp_const &before, debugList<T> &list2,
                          iterator_imp_const it1,
                          const iterator_imp_const &it2) {
  assert(it1.my == &list2);
  assert(it2.my == &list2);
  assert(before.my == this);

  assert(!before.val.expired());
  assert(!it1.val.expired());
  assert(!it2.val.expired());

  assert(it1.my != before.my);

  iterator_imp check = it1;
  while (check.val.lock() != list2.tail && check != it2) {
    check++;
  }

  assert(check == it2);

  while (it1 != list2.end() && it1.val.lock() != it2.val.lock()) {
    T temp = it1.val.lock()->obj;
    it1 = list2.erase(it1);
    insert(before, temp);
  }
}

// iterator_imp
template <typename T>
debugList<T>::iterator_imp::iterator_imp(std::weak_ptr<node> const &x,
                                         debugList<T> *i) {
  val = x;
  my = i;
}

template <typename T> debugList<T>::iterator_imp::iterator_imp() {
  val = std::weak_ptr<node>();
  my = nullptr;
}

template <typename T> T &debugList<T>::iterator_imp::operator*() const {
  assert(!val.expired());
  assert(val.lock() != my->tail);
  return val.lock()->obj;
}

template <typename T>
typename debugList<T>::iterator_imp &debugList<T>::iterator_imp::operator++() {
  assert(!val.expired());
  *this = iterator_imp((val.lock()->next), my);
  assert(!val.expired());
  return *this;
}

template <typename T>
typename debugList<T>::iterator_imp debugList<T>::iterator_imp::
operator++(int) {
  assert(!val.expired());
  iterator_imp it = *this;
  ++*this;
  return it;
}

template <typename T>
typename debugList<T>::iterator_imp &debugList<T>::iterator_imp::operator--() {
  assert(!val.expired());
  *this = iterator_imp(val.lock()->prev.lock(), my);
  assert(!val.expired());
  return *this;
}

template <typename T>
typename debugList<T>::iterator_imp debugList<T>::iterator_imp::
operator--(int) {
  assert(!val.expired());
  iterator_imp it = *this;
  --*this;
  return it;
}

template <typename T>
typename debugList<T>::iterator_imp &debugList<T>::iterator_imp::
operator=(const debugList<T>::iterator_imp &it) {
  val = it.val;
  my = it.my;
  return *this;
}

// iterator_imp_const
template <typename T>
debugList<T>::iterator_imp_const::iterator_imp_const(
    std::weak_ptr<node> const &x, const debugList<T> *i) {
  val = x;
  my = i;
}

template <typename T> debugList<T>::iterator_imp_const::iterator_imp_const() {
  val = std::weak_ptr<node>();
  my = nullptr;
}

template <typename T>
debugList<T>::iterator_imp_const::iterator_imp_const(
    debugList<T>::iterator_imp const &x) {
  val = x.val;
  my = x.my;
}
template <typename T>
const T &debugList<T>::iterator_imp_const::operator*() const {
  assert(!val.expired());
  assert(val.lock() != my->tail);
  return val.lock()->obj;
}

template <typename T>
typename debugList<T>::iterator_imp_const &debugList<T>::iterator_imp_const::
operator++() {
  assert(!val.expired());
  *this = iterator_imp_const((val.lock()->next), my);
  assert(!val.expired());
  return *this;
}

template <typename T>
typename debugList<T>::iterator_imp_const debugList<T>::iterator_imp_const::
operator++(int) {
  assert(!val.expired());
  iterator_imp_const it = *this;
  ++*this;
  return it;
}

template <typename T>
typename debugList<T>::iterator_imp_const &debugList<T>::iterator_imp_const::
operator--() {
  assert(!val.expired());
  *this = iterator_imp_const(val.lock()->prev.lock(), my);
  assert(!val.expired());
  return *this;
}

template <typename T>
typename debugList<T>::iterator_imp_const debugList<T>::iterator_imp_const::
operator--(int) {
  assert(!val.expired());
  iterator_imp_const it = *this;
  --*this;
  return it;
}

template <typename T>
typename debugList<T>::iterator_imp_const &debugList<T>::iterator_imp_const::
operator=(const debugList<T>::iterator_imp_const &it) {
  val = it.val;
  my = it.my;
  return *this;
}

#endif
