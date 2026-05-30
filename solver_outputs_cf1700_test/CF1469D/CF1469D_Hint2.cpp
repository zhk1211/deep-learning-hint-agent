// Hint2
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
        vector<pair<int, int>> ops;
        // We'll use a value 'cur' that we repeatedly divide by 2 using ceil division
        // until it becomes 1. The element that stays as 2 will be at index 2.
        // Strategy: process numbers from n down to 3.
        // For each number i > 2, we will turn it into 1 by repeatedly applying
        // ceil(i / cur) where cur is the current "large" number we are reducing.
        // Initially cur = n. We'll keep a variable 'big' that holds the current
        // value we are reducing. We'll use index of 'big' as the denominator.
        // Actually we can just use index n as the initial big, then update.
        // But we need to output indices, not values.
        // We'll maintain the index of the element that is currently >2 and we are reducing.
        // Start with index n.
        int big = n;
        for (int i = n - 1; i >= 3; --i) {
            // turn a[i] into 1 by dividing by a[big]
            ops.emplace_back(i, big);
            // after operation, a[i] becomes ceil(i / a[big])
            // but we don't need to track exact values, just the process.
            // Actually we need to know when big becomes 2.
            // We'll simulate values to know when to update big.
            // Since we only care about the sequence, we can simulate with actual values.
        }
        // Now we have a[3..n] all 1? Not exactly, we need to handle big.
        // The known solution: repeatedly apply operation (big, 2) until big becomes 1.
        // But we need big to become 2 at the end, so we stop when big becomes 2.
        // Let's simulate properly.
        vector<int> a(n + 1);
        for (int i = 1; i <= n; ++i) a[i] = i;
        ops.clear();
        // We'll pick x = n as the one we reduce to 2.
        // For i from 3 to n-1, we do (i, n) to make them 1 immediately.
        for (int i = 3; i < n; ++i) {
            ops.emplace_back(i, n);
            a[i] = (a[i] + a[n] - 1) / a[n]; // ceil division
        }
        // Now reduce a[n] to 2 by repeatedly doing (n, 2)
        while (a[n] > 2) {
            ops.emplace_back(n, 2);
            a[n] = (a[n] + a[2] - 1) / a[2];
        }
        // If a[n] is 2, we are done. But if a[n] became 1? That can't happen because we stop when >2.
        // However, if n is small, we might need a different approach.
        // For n=3: a=[1,2,3]. We can do (3,2) twice.
        if (n == 3) {
            ops.clear();
            ops.emplace_back(3, 2);
            ops.emplace_back(3, 2);
        }
        cout << ops.size() << '\n';
        for (auto [x, y] : ops) cout << x << ' ' << y << '\n';
    }
    return 0;
}
