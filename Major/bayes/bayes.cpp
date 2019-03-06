#include <cmath>
#include <ctime>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

typedef unsigned int uint;


struct letter {
    uint k;
    char c;
    vector<uint> w;
};

uint n, t;
vector<letter> train, test;
uint counter[2][1000001];
uint letters[2], all_letters;
uint uniq_amount;
uint amount[2];

const uint B = 6;
double border;
vector<double> bord = {1.04, 1.045, 1.05, 1.055, 1.06, 1.065};
vector<uint> b_suc(B, 0);
vector<bool> b_fail(B, false);
uint max_suc;


int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);
    srand(time(0));

    // --------------- reading ----------------

    cin >> n;
    train.resize(n);
    for (uint i = 0; i < n; ++i) {
        cin >> train[i].k >> train[i].c;
        train[i].w.resize(train[i].k);
        for (uint j = 0; j < train[i].k; ++j) {
            cin >> train[i].w[j];
        }
    }

    cin >> t;
    test.resize(t);
    for (uint i = 0; i < t; ++i) {
        cin >> test[i].k;
        test[i].w.resize(test[i].k);
        for (uint j = 0; j < test[i].k; ++j) {
            cin >> test[i].w[j];
        }
    }

    // --------------- counting ---------------

    for (uint i = 0; i < n; ++i) {
        bool is_leg = train[i].c == 'L';
        ++letters[is_leg];
        ++all_letters;
        for (uint j = 0; j < train[i].k; ++j) {
            if (!counter[false][train[i].w[j]] && !counter[true][train[i].w[j]]) {
                ++uniq_amount;
            }
            ++amount[is_leg];
            ++counter[is_leg][train[i].w[j]];
        }
    }

    // --------------- training ---------------

    random_shuffle(bord.begin(), bord.end());
    for (uint i = 0; i < n; ++i) {
        double p_leg_log = log((double)letters[true] / all_letters);
        double p_spam_log = log((double)letters[false] / all_letters);
        for (uint j = 0; j < train[i].k; ++j) {
            p_leg_log += log(((double)(counter[true][train[i].w[j]]) + 0.01) / (amount[true] + 0.01 * (double)uniq_amount));
            p_spam_log += log(((double)(counter[false][train[i].w[j]]) + 0.01) / (amount[false] + 0.01 * (double)uniq_amount));
        }
        double p_log = p_leg_log / p_spam_log;
        bool is_leg = train[i].c == 'L';
        for (uint j = 0; j < B; ++j) {
            if ((is_leg && p_log < bord[j]) || (!is_leg && p_log >= bord[j])) {
                ++b_suc[j];
            }
            if (is_leg && p_log >= bord[j]) {
                b_fail[j] = true;
            }
        }
        for (uint j = 0; j < B; ++j) {
            if (!b_fail[j] && b_suc[j] >= max_suc) {
                max_suc = b_suc[j];
                border = bord[j];
            }
        }
    }

    // --------------- choosing ---------------

    double p_leg_log, p_spam_log;
    for (uint i = 0; i < t; ++i) {
        p_leg_log = log((double)letters[true] / all_letters);
        p_spam_log = log((double)letters[false] / all_letters);
        for (uint j = 0; j < test[i].k; ++j) {
            p_leg_log += log(((double)(counter[true][test[i].w[j]]) + 0.01) / (amount[true] + 0.01 * (double)uniq_amount));
            p_spam_log += log(((double)(counter[false][test[i].w[j]]) + 0.01) / (amount[false] + 0.01 * (double)uniq_amount));
        }
        if (p_leg_log / p_spam_log < border) {
            cout << "L\n";
        } else {
            cout << "S\n";
        }
    }

    return 0;
}