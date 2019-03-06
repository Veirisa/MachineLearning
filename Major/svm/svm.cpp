#include <iostream>
#include <vector>
#include <algorithm>
#include <vector>
#include <cmath>
#include <ctime>
#include <array>
#include <cstdlib>
#include <iomanip>
using namespace std;

typedef unsigned int uint;


// Svm

struct object {
    vector<double> f;
    int c;
};

struct svm {

    svm(uint mm, uint nn, const vector<object>& ddata) : m(mm), n(nn), data(ddata) {
        int pos = 0, neg = 0;
        for (int i = 0; i < n; ++i) {
            pos += (data[i].c == 1);
            neg += (data[i].c == -1);
        }
        for (int i = 0; i < n; ++i) {
            lambda.push_back(data[i].c == 1 ? neg : pos);
        }
        func = calc_func(lambda);
        find_lambda();
    };

    void write_kernel() {
        cout << "sum(prod(A0, B0)";
        for (int j = 1; j < m; ++j) {
            cout << ",prod(A" << j << ",B" << j << ")";
        }
        cout << ")\n";
    }

    void write_lambda() {
        for (int i = 0; i < n; ++i) {
            cout << fixed << setprecision(10) << lambda[i] << "\n";
        }
    }

private:

    const double STEP = 100000000;
    const double EPS = 1E-7;
    const size_t ITER = 10000;

    uint m, n;
    double func;
    vector<object> data;
    vector<double> lambda;

    inline bool available(uint fix, const vector<bool>& blocked, uint ind) {
        return ind != fix && !blocked[ind];
    }

    double calc_func(const vector<double>& lam) {
        double res = 0;
        for (uint i1 = 0; i1 < n; ++i1) {
            for (uint i2 = 0; i2 < n; ++i2) {
                res += lam[i1] * lam[i2] * data[i1].c * data[i2].c * calc_kernel(data[i1], data[i2]);
            }
        }
        res /= 2;
        for (int i = 0; i < n; ++i) {
            res -= lam[i];
        }
        return res;
    }

    double calc_kernel(const object& a, const object& b) {
        double res = 0;
        for (int j = 0; j < m; ++j) {
            res += a.f[j] * b.f[j];
        }
        return res;
    }

    void find_lambda() {

        uint fix = 0;
        uint blocked_size = 0;
        vector<bool> blocked(n, false);

        vector<double> new_lambda(n, 0);
        vector<double> grad(n, 0);
        double max_grad = 0;
        double new_func = 0;

        while (blocked_size < n) {

            uint iter = 0;
            double step = STEP;
            bool repeat = false;

            while (step > EPS && iter < ITER) {

                // calc grad
                if (!repeat) {
                    for (uint k = 0; k < n; ++k) {
                        if (available(fix, blocked, k)) {
                            double add_grad = 0;
                            for (uint i = 0; i < n; ++i) {
                                if (available(fix, blocked, i)) {
                                    add_grad += lambda[i] *
                                                (calc_kernel(data[i], data[k]) -
                                                 calc_kernel(data[i], data[fix]) -
                                                 calc_kernel(data[k], data[fix]) +
                                                 calc_kernel(data[fix], data[fix]));
                                }
                            }
                            grad[k] = data[k].c * data[fix].c - 1 + add_grad;
                            if (abs(max_grad) < abs(grad[k])) {
                                max_grad = grad[k];
                            }
                        }
                    }
                }

                // norm grad + calc new_lambda
                for (uint i = 0; i < n; ++i) {
                    if (available(fix, blocked, i)) {
                        new_lambda[i] = lambda[i] - step * (grad[i] / max_grad);
                    }
                }

                // calc fix new_lambda
                new_lambda[fix] = 0;
                for (uint i = 0; i < n; ++i) {
                    if (available(fix, blocked, i)) {
                        new_lambda[fix] -= new_lambda[i] * data[i].c;
                    }
                }
                new_lambda[fix] /= data[fix].c;

                // start checking
                bool fail = false;

                // check: all lambda >= 0 ?
                for (uint i = 0; i < n; ++i) {
                    if (!blocked[i] && new_lambda[i] < 0) {
                        fail = true;
                        break;
                    }
                }

                // check: calc new_func + new_func < func
                if (!fail) {
                    new_func = calc_func(new_lambda);
                    if (new_func > func) {
                        fail = true;
                    }
                }

                // result checking
                if (fail) {
                    step /= 2;
                    repeat = true;
                } else {
                    for (int i = 0; i < n; ++i) {
                        lambda[i] = new_lambda[i];
                    }
                    func = new_func;
                    repeat = false;
                }

                ++iter;
            }

            // check border
            bool ok = true;
            uint min_i = fix;
            for (uint i = 0; i < n; ++i) {
                if (available(fix, blocked, i)) {
                    if (lambda[i] < lambda[min_i]) {
                        min_i = i;
                    }
                }
            }
            if (lambda[min_i] > EPS) {
                return;
            }
            lambda[min_i] = 0;
            new_lambda[min_i] = 0;
            blocked[min_i] = true;
            ++blocked_size;
            if (fix == min_i) {
                for (uint i = 0; i < n; ++i) {
                    if (available(fix, blocked, i)) {
                        fix = i;
                        break;
                    }
                }
            }

        }
    }

};

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);
    srand((time(0)));

    uint m, n;
    cin >> m >> n;

    vector<object> data;
    vector<double> f(m, 0);
    char symb_c;
    for (uint i = 0; i < n; ++i) {
        for (uint j = 0; j < m; ++j) {
            cin >> f[j];
        }
        cin >> symb_c;
        data.push_back({f, symb_c == '+' ? 1 : -1});
    }

    svm solver{m, n, data};
    solver.write_kernel();
    solver.write_lambda();

    return 0;
}