// Solution
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string s;
    cin >> s;
    int m = (int)s.size();
    
    vector<long long> pow10(m + 1);
    pow10[0] = 1;
    for (int i = 1; i <= m; ++i) {
        pow10[i] = (pow10[i - 1] * 10) % MOD;
    }
    
    vector<long long> sum_j(m + 1, 0);
    for (int j = 0; j < m; ++j) {
        sum_j[j + 1] = (sum_j[j] + (j + 1) * pow10[j]) % MOD;
    }
    
    long long ans = 0;
    for (int i = 0; i < m; ++i) {
        int digit = s[i] - '0';
        int pos = i + 1;
        int right_len = m - pos;
        
        long long left_ways = (1LL * pos * (pos - 1) / 2) % MOD;
        long long left_contrib = (left_ways * pow10[right_len]) % MOD;
        left_contrib = (left_contrib * digit) % MOD;
        
        long long right_contrib = (sum_j[right_len] * digit) % MOD;
        
        ans = (ans + left_contrib + right_contrib) % MOD;
    }
    
    cout << ans << '\n';
    return 0;
}
