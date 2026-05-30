// Hint0
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
        vector<int> a(n), b(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int i = 0; i < n; ++i) cin >> b[i];

        vector<int> freq(n + 1, 0);
        for (int x : a) freq[x]++;
        for (int x : b) freq[x]--;
        bool ok = true;
        for (int i = 1; i <= n; ++i) {
            if (freq[i] != 0) {
                ok = false;
                break;
            }
        }
        if (!ok) {
            cout << "NO\n";
            continue;
        }

        int i = n - 1, j = n - 1;
        vector<int> extra(n + 1, 0);
        while (j >= 0) {
            if (i >= 0 && a[i] == b[j]) {
                --i;
                --j;
            } else if (j + 1 < n && b[j] == b[j + 1]) {
                extra[b[j]]++;
                --j;
            } else if (i >= 0 && extra[a[i]] > 0) {
                extra[a[i]]--;
                --i;
            } else {
                ok = false;
                break;
            }
        }
        cout << (ok ? "YES" : "NO") << '\n';
    }
    return 0;
}
