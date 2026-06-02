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
        for (int i = 0; i < n; i++) {
            cin >> p[i];
        }
        
        // Check if already sorted ascending or descending
        bool asc = true, desc = true;
        for (int i = 0; i < n; i++) {
            if (p[i] != i + 1) asc = false;
            if (p[i] != n - i) desc = false;
        }
        // Problem guarantees not initially sorted, but we keep logic
        
        // Count misplaced positions for ascending and descending
        int a = 0, b = 0, c = 0;
        for (int i = 0; i < n; i++) {
            bool in_asc = (p[i] == i + 1);
            bool in_desc = (p[i] == n - i);
            if (!in_asc && !in_desc) c++;
            else if (!in_asc) a++;
            else if (!in_desc) b++;
        }
        
        // Determine winner based on counts
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
