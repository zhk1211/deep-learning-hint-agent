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
        
        int ans = n;
        int i = 0;
        while (i < n) {
            int j = i;
            while (j + 1 < n && a[j + 1] == a[j] + 1) {
                ++j;
            }
            int len = j - i + 1;
            ans -= (len - 1);
            i = j + 1;
        }
        cout << ans << '\n';
    }
    return 0;
}
