#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <ostream>
#include <random>
#include <string>
#include <vector>
#include <chrono>
#include "inverse-lyndon.hpp"

void verify(char const *text,
            int *nss,
            int *nss_lce,
            int *pss,
            int *pss_lce,
            int n) {
  std::cout << "Verifying..." << std::endl;
  if (n <= 20) {
    for (int i = 0; i < n; i++) std::cout << text[i] << ":" << nss[i] << ", ";
    std::cout << std::endl;
  }

  int rate = std::max(n / 10, 1);

  auto lce = [&](int i, int j) {
    int lce = 0;
    while (j + lce < n && text[i + lce] == text[j + lce]) ++lce;
    return lce;
  };

  for (int i = 0; i < n; ++i) {
    if ((n - i - 1) % rate == 0)
      std::cout << 100 - 10 * ((n - i - 1) / rate) << "% "  << std::flush;

    int ps = pss[i];
    int pl = pss_lce[i];

    for (int j = i - 1; j > ps; j = pss[j]) {
      auto l = lce(j, i);
      if (i + l < n && text[j + l] > text[i + l]) {
        std::cout << "pss[" << i << "]=" << j << ", but algorithm claims "
                  << "pss[" << i << "]=" << pss[j] << std::endl;
        std::abort();
      }
    }

    if (ps >= 0) {
      auto l = lce(ps, i);
      if (i + l >= n || text[ps + l] < text[i + l]) {
        std::cout << "pss[" << i << "]<" << ps << ", but algorithm claims "
                  << "pss[" << i << "]=" << ps << std::endl;
        std::abort();
      }

      if (l != pl) {
        std::cout << "plce[" << i << "]=" << l << ", but algorithm claims "
                  << "plce[" << i << "]=" << pl << std::endl;
        std::abort();
      }
    }
  }
  std::cout << std::endl;

  for (int i = n - 1; i >= 0; --i) {
     if (i % rate == 0)
       std::cout << 100 - (10 * (i / rate)) << "% "  << std::flush;

    int ns = nss[i];
    int nl = nss_lce[i];

    for (int j = i + 1; j < ns; j = nss[j]) {
      auto l = lce(i, j);
      if (j + l >= n || text[i + l] < text[j + l]) {
        std::cout << "nss[" << i << "]=" << j << ", but algorithm claims "
                  << "nss[" << i << "]=" << nss[j] << std::endl;
        std::abort();
      }
    }

    if (ns < n) {
      auto l = lce(i, ns);
      if (ns + l < n && text[i + l] > text[ns + l]) {
        std::cout << "nss[" << i << "]>" << ns << ", but algorithm claims "
                  << "nss[" << i << "]=" << ns << std::endl;
        std::abort();
      }

      if (l != nl) {
        std::cout << "nlce[" << i << "]=" << l << ", but algorithm claims "
                  << "nlce[" << i << "]=" << nl << std::endl;
        std::abort();
      }
    }
  }

  std::cout << "\nError free!" << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc != 3 && argc != 2) {
    std::cerr << "Wrong number of parameters." << std::endl;
    return -1;
  }

  static std::random_device seed;
  static std::mt19937_64 g(seed());
  static std::uniform_int_distribution<uint8_t> d(97, 98);

  std::string string;
  uint64_t max_size = (argc == 3) ?
    std::stoll(argv[2]) : std::numeric_limits<uint64_t>::max();

  if (max_size >= std::numeric_limits<int>::max()) {
    max_size = std::numeric_limits<int>::max() - 1;
    std::cerr << "Input truncated to length " << max_size << "." << std::endl;
  }

  std::string file = argv[1];

  if (file[0] == '*') {
    string.resize(file.size() - 1);
    for (size_t i = 0; i < string.size(); i++) string[i] = file[i + 1];
    std::cout << "Literal input string." << std::endl;
  } else if (file == std::string("random")) {
    string.resize(max_size);
    for (size_t i = 0; i < max_size; ++i) string[i] = d(g);
    std::cout << "Random string generated." << std::endl;
  } else {
    std::ifstream t(argv[1]);
    t.seekg(0, std::ios::end);
    uint64_t size = std::min((uint64_t)t.tellg(), max_size);
    t.seekg(0, std::ios::beg);
    string.resize(size);
    std::copy_n((std::istreambuf_iterator<char>(t)), size, string.begin());
    std::cout << "String loaded from file:\n" << file << std::endl;
  }

  int n = string.size();
  std::cout << "n = " << n << std::endl;
  if (n <= 80) {
    for (int i = 0; i < n; i++) std::cout << string[i];
    std::cout << std::endl;
  }

  std::vector<int> ngs(n);
  std::vector<int> ngs_lce(n);
  std::vector<int> pgs(n);
  std::vector<int> pgs_lce(n);
  std::vector<int> inverse_lyndon_array(n);

  // Timing the inverse_lyndon function
  auto start_ngs = std::chrono::high_resolution_clock::now();

  inverse_lyndon(string.data(), ngs.data(), ngs_lce.data(), pgs.data(), pgs_lce.data(),
         n);

  auto end_ngs = std::chrono::high_resolution_clock::now();

  auto duration_ngs = std::chrono::duration_cast<std::chrono::microseconds>(end_ngs - start_ngs).count();

  // Timing the inverse lyndon array calculation
  auto start_inverse = std::chrono::high_resolution_clock::now();

  for(size_t i = 0; i < n; i++){
    inverse_lyndon_array[i] = ngs[i] - i + ngs_lce[i];
  }

  auto end_inverse = std::chrono::high_resolution_clock::now();
  auto duration_inverse = std::chrono::duration_cast<std::chrono::microseconds>(end_inverse - start_inverse).count();

  verify(string.data(), ngs.data(), ngs_lce.data(), pgs.data(), pgs_lce.data(),
         n);

  std::cout << duration_ngs << "," << duration_inverse << std::endl;

  return 0;
}
