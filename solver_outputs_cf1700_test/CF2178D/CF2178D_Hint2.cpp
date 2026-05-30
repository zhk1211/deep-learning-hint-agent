// Hint2
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    if (m == n) {
        cout << -1 << '\n';
        return;
    }

    vector<int> idx(n);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [&](int i, int j) {
        return a[i] < a[j];
    });

    vector<int> pos(n);
    for (int i = 0; i < n; ++i) {
        pos[idx[i]] = i;
    }

    int survivors = n - m;
    if (survivors == 0) {
        cout << 0 << '\n';
        return;
    }

    if (survivors == 1) {
        int smallest = idx[0];
        vector<pair<int,int>> attacks;
        vector<bool> attacked(n, false);
        for (int i = 1; i < n; ++i) {
            int cur = idx[i];
            if (!attacked[cur]) {
                attacks.emplace_back(cur, smallest);
                attacked[cur] = true;
            }
        }
        cout << attacks.size() << '\n';
        for (auto &p : attacks) {
            cout << p.first + 1 << ' ' << p.second + 1 << '\n';
        }
        return;
    }

    if (survivors == 2) {
        int smallest = idx[0];
        int second_smallest = idx[1];
        vector<pair<int,int>> attacks;
        vector<bool> attacked(n, false);
        for (int i = 2; i < n; ++i) {
            int cur = idx[i];
            if (!attacked[cur]) {
                attacks.emplace_back(cur, smallest);
                attacked[cur] = true;
            }
        }
        if (!attacked[second_smallest]) {
            attacks.emplace_back(second_smallest, smallest);
            attacked[second_smallest] = true;
        }
        cout << attacks.size() << '\n';
        for (auto &p : attacks) {
            cout << p.first + 1 << ' ' << p.second + 1 << '\n';
        }
        return;
    }

    if (survivors >= 3) {
        vector<pair<int,int>> attacks;
        vector<bool> attacked(n, false);
        int smallest = idx[0];
        for (int i = 1; i < n; ++i) {
            int cur = idx[i];
            if (i < survivors) {
                if (!attacked[cur]) {
                    attacks.emplace_back(cur, smallest);
                    attacked[cur] = true;
                }
            } else {
                if (!attacked[cur]) {
                    attacks.emplace_back(cur, idx[1]);
                    attacked[cur] = true;
                }
            }
        }
        cout << attacks.size() << '\n';
        for (auto &p : attacks) {
            cout << p.first + 1 << ' ' << p.second + 1 << '\n';
        }
        return;
    }
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
