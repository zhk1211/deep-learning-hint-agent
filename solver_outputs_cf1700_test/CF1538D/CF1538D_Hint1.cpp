// Hint1
#include <bits/stdc++.h>
using namespace std;

int prime_exponents_sum(int x) {
    int cnt = 0;
    for (int i = 2; i * i <= x; ++i) {
        while (x % i == 0) {
            x /= i;
            ++cnt;
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
        int g = gcd(a, b);
        int max_moves = prime_exponents_sum(a / g) + prime_exponents_sum(b / g);
        int min_moves;
        if (a == b) {
            min_moves = 0;
        } else if (a % b == 0 || b % a == 0) {
            min_moves = 1;
        } else {
            min_moves = 2;
        }
        if (k == 1) {
            if (min_moves == 1) cout << "YES\n";
            else cout << "NO\n";
        } else {
            if (k >= min_moves && k <= max_moves) cout << "YES\n";
            else cout << "NO\n";
        }
    }
    return 0;
}
