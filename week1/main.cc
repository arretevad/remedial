#include <cassert>
#include <iostream>
#include <string>

#include "vector.h"
#include "sort.h"
#include "test_util.h"

using namespace rtl;
using std::cout;
using std::endl;
using std::string;

template <typename T, typename U>
bool compare(T tfirst, T tlast, U ufirst, U ulast) {
  if (tlast - tfirst != ulast - ufirst) return false;
  for (; tfirst != tlast; ++tfirst, ++ufirst) {
    if (*tfirst != *ufirst) return false;
  }
  return true;
}

typedef construction_counter<int> counter;

void test_construction_destruction_counts() {
  int total = 500;
  {
    vector<counter> v;
    for (int i = 0; i < 500; ++i) {
      v.push_back(counter(i));
    }
  }

  for (int i = 0; i < total; ++i) {
    assert(counter::constructed(i) == counter::destructed(i) && "Construction and destruction counts did not match");
  }
}

void test_constructors() {
  string arr[] = { "a", "a", "a", "a", "a" };

  vector<string> v1(5, "a");
  assert(v1.size() == 5);
  assert(compare(v1.begin(), v1.end(), arr, arr + 5));

  vector<int> v2;
  assert(v2.size() == 0);

  vector<string> v3(arr, arr + 5);
  assert(v3.size() == 5);
  assert(compare(v3.begin(), v3.end(), arr, arr + 5));
}

void test_push_back() {
  string arr[] = { "a", "a", "a", "a", "a" };

  vector<string> v1;
  assert(v1.size() == 0);
  assert(compare(v1.begin(), v1.end(), arr, arr));

  v1.push_back("a");
  assert(v1.size() == 1);
  assert(compare(v1.begin(), v1.end(), arr, arr + 1));

  v1.push_back("a");
  v1.push_back("a");
  v1.push_back("a");
  v1.push_back("a");
  assert(v1.size() == 5);
  assert(compare(v1.begin(), v1.end(), arr, arr + 5));

  vector<string> v2(4, "a");
  v2.push_back("a");
  assert(v2.size() == 5);
  assert(compare(v1.begin(), v1.end(), arr, arr + 5));
}

void test_pop_back() {
  string arr[] = { "a", "a", "a", "a", "a" };

  vector<string> v1(5, "a");
  v1.pop_back();
  assert(v1.size() == 4);
  assert(compare(v1.begin(), v1.end(), arr, arr + 4));

  v1.pop_back();
  v1.pop_back();
  v1.pop_back();
  v1.pop_back();
  assert(v1.size() == 0);
  assert(v1.begin() == v1.end());
}

void test_insert() {
  string arr[] = { "a", "b", "e" };
  string arr2[] = { "c", "d" };

  vector<string> v1(arr, arr + 3);
  v1.insert(v1.begin() + 2, arr2, arr2 + 2);

  string arr3[] = { "a", "b", "c", "d", "e" };
  assert(compare(v1.begin(), v1.end(), arr3, arr3 + 5));
}

void test_erase() {
}

void test_vector() {
  test_construction_destruction_counts();
  test_constructors();
  test_push_back();
  test_pop_back();
  test_insert();
  test_erase();
}

void test_sort() {
  int arr[] = { 5, 4, 3, 2, 1 };
  int sorted_arr[] = { 1, 2, 3, 4, 5 };

  vector<int> v(arr, arr + 5);
  quicksort(v.begin(), v.end());

  assert(compare(v.begin(), v.end(), sorted_arr, sorted_arr + 5));
}

int main(int argc, char** argv) {
  test_vector();
  test_sort();

  cout << "All tests passed" << endl;

  return 0;
}
