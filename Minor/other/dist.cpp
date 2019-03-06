#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
using namespace std;

typedef long long ll;
typedef unsigned int uint;


uint k, n;
vector<ll> obj_full;
vector<vector<ll>> obj;
ll in_dist, bet_dist;


ll calc_dist(vector<ll>& cur_obj) {
    ll cur_dist = 0;

    sort(cur_obj.begin(), cur_obj.end());

    ll add = 0, sub = 0;
    for (uint i = 0; i < cur_obj.size(); ++i) {
        add += cur_obj[i];
    }

    for (uint i = 0; i < cur_obj.size(); ++i) {
        add -= cur_obj[i];
        cur_dist += add;
        cur_dist -= sub;
        cur_dist += (ll)i * cur_obj[i];
        cur_dist -= (ll)(cur_obj.size() - 1 - i) * cur_obj[i];
        sub += cur_obj[i];
    }

    return cur_dist;
}

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);

    cin >> k >> n;
    obj.resize(k);
    ll x, y;
    for (uint i = 0; i < n; ++i) {
        cin >> x >> y;
        obj_full.push_back(x);
        obj[y - 1].push_back(x);
    }

    bet_dist = calc_dist(obj_full);
    for (uint j = 0; j < k; ++j) {
        in_dist += calc_dist(obj[j]);
    }
    bet_dist -= in_dist;

    cout << in_dist << "\n" << bet_dist;

    return 0;
}