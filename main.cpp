#include <boost/sort/sort.hpp>

void test(int num) {}

int main(int argc, char const *argv[]) {
  int num = argc < 2 ? 0 : std::atoi(argv[1]);
  test(num);
}