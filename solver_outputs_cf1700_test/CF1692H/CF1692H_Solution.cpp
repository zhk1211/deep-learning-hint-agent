// Solution
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
        vector<int> x(n);
        unordered_map<int, vector<int>> pos;
        for (int i = 0; i < n; ++i) {
            cin >> x[i];
            pos[x[i]].push_back(i + 1); // 1-indexed positions
        }
        
        int best_a = 0, best_l = 0, best_r = 0, best_sum = INT_MIN;
        
        for (auto& p : pos) {
            int a = p.first;
            auto& v = p.second;
            int m = (int)v.size();
            int min_val = INT_MAX, min_idx = -1;
            
            for (int idx = 0; idx < m; ++idx) {
                int k = idx + 1;               // occurrence number (1-indexed)
                int val = 2 * k - v[idx];      // transformed value
                
                if (val < min_val) {
                    min_val = val;
                    min_idx = idx;
                }
                
                int cur_sum = val - min_val + 1;
                if (cur_sum > best_sum) {
                    best_sum = cur_sum;
                    best_a = a;
                    best_l = v[min_idx];
                    best_r = v[idx];
                }
            }
        }
        
        cout << best_a << " " << best_l << " " << best_r << "\n";
    }
    
    return 0;
}
