// Solution
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int q;
        cin >> q;
        const ll INF = 4e18;
        ll L = 1, R = INF;
        vector<ll> ans;
        while (q--) {
            int type;
            cin >> type;
            if (type == 1) {
                ll a, b, n;
                cin >> a >> b >> n;
                ll l, r;
                if (n == 1) {
                    l = 1;
                    r = a;
                } else {
                    l = (n - 2) * (a - b) + a + 1;
                    r = (n - 1) * (a - b) + a;
                }
                ll newL = max(L, l);
                ll newR = min(R, r);
                if (newL <= newR) {
                    L = newL;
                    R = newR;
                    ans.push_back(1);
                } else {
                    ans.push_back(0);
                }
            } else {
                ll a, b;
                cin >> a >> b;
                auto days = [&](ll h) -> ll {
                    if (h <= a) return 1;
                    ll diff = a - b;
                    return (h - a + diff - 1) / diff + 1;
                };
                ll dL = days(L);
                ll dR = days(R);
                if (dL == dR) {
                    ans.push_back(dL);
                } else {
                    ans.push_back(-1);
                }
            }
        }
        for (size_t i = 0; i < ans.size(); ++i) {
            if (i > 0) cout << " ";
            cout << ans[i];
        }
        cout << "\n";
    }
    return 0;
}
