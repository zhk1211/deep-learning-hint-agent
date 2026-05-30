// Hint2
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
        
        // We need a prime in [floor(n/3), ceil(2n/3)]
        // We'll find a prime P in that range.
        int low = n / 3;
        int high = (2 * n + 2) / 3; // ceil(2n/3)
        int P = -1;
        for (int x = low; x <= high; ++x) {
            if (is_prime(x)) {
                P = x;
                break;
            }
        }
        // P is guaranteed to exist by problem statement / hint
        
        vector<int> perm(n);
        // We'll construct permutation such that prefix averages hit P many times.
        // Idea: place numbers so that sum after i elements is around i*P.
        // We'll use alternating small and large numbers.
        // Specifically, we want c_i = P for many i.
        // If we set p_i such that sum_i = i*P or i*P - something, ceil gives P.
        // We'll try to make sum_i = i*P for as many i as possible.
        // Since P is roughly n/2, we can pair numbers to sum to 2P.
        // We'll construct permutation: start with P, then for remaining numbers,
        // pair (P - k, P + k) for k=1..something.
        
        // Actually, simpler: we can output permutation as:
        // P, then alternating: P-1, P+1, P-2, P+2, ... until we run out.
        // But we must use numbers 1..n exactly once.
        // So we need to adjust.
        
        // Let's collect numbers 1..n.
        // We'll place P first.
        // Then we'll place pairs (P - d, P + d) for d=1,2,... as long as both are in [1,n].
        // Then remaining numbers (if any) at the end.
        
        vector<bool> used(n + 1, false);
        vector<int> ans;
        ans.push_back(P);
        used[P] = true;
        
        for (int d = 1; d <= n; ++d) {
            int a = P - d;
            int b = P + d;
            if (a >= 1 && !used[a]) {
                ans.push_back(a);
                used[a] = true;
            }
            if (b <= n && !used[b]) {
                ans.push_back(b);
                used[b] = true;
            }
        }
        // Any remaining numbers (shouldn't happen if n is covered, but just in case)
        for (int i = 1; i <= n; ++i) {
            if (!used[i]) ans.push_back(i);
        }
        
        for (int i = 0; i < n; ++i) {
            cout << ans[i] << (i + 1 == n ? '\n' : ' ');
        }
    }
    return 0;
}
