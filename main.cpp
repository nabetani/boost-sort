#include <algorithm>
#include <boost/sort/flat_stable_sort/flat_stable_sort.hpp>
#include <boost/sort/pdqsort/pdqsort.hpp>
#include <boost/sort/spinsort/spinsort.hpp>
#include <boost/sort/spreadsort/integer_sort.hpp>
#include <chrono>
#include <functional>
#include <iostream>
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

std::vector<int> sorted_src(int size) {
  std::vector<int> vec(size);
  for (int i = 0; i < size; ++i) {
    vec[i] = i;
  }
  return vec;
}

std::vector<int> almost_sorted_src(int seed, int size, double ratio) {
  auto v = sorted_src(size);
  std::mt19937_64 rng(seed);
  std::uniform_int_distribution<int> dist(0, size - 1);
  for (int i = 0; i < size * ratio; ++i) {
    int a = dist(rng);
    int b = dist(rng);
    std::swap(v[a], v[b]);
  }
  return v;
}
std::vector<int> reverse_src(int size) {
  std::vector<int> vec(size);
  for (int i = 0; i < size; ++i) {
    vec[i] = size - i;
  }
  return vec;
}

void std_sort(std::vector<int> &v) { //
  std::sort(v.begin(), v.end());
}

void boost_pdqsort(std::vector<int> &v) { //
  boost::sort::pdqsort(v.begin(), v.end());
}

void boost_spread(std::vector<int> &v) { //
  boost::sort::spreadsort::integer_sort(v.begin(), v.end());
}

void boost_flat_stable_sort(std::vector<int> &v) { //
  boost::sort::flat::flat_stable_sort(v.begin(), v.end());
}

void boost_spin(std::vector<int> &v) { //
  boost::sort::spinsort(v.begin(), v.end());
}

void std_stable_sort(std::vector<int> &v) { //
  std::stable_sort(v.begin(), v.end());
}

struct gen_t {
  std::string name;
  std::function<std::vector<int>()> proc;
};

struct sorter_t {
  std::string name;
  decltype(std_sort) *proc;
};

void test(sorter_t &sorter, gen_t const &gen, bool production) {
  using namespace std::chrono;
  std::vector<int> v = gen.proc();
  auto start = high_resolution_clock::now();
  sorter.proc(v);
  auto end = high_resolution_clock::now();
  auto ms = duration_cast<microseconds>(end - start).count() * 1e-3;
  if (production) {
    std::cout << sorter.name << ", " << gen.name
              << "\n"
                 "  res="
              << v[v.size() / 2]
              << "\n"
                 "  tick="
              << ms << "ms\n";
  }
}

void test(int seed, int size) {
  gen_t gens[] = {
      {"random", [&]() { return random_src(seed, size); }},
      {"sorted", [&]() { return sorted_src(size); }},
      {"almost_sorted", [&]() { return almost_sorted_src(seed, size, 0.01); }},
      {"reverse", [&]() { return reverse_src(size); }},
  };

  sorter_t sorters[] = {
      {"std::sort", std_sort},
      {"std::stable_sort", std_stable_sort},
      {"boost/pdq_sort", boost_pdqsort},
      {"boost/flat_stable_sort", boost_flat_stable_sort},
      {"boost/spread", boost_spread},
      {"boost/spin", boost_spin},
  };

  for (int i = 0; i < 3; ++i) {
    for (auto sorter : sorters) {
      for (auto const &gen : gens) {
        test(sorter, gen, 2 <= i);
      }
    }
  }
}

int main(int argc, char const *argv[]) {
  int seed = argc < 2 ? 0 : std::atoi(argv[1]);
  int size = argc < 3 ? 1'000'000 : std::atoi(argv[2]);
  test(seed, size);
}