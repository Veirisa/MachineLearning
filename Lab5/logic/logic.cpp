#include <cmath>
#include <vector>
#include <iostream>
using namespace std;

typedef unsigned int uint;

uint m, n, n_pos;
vector<uint> out;

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);

    cin >> m;
    n = (uint)pow(2, m);
    out.resize(n);
    for (uint i = 0; i < n; ++i) {
        cin >> out[i];
        n_pos += out[i];
    }

    if (!n_pos) {
        cout << 2 << " " << m << " " << 1 << "\n";
        for (uint i = 0; i < m; ++i) {
            cout << 0 << " ";
        }
        cout << -0.5;
        return 0;
    }

    cout << 3 << " " << m << " " << n_pos << " " << 1 << "\n";
    for (uint i = 0; i < n; ++i) {
        if (out[i]) {
            for (uint j = 0; j < m; ++j) {
                cout << (((i >> j) & 1) ? 1 : -1) << " ";
            }
            cout << 0.5 - (double)__builtin_popcount(i) << "\n";
        }
    }
    for (uint i = 0; i < n_pos; ++i) {
        cout << 1 << " ";
    }
    cout << -0.5;
    return 0;
}