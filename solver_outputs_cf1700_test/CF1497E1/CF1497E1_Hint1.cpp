// Hint1
#include <bits/stdc++.h>
using namespace std;

const int MAX_A = 10000000;
vector<int> spf(MAX_A + 1);

void sieve() {
    for (int i = 2; i <= MAX_A; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((long long)i * i <= MAX_A) {
                for (int j = i * i; j <= MAX_A; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }
}

int normalize(int x) {
    int res = 1;
    while (x > 1) {
        int p = spf[x];
        if (p == 0) p = x;
        int cnt = 0;
        while (x % p == 0) {
            x /= p;
            cnt ^= 1;
        }
        if (cnt) res *= p;
    }
    return res;
}

void solve() {
    int t;
    cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
            a[i] = normalize(a[i]);
        }
        int segments = 1;
        unordered_set<int> seen;
        for (int i = 0; i < n; ++i) {
            if (seen.count(a[i])) {
                segments++;
                seen.clear();
            }
            seen.insert(a[i]);
        }
        cout << segments << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    sieve();
    solve();
    return 0;
}
