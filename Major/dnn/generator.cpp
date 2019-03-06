#include <cmath>
#include <ctime>
#include <vector>
#include <random>
#include <iostream>
#include <iomanip>
using namespace std;

typedef unsigned int uint;

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);
    srand((time(0)));

    freopen("input.txt", "w", stdout);

    uint n;
    cin >> n;
    uint size = (uint)pow(2, n);
    cout << 3 << "\n";
    cout << n << " " << size << " " << 1 << "\n";
    cout << size << "\n";
    for (uint i = 0; i < size; ++i) {
        for (uint j = 0; j < n; ++j) {
            cout << (((i >> j) & 1) == 1)<< " ";
        }
        cout << (rand() % 2 == 0 ? 1 : -1) << "\n";
    }
}

/*
 *
3
1 2 1
2
0 1
1 0
 *
 */

/*
 * 3
3 8 1
8
0 0 0 1
1 0 0 1
0 1 0 0
1 1 0 1
0 0 1 1
1 0 1 0
0 1 1 1
1 1 1 0
 */

/*
 * 3
2 4 1
4
0 0 1
1 0 1
0 1 0
1 1 1
 */