// Hint3
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll INF = 1e18;

struct Interval {
    ll L, R;
    Interval() : L(1), R(INF) {}
    Interval(ll l, ll r) : L(l), R(r) {}
};

Interval intersect(const Interval& a, const Interval& b) {
    return Interval(max(a.L, b.L), min(a.R, b.R));
}

bool valid(const Interval& a) {
    return a.L <= a.R;
}

ll days_for(ll a, ll b, ll h) {
    if (h <= a) return 1;
    ll d = (h - a + (a - b) - 1) / (a - b);
    return d + 1;
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
                ll L, R;
                if (n == 1) {
                    L = 1;
                    R = a;
                } else {
                    L = (n - 2) * (a - b) + a + 1;
                    R = (n - 1) * (a - b) + a;
                }
                Interval new_int(L, R);
                Interval cand = intersect(cur, new_int);
                if (valid(cand)) {
                    cur = cand;
                    ans.push_back(1);
                } else {
                    ans.push_back(0);
                }
            } else {
                ll a, b;
                cin >> a >> b;
                if (cur.R == INF) {
                    ans.push_back(-1);
                } else {
                    ll d1 = days_for(a, b, cur.L);
                    ll d2 = days_for(a, b, cur.R);
                    if (d1 == d2) ans.push_back((int)d1);
                    else ans.push_back(-1);
                }
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
