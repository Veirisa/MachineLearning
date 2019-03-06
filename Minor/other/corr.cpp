#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
using namespace std;

typedef unsigned int uint;


uint n;
double mid_x, mid_y;
vector<double> x, x_dif;
vector<double> y, y_dif;

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);

    cin >> n;
    x.resize(n);
    y.resize(n);
    for (uint i = 0; i < n; ++i) {
        cin >> x[i] >> y[i];
        mid_x += x[i];
        mid_y += y[i];
    }
    mid_x /= n;
    mid_y /= n;

    for (uint i = 0; i < n; ++i) {
        x_dif.push_back(x[i] - mid_x);
        y_dif.push_back(y[i] - mid_y);
    }

    double num = 0;
    double den_x = 0, den_y = 0;
    for (uint i = 0; i < n; ++i) {
        num += x_dif[i] * y_dif[i];
        den_x += x_dif[i] * x_dif[i];
        den_y += y_dif[i] * y_dif[i];
    }

    cout << fixed << setprecision(10) << (num == 0 ? 0 : num / sqrt(den_x * den_y));

    return 0;
}