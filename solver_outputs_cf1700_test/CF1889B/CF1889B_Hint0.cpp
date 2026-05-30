// Hint0
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        ll c;
        cin >> n >> c;
        vector<ll> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        // We'll try to connect all nodes to node 1.
        // Sort other nodes by (i * c - a[i]) ascending.
        vector<pair<ll, int>> others;
        for (int i = 1; i < n; ++i) {
            others.emplace_back((i + 1LL) * c - a[i], i);
        }
        sort(others.begin(), others.end());

        ll sum = a[0];
        bool ok = true;
        for (auto &p : others) {
            int idx = p.second;
            if (sum + a[idx] >= (idx + 1LL) * c) {
                sum += a[idx];
            } else {
                ok = false;
                break;
            }
        }

        cout << (ok ? "YES" : "NO") << '\n';
    }

    return 0;
}
