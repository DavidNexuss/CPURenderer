#pragma once
#include <array>
#include <cstddef>
#include <forward_list>
#include <tuple>
#include <utility>

template <typename T, int count> struct vec : public std::array<T, count> {

  vec(const vec &other) = default;

  vec(T val) {
    for (int i = 0; i < count; i++) {
      (*this)[i] = val;
    }
  }

  inline vec &operator=(const vec &other) = default;

  template <typename... Args>
  vec(Args &&...args) : std::array<T, count>{{args...}} {}

  vec(vec &&other) {
    for (int i = 0; i < count; i++) {
      (*this)[i] = other[i];
    }
  };

#define OP(SIM)                                                                \
  vec operator SIM(const vec &other) const {                                   \
    vec result;                                                                \
    for (size_t i = 0; i < result.size(); i++) {                               \
      result[i] = ((*(this))[i])SIM(other[i]);                                 \
    }                                                                          \
    return result;                                                             \
  }
  OP(*)
  OP(/)
  OP(+)
  OP(-)
#undef OP

#define OP(SIM)                                                                \
  vec operator SIM(const T &other) const {                                     \
    vec result;                                                                \
    for (size_t i = 0; i < result.size(); i++) {                               \
      result[i] = ((*(this))[i])SIM(other);                                    \
    }                                                                          \
    return result;                                                             \
  }

  OP(*) OP(/) OP(+) OP(-)
#undef OP
};

using rgb = vec<unsigned char, 3>;
using rgba = vec<unsigned char, 4>;

//clang-format on
