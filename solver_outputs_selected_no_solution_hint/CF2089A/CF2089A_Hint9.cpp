#include <bits/stdc++.h>
using namespace std;

bool is_prime(int x) {
    if (x < 2) return false;
    for (int i = 2; i * i <= x; ++i)
        if (x % i == 0) return false;
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;

        vector<int> p(n);
        if (n == 2) {
            p = {2, 1};
        } else if (n == 3) {
            p = {2, 1, 3};
        } else if (n == 4) {
            p = {2, 1, 3, 4};
        } else if (n == 5) {
            p = {2, 1, 3, 4, 5};
        } else {
            // For n >= 6
            // We want c_1 to be a large prime, ideally n if n is prime, else the largest prime <= n
            int prime = n;
            while (!is_prime(prime)) --prime;
            
            // We'll place 'prime' at position 1, and fill the rest with remaining numbers
            // To keep c_i prime for many i, we can put 1 at position 2, then the rest in increasing order
            vector<bool> used(n + 1, false);
            p[0] = prime;
            used[prime] = true;
            p[1] = 1;
            used[1] = true;
            
            int idx = 2;
            for (int i = 2; i <= n; ++i) {
                if (!used[i]) {
                    p[idx++] = i;
                }
            }
        }

        for (int i = 0; i < n; ++i) {
            cout << p[i] << " \n"[i == n - 1];
        }
    }
    return 0;
}
