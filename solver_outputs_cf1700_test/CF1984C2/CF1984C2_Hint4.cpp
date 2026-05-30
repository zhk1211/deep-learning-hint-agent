// Hint4
#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        
        long long sum = 0, min_sum = 0;
        for (int i = 0; i < n; ++i) {
            sum += a[i];
            min_sum = min(min_sum, sum);
        }
        
        long long max_c = sum - 2 * min_sum;
        
        if (max_c == sum) {
            long long ans = 1;
            for (int i = 0; i < n; ++i) {
                ans = (ans * 2) % MOD;
            }
            cout << ans << '\n';
            continue;
        }
        
        long long cur = 0;
        long long best = 0;
        int first_idx = -1;
        for (int i = 0; i < n; ++i) {
            cur += a[i];
            if (cur == min_sum) {
                first_idx = i;
                break;
            }
        }
        
        long long ans = 1;
        for (int i = 0; i < first_idx; ++i) {
            ans = (ans * 2) % MOD;
        }
        
        ans = (ans * 2) % MOD;
        
        long long cur2 = 0;
        for (int i = first_idx + 1; i < n; ++i) {
            cur2 += a[i];
            if (cur2 == 0) {
                ans = (ans * 2) % MOD;
            }
        }
        
        cout << ans << '\n';
    }
    
    return 0;
}
