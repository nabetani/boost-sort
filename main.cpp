#include <algorithm>
#include <boost/sort/sort.hpp>
#include <chrono>
#include <random>
#include <string>
#include <vector>

std::vector<int> random_src(int seed, int size) {
  std::vector<int> vec(size);
  std::mt19937_64 rng(seed);
  std::uniform_int_distribution<int> dist(0, (1 << 30));
  for (auto &e : vec) {
    e = dist(rng);
  }
  return vec;
}

void std_sort(std::vector<int> &v) { //
  std::sort(v.begin(), v.end());
}

using sorter_type = decltype(std_sort);

template <typename gen_type>
void test(std::string const &title, sorter_type &sorter, gen_type gen,
          bool production) {
  using namespace std::chrono;
  std::vector<int> v = gen();
  auto start = high_resolution_clock::now();
  sorter(v);
  auto end = high_resolution_clock::now();
  auto ms = duration_cast<microseconds>(end - start).count() * 1e-3;
  if (production) {
    std::cout << title
              << "\n"
                 "  res="
              << v[v.size() / 2]
              << "\n"
                 "  tick="
              << ms << "ms\n";
  }
}

void test(int seed, int size) {
  for (int i = 0; i < 3; ++i) {
    test(
        "std_sort random_src", std_sort,
        [&]() { return random_src(seed, size); }, 2 <= i);
  }
}

int main(int argc, char const *argv[]) {
  int seed = argc < 2 ? 0 : std::atoi(argv[1]);
  int size = argc < 3 ? 1'000'000 : std::atoi(argv[2]);
  test(seed, size);
}