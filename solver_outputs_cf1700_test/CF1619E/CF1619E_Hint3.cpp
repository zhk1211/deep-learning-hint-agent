// Hint3
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
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        vector<int> freq(n + 1, 0);
        for (int x : a) {
            if (x <= n) freq[x]++;
        }

        vector<long long> ans(n + 1, -1);
        long long ops = 0;
        vector<int> extra; // stack of extra elements < i

        for (int i = 0; i <= n; ++i) {
            // To make MEX = i, we need all numbers 0..i-1 present
            // ops currently holds min operations to have all 0..i-1 present
            if (i > 0) {
                if (freq[i - 1] == 0) {
                    if (extra.empty()) {
                        // cannot make all previous numbers present
                        // all larger i will also fail
                        break;
                    }
                    int x = extra.back();
                    extra.pop_back();
                    ops += (i - 1 - x);
                } else {
                    // we have at least one i-1, extra copies can be used later
                    // extra stores indices of elements that can be increased
                    // we push i-1 for each extra copy beyond the first
                    for (int k = 1; k < freq[i - 1]; ++k) {
                        extra.push_back(i - 1);
                    }
                }
            }
            // Now we have all 0..i-1 present with ops operations
            // To achieve MEX = i, we must also ensure no i is present
            // We need to increment all occurrences of i
            long long cur = ops + freq[i];
            ans[i] = cur;
            // For next i, we need to handle freq[i] for presence of i
            // Already done in loop body for next iteration
            if (i < n && freq[i] > 0) {
                for (int k = 1; k < freq[i]; ++k) {
                    extra.push_back(i);
                }
            }
        }

        for (int i = 0; i <= n; ++i) {
            cout << ans[i] << " \n"[i == n];
        }
    }
    return 0;
}
