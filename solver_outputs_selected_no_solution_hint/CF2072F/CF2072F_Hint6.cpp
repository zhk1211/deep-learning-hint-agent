#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;
        vector<int> row(n);
        for (int i = 0; i < n; ++i) {
            if ((i & (n - 1)) == i) {
                row[i] = k;
            } else {
                row[i] = 0;
            }
        }
        for (int i = 0; i < n; ++i) {
            cout << row[i] << " \n"[i == n - 1];
        }
    }
    return 0;
}
