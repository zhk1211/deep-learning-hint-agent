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
    for (int i = 1; i <= m; i++) {
        pow10[i] = (pow10[i-1] * 10) % MOD;
    }
    
    vector<long long> pre(m + 1, 0);
    for (int j = 0; j < m; j++) {
        pre[j+1] = (pre[j] + (j + 1) * pow10[j]) % MOD;
    }
    
    long long ans = 0;
    for (int i = 0; i < m; i++) {
        int digit = s[i] - '0';
        long long left_ways = (long long)(i + 1) * i / 2 % MOD;
        long long left_contrib = left_ways * pow10[m - 1 - i] % MOD * digit % MOD;
        
        long long right_contrib = pre[m - 1 - i] * digit % MOD;
        
        ans = (ans + left_contrib + right_contrib) % MOD;
    }
    
    cout << ans << "\n";
    return 0;
}
