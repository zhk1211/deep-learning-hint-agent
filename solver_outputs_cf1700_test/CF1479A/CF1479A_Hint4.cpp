// Hint4
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
    
    // sentinels
    auto get = [&](int i) -> int {
        if (i == 0 || i == n + 1) return INT_MAX;
        return query(i);
    };
    
    int lo = 1, hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        int a_mid = get(mid);
        int a_next = get(mid + 1);
        if (a_mid < a_next) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    
    cout << "! " << lo << endl;
    return 0;
}
