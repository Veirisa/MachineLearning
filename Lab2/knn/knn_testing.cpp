#include <iostream>
#include <vector>
#include <algorithm>
#include <vector>
#include <cmath>
#include <ctime>
#include <array>
#include <cstdlib>
#include <iomanip>
#include <fstream>
using namespace std;

typedef unsigned int uint;


// Hyperparameters

enum metric {
    EUCLIDIAN, MANTHATTAN
};
const uint METRIC_SIZE = 2;
const vector<metric> metrics = {EUCLIDIAN, MANTHATTAN};

enum kernel {
    UNIFORM, TRIANGULAR, GAUSSIAN, EPANECHNIKOV, QUARTIC, TRIWEIGHT
};
const uint KERNEL_SIZE = 6;
const vector<kernel> kernels = {UNIFORM, TRIANGULAR, GAUSSIAN, EPANECHNIKOV, QUARTIC, TRIWEIGHT};

const uint NEIGHBORS_SIZE = 20;
const vector<uint> neighbors = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

const uint WINDOW_SIZE = 20;
const vector<double> windows = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};


// Score

struct score {

    static double micro_f1(uint k, uint n, const vector<vector<double>>& cm) {
        double w[k], row_sum[k], column_sum[k];
        double precision[k], recall[k];
        for (uint i = 0; i < k; ++i) {
            w[i] = row_sum[i] = column_sum[i] = 0;
            precision[i] = recall[i] = 0;
        }
        for (uint i = 0; i < k; ++i) {
            for (uint j = 0; j < k; ++j) {
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

    static double accuracy(uint k, uint n, const vector<vector<double>>& cm) {
        double right = 0;
        for (uint i = 0; i < k; ++i) {
            right += cm[i][i];
        }
        return right / n;
    }

};


// Knn

struct object {
    uint ind;
    vector<double> f;
    uint c;
};

struct knn {

    knn(uint mm, uint kk, uint nn, const vector<object>& ddata) : m(mm), k(kk), n(nn), data(ddata), data_dist() {
        data_dist.resize(n);
        for (uint i = 0; i < n; ++i) {
            data_dist[i].resize(n);
            data_dist[i][i] = 0;
        }
        cross_validation();
    }

    uint predict(const object& test) {
        return predict(test, n);
    }

    void write_predict_info(const object& test) {
        vector<pair<double, uint>> d;
        for (uint i = 0; i < n; ++i) {
            d.emplace_back(make_pair(get_distance(test, data[i]), data[i].ind));
        }
        random_shuffle(d.begin(), d.end());
        sort(d.begin(), d.end(), cmp_pair);
        uint cur_neighb;
        double cur_wind;
        if (is_width) {
            cur_neighb = 0;
            while (cur_neighb < min((size_t)20, d.size()) && d[cur_neighb].first <= wind) {
                ++cur_neighb;
            }
            cur_wind = wind;
        } else {
            cur_neighb = neighb;
            cur_wind = d[neighb].first;
        }
        if (cur_neighb == 0) {
            cout << 1 << " ";
            cout << d[0].second << " ";
            cout << fixed << setprecision(8) << 0 << " ";
        } else {
            cout << cur_neighb << " ";
            for (uint i = 0; i < cur_neighb; ++i) {
                cout << d[i].second << " ";
                cout << fixed << setprecision(8) << get_weight(cur_wind == 0 ? 0 : d[i].first / cur_wind) << " ";
            }
        }
        cout << "\n";
    }

    string get_metric() {
        switch (mc) {
            case EUCLIDIAN:
                return "EUCLIDIAN";
            case MANTHATTAN:
                return "MANTHATTAN";
            default:
                return "";
        }
    }

    string get_kernel() {
        switch (kl) {
            case UNIFORM:
                return "UNIFORM";
            case TRIANGULAR:
                return "TRIANGULAR";
            case GAUSSIAN:
                return "GAUSSIAN";
            case EPANECHNIKOV:
                return "EPANECHNIKOV";
            case QUARTIC:
                return "QUARTIC";
            case TRIWEIGHT:
                return "TRIWEIGHT";
            default:
                return "";
        }
    }

    string get_neighb() {
        return to_string(neighb);
    }

    string get_wind_ch() {
        return to_string(wind_ch);
    }

private:

    uint m, k, n;
    vector<object> data;
    vector<vector<double>> data_dist;
    metric mc;
    kernel kl;
    bool is_width;
    uint neighb;
    double wind, wind_ch;

    static bool cmp_pair(const pair<double, uint>& a, const pair<double, uint>& b) {
        return a.first < b.first;
    }

    double get_width(double w) {
        if (mc == EUCLIDIAN) {
            return sqrt((w / n) * (w / n) * m);
        }
        if (mc == MANTHATTAN) {
            return (w / n) * m;
        }
        return 0;
    }

    double get_distance(const object& x, const object& y) {
        double p = 0;
        if (mc == EUCLIDIAN) {
            for (int i = 0; i < m; ++i) {
                p += (x.f[i] - y.f[i]) * (x.f[i] - y.f[i]);
            }
            return sqrt(p);
        }
        if (mc == MANTHATTAN) {
            for (int i = 0; i < m; ++i) {
                p += abs(x.f[i] - y.f[i]);
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
        if (kl == GAUSSIAN) {
            return 1.0 / sqrt(2 * 3.1416) * exp(-0.5 * p * p);
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

    uint predict(const object& test, uint test_ind) {
        vector<pair<double, uint>> d;
        for (uint j = 0; j < n; ++j) {
            if (j != test_ind) {
                double dist = test_ind < n ? data_dist[test_ind][j] : get_distance(test, data[j]);
                d.emplace_back(make_pair(dist, data[j].c));
            }
        }
        random_shuffle(d.begin(), d.end());
        sort(d.begin(), d.end(), cmp_pair);
        vector<double> k_score(k, 0);
        if (is_width) {
            uint i = 0;
            while (i < min((size_t)20, d.size()) && d[i].first <= wind) {
                k_score[d[i].second] += get_weight(wind == 0 ? 0 : d[i].first / wind);
                ++i;
            }
        } else {
            for (uint i = 0; i < neighb; ++i) {
                k_score[d[i].second] += get_weight(d[neighb].first == 0 ? 0 : d[i].first / d[neighb].first);
            }
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

    void fill_data_dist() {
        for (uint i = 0; i < n; ++i) {
            for (uint j = i + 1; j < n; ++j) {
                data_dist[i][j] = data_dist[j][i] = get_distance(data[i], data[j]);
            }
        }
    }

    void cross_validation() {
        metric best_mc = mc;
        kernel best_kl = kl;
        bool best_is_width = is_width;
        uint best_neighb = neighb;
        double best_wind = wind;
        double best_wind_ch = wind_ch;
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
        vector<double> winds = windows;
        random_shuffle(winds.begin(), winds.end());
        for (uint im = 0; im < METRIC_SIZE; ++im) {
            mc = mcs[im];
            fill_data_dist();
            for (uint ik = 0; ik < KERNEL_SIZE; ++ik) {
                kl = kls[ik];
                is_width = rand() % 2 == 0;
                for (uint in = 0, iw = 0; in < NEIGHBORS_SIZE || iw < WINDOW_SIZE;
                     in += is_width ? 0 : 1, iw += is_width ? 1 : 0) {
                    if (in == NEIGHBORS_SIZE) {
                        is_width = true;
                    }
                    if (iw == WINDOW_SIZE) {
                        is_width = false;
                    }
                    if (is_width) {
                        neighb = 0;
                        wind = get_width(winds[iw]);
                        wind_ch = winds[iw];
                    } else {
                        neighb = neighbs[in];
                        wind = 0;
                        wind_ch = 0;
                    }
                    for (uint i = 0; i < k; ++i) {
                        for (uint j = 0; j < k; ++j) {
                            cm[i][j] = 0;
                        }
                    }
                    for (uint i = 0; i < n; ++i) {
                        ++cm[data[i].c][predict(data[i], i)];
                    }
                    score = score::micro_f1(k, n, cm);
                    if (score >= best_score) {
                        best_mc = mc;
                        best_kl = kl;
                        best_is_width = is_width;
                        if (is_width) {
                            best_neighb = 0;
                            best_wind = wind;
                            best_wind_ch = wind_ch;
                        } else {
                            best_neighb = neighb;
                            best_wind = 0;
                            best_wind_ch = 0;
                        }
                        best_score = score;
                    }
                }
            }
        }
        mc = best_mc;
        kl = best_kl;
        is_width = best_is_width;
        neighb = best_neighb;
        wind = best_wind;
        wind_ch = best_wind_ch;
    }

};


// Testing

void generate_max_test() {
    ofstream fout("tests/08");
    fout << 200 << " " << 25 << " " << 400 << "\n";
    for (uint i = 0; i < 400; ++i) {
        fout << 2 * i + (i / 16 + 1) << " ";
        for (uint j = 0; j < 199; ++j) {
            fout << 0 << " ";
        }
        fout << i / 16 + 1 << "\n";
    }
    fout << 400 << "\n";
    for (uint i = 0; i < 400; ++i) {
        fout << 2 * i + (i / 16 + 1) + 1 << " ";
        for (uint j = 0; j < 199; ++j) {
            fout << 0 << " ";
        }
        fout << "\n";
    }
    ofstream fout_a("tests/08.a");
    for (uint i = 0; i < 400; ++i) {
        fout_a << i / 16 + 1 << "\n";
    }
    fout_a << 1;
}

void testing() {
    for (uint test = 1; test <= 8; ++test) {

        clock_t t = clock();

        cout << "----- test" << test << " -----" << "\n";
        ifstream fin("tests/0" + to_string(test));
        ifstream fin_ans("tests/0" + to_string(test) + ".a");

        uint m, k, n;
        fin >> m >> k >> n;

        vector<double> min_v(m, 1000000000);
        vector<double> max_v(m, 0);

        vector<object> data;
        vector<double> a(m, 0);
        uint c;
        for (uint i = 0; i < n; ++i) {
            for (uint j = 0; j < m; ++j) {
                fin >> a[j];
                min_v[j] = min(min_v[j], a[j]);
                max_v[j] = max(max_v[j], a[j]);
            }
            fin >> c;
            data.push_back({i + 1, a, c - 1});
        }

        vector<vector<double>> cm;
        cm.resize(k);
        for (uint i = 0; i < k; ++i) {
            cm[i].resize(k);
            for (uint j = 0; j < k; ++j) {
                cm[i][j] = 0;
            }
        }

        uint q;
        fin >> q;

        uint c_ans[q];

        vector<object> tests;
        vector<double> b(m, 0);
        for (uint i = 0; i < q; ++i) {
            for (uint j = 0; j < m; ++j) {
                fin >> b[j];
                min_v[j] = min(min_v[j], b[j]);
                max_v[j] = max(max_v[j], b[j]);
            }
            fin_ans >> c_ans[i];
            tests.push_back({i + 1, b, 0});
        }

        for (uint j = 0; j < m; ++j) {
            max_v[j] -= min_v[j];
            if (max_v[j] > 0) {
                for (uint i = 0; i < n; ++i) {
                    data[i].f[j] -= min_v[j];
                    data[i].f[j] /= max_v[j];
                }
                for (uint i = 0; i < q; ++i) {
                    tests[i].f[j] -= min_v[j];
                    tests[i].f[j] /= max_v[j];
                }
            }
        }

        knn solver{m, k, n, data};

        for (uint i = 0; i < q; ++i) {
            ++cm[c_ans[i] - 1][solver.predict(tests[i])];
        }

        double f, f_ans;
        f = score::micro_f1(k, q, cm);
        fin_ans >> f_ans;
        cout << "have: " << f << "   ans: " << f_ans << "\n";
        cout << solver.get_metric() << " - " << solver.get_kernel() << " - "
             << solver.get_neighb() << " - " << solver.get_wind_ch() << "\n";
        cout << "(time = " << (double)(clock() - t) / CLOCKS_PER_SEC << ")\n\n";
    }
}


// Main

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);
    srand((time(0)));

    //generate_max_test();
    testing();

    return 0;
}