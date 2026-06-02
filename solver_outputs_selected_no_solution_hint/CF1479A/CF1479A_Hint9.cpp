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
    
    // Get values at boundaries (virtual +infinity)
    auto get = [&](int i) -> int {
        if (i == 0 || i == n + 1) return n + 1; // larger than any possible value
        return query(i);
    };
    
    int lo = 1, hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        int val_mid = get(mid);
        int val_mid1 = get(mid + 1);
        if (val_mid < val_mid1) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    
    cout << "! " << lo << endl;
    return 0;
}
