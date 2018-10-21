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
const vector<metric> metrics = {EUCLIDIAN, MANTHATTAN};

enum kernel {
    UNIFORM, TRIANGULAR, EPANECHNIKOV, QUARTIC, TRIWEIGHT
};
const uint KERNEL_SIZE = 5;
const vector<kernel> kernels = {UNIFORM, TRIANGULAR, EPANECHNIKOV, QUARTIC, TRIWEIGHT};

const uint NEIGHBORS_SIZE = 10;
const vector<uint> neighbors = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};


// Score

struct score {

    static double micro_f1(const vector<vector<double>>& cm, uint k) {
        double n = 0;
        double w[k], row_sum[k], column_sum[k];
        double precision[k], recall[k];
        for (uint i = 0; i < k; ++i) {
            w[i] = row_sum[i] = column_sum[i] = 0;
            precision[i] = recall[i] = 0;
        }
        for (uint i = 0; i < k; ++i) {
            for (uint j = 0; j < k; ++j) {
                n += cm[i][j];
                w[i] += cm[i][j];
                row_sum[i] += cm[i][j];
                column_sum[j] += cm[i][j];
            }
        }
        for (uint i = 0; i < k; ++i) {
            precision[i] = row_sum[i] == 0 ? 0 : cm[i][i] / row_sum[i];
            recall[i] = column_sum[i] == 0 ? 0 : cm[i][i] / column_sum[i];
        }
        double aver_precision = 0, aver_recall = 0;
        for (uint i = 0; i < k; ++i) {
            aver_precision += w[i] * precision[i];
            aver_recall += w[i] * recall[i];
        }
        aver_precision /= n;
        aver_recall /= n;
        double denominator = aver_precision + aver_recall;
        double aver_f = denominator == 0 ? 0 : 2 * aver_precision * aver_recall / denominator;
        return aver_f;
    }

};


// Knn

struct object {
    uint ind;
    vector<int> a;
    uint c;
};

struct knn {

    knn(uint mm, uint kk, uint nn, const vector<object>& ddata) : m(mm), k(kk), n(nn), data(ddata) {
        cross_validation();
    }

    uint predict(const object& test) {
        return predict(test, (uint)data.size());
    }

    void write_predict_info(const object& test) {
        vector<pair<double, uint>> d;
        for (uint i = 0; i < data.size(); ++i) {
            d.emplace_back(make_pair(get_distance(test, data[i]), data[i].ind));
        }
        random_shuffle(d.begin(), d.end());
        sort(d.begin(), d.end(), cmp_pair);
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
    vector<object> data;
    metric mc;
    kernel kl;
    uint neighb;

    static bool cmp_pair(const pair<double, uint>& a, const pair<double, uint>& b) {
        return a.first < b.first;
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
        if (kl == UNIFORM) {
            return 0.5;
        }
        if (kl == TRIANGULAR) {
            return 1 - p;
        }
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

    uint predict(const object& test, uint blocked) {
        vector<pair<double, uint>> d;
        for (uint j = 0; j < data.size(); ++j) {
            if (j != blocked) {
                d.emplace_back(make_pair(get_distance(test, data[j]), data[j].c));
            }
        }
        random_shuffle(d.begin(), d.end());
        sort(d.begin(), d.end(), cmp_pair);
        double h = d[neighb].first;
        vector<double> k_score(k, 0);
        for (uint i = 0; i < neighb; ++i) {
            k_score[d[i].second] += get_weight(d[i].first / h);
        }
        double best_k_score = 0;
        uint best_k = 0;
        for (uint i = 0; i < k; ++i) {
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
        double best_score = 0;
        double score;
        vector<vector<double>> cm;
        cm.resize(k);
        for (uint i = 0; i < k; ++i) {
            cm[i].resize(k);
        }
        vector<metric> mcs = metrics;
        random_shuffle(mcs.begin(), mcs.end());
        vector<kernel> kls = kernels;
        random_shuffle(kls.begin(), kls.end());
        vector<uint> neighbs = neighbors;
        random_shuffle(neighbs.begin(), neighbs.end());
        for (uint im = 0; im < METRIC_SIZE; ++im) {
            mc = mcs[im];
            for (uint ik = 0; ik < KERNEL_SIZE; ++ik) {
                kl = kls[ik];
                for (uint in = 0; in < NEIGHBORS_SIZE; ++in) {
                    neighb = neighbs[in];
                    for (uint i = 0; i < k; ++i) {
                        for (uint j = 0; j < k; ++j) {
                            cm[i][j] = 0;
                        }
                    }
                    for (uint i = 0; i < data.size(); ++i) {
                        ++cm[data[i].c][predict(data[i], i)];
                    }
                    score = score::micro_f1(cm, k);
                    if (score >= best_score) {
                        best_mc = mc;
                        best_kl = kl;
                        best_neighb = neighb;
                        best_score = score;
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
        data.push_back({i + 1, a, c - 1});
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
        //cout << "class: " << solver.predict({i + 1, b, 0}) + 1 << "\n";
    }

    return 0;
}