// Hint1
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
        
        long long sum0 = 0, sum1 = 0;
        long long cnt0 = 0, cnt1 = 0;
        long long total = 0;
        int pref = 0;
        
        for (int i = 0; i < n; ++i) {
            if (pref == 0) {
                cnt0++;
                sum0 = (sum0 + i) % MOD;
            } else {
                cnt1++;
                sum1 = (sum1 + i) % MOD;
            }
            
            pref ^= b[i];
            
            if (pref == 0) {
                long long add = (cnt1 * (i + 1) - sum1) % MOD;
                if (add < 0) add += MOD;
                total = (total + add) % MOD;
            } else {
                long long add = (cnt0 * (i + 1) - sum0) % MOD;
                if (add < 0) add += MOD;
                total = (total + add) % MOD;
            }
        }
        
        ans = (ans + total * (1LL << bit)) % MOD;
    }
    
    cout << ans << '\n';
    return 0;
}
