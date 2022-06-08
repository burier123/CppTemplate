//
// Created by wangxz on 6/7/22.
//

#pragma once

#include <type_traits>

namespace MyTemplates {

template<size_t... Idx>
struct index_sequence {};

template<size_t t, typename>
struct add_index;

template<size_t t, size_t... Idx>
struct add_index<t, index_sequence<Idx...>> {
  using result = index_sequence<Idx..., t>;
};

template<size_t N, typename = std::enable_if<(N > 1)>>
struct make_index_sequence {
  using result = typename add_index<N-1, typename make_index_sequence<N-1>::result>::result;
};

template<>
struct make_index_sequence<1> {
  using result = index_sequence<0>;
};


template<size_t num, size_t... nums>
struct max_arg {
  static constexpr size_t value = num > max_arg<nums...>::value ? num : max_arg<nums...>::value;
};

template<size_t num>
struct max_arg<num> {
  static constexpr size_t value = num;
};

template<size_t N>
struct static_string {
  static constexpr auto size = N;
  char content[N+1];

  template<size_t... Idxs, typename = std::enable_if<(max_arg<Idxs...>::value + 1 == N)>>
  constexpr static_string(const char (&s)[N+1], index_sequence<Idxs...>): content{s[Idxs]..., '\0'} {}

  constexpr static_string(const char (&s)[N+1]):
  static_string(s, typename make_index_sequence<N>::result {}) {}

  template<char... cs, typename = std::enable_if<(sizeof...(cs) == N)>>
  constexpr static_string(): content{cs..., '\0'} {}

  template<size_t N1, size_t N2, size_t... Idx1s, size_t... Idx2s,
           typename = std::enable_if<(N1 + N2 == N &&
               max_arg<Idx1s...>::value + 1 == N1 &&
               max_arg<Idx2s...>::value + 1 == N2)>>
  constexpr static_string(const static_string<N1>& s1, const static_string<N2>& s2,
                          index_sequence<Idx1s...>, index_sequence<Idx2s...>):
                          content{s1.content[Idx1s]..., s2.content[Idx2s]..., '\0'}
                          {}

};

template<size_t N1, size_t N2>
constexpr static_string<N1+N2> operator+ (const static_string<N1>& s1, const static_string<N2>& s2) {
  return static_string<N1+N2>(s1, s2,
                              typename make_index_sequence<N1>::result {},
                              typename make_index_sequence<N2>::result {});
}
}
