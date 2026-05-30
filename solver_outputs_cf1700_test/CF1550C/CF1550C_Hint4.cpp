// Hint4
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
                bool good = true;
                for (int k1 = i; k1 <= j && good; ++k1) {
                    for (int k2 = k1 + 1; k2 <= j && good; ++k2) {
                        for (int k3 = k2 + 1; k3 <= j; ++k3) {
                            if ((a[k1] <= a[k2] && a[k2] <= a[k3]) ||
                                (a[k1] >= a[k2] && a[k2] >= a[k3])) {
                                good = false;
                                break;
                            }
                        }
                    }
                }
                if (!good) break;
                ++j;
            }
            ans += (j - i);
        }
        cout << ans << '\n';
    }
    return 0;
}
