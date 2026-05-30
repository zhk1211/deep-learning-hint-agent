// Hint0
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
            int cnt = 0, neg = 0;
            for (int j = i; j < n; ++j) {
                if (s[j] == '+') {
                    cnt++;
                } else {
                    cnt--;
                    neg++;
                }
                if (cnt <= 0 && (-cnt) % 3 == 0 && (-cnt) / 3 <= neg) {
                    ans++;
                }
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
