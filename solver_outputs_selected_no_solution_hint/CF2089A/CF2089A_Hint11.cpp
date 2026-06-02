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
            // Find largest prime <= n
            int prime = n;
            while (!is_prime(prime)) --prime;

            // Place the largest prime at position 1
            p[0] = prime;

            // Place n at position 2
            p[1] = n;

            // Fill the rest with numbers 1..n excluding prime and n
            vector<int> rest;
            for (int i = 1; i <= n; ++i) {
                if (i != prime && i != n) rest.push_back(i);
            }
            // To make c_i prime for many i, we can put 1 early
            // Actually we want c_1 = prime, c_2 = ceil((prime+n)/2)
            // For i>=3 we want c_i to be prime as much as possible.
            // The known construction: after prime and n, put 1, then the rest in any order.
            // Let's follow the pattern from hints: put 1 at position 3.
            if (!rest.empty()) {
                // Put 1 at position 3 if available
                auto it = find(rest.begin(), rest.end(), 1);
                if (it != rest.end()) {
                    p[2] = 1;
                    rest.erase(it);
                }
                // Fill remaining positions
                int idx = 3;
                for (int x : rest) p[idx++] = x;
            }
        }

        for (int i = 0; i < n; ++i) {
            cout << p[i] << " \n"[i == n - 1];
        }
    }
    return 0;
}
