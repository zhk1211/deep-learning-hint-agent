// Hint3
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
        
        int current_xor = 0;
        for (int i = 0; i < n; i++) {
            int y = i ^ current_xor;
            cout << y << endl;
            int r;
            cin >> r;
            if (r == 1) break;
            if (r == -1) return 0;
            current_xor ^= y;
        }
    }
    return 0;
}
