// Hint3
#include <bits/stdc++.h>
using namespace std;

int prime_factors(int x) {
    int cnt = 0;
    for (int i = 2; i * i <= x; ++i) {
        while (x % i == 0) {
            cnt++;
            x /= i;
        }
    }
    if (x > 1) cnt++;
    return cnt;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int a, b, k;
        cin >> a >> b >> k;
        
        if (k == 1) {
            if ((a % b == 0 || b % a == 0) && a != b)
                cout << "YES\n";
            else
                cout << "NO\n";
            continue;
        }
        
        int max_moves = prime_factors(a) + prime_factors(b);
        if (k <= max_moves)
            cout << "YES\n";
        else
            cout << "NO\n";
    }
    return 0;
}
