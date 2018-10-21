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


// Hyperparameters

enum metric {
    EUCLIDIAN, MANTHATTAN
};
const uint METRIC_SIZE = 2;
const metric metrics[METRIC_SIZE] = {EUCLIDIAN, MANTHATTAN};

enum kernel {
    EPANECHNIKOV, QUARTIC, TRIWEIGHT
};
const uint KERNEL_SIZE = 3;
const kernel kernels[KERNEL_SIZE] = {EPANECHNIKOV, QUARTIC, TRIWEIGHT};

const uint NEIGHBORS_SIZE = 3;
const uint neighbors[NEIGHBORS_SIZE] = {3, 5, 7};


// KNN

struct object {
    uint ind;
    vector<int> a;
    uint c;
};

struct knn {

    knn(uint mm, uint kk, uint nn, const vector<object>& ddata) : m(mm), k(kk), n(nn) {
        data = ddata;
        random_shuffle(data.begin(), data.end());
        for (uint i = 0; i < n / 4; ++i) {
            valid.push_back(data[i]);
        }
        for (uint i = n / 4; i < n; ++i) {
            train.push_back(data[i]);
        }
        cross_validation();
    }

    uint predict(const object& test) {
        return predict(test, data);
    }


    void write_predict_info(const object& test) {
        vector<pair<double, uint>> d;
        for (uint i = 0; i < data.size(); ++i) {
            d.emplace_back(make_pair(get_distance(test, data[i]), data[i].ind));
        }
        sort(d.begin(), d.end());
        double h = d[neighb].first;
        cout << neighb << " ";
        for (uint i = 0; i < neighb; ++i) {
            cout << d[i].second << " ";
            cout << fixed << setprecision(8) << get_weight(d[i].first / h) << " ";
        }
        cout << "\n";
    }

private:

    uint m, k, n;
    vector<object> data, train, valid;
    metric mc = EUCLIDIAN;
    kernel kl = QUARTIC;
    uint neighb = 5;

    double get_accuracy(const vector<object>& objs, const vector<uint>& pred) {
        double right = 0;
        for (uint i = 0; i < objs.size(); ++i) {
            if (objs[i].c == pred[i]) {
                ++right;
            }
        }
        return right / objs.size();
    }

    double get_distance(const object& x, const object& y) {
        double p = 0;
        if (mc == EUCLIDIAN) {
            for (int i = 0; i < m; ++i) {
                p += (x.a[i] - y.a[i]) * (x.a[i] - y.a[i]);
            }
            return sqrt(p);
        }
        if (mc == MANTHATTAN) {
            for (int i = 0; i < m; ++i) {
                p += abs(x.a[i] - y.a[i]);
            }
            return p;
        }
        return 0;
    }

    double get_weight(double p) {
        double dif = 1 - p * p;
        if (kl == EPANECHNIKOV) {
            return 3.0 / 4.0 * dif;
        }
        if (kl == QUARTIC) {
            return 15.0 / 16.0 * dif * dif;
        }
        if (kl == TRIWEIGHT) {
            return 35.0 / 32.0 * dif * dif * dif;
        }
        return 0;
    }

    uint predict(const object& test, const vector<object>& objs) {
        vector<pair<double, uint>> d;
        for (uint j = 0; j < train.size(); ++j) {
            d.emplace_back(make_pair(get_distance(test, objs[j]), objs[j].c));
        }
        sort(d.begin(), d.end());
        double h = d[neighb].first;
        vector<double> k_score(k + 1, 0);
        for (uint i = 0; i < neighb; ++i) {
            k_score[d[i].second] += get_weight(d[i].first / h);
        }
        double best_k_score = 0;
        uint best_k = 0;
        for (uint i = 1; i <= k; ++i) {
            if (k_score[i] >= best_k_score) {
                best_k_score = k_score[i];
                best_k = i;
            }
        }
        return best_k;
    }

    void cross_validation() {
        metric best_mc = mc;
        kernel best_kl = kl;
        uint best_neighb = neighb;
        double best_accuracy = 0;
        vector<uint> pred;
        double accuracy;
        for (uint im = 0; im < METRIC_SIZE; ++im) {
            mc = metrics[im];
            for (uint ik = 0; ik < KERNEL_SIZE; ++ik) {
                kl = kernels[ik];
                for (uint in = 0; in < NEIGHBORS_SIZE; ++in) {
                    neighb = neighbors[in];
                    pred.clear();
                    for (uint i = 0; i < valid.size(); ++i) {
                        pred.push_back(predict(valid[i], train));
                    }
                    accuracy = get_accuracy(valid, pred);
                    if (accuracy >= best_accuracy) {
                        best_mc = mc;
                        best_kl = kl;
                        best_neighb = neighb;
                        best_accuracy = accuracy;
                    }
                }
            }
        }
        mc = best_mc;
        kl = best_kl;
        neighb = best_neighb;
    }

};


// Main

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);
    srand((time(0)));

    uint m, k, n;
    vector<object> data;

    cin >> m >> k >> n;
    vector<int> a(m, 0);
    uint c;
    for (uint i = 0; i < n; ++i) {
        for (uint j = 0; j < m; ++j) {
            cin >> a[j];
        }
        cin >> c;
        data.push_back({i + 1, a, c});
    }

    knn solver{m, k, n, data};

    uint q;
    cin >> q;
    vector<int> b(m, 0);
    for (uint i = 0; i < q; ++i) {
        for (uint j = 0; j < m; ++j) {
            cin >> b[j];
        }
        solver.write_predict_info({i + 1, b, 0});
        //cout << "class: " << solver.predict({i + 1, b, 0}) << "\n";
    }

    return 0;
}