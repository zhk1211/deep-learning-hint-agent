// Hint11
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        
        long long total = 0;
        for (int x : a) total += x;
        
        vector<long long> gain;
        for (int i = 0; i < n; ++i) {
            gain.push_back(a[i] + i);
        }
        sort(gain.rbegin(), gain.rend());
        
        long long sum_gain = 0;
        for (int i = 0; i < k; ++i) {
            sum_gain += gain[i];
        }
        
        long long penalty = (long long)k * (k - 1) / 2;
        long long ans = total - sum_gain + penalty;
        cout << ans << '\n';
    }
    return 0;
}
