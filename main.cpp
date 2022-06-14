#include <iostream>

#include "templates.h"

constexpr char s1[] = "Hello cpp template!";
constexpr char s2[] = "Hello again!";

constexpr char DELIMITER[] = " ";

constexpr auto fs = MyTemplates::get_format_string(DELIMITER, 1, 2, 3, 4, 'a', 3.2, 4.f);
const char *cs = fs.content;

struct TestSt {
  int a;
  long b;
  float c;
  double d;
  char e;
  char s[16];

  static constexpr auto fmt_string = MyTemplates::get_format_string(
      ",",
      decltype(TestSt::a){},
      decltype(TestSt::b){},
      decltype(TestSt::c){},
      decltype(TestSt::d){},
      decltype(TestSt::e){},
      std::decay<decltype(TestSt::s)>::type{}
  );
};

int main(int argc, char **argv) {
  MyTemplates::static_string<sizeof(s1) - 1> ss1(s1);
  MyTemplates::static_string<sizeof(s2) - 1> ss2(s2);
  auto ss3 = ss1 + ss2;
  MyTemplates::static_string<0> s0("");
  auto ss4 = ss3 + s0;
  std::cout << ss4.content << std::endl;

  auto ss5 = MyTemplates::make_static_string("Another hello!");
  std::cout << ss5.content << std::endl;

  auto ss6 = MyTemplates::make_static_string("");
  std::cout << ss6.content << std::endl;

  std::cout << cs << std::endl;

  std::cout << TestSt::fmt_string.content << std::endl;

  std::cout << MyTemplates::IntegerSequence_Back<MyTemplates::IntegerSequence<5, 2, 6, 9, 1, 3>>::value << std::endl;
  using SEQUENCE = MyTemplates::IntegerSequence_GetSequence<MyTemplates::IntegerSequence<5, 2, 6, 9, 1, 3>>;
  for (int i = 0; i < SEQUENCE::size; ++i) {
    std::cout << SEQUENCE::values[i];
  }
  std::cout << std::endl;

  using SEQUENCE2 = MyTemplates::IntegerSequence_GetSequence<
      MyTemplates::IntegerSequence_PopBack<MyTemplates::IntegerSequence<5, 2, 6, 9, 1, 3>>::type
  >;
  for (int i = 0; i < SEQUENCE2::size; ++i) {
    std::cout << SEQUENCE2::values[i];
  }
  std::cout << std::endl;

  using SEQUENCE3 = MyTemplates::IntegerSequence_GetSequence<
      MyTemplates::IntegerSequence_Concat<
          MyTemplates::IntegerSequence<1, 2, 3>,
          MyTemplates::IntegerSequence<4, 5, 6>>::type
  >;
  for (int i = 0; i < SEQUENCE3::size; ++i) {
    std::cout << SEQUENCE3::values[i];
  }
  std::cout << std::endl;

  std::cout << MyTemplates::IntegerSequence_Get<2, MyTemplates::IntegerSequence<1, 2, 3>>::value << std::endl;

  using SEQUENCE4 = MyTemplates::IntegerSequence_GetSequence<
      typename MyTemplates::IntegerSequence_Insert<3, 10,
                                                   MyTemplates::IntegerSequence<1, 2, 3, 4, 5>>::type>;
  for (int i = 0; i < SEQUENCE4::size; ++i) {
    std::cout << SEQUENCE4::values[i];
  }
  std::cout << std::endl;

  using SEQUENCE5 = MyTemplates::IntegerSequence_GetSequence<MyTemplates::IntegerSequence_AscendingInsert<3,
  MyTemplates::IntegerSequence<1,2,3,4,5,6,7>>::type>;
  for (int i = 0; i < SEQUENCE5::size; ++i) {
    std::cout << SEQUENCE5::values[i];
  }
  std::cout << std::endl;

  using SEQUENCE6 = MyTemplates::IntegerSequence_GetSequence<
      MyTemplates::IntegerSequence_InsertionSortAscending<9,5,2,6,4,8,7,1,3>::type>;
  for (int i = 0; i < SEQUENCE6::size; ++i) {
    std::cout << SEQUENCE6::values[i];
  }
  std::cout << std::endl;
  return 0;
}
