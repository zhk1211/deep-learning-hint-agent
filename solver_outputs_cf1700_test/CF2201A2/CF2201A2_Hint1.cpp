// Hint1
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    vector<int> stk;
    stk.reserve(n);
    vector<int> L(n), R(n);
    for (int i = 0; i < n; ++i) {
        while (!stk.empty() && a[stk.back()] < a[i]) stk.pop_back();
        L[i] = stk.empty() ? -1 : stk.back();
        stk.push_back(i);
    }
    stk.clear();
    for (int i = n - 1; i >= 0; --i) {
        while (!stk.empty() && a[stk.back()] <= a[i]) stk.pop_back();
        R[i] = stk.empty() ? n : stk.back();
        stk.push_back(i);
    }

    vector<int> nxt(n, n);
    stk.clear();
    for (int i = n - 1; i >= 0; --i) {
        while (!stk.empty() && a[stk.back()] < a[i]) stk.pop_back();
        if (!stk.empty()) nxt[i] = stk.back();
        stk.push_back(i);
    }

    vector<int> prv(n, -1);
    stk.clear();
    for (int i = 0; i < n; ++i) {
        while (!stk.empty() && a[stk.back()] <= a[i]) stk.pop_back();
        if (!stk.empty()) prv[i] = stk.back();
        stk.push_back(i);
    }

    ll ans = 0;
    for (int i = 0; i < n; ++i) {
        int l = L[i], r = R[i];
        ll cnt = 1LL * (i - l) * (r - i);
        ans += cnt;

        int p = prv[i];
        if (p != -1) {
            int rr = min(r, nxt[p]);
            if (rr > i) {
                ll bad = 1LL * (p - max(l, prv[p])) * (rr - i);
                ans -= bad;
            }
        }
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) solve();
    return 0;
}
