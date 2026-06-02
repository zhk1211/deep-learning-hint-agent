// Solution
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
    
    if (n == 1) {
        cout << "! 1" << endl;
        return 0;
    }
    
    int l = 1, r = n;
    int a_l = query(l);
    int a_r = query(r);
    
    while (l < r) {
        int m = (l + r) / 2;
        int a_m = query(m);
        int a_m1 = query(m + 1);
        
        if (a_m < a_m1) {
            r = m;
        } else {
            l = m + 1;
        }
    }
    
    cout << "! " << l << endl;
    return 0;
}
