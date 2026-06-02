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
        string s;
        cin >> s;
        vector<int> p(n);
        for (int i = 0; i < n; ++i) {
            cin >> p[i];
            --p[i];
        }

        vector<bool> visited(n, false);
        long long ans = 1;
        for (int i = 0; i < n; ++i) {
            if (!visited[i]) {
                vector<int> cycle;
                int cur = i;
                while (!visited[cur]) {
                    visited[cur] = true;
                    cycle.push_back(cur);
                    cur = p[cur];
                }

                string orig, curr;
                for (int idx : cycle) orig += s[idx];
                curr = orig;
                int steps = 0;
                do {
                    string nxt(curr.size(), ' ');
                    for (size_t j = 0; j < cycle.size(); ++j) {
                        nxt[j] = curr[(j + 1) % cycle.size()];
                    }
                    curr = nxt;
                    ++steps;
                } while (curr != orig);

                ans = lcm(ans, (long long)steps);
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
