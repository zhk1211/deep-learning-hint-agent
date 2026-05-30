// Hint1
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string s;
    cin >> s;
    int n = (int)s.size();
    
    vector<long long> pow10(n + 1);
    pow10[0] = 1;
    for (int i = 1; i <= n; i++) {
        pow10[i] = (pow10[i - 1] * 10) % MOD;
    }
    
    vector<long long> pref(n + 1, 0);
    for (int i = 0; i < n; i++) {
        pref[i + 1] = (pref[i] * 10 + (s[i] - '0')) % MOD;
    }
    
    long long ans = 0;
    long long sum_right = 0;
    
    for (int i = n - 1; i >= 0; i--) {
        int digit = s[i] - '0';
        long long left_ways = i;
        long long right_len = n - 1 - i;
        
        long long contrib = (pref[i] * pow10[right_len]) % MOD;
        contrib = (contrib * left_ways) % MOD;
        ans = (ans + contrib) % MOD;
        
        long long right_contrib = (digit * sum_right) % MOD;
        ans = (ans + right_contrib) % MOD;
        
        sum_right = (sum_right + pow10[n - 1 - i]) % MOD;
    }
    
    cout << ans % MOD << "\n";
    return 0;
}
