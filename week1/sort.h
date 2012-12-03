// Sorting algorithms.
#pragma once

#include <algorithm>
#include <iterator>
#include <functional>

namespace rtl {

// Comb Sort implementation very closely based on Wikipedia's in-place pseudocode.
// Comb Sort is an efficient variation of Bubble Sort.
// Untested!
template <typename Iter, typename Comp = std::less<typename std::iterator_traits<Iter>::value_type>>
void combsort(Iter first, Iter last, Comp comp = Comp()) {
  using std::iterator_traits<Iter>::size_type;

  if (first >= last - 1) return;

  size_type gap = (last - first);
  bool swapped = false;

  while ((gap > 1) || swapped) {
    if (gap > 1) gap = static_cast<size_type>(static_cast<double>(gap) / 1.247330950103979);
    swapped = false;
    
    for (Iter it = first; it + gap < last; ++it) {
      if (comp(*(it + gap), *it)) {
        std::iter_swap(it, it + gap);
        swapped = true;
      }
    }
  }
}

// Quicksort implementation very closely based on Wikipedia's in-place quicksort.
// The determination of the pivot leaves much to be desired. 
template <typename Iter, typename Comp>
Iter partition(Iter left, Iter right, Iter pivot, const Comp& comp) {
  std::iter_swap(pivot, right - 1);
  const auto& pivot_value = *(right - 1);

  Iter store = left;
  for (Iter it = left; it != (right - 1); ++it) {
    if (comp(*it, pivot_value)) {
      std::iter_swap(it, store++);
    }
  }
  std::iter_swap(store, right - 1);
  return store;
}

template <typename Iter, typename Comp = std::less<typename std::iterator_traits<Iter>::value_type>>
void quicksort(Iter left, Iter right, const Comp& comp = Comp()) {
  if (left >= (right - 1)) return;

  Iter pivot = left;  // <- Right here! This is a bad choice.
  Iter new_pivot = partition(left, right, pivot, comp);
  quicksort(left, new_pivot);
  quicksort(new_pivot + 1, right);
}

}  // namespace rtl
