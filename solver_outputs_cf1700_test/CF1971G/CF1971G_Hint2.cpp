// Hint2
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        
        // Group numbers by their value with last two bits removed
        map<int, vector<int>> groups;
        for (int x : a) {
            int key = x >> 2;
            groups[key].push_back(x);
        }
        
        // Sort each group's values
        for (auto& p : groups) {
            sort(p.second.begin(), p.second.end());
        }
        
        // For each position, we need the smallest available number from its group
        // We'll use an iterator for each group to pick in sorted order
        map<int, size_t> idx;
        vector<int> ans(n);
        for (int i = 0; i < n; ++i) {
            int key = a[i] >> 2;
            ans[i] = groups[key][idx[key]++];
        }
        
        for (int i = 0; i < n; ++i) {
            cout << ans[i] << " \n"[i == n - 1];
        }
    }
    
    return 0;
}
