#include <iostream>

#include "templates.h"

constexpr char s1[] = "Hello cpp template!";
constexpr char s2[] = "Hello again!";

int main(int argc, char **argv) {
  MyTemplates::static_string<sizeof(s1)-1> ss1(s1);
  MyTemplates::static_string<sizeof(s2)-1> ss2(s2);
  auto ss3 = ss1 + ss2;
  MyTemplates::static_string<0> s0("");
  auto ss4 = ss3 + s0;
  std::cout << ss4.content << std::endl;

  auto ss5 = MyTemplates::make_static_string("Another hello!");
  std::cout << ss5.content << std::endl;

  auto ss6 = MyTemplates::make_static_string("");
  std::cout << ss6.content << std::endl;

  std::cout << MyTemplates::get_format_string(1,2,3,4,'a',3.2,4.f).content << std::endl;
  return 0;
}
