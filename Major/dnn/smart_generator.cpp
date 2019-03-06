#include <cmath>
#include <ctime>
#include <vector>
#include <random>
#include <iostream>
#include <iomanip>
using namespace std;

typedef unsigned int uint;

uint s[7] = {0, 0, 100, 31, 22, 18, 14};
// 2 : 1000 1
// 3 : 31 31 1
// 4 : 22 22 22 1
// 5 : 18 18 18 18 1
// 6 : 14 14 14 14 14 1

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);
    srand((time(0)));

    freopen("input.txt", "w", stdout);

    uint d = rand() % 5 + 2;
    cout << d << "\n";
    uint m = s[d];
    for (uint i = 0; i < d - 1; ++i) {
        cout << m << " ";
    }
    cout << 1 << "\n";
    uint n = rand() % 1024 + 1;
    cout << n << "\n";
    for (uint i = 0; i < n; ++i) {
        for (uint j = 0; j < m; ++j) {
            cout << (rand() - rand()) % 1000000001 << " ";
        }
        cout << (rand() % 2 == 0 ? 1 : -1) << "\n";
    }
}