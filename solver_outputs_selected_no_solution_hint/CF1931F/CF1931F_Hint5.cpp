#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<vector<int>> screens(k, vector<int>(n));
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> screens[i][j];
        }
    }
    if (k == 1) {
        cout << "YES\n";
        return;
    }
    vector<int> pos0(n + 1);
    for (int j = 0; j < n; ++j) {
        pos0[screens[0][j]] = j;
    }
    vector<int> order;
    for (int j = 1; j < n; ++j) {
        order.push_back(screens[0][j]);
    }
    int root = screens[0][0];
    vector<int> parent(n + 1, 0);
    for (int i = 0; i < n - 2; ++i) {
        parent[order[i + 1]] = order[i];
    }
    if (n >= 2) parent[order[0]] = root;
    for (int i = 1; i < k; ++i) {
        int author = screens[i][0];
        vector<int> seq;
        for (int j = 1; j < n; ++j) {
            seq.push_back(screens[i][j]);
        }
        if (seq.empty()) continue;
        if (author != root) {
            int p = parent[author];
            auto it = find(seq.begin(), seq.end(), p);
            if (it == seq.end()) {
                cout << "NO\n";
                return;
            }
            seq.erase(it);
        }
        if (seq != order) {
            cout << "NO\n";
            return;
        }
    }
    cout << "YES\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
