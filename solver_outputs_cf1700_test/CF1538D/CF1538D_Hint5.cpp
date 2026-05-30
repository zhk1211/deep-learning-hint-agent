// Hint5
#include <bits/stdc++.h>
using namespace std;

int prime_exponents_sum(int x) {
    int sum = 0;
    for (int i = 2; i * i <= x; ++i) {
        while (x % i == 0) {
            sum++;
            x /= i;
        }
    }
    if (x > 1) sum++;
    return sum;
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
            if ((a % b == 0 || b % a == 0) && a != b) {
                cout << "YES\n";
            } else {
                cout << "NO\n";
            }
            continue;
        }
        
        int total_ops = prime_exponents_sum(a) + prime_exponents_sum(b);
        if (k <= total_ops) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }
    return 0;
}
