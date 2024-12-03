#pragma once

void inverse_lyndon(char const *text, int *ngs, int *nlce, int *pgs, int *plce, int *borders, int n) {
  auto T_l = [&](int x) { return (x >= 0) ? ((int)text[x]) : ((int)+256); };
  auto T_r = [&](int x) { return (x < n) ? ((int)text[x]) : ((int)+256); };

  int d, rhs = -1;

  auto extend_lce = [&](int l, int r, int known_lce = 0) {
    if (r + known_lce < rhs) {
      known_lce = (ngs[l - d] == r - d) ? nlce[l - d] : plce[r - d];
      if (r + known_lce < rhs) return known_lce;
      known_lce = rhs - r;
    }

    while (T_l(l + known_lce) == T_r(r + known_lce)) ++known_lce;

    rhs = r + known_lce;
    d = r - l;
    return known_lce;
  };

  auto border_size = [&](int l, int r) {
    int border = 0;
    while(T_l(l) == T_r(r)){
      ++l;
      ++r;
      ++border;
    }
    return border;
  };

  for (int r = 0; r < n; ++r) {
    int l = r - 1;
    int lce = extend_lce(l, r, 0);
    while (T_l(l + lce) < T_r(r + lce)) {
      ngs[l] = r;
      nlce[l] = lce;
      borders[l] = border_size(l, r);
      if (lce == plce[l]) {
        lce = extend_lce(pgs[l], r, lce);
      } else if (lce > plce[l]) {
        lce = plce[l];
      }
      l = pgs[l];
    }
    pgs[r] = l;
    plce[r] = lce;
  }

  int l = n - 1;
  while (l >= 0) {
    ngs[l] = n;
    nlce[l] = 0;
    l = pgs[l];
  }
}
