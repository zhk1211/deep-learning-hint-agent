// Hint1
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
        string s;
        cin >> s;
        
        int ans = 0;
        for (int i = 0; i < n; ++i) {
            int plus = 0, minus = 0;
            for (int j = i; j < n; ++j) {
                if (s[j] == '+') plus++;
                else minus++;
                
                if (minus >= plus && (minus - plus) % 3 == 0) {
                    ans++;
                }
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
