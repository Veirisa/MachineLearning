#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <array>
#include <cstdlib>
using namespace std;

typedef unsigned int uint;


// DT

struct object {
    vector<int> f;
    uint c;
};

struct dt {

    dt(uint mm, uint kk, uint nn, const vector<object>& ddata)
        : m(mm), k(kk), n(nn), data(ddata) {

        // sort data
        vector<vector<feature>> d;
        d.resize(m);
        for (uint j = 0; j < m; ++j) {
            for (uint i = 0; i < n; ++i) {
                d[j].push_back({i, data[i].f[j], data[i].c});
            }
            sort(d[j].begin(), d[j].end(), f_cmp);
        }

        // create tree
        vector<uint> count(k, 0);
        for (uint i = 0; i < n; ++i) {
            ++count[data[i].c];
        }
        create_tree(root, n, d, get_entropy(count, n), 0);
    }

    void write_tree() {
        cout << amount << "\n";
        write_tree(root);
    }

private:

    struct feature {
        uint obj_ind;
        int f;
        uint c;
    };

    uint m, k, n;
    vector<object> data;

    struct node {
        uint num;
        uint j;
        int f;
        uint c;
        node *l, *r;

        node() : num(), f(), c(), l(nullptr), r(nullptr) {};
        node(uint nnum) : num(nnum), f(), c(), l(nullptr), r(nullptr) {};
    };

    uint amount = 1;
    node *root = new node(1);


    static bool f_cmp(const feature& x, const feature& y) {
        return x.f < y.f;
    }

    double get_entropy(const vector<uint>& count, uint den) {
        double entr = 0, p;
        for (uint w = 0; w < k; ++w) {
            if (count[w] > 0) {
                p = (double)count[w] / den;
                entr -= p * log2(p);
            }
        }
        return entr;
    }

    uint choose_c(uint size, const vector<vector<feature>>& d) {
        vector<uint> count(k, 0);
        for (uint i = 0; i < size; ++i) {
            ++count[d[0][i].c];
        }
        uint max_count = 0;
        for (uint w = 0; w < k; ++w) {
            max_count = max(max_count, count[w]);
        }
        vector<uint> best_c;
        for (uint w = 0; w < k; ++w) {
            if (count[w] == max_count) {
                best_c.push_back(w);
            }
        }
        return best_c[rand() % best_c.size()];
    }

    void create_tree(node* cur_node, uint size, const vector<vector<feature>>& d, double s, uint depth) {

        // leaf
        if (s == 0) {
            cur_node->c = d[0][0].c;
            return;
        }
        if (depth == 10) {
            cur_node->c = choose_c(size, d);
            return;
        }

        // choose splitting
        uint best_j = 0;           // f number
        int best_f = 0;            // f value
        double best_gain = 0;      // info gain
        double best_left_s = 0, best_right_s = 0;
        uint best_left_size = 0, best_right_size = 0;
        for (uint j = 0; j < m; ++j) {
            vector<uint> left_count(k, 0);
            vector<uint> right_count(k, 0);
            for (uint i = 0; i < size; ++i) {
                ++right_count[d[j][i].c];
            }
            for (uint i = 1; i < size; ++i) {
                ++left_count[d[j][i - 1].c];
                --right_count[d[j][i - 1].c];
                if (d[j][i - 1].f == d[j][i].f) {
                    continue;
                }
                double left_s = get_entropy(left_count, i);
                double right_s = get_entropy(right_count, size - i);
                double gain = s - (double)i / size * left_s - (double)(size - i) / size * right_s;
                if (gain > best_gain) {
                    best_j = j;
                    best_f = d[j][i].f;
                    best_gain = gain;
                    best_left_s = left_s;
                    best_right_s = right_s;
                    best_left_size = i;
                    best_right_size = size - i;
                }
            }
        }
        if (best_gain == 0) {
            cur_node->c = choose_c(size, d);
            return;
        }

        // do spliting
        vector<vector<feature>> left_d, right_d;
        left_d.resize(m);
        right_d.resize(m);
        for (uint j = 0; j < m; ++j) {
            for (uint i = 0; i < size; ++i) {
                uint ind = d[j][i].obj_ind;
                if (data[ind].f[best_j] < best_f) {
                    left_d[j].push_back(d[j][i]);
                } else {
                    right_d[j].push_back(d[j][i]);
                }
            }
        }

        // create node
        cur_node->j = best_j;
        cur_node->f = best_f;
        ++amount;
        cur_node->l = new node(amount);
        create_tree(cur_node->l, best_left_size, left_d, best_left_s, depth + 1);
        ++amount;
        cur_node->r = new node(amount);
        create_tree(cur_node->r, best_right_size, right_d, best_right_s, depth + 1);
    }

    void write_tree(node* cur_node) {
        if (cur_node->l == nullptr && cur_node->r == nullptr) {
            cout << "C " << cur_node->c + 1 << "\n";
            return;
        }
        cout << "Q " << cur_node->j + 1 << " " << cur_node->f << " "
             << cur_node->l->num << " " << cur_node->r->num << "\n";
        write_tree(cur_node->l);
        write_tree(cur_node->r);
    }

};


// Main

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);
    srand((time(0)));

    uint m, k, n;
    cin >> m >> k >> n;

    vector<object> data;
    vector<int> f(m, 0);
    uint c;
    for (uint i = 0; i < n; ++i) {
        for (uint j = 0; j < m; ++j) {
            cin >> f[j];
        }
        cin >> c;
        data.push_back({f, c - 1});
    }

    dt solver{m, k, n, data};
    solver.write_tree();

    return 0;
}