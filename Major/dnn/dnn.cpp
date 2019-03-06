#include <cmath>
#include <ctime>
#include <vector>
#include <random>
#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

typedef unsigned int uint;

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

};


// Dnn

struct object {
    vector<double> f;
    int c;
};

struct dnn {

    dnn(uint dd, const vector<uint>& ssize, uint mm, uint nn, const vector<object>& ddata, const clock_t& sstart) :
            depth(dd), size(ssize), m(mm), n(nn), data(ddata), start(sstart) {

        random_shuffle(data.begin(), data.end());

        uint edges = 0;
        for (uint d = 1; d < depth; ++d) {
            edges += size[d] * (size[d - 1] + 1);
        }
        vector<uint> num;
        for (uint i = 1; i <= edges; ++i) {
            num.push_back(i);
        }
        random_shuffle(num.begin(), num.end());

        uint ind_edge = 0;
        w.resize(depth);
        for (int d = 1; d < depth; ++d) {
            w[d].resize(size[d]);
            for (uint j = 0; j < size[d]; ++j) {
                for (uint i = 0; i < size[d - 1] + 1; ++i) {
                    w[d][j].push_back(-0.5 + (double)1 / (edges + 1) * num[ind_edge]);
                    ++ind_edge;
                }
            }
        }
        train();
    }

    void write_network() {
        for (int d = 1; d < depth; ++d) {
            for (uint j = 0; j < size[d]; ++j) {
                for (uint i = 0; i < size[d - 1] + 1; ++i) {
                    cout << fixed << setprecision(20) << w[d][j][i] << " ";
                }
                cout << "\n";
            }
        }
    }

private:

    uint depth, m, n;
    vector<uint> size;
    vector<vector<vector<double>>> w;
    vector<object> data;
    mt19937 gen = mt19937(time(0));
    clock_t start;

    inline double activate(double x) {
        return (exp(x) - exp(-x)) / (exp(x) + exp(-x));
    }

    inline double activ_derivate(double x) {
        return (1 + activate(x)) * (1 - activate(x));
    }

    inline uint get_class(double x) {
        return x == 0 ? 2 : (x > 0 ? 1 : 0);
    }

    void predict(uint obj, vector<vector<double>>& in, vector<vector<double>>& out) {
        for (uint j = 0; j < m; ++j) {
            out[0][j] = data[obj].f[j];
        }
        out[0][m] = 1;
        for (uint d = 1; d < depth; ++d) {
            for (uint j = 0; j < size[d]; ++j) {
                in[d][j] = 0;
                for (uint i = 0; i < size[d - 1] + 1; ++i) {
                    in[d][j] += w[d][j][i] * out[d - 1][i];
                }
                out[d][j] = activate(in[d][j]);
            }
            out[d][size[d]] = 1;
        }
    }

    void train() {
        uint iter = 0;
        double s = 0.0015;
        vector<vector<double>> cm(3);
        for (uint i = 0; i < 3; ++i) {
            cm[i].resize(3);
        }
        vector<vector<double>> in(depth), out(depth);
        for (uint d = 0; d < depth; ++d) {
            in[d].resize(size[d]);
            out[d].resize(size[d] + 1);
        }
        vector<vector<double>> sigma(depth);
        for (uint d = 0; d < depth; ++d) {
            sigma[d].resize(size[d] + 1);
        }

        double save_f1 = 0, save_s = 0;

        while (/*true*/(double)(clock() - start) / CLOCKS_PER_SEC < 9.8) {

            // checking
            for (uint i = 0; i < 3; ++i) {
                for (uint j = 0; j < 3; ++j) {
                    cm[i][j] = 0;
                }
            }
            for (uint obj = 0; obj < n; ++obj) {
                predict(obj, in, out);
                ++cm[data[obj].c][get_class(out[depth - 1][0])];
            }
            double f1 = score::micro_f1(3, n, cm);
            if (f1 == 1) {
                //cout << "success!\n";
                return;
            }


            if (f1 < save_f1) {
                for (int d = 1; d < depth; ++d) {
                    for (uint j = 0; j < size[d]; ++j) {
                        for (uint i = 0; i < size[d - 1] + 1; ++i) {
                            w[d][j][i] -= save_s * sigma[d][j] * activ_derivate(in[d][j]) * out[d - 1][i];
                        }
                    }
                }
                f1 = save_f1;
            }
            //cout << "f1: " << f1 << " (iter = " << iter << ", s = " << fixed << setprecision(10) << s << ")\n";
            save_f1 = f1;

            // iteration
            for (uint obj = 0; obj < n; ++obj) {
                // predict --->
                predict(obj, in, out);
                double final_out = out[depth - 1][0];

                // ????
                double err = 0;
                if (data[obj].c == 1) {
                    err =  1 - final_out;
                }
                if (data[obj].c == 0){
                    err = -1 - final_out;
                }
                if (err == 0) {
                    continue;
                }

                // <--- sigma
                sigma[depth - 1][0] = err;
                for (uint d = depth - 1; d > 0; --d) {
                    for (uint i = 0; i < size[d - 1] + 1; ++i) {
                        sigma[d - 1][i] = 0;
                        for (uint j = 0; j < size[d]; ++j) {
                            sigma[d - 1][i] += sigma[d][j] * w[d][j][i];
                        }
                    }
                }

                // weights --->
                for (int d = 1; d < depth; ++d) {
                    for (uint j = 0; j < size[d]; ++j) {
                        for (uint i = 0; i < size[d - 1] + 1; ++i) {
                            w[d][j][i] += s * sigma[d][j] * activ_derivate(in[d][j]) * out[d - 1][i];
                        }
                    }
                }
            }

            // changing
            save_s = s;
            s *= 0.9999;
            ++iter;
        }

    }
};


// Main

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);
    srand((time(0)));
    clock_t start = clock();

    //freopen("input.txt", "r", stdin);

    uint d;
    vector<uint> size;

    cin >> d;
    size.resize(d);
    for (uint i = 0; i < d; ++i) {
        cin >> size[i];
    }

    uint m = size[0];
    vector<double> min_v(m, 1000000000);
    vector<double> max_v(m, 0);

    uint n;
    vector<object> data;
    vector<double> f(m, 0);
    cin >> n;
    int c;
    for (uint i = 0; i < n; ++i) {
        for (uint j = 0; j < m; ++j) {
            cin >> f[j];
            min_v[j] = min(min_v[j], f[j]);
            max_v[j] = max(max_v[j], f[j]);
        }
        cin >> c;
        data.push_back({f, (c + 1) / 2});
    }


    for (uint j = 0; j < m; ++j) {
        max_v[j] -= min_v[j];
        if (max_v[j] > 0) {
            for (uint i = 0; i < n; ++i) {
                data[i].f[j] -= min_v[j];
                data[i].f[j] /= max_v[j];
            }
        }
    }

    dnn solver(d, size, m, n, data, start);
    solver.write_network();

    return 0;
}