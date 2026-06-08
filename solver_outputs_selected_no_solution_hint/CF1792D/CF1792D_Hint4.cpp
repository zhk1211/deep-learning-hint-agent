#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<vector<int>> a(n, vector<int>(m));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                cin >> a[i][j];
                --a[i][j]; // 0-indexed
            }
        }

        // For each permutation, compute its inverse
        vector<vector<int>> inv(n, vector<int>(m));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                inv[i][a[i][j]] = j;
            }
        }

        // Precompute for each permutation the longest prefix that is a permutation of 1..k
        // i.e., the maximum k such that the first k elements are exactly {0,1,...,k-1}
        // Actually we need the sequence of positions of 0,1,2,... in the inverse.
        // For a fixed i, we want max beauty over j of a_i * a_j.
        // beauty = largest k such that (a_i * a_j)[x] = x for all x < k.
        // (a_i * a_j)[x] = a_j[a_i[x]].
        // So condition: a_j[a_i[x]] = x for x=0..k-1.
        // Let pos = a_i[x]. Then a_j[pos] = x.
        // So we need a_j to map the first k elements of a_i to 0..k-1 respectively.
        // Equivalently, for the inverse of a_j, we need inv_j[x] = a_i[x] for x=0..k-1.
        // So the sequence inv_j[0], inv_j[1], ..., inv_j[k-1] must equal a_i[0], a_i[1], ..., a_i[k-1].
        // Thus we can precompute for each permutation j a "prefix signature" of its inverse.
        // Since m <= 10, we can encode the sequence of first L elements of inv_j as an integer (base m, or just a vector).
        // Then for each i, we want the maximum L such that there exists j with matching prefix of length L.
        // We can store all possible prefixes of all inverses in a set or boolean array.
        // Since m <= 10, total possible prefixes sum_{L=0..m} m^L is small (m=10 -> ~1.1e6 max per test case, but sum n <= 5e4, so total prefixes over all j is n*m <= 5e5, which is fine).
        // We can just insert all prefixes of each inv_j into a hash set.

        unordered_set<int> prefixes;
        for (int j = 0; j < n; ++j) {
            int hash = 0;
            for (int L = 0; L <= m; ++L) {
                prefixes.insert(hash);
                if (L < m) {
                    hash = hash * (m + 1) + (inv[j][L] + 1); // +1 to avoid 0 ambiguity with empty prefix
                }
            }
        }

        vector<int> ans(n);
        for (int i = 0; i < n; ++i) {
            int hash = 0;
            int best = 0;
            for (int L = 0; L <= m; ++L) {
                if (prefixes.count(hash)) {
                    best = L;
                }
                if (L < m) {
                    hash = hash * (m + 1) + (a[i][L] + 1);
                }
            }
            ans[i] = best;
        }

        for (int i = 0; i < n; ++i) {
            cout << ans[i] << " \n"[i == n - 1];
        }
    }
    return 0;
}
