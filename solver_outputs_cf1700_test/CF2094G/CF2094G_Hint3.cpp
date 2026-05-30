// Hint3
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

        vector<ll> a;
        ll sum = 0;
        ll rizz = 0;
        bool reversed = false;
        int shift = 0;

        auto get_idx = [&](int pos) -> int {
            int n = (int)a.size();
            if (!reversed) {
                return (shift + pos) % n;
            } else {
                return (shift - pos + n) % n;
            }
        };

        auto get_val = [&](int pos) -> ll {
            return a[get_idx(pos)];
        };

        for (int i = 0; i < q; ++i) {
            int s;
            cin >> s;
            if (s == 1) {
                if (!reversed) {
                    shift = (shift + 1) % (int)a.size();
                } else {
                    shift = (shift - 1 + (int)a.size()) % (int)a.size();
                }
                int n = (int)a.size();
                ll last_val = get_val(n - 1);
                rizz = rizz + sum - (ll)n * last_val;
            } else if (s == 2) {
                reversed = !reversed;
                int n = (int)a.size();
                rizz = (ll)(n + 1) * sum - rizz;
            } else {
                ll k;
                cin >> k;
                int n = (int)a.size();
                if (!reversed) {
                    a.push_back(k);
                } else {
                    a.insert(a.begin() + ((shift + 1) % (n + 1)), k);
                    if (shift >= (int)a.size() - 1) shift = 0;
                }
                sum += k;
                rizz += k * (n + 1);
            }
            cout << rizz << '\n';
        }
    }
    return 0;
}
