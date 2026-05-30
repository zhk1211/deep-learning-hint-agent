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
        vector<int> a(n), b(n);
        vector<int> cntA(n + 1, 0), cntB(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            cin >> a[i] >> b[i];
            cntA[a[i]]++;
            cntB[b[i]]++;
        }

        long long total = (long long)n * (n - 1) * (n - 2) / 6;
        long long bad = 0;
        for (int i = 0; i < n; ++i) {
            bad += (long long)(cntA[a[i]] - 1) * (cntB[b[i]] - 1);
        }
        cout << total - bad << '\n';
    }
    return 0;
}
