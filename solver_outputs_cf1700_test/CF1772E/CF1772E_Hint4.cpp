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
        vector<int> p(n);
        for (int i = 0; i < n; ++i) {
            cin >> p[i];
        }
        
        // Find longest prefix that matches 1,2,3...
        int a = 0;
        for (int i = 0; i < n; ++i) {
            if (p[i] == i + 1) {
                ++a;
            } else {
                break;
            }
        }
        
        // Find longest suffix that matches n, n-1, ...
        int b = 0;
        for (int i = n - 1; i >= 0; --i) {
            if (p[i] == n - (n - 1 - i)) {
                ++b;
            } else {
                break;
            }
        }
        
        // c is the remaining elements
        int c = n - a - b;
        
        // Conditions from hints
        if (a + c <= b) {
            cout << "First\n";
        } else if (b + c < a) {
            cout << "Second\n";
        } else {
            cout << "Tie\n";
        }
    }
    
    return 0;
}
