// Hint1
#include <bits/stdc++.h>
using namespace std;

struct Friend {
    int h, w, id;
};

void solve() {
    int n;
    cin >> n;
    vector<Friend> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i].h >> a[i].w;
        a[i].id = i;
    }
    // We will create a list of all possible orientations
    // For each friend we have two states: (h, w) and (w, h)
    // We'll sort by height, then by width.
    vector<tuple<int, int, int, int>> states; // (h, w, id, orientation)
    for (int i = 0; i < n; ++i) {
        states.emplace_back(a[i].h, a[i].w, i, 0);
        states.emplace_back(a[i].w, a[i].h, i, 1);
    }
    sort(states.begin(), states.end(), [](const auto& x, const auto& y) {
        if (get<0>(x) != get<0>(y)) return get<0>(x) < get<0>(y);
        return get<1>(x) < get<1>(y);
    });

    vector<int> ans(n, -1);
    // We'll maintain the minimum width seen so far for each height group
    // Actually we need for each state the best (smallest width) among all states with strictly smaller height.
    // Since heights are sorted, we can keep prefix minimum of width and corresponding id.
    int min_w = INT_MAX;
    int min_id = -1;
    // We'll process in order of increasing height.
    // But we need to handle equal heights carefully: a friend cannot be placed in front of another with same height unless width is strictly smaller.
    // Since we sorted by height then width, we can group by height.
    int i = 0;
    while (i < (int)states.size()) {
        int cur_h = get<0>(states[i]);
        // Find all states with this height
        int j = i;
        while (j < (int)states.size() && get<0>(states[j]) == cur_h) {
            ++j;
        }
        // For all states in this height group, we can try to use the best from previous heights
        for (int k = i; k < j; ++k) {
            int h, w, id, orient;
            tie(h, w, id, orient) = states[k];
            if (min_w < w) {
                ans[id] = min_id + 1; // 1-indexed
            }
        }
        // Now update the prefix minimum with this height group
        for (int k = i; k < j; ++k) {
            int h, w, id, orient;
            tie(h, w, id, orient) = states[k];
            if (w < min_w) {
                min_w = w;
                min_id = id;
            }
        }
        i = j;
    }

    for (int i = 0; i < n; ++i) {
        cout << ans[i] << (i + 1 == n ? '\n' : ' ');
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
