#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n), b(n);
        vector<int> cntTopic(n + 1, 0), cntDiff(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            cin >> a[i] >> b[i];
            ++cntTopic[a[i]];
            ++cntDiff[b[i]];
        }
        ll total = (ll)n * (n - 1) * (n - 2) / 6;
        ll bad = 0;
        for (int i = 0; i < n; ++i) {
            bad += (ll)(cntTopic[a[i]] - 1) * (cntDiff[b[i]] - 1);
        }
        cout << total - bad << '\n';
    }
    return 0;
}
