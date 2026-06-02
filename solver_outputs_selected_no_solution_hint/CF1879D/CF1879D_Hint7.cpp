#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    
    long long ans = 0;
    
    for (int bit = 0; bit < 30; ++bit) {
        vector<int> b(n);
        for (int i = 0; i < n; ++i) {
            b[i] = (a[i] >> bit) & 1;
        }
        
        vector<int> pref(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            pref[i + 1] = pref[i] ^ b[i];
        }
        
        long long cnt0 = 0, cnt1 = 0;
        long long sum0 = 0, sum1 = 0;
        
        for (int r = 0; r <= n; ++r) {
            if (pref[r] == 0) {
                ans = (ans + ((1LL << bit) % MOD) * ((cnt1 * r - sum1) % MOD)) % MOD;
                cnt0++;
                sum0 += r;
            } else {
                ans = (ans + ((1LL << bit) % MOD) * ((cnt0 * r - sum0) % MOD)) % MOD;
                cnt1++;
                sum1 += r;
            }
        }
    }
    
    cout << (ans % MOD + MOD) % MOD << "\n";
    return 0;
}
