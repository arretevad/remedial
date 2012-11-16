// A dynamically resized array.
#pragma once

#include <cstddef>  // for size_t
#include <cstdint>  // for SIZE_MAX
#include <memory>  // for unique_ptr

namespace rtl {

template <typename T>
class vector {
 public:
  typedef size_t size_type;
  typedef ptr_iterator<T, 1> iterator;
  typedef ptr_iterator<const T, 1> const_iterator;
  typedef ptr_iterator<T, -1> reverse_iterator;
  typedef ptr_iterator<const T, -1> const_reverse_iterator;

  explicit vector(size_type n = 0, const T& x = T());
  vector(const vector<T>& v);
  template <typename I> vector(I first, I last);

  iterator begin() { return iterator(elems_.get()); }
  const_iterator begin() const { return const_iterator(elems_.get()); }

  iterator end() { return iterator(elems_.get() + size_); }
  const_iterator end() const { return const_iterator(elems_.get() + size_); }

  reverse_iterator rbegin() { return reverse_iterator(elems_.get() + size_ - 1); }
  const_reverse_iterator rbegin() const;

  reverse_iterator rend();
  const_reverse_iterator rend() const;

  size_type size() const { return size_; }
  size_type max_size() const { return SIZE_MAX / sizeof(T); }
  void resize(size_type size, T copy = T());
  size_type capacity() const { return capacity_; }
  bool empty() const { return size() == 0; }
  void reserve(size_type n) {}

  T& operator[](size_type n) { return at(n); }
  const T& operator[](size_type n) const { return at(n); }
  T& at(size_type n) { return elems_[n]; }
  const T& at(size_type n) const { return elems_[n]; }
  T& front() { return at(0); }
  const T& front() const { return at(0); }
  T& back() { return at(size_ - 1); }
  const T& back() const { return at(size_ - 1); }

  template <typename I> void assign(I first, I last);
  void assign(size_type n, const T& x);
  void push_back(const T& x);
  void pop_back();
  iterator insert(iterator p, const T& x);
  void insert(iterator p, size_type n, const T& x);
  template <typename I> void insert(I p, I first, I last);
  iterator erase(iterator p);
  iterator erase(iterator first, iterator last);
  void swap(vector<T>& other) {
    std::swap(elems_, other.elems_);
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
  }
  void clear() { elems_.reset(); capacity_ = 0; size_ = 0; }
  
 private:
  std::unique_ptr<T[]> elems_;
  size_type capacity_;
  size_type size_;
};

template <typename T>
vector<T>::vector(size_type n, const T& x) {
  assign(n, x);
}

template <typename T>
vector<T>::vector(const vector<T>& o) {
}

template <typename T> template <typename I>
vector<T>::vector(I first, I last) {
}

}  // namespace rtl
