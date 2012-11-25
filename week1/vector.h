// A dynamically resized array.
#pragma once

#include <cstddef>  // for size_t
#include <cstdint>  // for SIZE_MAX
#include <iterator>  // for reverse_iterator
#include <type_traits>  // for is_same
#include <memory>  // for allocator, unique_ptr

namespace rtl {

const size_t kInitialVectorSize = 8;

template <typename T>
class vector {
 private:
  typedef vector<T> vector_type;

 public:
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  typedef T* iterator;
  typedef const T* const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  // Constructors
  explicit vector() : elems_(nullptr), size_(0), capacity_(0), alloc_() {}

  explicit vector(size_type n) : elems_(nullptr), size_(0), capacity_(0), alloc_() {
    assign(n, T());
  }

  explicit vector(size_type n, const T& x) : elems_(nullptr), size_(0), capacity_(0), alloc_() {
    assign(n, x);
  }

  template <typename I> vector(I first, I last) : elems_(nullptr), size_(0), capacity_(0), alloc_() {
    assign(first, last);
  }

  vector(const vector<T>& v) : elems_(nullptr), size_(0), capacity_(0), alloc_() {
    reserve(v.size());
    assign(end(), v.begin(), v.end());
  }

  ~vector() {
    clear();
  }

  // Iterator creation
  iterator begin() { return iterator(elems_); }
  const_iterator begin() const { return const_iterator(elems_); }

  iterator end() { return iterator(elems_ + size_); }
  const_iterator end() const { return const_iterator(elems_ + size_); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

  // Size querying and manipulation.
  size_type size() const {
    return size_;
  }

  size_type max_size() const {
    return SIZE_MAX / sizeof(T);
  }

  void resize(size_type size, const T& copy = T()) {
    reserve(size);

    if (size < size_) {
      while (size < size_) {
        push_back(copy);
      }
    } else if (size > size_) {
      while (size > size_) {
        pop_back();
      }
    }
  }

  size_type capacity() const {
    return capacity_;
  }

  bool empty() const {
    return size() == 0;
  }

  void reserve(size_type n) {
    if (n > capacity_) {
      size_type new_capacity = capacity_ ? capacity_ : 1;
      while (new_capacity < n) {
        new_capacity <<= 1;
      }

      T* new_elems = alloc_.allocate(new_capacity);

      for (size_type i = 0; i < size_; ++i) {
        alloc_.construct(new_elems + i, elems_[i]);
        alloc_.destroy(elems_ + i);
      }

      alloc_.deallocate(elems_, capacity_);
      elems_ = new_elems;
      capacity_ = new_capacity;
    }
  }

  // Element access.
  T& operator[](size_type n) { return at(n); }
  const T& operator[](size_type n) const { return at(n); }
  T& at(size_type n) { return elems_[n]; }
  const T& at(size_type n) const { return elems_[n]; }
  T& front() { return at(0); }
  const T& front() const { return at(0); }
  T& back() { return at(size_ - 1); }
  const T& back() const { return at(size_ - 1); }

  // Element creation.
  template <typename I> void assign(I first, I last) {
    clear();
    for (; first != last; ++first) {
      push_back(*first);
    }
  }

  void assign(size_type n, const T& x) {
    clear();
    reserve(n);
    for (; n != 0; --n) {
      push_back(x);
    }
  }

  void push_back(const T& x) {
    reserve(size_ + 1);
    alloc_.construct(elems_ + size_, x);
    ++size_;
  }

  void pop_back() {
    --size_;
    alloc_.destroy(elems_ + size_);
  }

  // Insert and erase.
  iterator insert(iterator p, const T& x) {
    size_t distance = p - begin();
    reserve(size_ + 1);
    p = begin() + distance;

    for (iterator from = end() - 1, to = end(); from >= p; --from, --to) {
      alloc_.construct(to, *from);
      alloc_.destroy(alloc_.address(*from));
    }
    alloc_.construct(p, x);
    ++size_;
    return p;
  }

  iterator insert(iterator p, size_type n, const T& x) {
    size_t distance = p - begin();
    reserve(size_ + n);
    p = begin() + distance;

    iterator it;
    for (; n != 0; --n) {
      it = insert(p, x);
    }
    return it;
  }

  template <typename I> iterator insert(iterator p, I first, I last) {
    --last;
    for (; last >= first; --last) {
      p = insert(p, *last);
    }
    return p;
  }

  iterator erase(iterator p) {
    return erase(p, p + 1);
  }

  iterator erase(iterator first, iterator last) {
    for (iterator i = first; i != last; ++i) {
      alloc_.destroy(alloc_.address(*i));
    }
    for (iterator from = last, to = first; from != end(); ++from, ++to) {
      alloc_.construct(to, *from);
      alloc_.destroy(alloc_.address(*from));
    }
    size_ -= (last - first);
    return first;
  }

  void clear() {
    while (!empty()) {
      pop_back();
    }
    alloc_.deallocate(elems_, capacity_);
    capacity_ = 0;
    size_ = 0;
  }

  // Swap.
  void swap(vector<T>& other) {
    std::swap(elems_, other.elems_);
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
  }

 private:
  T* elems_;
  size_type capacity_;
  size_type size_;
  std::allocator<T> alloc_;
};

template <class T> bool operator==(const vector<T>& x, const vector<T>& y) {
  if (x.size() != y.size()) return false;
  for (auto i = x.begin(), j = y.begin(); i != x.end(); ++i, ++j) {
    if (*i != *j) return false;
  }
  return true;
}

template <class T> bool operator!=(const vector<T>& x, const vector<T>& y) {
  return !(x == y);
}

// template <class T> bool operator< (const vector<T>& x, const vector<T>& y);
// template <class T> bool operator> (const vector<T>& x, const vector<T>& y);
// template <class T> bool operator>=(const vector<T>& x, const vector<T>& y);
// template <class T> bool operator<=(const vector<T>& x, const vector<T>& y);

}  // namespace rtl
