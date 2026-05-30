// Hint3
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
        
        long long ans = 0;
        for (int i = 0; i < n; ++i) {
            int j = i;
            while (j < n) {
                bool bad = false;
                for (int k1 = i; k1 <= j && !bad; ++k1) {
                    for (int k2 = k1 + 1; k2 <= j && !bad; ++k2) {
                        for (int k3 = k2 + 1; k3 <= j && !bad; ++k3) {
                            if ((a[k1] <= a[k2] && a[k2] <= a[k3]) ||
                                (a[k1] >= a[k2] && a[k2] >= a[k3])) {
                                bad = true;
                            }
                        }
                    }
                }
                if (bad) break;
                ++j;
            }
            ans += (j - i);
        }
        cout << ans << '\n';
    }
    return 0;
}
