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
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    
    long long ans = 0;
    
    for (int bit = 0; bit < MAX_BITS; bit++) {
        vector<int> b(n);
        for (int i = 0; i < n; i++) {
            b[i] = (a[i] >> bit) & 1;
        }
        
        vector<int> pref(n + 1, 0);
        for (int i = 0; i < n; i++) {
            pref[i + 1] = pref[i] ^ b[i];
        }
        
        vector<long long> cnt0(n + 2, 0), cnt1(n + 2, 0);
        vector<long long> sum0(n + 2, 0), sum1(n + 2, 0);
        
        for (int r = 0; r <= n; r++) {
            cnt0[r + 1] = cnt0[r];
            cnt1[r + 1] = cnt1[r];
            sum0[r + 1] = sum0[r];
            sum1[r + 1] = sum1[r];
            
            if (pref[r] == 0) {
                cnt0[r + 1]++;
                sum0[r + 1] += r;
            } else {
                cnt1[r + 1]++;
                sum1[r + 1] += r;
            }
        }
        
        long long bit_contrib = 0;
        for (int r = 1; r <= n; r++) {
            long long cnt, sum;
            if (pref[r] == 0) {
                cnt = cnt1[r];
                sum = sum1[r];
            } else {
                cnt = cnt0[r];
                sum = sum0[r];
            }
            bit_contrib = (bit_contrib + (1LL * r * cnt - sum) % MOD) % MOD;
        }
        
        ans = (ans + bit_contrib * ((1LL << bit) % MOD)) % MOD;
    }
    
    cout << (ans % MOD + MOD) % MOD << "\n";
    
    return 0;
}
