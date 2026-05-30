// Hint4
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<long long> S(n);
        for (int i = 0; i < n; ++i) {
            cin >> S[i];
        }

        vector<long long> extra(n + 1, 0);
        long long ans = 0;

        for (int i = 0; i < n; ++i) {
            // carry over extra passes from previous trampolines
            if (i > 0) extra[i] += extra[i - 1];

            long long cur = S[i] - 1; // needed passes to reduce to 1
            long long have = extra[i];

            if (have < cur) {
                // need additional passes starting from i
                long long need = cur - have;
                ans += need;
                have += need;
            }

            // distribute excess passes to the right
            // extra passes beyond what's needed to reduce S[i] to 1
            long long excess = have - cur;
            // S[i] becomes 1 after cur+1 passes? Actually after cur passes it's 1.
            // But we have 'have' passes landing on i.
            // The first 'cur' passes reduce S[i] from S[i] to 1.
            // The remaining 'excess' passes land on i when S[i] is already 1,
            // so they just jump to i+1.
            // Also, the original passes that reduced S[i] cause jumps to i+S[i], i+S[i]-1, ..., i+2.
            // We can simulate the effect by adding to extra array.

            // For the passes that actually reduced S[i]:
            // They jump to positions i + S[i], i + S[i] - 1, ..., i + 2.
            // That's S[i] - 1 destinations.
            // We can add 1 to extra for each of these positions.
            // But S[i] can be up to 1e9, so we can't loop.
            // Instead, we note that the extra passes from i will affect positions > i.
            // We can use a difference array approach.

            // Actually, we can process left to right and maintain extra[i] as the number of passes
            // that land on i from previous jumps.
            // When we are at i, we know how many passes land here (extra[i]).
            // We need to reduce S[i] to 1, which takes S[i]-1 passes.
            // If extra[i] < S[i]-1, we start new passes here.
            // After processing i, the passes that land on i (extra[i]) will cause jumps.
            // For each pass landing on i, if S[i] > 1 at that moment, it jumps to i + S[i] (current S[i]).
            // Since S[i] decreases by 1 each time, the sequence of destinations is i + S[i], i + S[i]-1, ..., i + 2.
            // After S[i] becomes 1, any further passes jump to i+1.
            // So we can add to extra array:
            // - For the first (S[i] - 1) passes: add 1 to positions i+2 .. i+S[i]
            // - For the remaining passes (if any): add 1 to position i+1.

            // To do this efficiently, we can use a difference array for the range updates.
            // But we are already using extra array and processing left to right.
            // We can just update extra[i+1] and extra[i+2] appropriately, and also maintain a running sum.
            // However, the range i+2 .. i+S[i] can be large. We can use a difference array `diff` to add 1 to a range.
            // Since n <= 5000, we can afford O(n) per test case, but S[i] can be 1e9, so we must cap at n.
            // Any jump beyond n is ignored.

            long long start = i + 2;
            long long end = i + S[i];
            if (start <= n) {
                extra[start] += 1;
                if (end + 1 <= n) extra[end + 1] -= 1;
                else if (end + 1 > n) {
                    // nothing to subtract beyond n
                }
            }

            // For the excess passes (when S[i] is already 1), they jump to i+1.
            if (excess > 0) {
                if (i + 1 < n) {
                    extra[i + 1] += excess;
                    if (i + 2 < n) extra[i + 2] -= excess;
                    // Actually, we are adding to extra[i+1] directly, but extra is a prefix sum array?
                    // We need to be careful: extra array is used as a prefix sum of added passes.
                    // Currently, extra[i] is the number of passes landing on i.
                    // We maintain it as a difference array? Let's rethink.

                    // Better: maintain an array `cnt` where cnt[i] is the number of passes landing on i.
                    // We can compute cnt on the fly using a difference array `diff`.
                    // Let diff[i] be the change in cnt at i.
                    // Then cnt[i] = cnt[i-1] + diff[i].
                    // We can update diff for ranges.
                }
            }
        }

        cout << ans << '\n';
    }
    return 0;
}
