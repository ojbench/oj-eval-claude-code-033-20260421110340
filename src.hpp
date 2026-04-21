#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace sjtu {

template <typename T>
class list {
private:
  struct node {
    node *prev{nullptr};
    node *next{nullptr};
    T *val{nullptr};
    // sentinel constructor
    node() : prev(this), next(this), val(nullptr) {}
    // value node constructor
    explicit node(const T &v) : prev(nullptr), next(nullptr), val(new T(v)) {}
    ~node() { if (val) { delete val; val = nullptr; } }
  };

  node *head; // circular sentinel
  std::size_t n;

  static void link_before(node *pos, node *cur) {
    // insert cur before pos
    cur->prev = pos->prev;
    cur->next = pos;
    pos->prev->next = cur;
    pos->prev = cur;
  }

  static node *unlink(node *pos) {
    pos->prev->next = pos->next;
    pos->next->prev = pos->prev;
    pos->prev = pos->next = nullptr;
    return pos;
  }

public:
  class const_iterator;
  class iterator {
    friend class list;
    friend class const_iterator;
    node *cur{nullptr};
    node *sentinel{nullptr};
    explicit iterator(node *c, node *s) : cur(c), sentinel(s) {}
  public:
    iterator() = default;

    iterator &operator++() {
      if (!cur || !sentinel) throw std::runtime_error("invalid iterator");
      if (cur->next == nullptr) throw std::runtime_error("invalid iterator");
      cur = cur->next; return *this;
    }
    iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

    iterator &operator--() {
      if (!cur || !sentinel) throw std::runtime_error("invalid iterator");
      if (cur->prev == nullptr) throw std::runtime_error("invalid iterator");
      cur = cur->prev; return *this;
    }
    iterator operator--(int) { iterator tmp = *this; --(*this); return tmp; }

    T &operator*() const {
      if (!cur || cur == sentinel || !cur->val) throw std::runtime_error("dereference end or invalid iterator");
      return *cur->val;
    }
    T *operator->() const noexcept {
      return (!cur || cur == sentinel) ? nullptr : cur->val;
    }

    friend bool operator==(const iterator &a, const iterator &b) {
      return a.cur == b.cur; }
    friend bool operator!=(const iterator &a, const iterator &b) {
      return a.cur != b.cur; }
  };

  class const_iterator {
    friend class list;
    node *cur{nullptr};
    node *sentinel{nullptr};
    explicit const_iterator(node *c, node *s) : cur(c), sentinel(s) {}
  public:
    const_iterator() = default;
    const_iterator(const iterator &it) : cur(it.cur), sentinel(it.sentinel) {}

    const_iterator &operator++() {
      if (!cur || !sentinel) throw std::runtime_error("invalid const_iterator");
      cur = cur->next; return *this; }
    const_iterator operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }

    const_iterator &operator--() {
      if (!cur || !sentinel) throw std::runtime_error("invalid const_iterator");
      cur = cur->prev; return *this; }
    const_iterator operator--(int) { const_iterator tmp = *this; --(*this); return tmp; }

    const T &operator*() const {
      if (!cur || cur == sentinel || !cur->val) throw std::runtime_error("dereference end or invalid const_iterator");
      return *cur->val; }
    const T *operator->() const noexcept { return (!cur || cur == sentinel) ? nullptr : cur->val; }

    friend bool operator==(const const_iterator &a, const const_iterator &b) { return a.cur == b.cur; }
    friend bool operator!=(const const_iterator &a, const const_iterator &b) { return a.cur != b.cur; }
    friend bool operator==(const const_iterator &a, const iterator &b) { return a.cur == b.cur; }
    friend bool operator!=(const const_iterator &a, const iterator &b) { return a.cur != b.cur; }
  };

  // Constructors & assignment
  list() : head(new node()), n(0) {}

  list(const list &other) : head(new node()), n(0) {
    for (auto it = other.cbegin(); it != other.cend(); ++it) {
      push_back(*it);
    }
  }

  list &operator=(const list &other) {
    if (this == &other) return *this;
    list tmp(other);
    swap(tmp);
    return *this;
  }

  ~list() {
    clear();
    delete head; head = nullptr; n = 0;
  }

  // Element access
  T &front() noexcept(false) {
    if (empty()) throw std::runtime_error("list::front on empty");
    return *head->next->val;
  }
  T &back() noexcept(false) {
    if (empty()) throw std::runtime_error("list::back on empty");
    return *head->prev->val;
  }
  const T &front() const noexcept(false) {
    if (empty()) throw std::runtime_error("list::front on empty");
    return *head->next->val;
  }
  const T &back() const noexcept(false) {
    if (empty()) throw std::runtime_error("list::back on empty");
    return *head->prev->val;
  }

  // Iterators
  iterator begin() noexcept { return iterator(head->next, head); }
  const_iterator cbegin() const noexcept { return const_iterator(head->next, head); }
  iterator end() noexcept { return iterator(head, head); }
  const_iterator cend() const noexcept { return const_iterator(head, head); }

  // Capacity
  bool empty() const noexcept { return n == 0; }
  std::size_t size() const noexcept { return n; }

  // Modifiers
  void clear() noexcept {
    node *p = head->next;
    while (p != head) {
      node *nx = p->next;
      delete p;
      p = nx;
    }
    head->next = head->prev = head;
    n = 0;
  }

  iterator insert(iterator pos, const T &value) {
    if (pos.sentinel != head) throw std::runtime_error("iterator does not belong to this list");
    node *p = pos.cur;
    node *nn = new node(value);
    link_before(p, nn);
    ++n;
    return iterator(nn, head);
  }

  iterator erase(iterator pos) {
    if (pos.sentinel != head) throw std::runtime_error("iterator does not belong to this list");
    node *p = pos.cur;
    if (p == head) throw std::runtime_error("erase end iterator");
    node *nxt = p->next;
    unlink(p);
    delete p;
    --n;
    return iterator(nxt, head);
  }

  void push_front(const T &value) { insert(begin(), value); }
  void push_back(const T &value) { insert(end(), value); }

  void pop_front() { if (empty()) throw std::runtime_error("pop_front on empty"); erase(begin()); }
  void pop_back() { if (empty()) throw std::runtime_error("pop_back on empty"); iterator it = end(); --it; erase(it); }

  // Utility
  void swap(list &other) noexcept {
    if (this == &other) return;
    // swap sentinels by swapping links around them and sizes, or simpler: swap internals
    std::swap(head, other.head);
    std::swap(n, other.n);
  }
};

} // namespace sjtu

#endif // SJTU_LIST_HPP
