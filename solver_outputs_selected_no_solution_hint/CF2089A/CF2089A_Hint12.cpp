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
            cout << "2 1\n";
            continue;
        }
        if (n == 3) {
            cout << "2 1 3\n";
            continue;
        }
        // find largest prime <= n
        int prime = n;
        while (!is_prime(prime)) --prime;
        // we want c1 = prime
        // p1 = prime
        p[0] = prime;
        // we need sum after i=1 to be such that ceil(sum/i) is prime for many i
        // put n at position 2
        p[1] = n;
        // fill remaining with 1..n except prime and n
        vector<bool> used(n + 1, false);
        used[prime] = used[n] = true;
        int idx = 2;
        for (int x = 1; x <= n; ++x) {
            if (!used[x]) {
                p[idx++] = x;
            }
        }
        // output
        for (int i = 0; i < n; ++i) {
            cout << p[i] << " \n"[i == n - 1];
        }
    }
    return 0;
}
