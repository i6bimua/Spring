#include <bits/stdc++.h>
using namespace std;
#define fi first
#define se second
using ll = long long;
using pii = pair<int, int>;
void solve() {
  ofstream f;
  f.open("D:/cppfinal/Gwidget/LegalMap.txt", ios::out);
  if (!f.is_open()) {
    std::cout << "cannot open the file";
  }
  vector<pii> v(7);
  for (int r1 = 0; r1 + 3 < 6; ++r1) {
    for (int c1 = 0; c1 + 3 < 6; ++c1) {
      v.push_back({r1, c1});
      for (int r2 = 0; r2 + 2 < 6; ++r2) {
        for (int c2 = 0; c2 + 2 < 6; ++c2) {
          v.push_back({r2, c2});
          for (int r3 = 0; r3 + 2 < 6; ++r3) {
            for (int c3 = 0; c3 + 2 < 6; ++c3) {
              v.push_back({r3, c3});
              for (int r4 = 0; r4 + 1 < 6; ++r4) {
                for (int c4 = 0; c4 + 1 < 6; ++c4) {
                  v.push_back({r4, c4});
                  for (int r5 = 0; r5 + 1 < 6; ++r5) {
                    for (int c5 = 0; c5 + 1 < 6; ++c4) {
                      v.push_back({r5, c5});
                      for (int r6 = 0; r6 < 6; ++r6) {
                        for (int c6 = 0; c6 < 6; ++c6) {
                          v.push_back({r6, c6});
                          for (int r7 = 0; r7 < 6; ++r7) {
                            for (int c7 = 0; c7 < 6; ++c7) {
                              v.push_back({r7, c7});
                              bool fg = 1;
                              vector<vector<int>> a(6, vector<int>(6, 0));
                              auto [x1, y1] = v[0];
                              for (int i = x1; i < x1 + 4; ++i) {
                                for (int j = y1; j < y1 + 4; ++j) ++a[i][j];
                              }
                              auto [x2, y2] = v[1];
                              for (int i = x2; i < x2 + 3; ++i) {
                                for (int j = y2; j < y2 + 3; ++j) ++a[i][j];
                              }
                              auto [x3, y3] = v[2];
                              for (int i = x3; i < x3 + 3; ++i) {
                                for (int j = y3; j < y3 + 3; ++j) ++a[i][j];
                              }
                              auto [x4, y4] = v[3];
                              for (int i = x4; i < x4 + 2; ++i) {
                                for (int j = y4; j < y4 + 2; ++j) ++a[i][j];
                              }
                              auto [x5, y5] = v[4];
                              for (int i = x5; i < x5 + 2; ++i) {
                                for (int j = y5; j < y5 + 2; ++j) ++a[i][j];
                              }
                              auto [x6, y6] = v[5];
                              ++a[x6][y6];
                              auto [x7, y7] = v[6];
                              ++a[x7][y7];
                              for (int i = 0; i < 6; ++i) {
                                for (int j = 0; j < 6; ++j) {
                                  if (a[i][j] > 4) fg = 0;
                                }
                              }
                              if (fg) {
                                for (int i = 0; i < 6; ++i) {
                                  for (int j = 0; j < 6; ++j) {
                                    f << a[i][j];
                                  }
                                  f << endl;
                                }
                              }
                              v.pop_back();
                            }
                          }
                           v.pop_back();
                        }
                      }
                       v.pop_back();
                    }
                  }
                   v.pop_back();
                }
              }
               v.pop_back();
            }
          }
           v.pop_back();
        }
      }
       v.pop_back();
    }
  }
  f.close();
  cout << 1;
}

int main() {
  // cin.tie(nullptr)->sync_with_stdio(false);
  // cout << fixed << setprecision(20);
  int tt = 1;
  cin >> tt;
  while (tt--) {
    solve();
  }
  return 0;
}