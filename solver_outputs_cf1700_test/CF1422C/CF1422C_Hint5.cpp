// Hint5
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
        pow10[i] = (pow10[i-1] * 10) % MOD;
    }
    
    vector<long long> pref(n + 1, 0);
    for (int i = 0; i < n; i++) {
        pref[i+1] = (pref[i] * 10 + (s[i] - '0')) % MOD;
    }
    
    long long ans = 0;
    long long sum_right = 0;
    
    for (int i = n - 1; i >= 0; i--) {
        int digit = s[i] - '0';
        long long left_ways = i;
        long long right_len = n - 1 - i;
        
        long long contrib_left = (left_ways * (left_ways + 1) / 2) % MOD;
        contrib_left = (contrib_left * digit) % MOD;
        contrib_left = (contrib_left * pow10[right_len]) % MOD;
        
        long long contrib_right = (digit * sum_right) % MOD;
        
        ans = (ans + contrib_left + contrib_right) % MOD;
        
        sum_right = (sum_right + (right_len + 1) * pow10[right_len]) % MOD;
    }
    
    cout << ans << '\n';
    return 0;
}
