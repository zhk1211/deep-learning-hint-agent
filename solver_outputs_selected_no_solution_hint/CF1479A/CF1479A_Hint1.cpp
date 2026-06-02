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
    
    // Handle n=1 separately
    if (n == 1) {
        cout << "! 1" << endl;
        return 0;
    }
    
    // Get values at boundaries
    int a0 = n + 1; // virtual +inf
    int an1 = n + 1; // virtual +inf
    
    // Query first and last elements
    int a1 = query(1);
    int an = query(n);
    
    // Check boundaries
    if (a1 < a0 && a1 < query(2)) {
        cout << "! 1" << endl;
        return 0;
    }
    if (an < an1 && an < query(n-1)) {
        cout << "! " << n << endl;
        return 0;
    }
    
    // Binary search on the rest
    int lo = 2, hi = n - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        int amid = query(mid);
        int aleft = query(mid - 1);
        int aright = query(mid + 1);
        
        if (amid < aleft && amid < aright) {
            cout << "! " << mid << endl;
            return 0;
        }
        
        if (amid > aleft) {
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    
    // Should never reach here
    return 0;
}
