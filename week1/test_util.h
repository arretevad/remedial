#pragma once

#include <map>

namespace rtl {

template <typename T>
class construction_counter {
 public:
  struct counts {
    counts() : constructed(0), destructed(0) {}
    int constructed;
    int destructed;
  };

  typedef std::map<T, counts> counts_map;

  construction_counter(const T& t) : t_(t) {
    construct();
  }

  construction_counter(const construction_counter& c) : t_(c.t_) {
    construct();
  }

  ~construction_counter() {
    (*map())[t_].destructed++;
  }

  static int constructed(const T& t) { return (*map())[t].constructed; }
  static int destructed(const T& t) { return (*map())[t].destructed; }

 private:
  void construct() {
    (*map())[t_].constructed++;
  }

  T t_;
  static counts_map* map() {
    static counts_map* m = new counts_map();
    return m;
  }
};

}  // namespace rtl
