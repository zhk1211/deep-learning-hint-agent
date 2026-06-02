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
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        
        // We will choose k traps to jump over.
        // If we jump over a trap at position i, we save a[i] damage but we also avoid the bonus damage
        // that would have been added to all subsequent traps.
        // The total damage if we jump over a set S of traps:
        // sum_{i not in S} a[i] + sum_{i not in S} (number of jumped traps before i)
        // = sum_{i=1..n} a[i] - sum_{i in S} a[i] + sum_{i not in S} (number of jumped traps before i)
        // The bonus part can be reorganized: each jumped trap at position j contributes 1 bonus damage
        // to each trap after j that is not jumped.
        // So total damage = sum a[i] - sum_{i in S} a[i] + sum_{j in S} (number of non-jumped traps after j)
        // = sum a[i] - sum_{i in S} a[i] + sum_{j in S} ((n - j) - (number of jumped traps after j))
        // = sum a[i] - sum_{i in S} a[i] + sum_{j in S} (n - j) - sum_{j in S} (number of jumped traps after j)
        // The last term sum_{j in S} (number of jumped traps after j) = C(|S|, 2) = k*(k-1)/2
        // So total damage = sum a[i] - sum_{i in S} a[i] + sum_{j in S} (n - j) - k*(k-1)/2
        // = sum a[i] - sum_{i in S} (a[i] - (n - i)) - k*(k-1)/2
        // Note: positions are 1-indexed in the formula, so i from 1..n.
        // We want to minimize damage, which is equivalent to maximizing sum_{i in S} (a[i] - (n - i))
        // because sum a[i] and -k*(k-1)/2 are constant.
        // So we compute value[i] = a[i] - (n - i) for i=1..n, sort descending, take top k values.
        
        long long total_a = accumulate(a.begin(), a.end(), 0LL);
        vector<long long> vals(n);
        for (int i = 0; i < n; ++i) {
            // i is 0-indexed, so position = i+1
            vals[i] = a[i] - (n - (i + 1));
        }
        sort(vals.begin(), vals.end(), greater<long long>());
        long long sum_top_k = 0;
        for (int i = 0; i < k; ++i) {
            sum_top_k += vals[i];
        }
        long long ans = total_a - sum_top_k - (long long)k * (k - 1) / 2;
        cout << ans << '\n';
    }
    return 0;
}
