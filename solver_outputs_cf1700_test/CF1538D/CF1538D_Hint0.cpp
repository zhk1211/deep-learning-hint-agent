// Hint0
#include <bits/stdc++.h>
using namespace std;

int prime_exponents_sum(int x) {
    int cnt = 0;
    for (int i = 2; i * i <= x; ++i) {
        while (x % i == 0) {
            ++cnt;
            x /= i;
        }
    }
    if (x > 1) ++cnt;
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
        
        int sum_exp = prime_exponents_sum(a) + prime_exponents_sum(b);
        if (k <= sum_exp)
            cout << "YES\n";
        else
            cout << "NO\n";
    }
    return 0;
}
