#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

typedef unsigned int uint;
typedef vector<vector<double>> matrix;


uint m, n;
matrix tx, x, y;
vector<double> a;


// (a x b) * (b x c)
// i : 1 .. a
//    j : 1 .. c
//       k : 1 .. b
//          c[i][j] = a[i][k] * b[k][j];

matrix multiply(const matrix& m1, const matrix& m2) {
    matrix res;
    res.resize(m1.size());
    for (uint i = 0; i < m1.size(); ++i) {
        for (uint j = 0; j < m2[0].size(); ++j) {
            res[i].push_back(0);
            for (uint k = 0; k < m2.size(); ++k) {
                res[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return res;
}


bool change_matrix(matrix& l, matrix& r, uint cur_i, uint cur_j) {
    uint save_i = cur_i;
    for (uint i = cur_i + 1; i < m; ++i) {
        if (l[i][cur_j] != 0) {
            save_i = i;
            break;
        }
    }
    if (save_i == cur_i) {
        return false;
    }
    for (uint j = cur_j; j < m; ++j) {
        l[cur_i][j] += l[save_i][j];
    }
    r[cur_i][0] += r[save_i][0];
    return true;
}


void gauss(matrix& l, matrix& r) {
    bool ok;
    double ch;
    uint cur_i = 0, cur_j = 0;
    vector<bool> useless(m, false);
    while (cur_j < m) {
        if (l[cur_i][cur_j] == 0) {
            ok = change_matrix(l, r, cur_i, cur_j);
            if (!ok) {
                useless[cur_j] = true;
                ++cur_j;
                continue;
            }
        }
        for (uint i = cur_i + 1; i < m; ++i) {
            if (l[i][cur_j] != 0) {
                ch = l[cur_i][cur_j] / l[i][cur_j];
                for (uint j = cur_j; j < m; ++j) {
                    l[i][j] *= ch;
                    l[i][j] -= l[cur_i][j];
                }
                r[i][0] *= ch;
                r[i][0] -= r[cur_i][0];
            }
        }
        ++cur_i;
        ++cur_j;
    }

    a.resize(m);

    double cur, val;
    int i = cur_i - 1;
    int j = m - 1;
    while (j >= 0) {
        if (useless[j]) {
            a[j] = 0;
            --j;
            continue;
        }
        cur = l[i][j];
        val = r[i][0];
        for (int k = j + 1; k < m; ++k) {
            val -= l[i][k] * a[k];
        }
        a[j] = val / cur;
        --j;
        --i;
    }
}


int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);

    cin >> m >> n;
    x.resize(n);
    y.resize(n);
    for (uint i = 0; i < n; ++i) {
        x[i].resize(m);
        for (uint j = 0; j < m; ++j) {
            cin >> x[i][j];
        }
        y[i].resize(1);
        cin >> y[i][0];
    }

    tx.resize(m);
    for (uint j = 0; j < m; ++j) {
        tx[j].resize(n);
        for (uint i = 0; i < n; ++i) {
            tx[j][i] = x[i][j];
        }
    }

    matrix l = multiply(tx, x);
    matrix r = multiply(tx, y);
    gauss(l, r);

    for (uint j = 0; j < m; ++j) {
        cout << a[j] << "\n";
    }
    cout << 0 << "\n";

    return 0;
}

// f: n x m
// (m x n) * (n x m)  = m x m
// (m x n) * (n x 1) = m x 1
// (m x m) * a = m x 1
// a: m x 1