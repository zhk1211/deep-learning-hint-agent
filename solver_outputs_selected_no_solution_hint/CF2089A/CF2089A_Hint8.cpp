#include <bits/stdc++.h>
using namespace std;

bool is_prime(int x) {
    if (x < 2) return false;
    for (int i = 2; i * i <= x; ++i) {
        if (x % i == 0) return false;
    }
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
        // We want c_1 to be a large prime, so put a large prime at position 1.
        // Let's find the largest prime <= n.
        int large_prime = 2;
        for (int i = n; i >= 2; --i) {
            if (is_prime(i)) {
                large_prime = i;
                break;
            }
        }
        
        // We'll put large_prime at position 1, then fill the rest.
        // The idea: make c_i prime for many initial i.
        // We can set p[0] = large_prime, then fill remaining with numbers 1..n except large_prime.
        // But we need to ensure many c_i are prime.
        // Actually, we can put n at position 2 to make c_2 = ceil((large_prime + n)/2) which is often prime.
        // Then fill the rest with 1,2,... except used numbers.
        
        vector<bool> used(n + 1, false);
        p[0] = large_prime;
        used[large_prime] = true;
        
        if (n >= 2) {
            p[1] = n;
            used[n] = true;
        }
        
        // Fill remaining positions with unused numbers in increasing order.
        int idx = 2;
        for (int i = 1; i <= n; ++i) {
            if (!used[i]) {
                p[idx++] = i;
            }
        }
        
        for (int i = 0; i < n; ++i) {
            cout << p[i] << " \n"[i == n - 1];
        }
    }
    return 0;
}
