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
        vector<int> a(n), b(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int i = 0; i < n; ++i) cin >> b[i];
        
        // Reduce a to odd numbers
        unordered_map<int, int> freq;
        freq.reserve(n * 2);
        for (int x : a) {
            while (x % 2 == 0) x /= 2;
            ++freq[x];
        }
        
        // Process b in descending order
        sort(b.begin(), b.end(), greater<int>());
        bool ok = true;
        for (int y : b) {
            while (y > 0) {
                auto it = freq.find(y);
                if (it != freq.end() && it->second > 0) {
                    --it->second;
                    break;
                }
                y /= 2;
            }
            if (y == 0) {
                ok = false;
                break;
            }
        }
        cout << (ok ? "YES" : "NO") << '\n';
    }
    return 0;
}
