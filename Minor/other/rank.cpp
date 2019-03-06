#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
using namespace std;

typedef unsigned int uint;
typedef unsigned long long ull;

struct str {
    double x;
    double y;
    double rank_x;
    double rank_y;
};

ull n;
double sum;
vector<str> obj;

bool cmp_x(const str& a, const str& b) {
    return a.x > b.x;
}

bool cmp_y(const str& a, const str& b) {
    return a.y > b.y;
}

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);

    cin >> n;
    double x, y;
    for (uint i = 0; i < n; ++i) {
        cin >> x >> y;
        obj.push_back({x, y});
    }

    uint eq = 0;
    double cur_rank = 1, eq_rank = 0;
    sort(obj.begin(), obj.end(), cmp_x);
    for (uint i = 0; i < n; ++i) {
        if (i > 0 && obj[i].x == obj[i - 1].x) {
            eq_rank += cur_rank;
            ++eq;
            ++cur_rank;
            continue;
        }
        if (eq > 1 && obj[i].x != obj[i - 1].x) {
            eq_rank /= eq;
            for (uint j = i - eq; j < i; ++j) {
                obj[j].rank_x = eq_rank;
            }
        }
        obj[i].rank_x = cur_rank;
        eq_rank = cur_rank;
        eq = 1;
        ++cur_rank;
    }

    eq = 0;
    cur_rank = 1, eq_rank = 0;
    sort(obj.begin(), obj.end(), cmp_y);
    for (uint i = 0; i < n; ++i) {
        if (i > 0 && obj[i].y == obj[i - 1].y) {
            eq_rank += cur_rank;
            ++eq;
            ++cur_rank;
            continue;
        }
        if (eq > 1 && obj[i].y != obj[i - 1].y) {
            eq_rank /= eq;
            for (uint j = i - eq; j < i; ++j) {
                obj[j].rank_y = eq_rank;
            }
        }
        obj[i].rank_y = cur_rank;
        eq_rank = cur_rank;
        eq = 1;
        ++cur_rank;
    }

    for (uint i = 0; i < n; ++i) {
        sum += (obj[i].rank_x - obj[i].rank_y) * (obj[i].rank_x - obj[i].rank_y);
    }

    cout << fixed << setprecision(10) << (n == 1 ? 1 : 1 - 6 * sum / n / (n * n - 1));

    return 0;
}