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
    
    // Check boundaries first
    int a0 = query(1);
    int a1 = query(2);
    if (a0 < a1) {
        cout << "! 1" << endl;
        return 0;
    }
    
    int an = query(n);
    int an_1 = query(n-1);
    if (an < an_1) {
        cout << "! " << n << endl;
        return 0;
    }
    
    // Ternary search on the array
    int lo = 1, hi = n;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        int left_val = query(mid - 1);
        int mid_val = query(mid);
        int right_val = query(mid + 1);
        
        if (mid_val < left_val && mid_val < right_val) {
            cout << "! " << mid << endl;
            return 0;
        }
        
        if (mid_val > left_val) {
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    
    // Should never reach here
    return 0;
}
