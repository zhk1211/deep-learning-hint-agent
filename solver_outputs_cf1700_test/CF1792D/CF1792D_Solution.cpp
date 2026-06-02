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
            }
        }

        // Compute inverses of all permutations
        vector<vector<int>> inv(n, vector<int>(m + 1));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                inv[i][a[i][j]] = j + 1;
            }
        }

        // Sort inverses lexicographically
        sort(inv.begin(), inv.end());

        // For each permutation, find the longest common prefix with any inverse
        for (int i = 0; i < n; ++i) {
            // Build a vector representing the permutation p (1-indexed positions)
            // We need to compare p with inverses
            // p is a[i], but we need to compare element by element
            // We'll binary search for the best match
            // We can use lower_bound on the sorted inverses
            // We'll create a dummy vector for p, but we need to compare prefixes
            // Actually we can just do lower_bound with a custom comparator
            // Or we can iterate over possible k from m down to 0
            // Since m <= 10, we can just check all k
            int best = 0;
            // We can find the inverse that has the longest common prefix with p
            // p is a[i], we want to compare p[j] with inv[x][j+1]? Wait.
            // The editorial says: first k elements of p equal first k elements of q^{-1}
            // p is a permutation, q^{-1} is inverse of some permutation
            // p[j] is the value at position j+1 in p
            // q^{-1}[j] is the position of value j+1 in q
            // We need p[0..k-1] == q^{-1}[0..k-1]
            // So we compare a[i][j] with inv[x][j+1]? Let's check:
            // inv[x][v] = position of v in permutation x
            // So inv[x][1] is the position of 1, inv[x][2] position of 2, etc.
            // p[0] = a[i][0] is the value at position 1
            // We need p[0] = 1? No, beauty condition: p_1=1, p_2=2,... 
            // Wait, the beauty is defined on the product a_i * a_j.
            // The product r = a_i * a_j means r_x = a_j[a_i[x]]
            // We want r_1=1, r_2=2, ..., r_k=k.
            // This means a_j[a_i[1]] = 1, a_j[a_i[2]] = 2, ..., a_j[a_i[k]] = k.
            // Let p = a_i, q = a_j.
            // Then q[p[1]] = 1 => p[1] = q^{-1}[1]
            // q[p[2]] = 2 => p[2] = q^{-1}[2]
            // ...
            // q[p[k]] = k => p[k] = q^{-1}[k]
            // So the first k elements of p (values p[1..k]) must equal the first k elements of q^{-1} (values q^{-1}[1..k]).
            // Here p is 1-indexed: p[1] = a_i[0], p[2] = a_i[1], ...
            // q^{-1}[v] is the position of v in q.
            // So we need a_i[0] == inv_j[1], a_i[1] == inv_j[2], ..., a_i[k-1] == inv_j[k].
            // So we compare a_i[j] with inv_j[j+1] for j=0..k-1.
            // Our inv array stores inv[x][v] = position of v in permutation x.
            // So inv[x][1] is position of 1, inv[x][2] position of 2, etc.
            // So we need to find an inverse inv_x such that for as many j as possible, a_i[j] == inv_x[j+1].
            // We can just check all inverses? n up to 5e4, m<=10, total n*m = 5e5, n^2 is too big.
            // But we sorted inverses. We can binary search for the best prefix match.
            // Since m is small, we can just do a linear scan? No, n=5e4, n^2=2.5e9 too big.
            // We can use a trie, but m<=10, we can just use a set of strings or vectors.
            // Actually we can just do: for each inverse, insert all its prefixes into a set.
            // Then for each p, check longest prefix that exists.
            // Since m<=10, number of prefixes per inverse is m, total prefixes n*m <= 5e5.
            // We can store prefixes as integers (e.g., using base m+1 or just vector<int> in a set).
            // But set of vectors might be slow. We can encode prefix as a single integer: 
            // Since m<=10, values are 1..m, we can encode as base (m+1) number.
            // Or we can just use a set of strings, or a set of vectors (since total elements small, it's fine).
            // Let's use a set of vectors.
            set<vector<int>> prefixes;
            for (int j = 0; j < n; ++j) {
                vector<int> pref;
                for (int k = 1; k <= m; ++k) {
                    pref.push_back(inv[j][k]);
                    prefixes.insert(pref);
                }
            }
            // Now for each a_i, find longest prefix that matches
            vector<int> p(m);
            for (int j = 0; j < m; ++j) {
                p[j] = a[i][j];
            }
            int k = 0;
            vector<int> cur;
            for (int j = 0; j < m; ++j) {
                cur.push_back(p[j]);
                if (prefixes.count(cur)) {
                    k = j + 1;
                } else {
                    break;
                }
            }
            cout << k << (i == n-1 ? '\n' : ' ');
        }
    }
    return 0;
}
