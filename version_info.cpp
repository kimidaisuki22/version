#include "version_info.h"
#include <iostream>
#include "print.h"
const std::string_view version_info = "1.0.0";

void print_version_info() {
  std::cout << "Version: " << version_info;
  print("");
}
