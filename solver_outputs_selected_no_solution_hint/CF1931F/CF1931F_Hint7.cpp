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
    vector<int> pos_in_first(n + 1);
    for (int i = 0; i < n; ++i) {
        pos_in_first[screens[0][i]] = i;
    }
    vector<int> order;
    bool possible = true;
    for (int i = 0; i < k; ++i) {
        if (screens[i][0] == screens[0][0]) {
            for (int j = 1; j < n; ++j) {
                order.push_back(screens[i][j]);
            }
            break;
        }
    }
    if (order.empty()) {
        for (int i = 1; i < n; ++i) {
            order.push_back(screens[0][i]);
        }
    }
    int author0 = screens[0][0];
    int pos_author0_in_order = -1;
    for (int i = 0; i < n - 1; ++i) {
        if (order[i] == author0) {
            pos_author0_in_order = i;
            break;
        }
    }
    vector<int> order_with_author;
    if (pos_author0_in_order == -1) {
        order_with_author.push_back(author0);
        for (int x : order) order_with_author.push_back(x);
    } else {
        for (int i = 0; i < n - 1; ++i) {
            order_with_author.push_back(order[i]);
            if (i == pos_author0_in_order) {
                order_with_author.push_back(author0);
            }
        }
    }
    vector<int> pos_in_candidate(n + 1);
    for (int i = 0; i < n; ++i) {
        pos_in_candidate[order_with_author[i]] = i;
    }
    for (int i = 0; i < k; ++i) {
        int author = screens[i][0];
        if (screens[i][0] != order_with_author[0]) {
            possible = false;
            break;
        }
        for (int j = 1; j < n; ++j) {
            if (screens[i][j] == author) continue;
            if (pos_in_candidate[screens[i][j]] < pos_in_candidate[author]) {
                possible = false;
                break;
            }
        }
        if (!possible) break;
        int prev = -1;
        for (int j = 1; j < n; ++j) {
            if (screens[i][j] == author) continue;
            if (prev != -1) {
                if (pos_in_candidate[screens[i][j]] < pos_in_candidate[prev]) {
                    possible = false;
                    break;
                }
            }
            prev = screens[i][j];
        }
        if (!possible) break;
    }
    cout << (possible ? "YES" : "NO") << '\n';
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
