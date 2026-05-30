// Solution
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i].first;
        a[i].second = i + 1;
    }
    sort(a.begin(), a.end());

    if (2 * m > n) {
        cout << "-1\n";
        return;
    }

    vector<pair<int, int>> attacks;

    if (m == 0) {
        long long sum_others = 0;
        for (int i = 0; i < n - 1; ++i) sum_others += a[i].first;
        if (sum_others < a[n - 1].first) {
            cout << "-1\n";
            return;
        }
        // kill largest with some smaller ones
        vector<int> killers;
        long long cur = 0;
        for (int i = n - 2; i >= 0; --i) {
            cur += a[i].first;
            killers.push_back(i);
            if (cur >= a[n - 1].first) break;
        }
        // remaining smaller elves attack the next larger one
        vector<bool> used(n, false);
        for (int idx : killers) used[idx] = true;
        int last = -1;
        for (int i = 0; i < n - 1; ++i) {
            if (!used[i]) {
                if (last != -1) {
                    attacks.emplace_back(a[last].second, a[i].second);
                }
                last = i;
            }
        }
        // killers attack the largest
        for (int idx : killers) {
            attacks.emplace_back(a[idx].second, a[n - 1].second);
        }
    } else {
        // surviving: largest m
        // targets: next m
        // leftovers: smallest n-2m
        int surv_start = n - m;
        int target_start = n - 2 * m;
        int leftover_cnt = n - 2 * m;

        // leftovers attack the next larger
        for (int i = 0; i < leftover_cnt - 1; ++i) {
            attacks.emplace_back(a[i].second, a[i + 1].second);
        }
        if (leftover_cnt > 0) {
            // largest leftover attacks a target
            attacks.emplace_back(a[leftover_cnt - 1].second, a[target_start].second);
        }
        // survivors attack targets
        for (int i = 0; i < m; ++i) {
            attacks.emplace_back(a[surv_start + i].second, a[target_start + i].second);
        }
    }

    cout << attacks.size() << "\n";
    for (auto &p : attacks) {
        cout << p.first << " " << p.second << "\n";
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
