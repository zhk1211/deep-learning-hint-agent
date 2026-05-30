// Hint1
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, q;
        cin >> n >> q;
        vector<int> p(n);
        for (int i = 0; i < n; ++i) {
            cin >> p[i];
            --p[i];
        }
        string s;
        cin >> s;
        
        vector<int> pos(n);
        for (int i = 0; i < n; ++i) {
            pos[p[i]] = i;
        }
        
        set<int> bad;
        for (int i = 0; i < n - 1; ++i) {
            if (s[i] == 'L' && s[i + 1] == 'R') {
                bad.insert(i);
            }
        }
        
        auto check = [&]() -> bool {
            if (bad.empty()) return true;
            int left = *bad.begin();
            int right = *bad.rbegin();
            for (int x = left; x <= right; ++x) {
                if (pos[x] > x) return false;
            }
            return true;
        };
        
        auto update = [&](int idx) {
            if (idx > 0 && s[idx - 1] == 'L' && s[idx] == 'R') bad.erase(idx - 1);
            if (idx < n - 1 && s[idx] == 'L' && s[idx + 1] == 'R') bad.erase(idx);
            s[idx] = (s[idx] == 'L' ? 'R' : 'L');
            if (idx > 0 && s[idx - 1] == 'L' && s[idx] == 'R') bad.insert(idx - 1);
            if (idx < n - 1 && s[idx] == 'L' && s[idx + 1] == 'R') bad.insert(idx);
        };
        
        for (int i = 0; i < q; ++i) {
            int idx;
            cin >> idx;
            --idx;
            update(idx);
            cout << (check() ? "YES" : "NO") << '\n';
        }
    }
    return 0;
}
