// Hint5
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<int> a(n + 2);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }
    a[0] = 0;
    a[n + 1] = 0;

    long long total_subsegments = 1LL * n * (n + 1) / 2;
    long long answer = total_subsegments;

    auto contribution = [&](int pos) -> long long {
        if (pos < 1 || pos > n) return 0;
        if (a[pos] != a[pos - 1]) {
            return 1LL * (pos - 1) * (n - pos + 1);
        }
        return 0;
    };

    for (int i = 1; i <= n; ++i) {
        answer += contribution(i);
    }

    while (m--) {
        int idx, val;
        cin >> idx >> val;
        if (a[idx] != val) {
            set<int> affected;
            affected.insert(idx);
            if (idx > 1) affected.insert(idx - 1);
            if (idx < n) affected.insert(idx + 1);
            for (int p : affected) {
                answer -= contribution(p);
            }
            a[idx] = val;
            for (int p : affected) {
                answer += contribution(p);
            }
        }
        cout << answer << '\n';
    }

    return 0;
}
