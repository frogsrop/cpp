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

    friend bool operator==(iterator_imp const &x, iterator_imp const &y) {
      assert(!x.val.expired());
      assert(!y.val.expired());
      assert(x.val.lock()->my == y.val.lock()->my);
      return (x.val.lock() == y.val.lock());
    }

    friend bool operator!=(iterator_imp const &x, iterator_imp const &y) {
      assert(!x.val.expired());
      assert(!y.val.expired());
      assert(x.val.lock()->my == y.val.lock()->my);
      return !(x == y);
    }

  private:
    friend class debugList;

    iterator_imp(std::weak_ptr<node> const &);

    std::weak_ptr<node> val;
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
      assert(x.val.lock()->my == y.val.lock()->my);
      return (x.val.lock() == y.val.lock());
    }

    friend bool operator!=(iterator_imp_const const &x,
                           iterator_imp_const const &y) {
      assert(!x.val.expired());
      assert(!y.val.expired());
      assert(x.val.lock()->my == y.val.lock()->my);
      return !(x == y);
    }

  private:
    friend class debugList;

    iterator_imp_const(std::weak_ptr<node> const &);

    std::weak_ptr<node> val;
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

  void insert(iterator_imp_const const &it, T const &val);

  iterator_imp erase(iterator_imp_const const &it);

  void splice(iterator_imp_const const &before, debugList<T> &list2,
              iterator_imp_const it1, iterator_imp_const it2);

  size_t size_() { return size; }

private:
  class node {
  public:
    node(debugList *my) {
      prev = std::weak_ptr<node>();
      next = nullptr;
      this->my = my;
    }
    node(T const &x, std::weak_ptr<node> const &prv,
         std::shared_ptr<node> const &nxt, debugList *my) {
      obj = x;
      prev = prv;
      next = nxt;
      this->my = my;
    }
    T obj;
    std::shared_ptr<node> next;
    std::weak_ptr<node> prev;
    debugList *my;
  };
  int size;
  std::shared_ptr<node> head;
  std::shared_ptr<node> tail;
};

template <typename T> debugList<T>::debugList() {
  head = std::make_shared<node>(node(this));
  tail = head;
  size = 0;
}

template <typename T>
debugList<T> &debugList<T>::operator=(const debugList<T> &x) {
  debugList<T> y(x);
  swap(y);
  return *this;
}

template <typename T> debugList<T>::debugList(debugList<T> const &x) {
  head = std::make_shared<node>(node(this));
  tail = head;
  size = 0;
  if (!x.size) {
    return;
  }
  T val = x.front();
  std::shared_ptr<node> nhead(new node(val, std::weak_ptr<node>(), head, this));
  fflush(stdout);
  head->prev = nhead;
  fflush(stdout);
  head = nhead;
  size = 0;
  size++;
  auto it = x.begin();
  it++;
  while (it != x.end()) {
    nhead->next = std::make_shared<node>(node(*it, nhead, nullptr, this));
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
const typename debugList<T>::const_reverse_iterator
debugList<T>::rbegin() const {
  return const_reverse_iterator(debugList<T>::end());
}

template <typename T>
typename debugList<T>::reverse_iterator debugList<T>::rend() {
  return reverse_iterator(debugList<T>::begin());
}

template <typename T>
const typename debugList<T>::const_reverse_iterator debugList<T>::rend() const {
  return const_reverse_iterator((iterator_imp_const)debugList<T>::begin());
}
template <typename T> void debugList<T>::swap(debugList<T> &x) {
  head.swap(x.head);
  tail.swap(x.tail);
  std::swap(x.size, size);
  auto from = head;
  while (from != nullptr) {
    from->my = this;
    from = from->next;
  }
  from = x.head;
  while (from != nullptr) {
    from->my = &x;
    from = from->next;
  }
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
  return iterator_imp(head);
}
template <typename T>
const typename debugList<T>::iterator_imp_const debugList<T>::begin() const {
  return iterator_imp_const(head);
}

template <typename T> typename debugList<T>::iterator_imp debugList<T>::end() {
  return iterator_imp(tail);
}

template <typename T>
const typename debugList<T>::iterator_imp_const debugList<T>::end() const {
  return iterator_imp_const(tail);
}
template <typename T>
void debugList<T>::insert(const iterator_imp_const &it, const T &val) {
  assert(it.val.lock()->my == this);
  assert(!it.val.expired());

  if (it.val.lock() == head) {
    std::shared_ptr<node> nhead(
        new node(val, std::weak_ptr<node>(), head, this));
    head->prev = nhead;
    head = nhead;
    size++;
    return;
  }
  std::shared_ptr<node> next = it.val.lock();
  std::weak_ptr<node> prev = next->prev;
  std::shared_ptr<node> nnode(new node(val, prev, next, this));
  prev.lock()->next = nnode;
  next->prev = nnode;
  size++;
}
template <typename T>
typename debugList<T>::iterator_imp
debugList<T>::erase(const iterator_imp_const &it) {
  assert(it.val.lock()->my == this);
  assert(!it.val.expired());
  assert(size > 0);
  assert(it.val.lock() != tail);
  std::shared_ptr<node> next = it.val.lock()->next;

  if (it.val.lock() == head) {
    head = next;
    head->prev = std::weak_ptr<node>();
    size--;
    return begin();
  }

  if (next == tail) {
    std::shared_ptr<node> prev = next->prev.lock();
    prev->prev.lock()->next = next;
    next->prev = prev->prev;
    size--;
    return end();
  }

  std::shared_ptr<node> nxt = it.val.lock()->next;
  std::shared_ptr<node> prv = it.val.lock()->prev.lock();
  nxt->prev = prv;
  prv->next = nxt;
  iterator_imp ret(nxt);
  size--;
  return ret;
}

template <typename T>
void debugList<T>::splice(iterator_imp_const const &before, debugList<T> &list2,
                          iterator_imp_const it1, iterator_imp_const it2) {
  assert(it1.val.lock()->my == &list2);
  assert(it2.val.lock()->my == &list2);
  assert(before.val.lock()->my == this);

  assert(!before.val.expired());
  assert(!it1.val.expired());
  assert(!it2.val.expired());

  assert(this != &list2);

  iterator_imp_const check = it1;
  int amount = 0;
  while (check.val.lock() != list2.tail && check != it2) {
    check++;
    amount++;
  }
  amount--;
  assert(check == it2);
  if (it1 == it2)
    return;

  it2--;

  std::shared_ptr<node> left = it1.val.lock();
  std::shared_ptr<node> right = it2.val.lock();

  std::shared_ptr<node> end = before.val.lock();
  if (list2.head == left) {
      list2.head = right ->next;
  }
  if (end == head) {
    right->next = end;
    end->prev = right;
    head = left;
  } else {
    std::shared_ptr<node> begin = end->prev.lock();
    left->prev = begin;
    right->next = end;
    end->prev = right;
    begin->next = left;
  }
  /*
  it1.val.lock() -> prev = obf;
  it2.val.lock() -> next = before.val.lock();
  before.val.lock() -> prev = it2.val.lock();
*/
  size += amount;
  list2.size -= amount;
  while (it1 != it2) {
    it1.val.lock()->my = this;
    it1++;
  }
  it2.val.lock()->my = this;
}

// iterator_imp
template <typename T>
debugList<T>::iterator_imp::iterator_imp(std::weak_ptr<node> const &x) {
  val = x;
}

template <typename T> debugList<T>::iterator_imp::iterator_imp() {
  val = std::weak_ptr<node>();
}

template <typename T> T &debugList<T>::iterator_imp::operator*() const {
  assert(!val.expired());
  assert(val.lock() != val.lock()->my->tail);
  return val.lock()->obj;
}

template <typename T>
typename debugList<T>::iterator_imp &debugList<T>::iterator_imp::operator++() {
  assert(!val.expired());
  *this = iterator_imp((val.lock()->next));
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
  *this = iterator_imp(val.lock()->prev.lock());
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
  return *this;
}

// iterator_imp_const
template <typename T>
debugList<T>::iterator_imp_const::iterator_imp_const(
    std::weak_ptr<node> const &x) {
  val = x;
}

template <typename T> debugList<T>::iterator_imp_const::iterator_imp_const() {
  val = std::weak_ptr<node>();
}

template <typename T>
debugList<T>::iterator_imp_const::iterator_imp_const(
    debugList<T>::iterator_imp const &x) {
  val = x.val;
}
template <typename T>
const T &debugList<T>::iterator_imp_const::operator*() const {
  assert(!val.expired());
  assert(val.lock() != val.lock()->my->tail);
  return val.lock()->obj;
}

template <typename T>
typename debugList<T>::iterator_imp_const &debugList<T>::iterator_imp_const::
operator++() {
  assert(!val.expired());
  *this = iterator_imp_const((val.lock()->next));
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
  *this = iterator_imp_const(val.lock()->prev.lock());
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
  return *this;
}

#endif
