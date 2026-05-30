// Hint1
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    string s;
    cin >> s;

    // nxt[i][c] = next position of character c at or after index i
    // We'll build nxt for indices 0..n, where n is a sentinel (past the end)
    vector<array<int, 26>> nxt(n + 1);
    for (int c = 0; c < 26; ++c) {
        nxt[n][c] = n;
    }
    for (int i = n - 1; i >= 0; --i) {
        nxt[i] = nxt[i + 1];
        nxt[i][s[i] - 'a'] = i;
    }

    int q;
    cin >> q;
    while (q--) {
        string t;
        cin >> t;
        int m = (int)t.size();
        // We simulate the subsequence matching greedily.
        // pos = current index in s (0..n), where n means "not found"
        int pos = 0;
        for (char ch : t) {
            int c = ch - 'a';
            pos = nxt[pos][c];
            if (pos == n) {
                // The character cannot be matched, so t is not a subsequence of s.
                // We need 0 additional letters.
                break;
            }
            // Move past this character for the next match
            ++pos;
        }
        if (pos == n) {
            // We broke early because a character was not found
            cout << 0 << '\n';
        } else {
            // t is a subsequence of s. We need to append letters to make it not a subsequence.
            // The answer is (n - pos) + 1? Wait, let's think.
            // After matching the whole t, pos is the index in s right after the last matched character.
            // The remaining suffix of s has length (n - pos).
            // If we append any string that is NOT a subsequence of the remaining suffix,
            // then the whole t + appended will not be a subsequence.
            // The minimum number of letters to append is the length of the shortest string
            // that is NOT a subsequence of the suffix s[pos..n-1].
            // This is a classic problem: for a string over alphabet of size k,
            // the length of the shortest non-subsequence is the number of times we can
            // partition the suffix into blocks that contain all k letters, plus 1.
            // Actually, we can compute it greedily from pos.
            int ans = 0;
            int cur = pos;
            while (cur < n) {
                // We need to find the farthest we can go while covering all k letters.
                // We'll use the nxt table to jump.
                // We want the maximum next occurrence among all k letters from cur.
                int max_next = -1;
                for (int c = 0; c < k; ++c) {
                    max_next = max(max_next, nxt[cur][c]);
                }
                if (max_next == n) {
                    // Not all k letters appear in the remaining suffix.
                    // So the shortest non-subsequence is just one letter (the missing one).
                    // We need 1 more letter to append.
                    ans += 1;
                    break;
                } else {
                    // All k letters appear. We can form a block that contains all of them.
                    // The shortest non-subsequence length increases by 1.
                    ans += 1;
                    cur = max_next + 1;
                }
            }
            if (cur >= n) {
                // We exhausted the string while still having all letters in each block.
                // The suffix itself contains all possible strings of length ans as subsequences.
                // We need one more letter to break it.
                ans += 1;
            }
            cout << ans << '\n';
        }
    }

    return 0;
}
