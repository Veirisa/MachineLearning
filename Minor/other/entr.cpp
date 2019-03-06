#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
using namespace std;

typedef unsigned int uint;
typedef unsigned long long ull;

uint k1, k2, n;
uint x1, x2;
vector<double> row;
vector<vector<uint>> ind;
unordered_map<ull, double> val;
double ans;

inline ull key(ull lt, ull rt) {
    return lt * 1000000 + rt;
}

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);

    cin >> k1 >> k2 >> n;
    row.resize(k1);
    ind.resize(k1);
    ull cur_key;
    for (uint i = 0; i < n; ++i) {
        cin >> x1 >> x2;
        --x1;
        --x2;
        cur_key = key(x1, x2);
        ++val[cur_key];
        if (val[cur_key] == 1) {
            ind[x1].push_back(x2);
        }
        ++row[x1];
    }

    uint j;
    double cur_val;
    double p_log, p;
    for (uint i = 0; i < k1; ++i) {
        for (uint w = 0; w < ind[i].size(); ++w) {
            j = ind[i][w];
            cur_val = val[key(i, j)];
            p = cur_val / n;
            p_log = (cur_val == 0 || row[i] == 0) ? 0 : log(cur_val / row[i]);
            ans -= p * p_log;
        }
    }

    cout << fixed << setprecision(10) << ans;

    return 0;
}
