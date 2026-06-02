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
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        
        int ans = n;
        for (int i = 1; i < n; i++) {
            if (a[i] <= a[i-1]) {
                ans = i;
                break;
            }
        }
        
        cout << ans << '\n';
    }
    
    return 0;
}
