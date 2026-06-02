#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        
        map<int, int> freq;
        for (int x : a) freq[x]++;
        
        vector<int> counts;
        for (auto& p : freq) counts.push_back(p.second);
        sort(counts.rbegin(), counts.rend());
        
        int max_count = counts[0];
        int num_max = 0;
        for (int c : counts) if (c == max_count) num_max++;
        
        int ans = (n - num_max * max_count) / (max_count - 1) + (num_max - 1);
        cout << ans << '\n';
    }
    return 0;
}
