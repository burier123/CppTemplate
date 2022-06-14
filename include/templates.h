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

template<size_t N, typename = std::enable_if<(N > 0)>>
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

template <>
struct static_string<0> {
  static constexpr auto size = 0;
  char content[1];
  constexpr static_string(const char *): content{'\0'} {}
  constexpr static_string(char): content{'\0'} {}
};

template<size_t N1, size_t N2>
constexpr static_string<N1+N2> operator+ (const static_string<N1>& s1, const static_string<N2>& s2) {
  return static_string<N1+N2>(s1, s2,
                              typename make_index_sequence<N1>::result {},
                              typename make_index_sequence<N2>::result {});
}

template <size_t N, typename = std::enable_if<(N > 0)>>
constexpr static_string<N> operator+ (const static_string<N>& s1, const static_string<0>& s2) {
  return s1;
}

template <size_t N, typename = std::enable_if<(N > 0)>>
constexpr static_string<N> operator+ (const static_string<0>& s1, const static_string<N>& s2) {
  return s2;
}

template <size_t N, typename = std::enable_if<(N > 0)>>
constexpr static_string<N-1> make_static_string(const char (&s)[N]) {
  return static_string<N-1>(s);
}

template <size_t N, typename = std::enable_if<(N > 0)>>
constexpr static_string<N-1> make_static_string(const char (&&s)[N]) {
  return static_string<N-1>(s);
}

template<typename T>
struct type_format;

template<>
struct type_format<char> {
  static constexpr auto fmt_str = make_static_string("%c");
};

template<>
struct type_format<int8_t> {
  static constexpr auto fmt_str = make_static_string("%hhd");
};

template<>
struct type_format<uint8_t> {
  static constexpr auto fmt_str = make_static_string("%hhu");
};

template<>
struct type_format<int16_t> {
  static constexpr auto fmt_str = make_static_string("%hd");
};

template<>
struct type_format<uint16_t> {
  static constexpr auto fmt_str = make_static_string("%hu");
};

template<>
struct type_format<int32_t> {
  static constexpr auto fmt_str = make_static_string("%d");
};

template<>
struct type_format<uint32_t> {
  static constexpr auto fmt_str = make_static_string("%u");
};

template<>
struct type_format<int64_t> {
  static constexpr auto fmt_str = make_static_string("%ld");
};

template<>
struct type_format<uint64_t> {
  static constexpr auto fmt_str = make_static_string("%lu");
};

template<>
struct type_format<float> {
  static constexpr auto fmt_str = make_static_string("%f");
};

template<>
struct type_format<double> {
  static constexpr auto fmt_str = make_static_string("%lf");
};

template<>
struct type_format<char *> {
  static constexpr auto fmt_str = make_static_string("%s");
};

template<>
struct type_format<char []> {
  static constexpr auto fmt_str = make_static_string("%s");
};

template<size_t N>
struct type_format<char [N]> {
  static constexpr auto fmt_str = make_static_string("%s");
};

template<typename T>
using remove_cvr = std::remove_reference<typename std::remove_cv<T>::type>;

//template<size_t N, const static_string<N>& DELIMITER, typename T, typename... Args>
//struct format_string {
//  static constexpr auto str = type_format<typename remove_cvr<T>::type>::fmt_str + DELIMITER +
//      format_string<N, DELIMITER, Args...>::str;
//};
//
//template<size_t N, const static_string<N>& DELIMITER, typename T>
//struct format_string<N, DELIMITER, T> {
//  static constexpr auto str = type_format<typename remove_cvr<T>::type>::fmt_str;
//};


template <size_t N, typename T>
constexpr auto get_format_string(const char (&)[N], T) {
  return type_format<T>::fmt_str;
}


template <size_t N, typename T, typename... Args>
constexpr auto get_format_string(const char (&DELIMITER)[N], T, Args... args) {
  return type_format<typename remove_cvr<T>::type>::fmt_str +
  make_static_string(DELIMITER) +
  get_format_string(DELIMITER, std::forward<Args>(args)...);
}

template<size_t ...Is>
struct IntegerSequence{};

template<typename>
struct IntegerSequence_Front;

template<size_t Front, size_t ...Is>
struct IntegerSequence_Front<IntegerSequence<Front, Is...>> {
  static constexpr auto value = Front;
};

template<typename>
struct IntegerSequence_Back;

template<size_t Back>
struct IntegerSequence_Back<IntegerSequence<Back>> {
  static constexpr auto value = Back;
};

template<size_t Front, size_t ...Is>
struct IntegerSequence_Back<IntegerSequence<Front, Is...>> {
  static constexpr auto value = IntegerSequence_Back<IntegerSequence<Is...>>::value;
};

template<typename>
struct IntegerSequence_GetSequence;

template<size_t ...Is>
struct IntegerSequence_GetSequence<IntegerSequence<Is...>> {
  static constexpr auto size = sizeof...(Is);
  static constexpr size_t values[] = {Is...};
};

template<size_t, typename>
struct IntegerSequence_PushFront;

template<size_t Front, size_t ...Is>
struct IntegerSequence_PushFront<Front, IntegerSequence<Is...>> {
  using type = IntegerSequence<Front, Is...>;
};

template<typename>
struct IntegerSequence_PopFront;

template<size_t Front, size_t ...Is>
struct IntegerSequence_PopFront<IntegerSequence<Front, Is...>> {
  using type = IntegerSequence<Is...>;
};

template<size_t, typename>
struct IntegerSequence_PushBack;

template<size_t Back, size_t ...Is>
struct IntegerSequence_PushBack<Back, IntegerSequence<Is...>> {
  using type = IntegerSequence<Is..., Back>;
};

template<typename>
struct IntegerSequence_PopBack;

template<size_t Back>
struct IntegerSequence_PopBack<IntegerSequence<Back>> {
  using type = IntegerSequence<>;
};

template<size_t Front, size_t ...Is>
struct IntegerSequence_PopBack<IntegerSequence<Front, Is...>> {
  using type = typename IntegerSequence_PushFront<Front, typename IntegerSequence_PopBack<IntegerSequence<Is...>>::type>::type;
};

template<typename, typename>
struct IntegerSequence_Concat;

template<size_t... I1s, size_t... I2s>
struct IntegerSequence_Concat<IntegerSequence<I1s...>, IntegerSequence<I2s...>> {
  using type = IntegerSequence<I1s..., I2s...>;
};

template<size_t, typename>
struct IntegerSequence_Get;

template<size_t... Is>
struct IntegerSequence_Get<0, IntegerSequence<Is...>> {
  static constexpr auto value = IntegerSequence_Front<IntegerSequence<Is...>>::value;
};

template<size_t Idx, size_t... Is>
struct IntegerSequence_Get<Idx, IntegerSequence<Is...>> {
  static constexpr auto value = IntegerSequence_Get<Idx-1,
  typename IntegerSequence_PopFront<IntegerSequence<Is...>>::type
  >::value;
};

template<size_t, size_t, typename>
struct IntegerSequence_Insert;

template<size_t Element, size_t Front, size_t... Is>
struct IntegerSequence_Insert<0, Element, IntegerSequence<Front, Is...>> {
  using type = typename IntegerSequence_PushFront<Element, IntegerSequence<Front, Is...>>::type;
};

template<size_t Idx, size_t Element, size_t Front, size_t... Is>
struct IntegerSequence_Insert<Idx, Element, IntegerSequence<Front, Is...>> {
  using type = typename IntegerSequence_PushFront<Front,
  typename IntegerSequence_Insert<Idx-1, Element, IntegerSequence<Is...>>::type>::type;
};

template<size_t Element, typename T, bool CONT = (Element > IntegerSequence_Front<T>::value)>
struct IntegerSequence_AscendingInsert;

template<size_t Element, size_t Front, size_t... Is>
struct IntegerSequence_AscendingInsert<Element, IntegerSequence<Front, Is...>, false> {
  using type = typename IntegerSequence_PushFront<Element, IntegerSequence<Front, Is...>>::type;
};

template<size_t Element, size_t Front, size_t... Is>
struct IntegerSequence_AscendingInsert<Element, IntegerSequence<Front, Is...>, true> {
  using type = typename IntegerSequence_PushFront<Front,
                                                  typename IntegerSequence_AscendingInsert<
                                                      Element,
                                                      IntegerSequence<Is...>,
                                                      (Element > IntegerSequence_Front<IntegerSequence<Is...>>::value)>::type>::type;
};

template<size_t Element, size_t Front>
struct IntegerSequence_AscendingInsert<Element, IntegerSequence<Front>, false> {
  using type = IntegerSequence<Element, Front>;
};

template<size_t Element, size_t Front>
struct IntegerSequence_AscendingInsert<Element, IntegerSequence<Front>, true> {
  using type = IntegerSequence<Front, Element>;
};

template<size_t Element, size_t... Is>
struct IntegerSequence_InsertionSortAscending {
  using type = typename IntegerSequence_AscendingInsert<Element,
                                                        typename IntegerSequence_InsertionSortAscending<Is...>::type>::type;
};

template<size_t Element>
struct IntegerSequence_InsertionSortAscending<Element> {
  using type = IntegerSequence<Element>;
};

}
