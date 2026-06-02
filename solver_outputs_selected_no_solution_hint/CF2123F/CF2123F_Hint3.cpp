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
        
        vector<int> p(n + 1);
        p[1] = 1;
        
        if (n % 2 == 0) {
            for (int i = 2; i <= n; i += 2) {
                p[i] = i + 1;
                p[i + 1] = i;
            }
        } else {
            for (int i = 2; i <= n - 3; i += 2) {
                p[i] = i + 1;
                p[i + 1] = i;
            }
            p[n - 2] = n;
            p[n - 1] = n - 1;
            p[n] = n - 2;
        }
        
        for (int i = 1; i <= n; ++i) {
            cout << p[i] << " \n"[i == n];
        }
    }
    return 0;
}
