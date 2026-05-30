// Hint3
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    string s;
    cin >> s;

    // nxt[i][c] = next occurrence of character c at or after position i
    // We'll use 0-indexed positions, and n as sentinel (meaning not found)
    vector<vector<int>> nxt(n + 1, vector<int>(k, n));
    for (int i = n - 1; i >= 0; --i) {
        for (int c = 0; c < k; ++c) {
            nxt[i][c] = nxt[i + 1][c];
        }
        nxt[i][s[i] - 'a'] = i;
    }

    int q;
    cin >> q;
    while (q--) {
        string t;
        cin >> t;
        int m = (int)t.size();

        // dp[i] = minimum length of prefix of s needed to match prefix of t of length i
        // dp[0] = 0 (empty prefix matched at position 0)
        // We'll compute dp iteratively.
        vector<int> dp(m + 1, n + 1);
        dp[0] = 0;
        for (int i = 0; i < m; ++i) {
            if (dp[i] >= n) {
                dp[i + 1] = n + 1; // cannot match further
            } else {
                int pos = nxt[dp[i]][t[i] - 'a'];
                if (pos == n) {
                    dp[i + 1] = n + 1;
                } else {
                    dp[i + 1] = pos + 1;
                }
            }
        }

        // Now we need to find the minimum number of letters to append to t
        // so that it is no longer a subsequence of s.
        // We can think of building the longest prefix of t that is a subsequence.
        // Let L be the length of the longest prefix of t that is a subsequence.
        // If L < m, then t is already not a subsequence -> answer 0.
        // Otherwise, we need to append letters. The answer is the minimum number
        // of letters to append such that the new string is not a subsequence.
        // This is equivalent to: we have matched the whole t, ending at some position
        // in s (the earliest possible is dp[m]). To make it not a subsequence,
        // we need to append a character that cannot be matched after dp[m].
        // The number of letters needed is the minimum d such that there exists
        // a string of length d (over the first k letters) that cannot be matched
        // starting from dp[m] in s.
        // This is exactly the "shortest non-subsequence" problem from a given position.
        // We can precompute for each position the length of the shortest string
        // that is not a subsequence starting from that position.
        // Let f[pos] = length of shortest non-subsequence starting from pos.
        // f[n] = 1 (empty string? Actually from n, no characters can be matched,
        // so the empty string is a subsequence, but any non-empty string of length 1
        // is not. So f[n] = 1.)
        // For pos < n: f[pos] = 1 + min_{c} f[nxt[pos][c] + 1]? Wait.
        // Actually, we want the shortest string that is NOT a subsequence.
        // If we start at pos, we can choose the first character c.
        // If for some c, nxt[pos][c] == n, then the single character c is not a subsequence,
        // so f[pos] = 1.
        // Otherwise, for every c, nxt[pos][c] < n. Then after matching c, we move to
        // nxt[pos][c] + 1. The shortest non-subsequence from pos will be c + (shortest non-subsequence from nxt[pos][c] + 1).
        // To minimize the total length, we take min over c of (1 + f[nxt[pos][c] + 1]).
        // So f[pos] = 1 if exists c with nxt[pos][c] == n, else 1 + min_c f[nxt[pos][c] + 1].

        vector<int> f(n + 1);
        f[n] = 1;
        for (int pos = n - 1; pos >= 0; --pos) {
            bool all_exist = true;
            int best = INT_MAX;
            for (int c = 0; c < k; ++c) {
                int nxt_pos = nxt[pos][c];
                if (nxt_pos == n) {
                    all_exist = false;
                    break;
                }
                best = min(best, f[nxt_pos + 1]);
            }
            if (!all_exist) {
                f[pos] = 1;
            } else {
                f[pos] = 1 + best;
            }
        }

        // Now for query t:
        // Find the earliest position after matching whole t.
        int pos_after_match = dp[m];
        if (pos_after_match > n) {
            // t is not a subsequence at all
            cout << 0 << '\n';
        } else {
            // t is a subsequence, matched ending before pos_after_match.
            // The answer is f[pos_after_match].
            cout << f[pos_after_match] << '\n';
        }
    }

    return 0;
}
