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
        for (int i = 0; i < n; ++i) {
            cin >> x[i];
        }
        
        int best_a = x[0], best_l = 1, best_r = 1;
        int max_diff = 0;
        
        int i = 0;
        while (i < n) {
            int j = i;
            while (j < n && x[j] == x[i]) {
                ++j;
            }
            int val = x[i];
            int cnt = j - i;
            
            int cur = 0, min_pref = 0, best_start = 0;
            int cur_l = i, best_local_l = i, best_local_r = i;
            for (int k = i; k < j; ++k) {
                cur += 1;
                if (cur - min_pref > max_diff) {
                    max_diff = cur - min_pref;
                    best_a = val;
                    best_l = cur_l + 1;
                    best_r = k + 1;
                }
                if (cur < min_pref) {
                    min_pref = cur;
                    cur_l = k + 1;
                }
            }
            
            int cur2 = 0, min_pref2 = 0, best_start2 = j;
            int cur_l2 = j;
            for (int k = j; k < n; ++k) {
                cur2 += (x[k] == val ? 1 : -1);
                if (cur2 - min_pref2 + cnt > max_diff) {
                    max_diff = cur2 - min_pref2 + cnt;
                    best_a = val;
                    best_l = i + 1;
                    best_r = k + 1;
                }
                if (cur2 < min_pref2) {
                    min_pref2 = cur2;
                    cur_l2 = k + 1;
                }
            }
            
            i = j;
        }
        
        cout << best_a << ' ' << best_l << ' ' << best_r << '\n';
    }
    
    return 0;
}
