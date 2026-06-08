#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    vector<ll> pref(n + 1, 0);
    for (int i = 0; i < n; ++i) pref[i + 1] = pref[i] + a[i];
    ll total = pref[n];
    auto sum_cycle = [&](ll k) -> ll {
        if (k <= 0) return 0;
        ll full = k / n;
        ll rem = k % n;
        return full * total + pref[rem];
    };
    auto query = [&](ll idx) -> ll {
        // 1-indexed idx in b
        ll block = (idx - 1) / n;
        ll pos = (idx - 1) % n;
        ll start = block + pos;
        if (start < n) {
            return sum_cycle(start + 1) - sum_cycle(start);
        } else {
            ll first_part = total - sum_cycle(block);
            ll second_part = sum_cycle(start - n + 1);
            return first_part + second_part;
        }
    };
    while (q--) {
        ll l, r;
        cin >> l >> r;
        ll ans = 0;
        // We can compute prefix sum of b up to index x using query
        auto pref_b = [&](ll x) -> ll {
            if (x <= 0) return 0;
            ll full_blocks = x / n;
            ll rem = x % n;
            ll sum_full = 0;
            // sum of first full_blocks blocks
            // each block is a cyclic shift sum = total
            // but careful: the blocks are c_1, c_2, ..., c_n, then repeat?
            // Actually b = c_1 + c_2 + ... + c_n, so there are exactly n blocks.
            // So full_blocks can be > n? No, x <= n^2, so full_blocks <= n.
            // So we can just sum the first full_blocks cyclic shifts.
            // We can compute sum of first k cyclic shifts efficiently.
            // sum of c_i = total for all i.
            // So sum_full = full_blocks * total.
            sum_full = full_blocks * total;
            // Now add the partial block (the (full_blocks+1)-th cyclic shift)
            ll part = 0;
            if (rem > 0) {
                ll block_idx = full_blocks; // 0-indexed block
                // c_{block_idx+1}
                ll start = block_idx;
                if (start + rem <= n) {
                    part = pref[start + rem] - pref[start];
                } else {
                    part = total - (pref[start] - pref[0]) + pref[start + rem - n];
                }
            }
            return sum_full + part;
        };
        ans = pref_b(r) - pref_b(l - 1);
        cout << ans << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) solve();
    return 0;
}
