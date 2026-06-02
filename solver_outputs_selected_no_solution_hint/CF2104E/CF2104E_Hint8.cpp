#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    string s;
    cin >> s;

    // next_pos[i][c] = first position >= i where character c occurs
    vector<array<int, 26>> next_pos(n + 2);
    for (int c = 0; c < 26; ++c) {
        next_pos[n + 1][c] = n + 1;
    }
    for (int i = n; i >= 0; --i) {
        for (int c = 0; c < 26; ++c) {
            next_pos[i][c] = next_pos[i + 1][c];
        }
        if (i < n) {
            next_pos[i][s[i] - 'a'] = i + 1;
        }
    }

    // For each position i, we want to know the maximum length of a prefix of
    // any string that can be matched starting at i. Actually we need the
    // "answer" for a string: the minimum number of letters to append so that
    // it stops being a subsequence. This is equivalent to the minimum length
    // of a suffix we need to add so that the string cannot be embedded.
    // We can precompute for each position i the "jump" to the next position
    // after matching one character of each type? Not exactly.
    // Let's define dp[i] = the maximum length L such that every string of length L
    // consisting of allowed letters is a subsequence of s starting at i.
    // Then for a query string t, we can greedily match it in s. If we can match
    // the whole t, then it's pleasant. The answer is the minimum number of
    // characters to append so that it becomes unpleasant. This is equivalent to
    // the minimum length of a string that is NOT a subsequence of s starting
    // from the position after matching t.
    // So we need to know, for each position i, the length of the shortest string
    // that is NOT a subsequence of s[i..n-1]. Let's call this f[i].
    // Then for a query t, we match t greedily in s. If we can't match fully,
    // answer is 0. Otherwise, let pos be the position after the last matched
    // character. Then answer is f[pos].

    // How to compute f[i]?
    // f[i] = 1 + min_{c in allowed} f[next_pos[i][c]]
    // Because to make a string that is not a subsequence, we can pick a first
    // character c, and then we need a string that is not a subsequence from
    // the position after that character. The shortest such string overall
    // is 1 + min over c of f[next_pos[i][c]].
    // Base case: f[n] = 1 (empty string is a subsequence, but any single character
    // is not, so shortest non-subsequence length is 1).
    // Actually f[n] = 1 because from position n (end of string), no characters
    // are available, so any single character is not a subsequence.
    // f[n+1] is not needed.

    vector<int> f(n + 1);
    f[n] = 1;
    for (int i = n - 1; i >= 0; --i) {
        int mn = n + 1; // large
        for (int c = 0; c < k; ++c) {
            int nxt = next_pos[i][c];
            mn = min(mn, f[nxt]);
        }
        f[i] = 1 + mn;
    }

    int q;
    cin >> q;
    while (q--) {
        string t;
        cin >> t;
        int pos = 0;
        bool matched = true;
        for (char ch : t) {
            int c = ch - 'a';
            pos = next_pos[pos][c];
            if (pos == n + 1) {
                matched = false;
                break;
            }
        }
        if (!matched) {
            cout << 0 << '\n';
        } else {
            cout << f[pos] << '\n';
        }
    }

    return 0;
}
