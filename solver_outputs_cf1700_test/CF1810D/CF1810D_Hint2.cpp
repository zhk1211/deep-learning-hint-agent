// Hint2
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll INF = 1e18;

struct Interval {
    ll L, R;
    Interval() : L(1), R(INF) {}
    Interval(ll l, ll r) : L(l), R(r) {}
    bool empty() const { return L > R; }
};

Interval intersect(const Interval& a, const Interval& b) {
    return Interval(max(a.L, b.L), min(a.R, b.R));
}

Interval snail_interval(ll a, ll b, ll n) {
    if (n == 1) {
        return Interval(1, a);
    }
    ll base = (n - 2) * (a - b) + a;
    return Interval(base + 1, base + (a - b));
}

ll days_needed(ll a, ll b, ll L, ll R) {
    if (L > R) return -1;
    if (a >= L) return 1;
    ll per_day = a - b;
    ll need = L - a;
    ll days = (need + per_day - 1) / per_day + 1;
    ll max_h = (days - 1) * per_day + a;
    if (max_h >= L && max_h <= R) return days;
    if (max_h < L) return -1;
    if (days == 1) {
        if (a >= L && a <= R) return 1;
        return -1;
    }
    ll min_h = (days - 2) * per_day + a + 1;
    if (min_h > R) return -1;
    return days;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int q;
        cin >> q;
        Interval cur;
        vector<int> ans;
        while (q--) {
            int type;
            cin >> type;
            if (type == 1) {
                ll a, b, n;
                cin >> a >> b >> n;
                Interval iv = snail_interval(a, b, n);
                Interval inter = intersect(cur, iv);
                if (inter.empty()) {
                    ans.push_back(0);
                } else {
                    cur = inter;
                    ans.push_back(1);
                }
            } else {
                ll a, b;
                cin >> a >> b;
                ll res = days_needed(a, b, cur.L, cur.R);
                ans.push_back(res);
            }
        }
        for (size_t i = 0; i < ans.size(); ++i) {
            if (i) cout << ' ';
            cout << ans[i];
        }
        cout << '\n';
    }
    return 0;
}
