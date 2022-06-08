#include <iostream>

#include "templates.h"

constexpr char s1[] = "Hello cpp template!";
constexpr char s2[] = "Hello again!";

int main(int argc, char **argv) {
  MyTemplates::static_string<sizeof(s1)-1> ss1(s1);
  MyTemplates::static_string<sizeof(s2)-1> ss2(s2);
  auto ss3 = ss1 + ss2;
  std::cout << ss3.content << std::endl;
  return 0;
}
