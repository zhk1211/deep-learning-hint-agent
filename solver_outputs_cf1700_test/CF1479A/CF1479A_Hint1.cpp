// Hint1
#include <bits/stdc++.h>
using namespace std;

int query(int i) {
    cout << "? " << i << endl;
    int x;
    cin >> x;
    return x;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    // Handle n = 1 separately
    if (n == 1) {
        cout << "! 1" << endl;
        return 0;
    }
    
    // Check boundaries first
    int a1 = query(1);
    int a2 = query(2);
    if (a1 < a2) {
        cout << "! 1" << endl;
        return 0;
    }
    
    int an = query(n);
    int an_1 = query(n - 1);
    if (an < an_1) {
        cout << "! " << n << endl;
        return 0;
    }
    
    // Binary search in [2, n-1]
    int lo = 2, hi = n - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        int amid = query(mid);
        int amid1 = query(mid + 1);
        
        if (amid < amid1) {
            // Check left neighbor
            int amid_1 = query(mid - 1);
            if (amid < amid_1) {
                cout << "! " << mid << endl;
                return 0;
            } else {
                hi = mid - 1;
            }
        } else {
            lo = mid + 1;
        }
    }
    
    // Should never reach here
    return 0;
}
