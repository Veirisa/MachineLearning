#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
using namespace std;

typedef unsigned int uint;


uint k, n;
double ans;
vector<vector<double>> y;
vector<double> mid_y, sum_y;

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);

    cin >> k >> n;
    y.resize(k);
    int cur_x, cur_y;
    for (uint i = 0; i < n; ++i) {
        cin >> cur_x >> cur_y;
        y[cur_x - 1].push_back(cur_y);
    }

    for (uint i = 0; i < k; ++i) {
        mid_y.push_back(0);
        for (uint j =  0; j < y[i].size(); ++j) {
            mid_y[i] += y[i][j];
        }
        mid_y[i] /= y[i].size();

        sum_y.push_back(0);
        for (uint j =  0; j < y[i].size(); ++j) {
            sum_y[i] += (y[i][j] - mid_y[i]) * (y[i][j] - mid_y[i]);
        }
        sum_y[i] /= n;
    }

    for (uint i = 0; i < k; ++i) {
        ans += sum_y[i];
    }
    cout << fixed << setprecision(10) << ans;

    return 0;
}
