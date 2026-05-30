// Solution
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> len(m);
    for (int i = 0; i < m; ++i) {
        string s(m, '0');
        s[i] = '1';
        cout << "? " << s << endl;
        cin >> len[i];
    }

    vector<int> idx(m);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [&](int i, int j) {
        return len[i] < len[j];
    });

    int prev = 0;
    string cur(m, '0');
    vector<bool> in_mst(m, false);

    for (int i = 0; i < m; ++i) {
        int e = idx[i];
        cur[e] = '1';
        cout << "? " << cur << endl;
        int res;
        cin >> res;
        if (res == prev + len[e]) {
            in_mst[e] = true;
            prev = res;
        } else {
            cur[e] = '0';
        }
    }

    int ans = 0;
    for (int i = 0; i < m; ++i) {
        if (in_mst[i]) ans += len[i];
    }
    cout << "! " << ans << endl;

    return 0;
}
