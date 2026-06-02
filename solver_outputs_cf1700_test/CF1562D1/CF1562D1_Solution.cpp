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
        string s;
        cin >> s;
        
        vector<int> pref(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            int val = (s[i] == '+') ? 1 : -1;
            if (i % 2 == 0) {
                pref[i + 1] = pref[i] + val;
            } else {
                pref[i + 1] = pref[i] - val;
            }
        }
        
        while (q--) {
            int l, r;
            cin >> l >> r;
            int sum = pref[r] - pref[l - 1];
            if (sum == 0) {
                cout << "0\n";
            } else if ((r - l + 1) % 2 == 1) {
                cout << "1\n";
            } else {
                cout << "2\n";
            }
        }
    }
    return 0;
}
