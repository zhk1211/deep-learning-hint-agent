#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
const int MAX_BITS = 30;

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
    
    for (int bit = 0; bit < MAX_BITS; ++bit) {
        vector<int> b(n);
        for (int i = 0; i < n; ++i) {
            b[i] = (a[i] >> bit) & 1;
        }
        
        vector<long long> pref(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            pref[i + 1] = pref[i] ^ b[i];
        }
        
        long long sum0 = 0, sum1 = 0;
        long long cnt0 = 0, cnt1 = 0;
        long long total = 0;
        
        for (int r = 0; r <= n; ++r) {
            if (pref[r] == 0) {
                total = (total + cnt1 * r - sum1) % MOD;
                cnt0++;
                sum0 += r;
            } else {
                total = (total + cnt0 * r - sum0) % MOD;
                cnt1++;
                sum1 += r;
            }
        }
        
        ans = (ans + total * (1LL << bit)) % MOD;
    }
    
    cout << (ans % MOD + MOD) % MOD << '\n';
    return 0;
}
