#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

typedef unsigned int uint;

uint n, m, k;
vector<pair<uint, uint>> el;
vector<vector<uint>> sep;

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);

    cin >> n >> m >> k;
    uint c;
    for (uint i = 0; i < n; ++i) {
        cin >> c;
        el.emplace_back(make_pair(c, i + 1));
    }

    sort(el.begin(), el.end());

    sep.resize(k);
    for (uint i = 0; i < n; ++i) {
        sep[i % k].push_back(el[i].second);
    }

    for (uint i = 0; i < k; ++i) {
        cout << sep[i].size() << " ";
        for (uint j = 0; j < sep[i].size(); ++j) {
            cout << sep[i][j] << " ";
        }
        cout << "\n";
    }

    return 0;
}