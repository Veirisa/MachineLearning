#include <iostream>
#include <iomanip>
using namespace std;

typedef unsigned int uint;

uint k;
double n;
const uint SIZE = 21;
double cm[SIZE][SIZE], w[SIZE];
double row_sum_cm[SIZE], column_sum_cm[SIZE];
double precision[SIZE], recall[SIZE];

double micro_f() {
    double aver_precision = 0, aver_recall = 0;
    double aver_f = 0;

    for (uint i = 0; i < k; ++i) {
        aver_precision += w[i] * precision[i];
        aver_recall += w[i] * recall[i];
    }
    aver_precision /= n;
    aver_recall /= n;

    double denominator = aver_precision + aver_recall;
    aver_f = denominator == 0 ? 0 : 2 * aver_precision * aver_recall / denominator;

    return aver_f;
}

double macro_f() {
    double aver_f = 0;

    double denominator;
    for (uint i = 0; i < k; ++i) {
        denominator = precision[i] + recall[i];
        aver_f += denominator == 0 ? 0 : w[i] * 2 * precision[i] * recall[i] / denominator;
    }
    aver_f /= n;

    return aver_f;
}

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);

    cin >> k;
    for (uint i = 0; i < k; ++i) {
        for (uint j = 0; j < k; ++j) {
            cin >> cm[i][j];
            n += cm[i][j];
            w[i] += cm[i][j];
            row_sum_cm[i] += cm[i][j];
            column_sum_cm[j] += cm[i][j];
        }
    }

    for (uint i = 0; i < k; ++i) {
        precision[i] = row_sum_cm[i] == 0 ? 0 : cm[i][i] / row_sum_cm[i];
        recall[i] = column_sum_cm[i] == 0 ? 0 : cm[i][i] / column_sum_cm[i];
    }

    cout << fixed << setprecision(8) << (n == 0 ? 0 : micro_f()) << "\n";
    cout << fixed << setprecision(8) << (n == 0 ? 0 : macro_f()) << "\n";

    return 0;
}