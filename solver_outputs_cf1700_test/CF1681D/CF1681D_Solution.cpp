#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long x;
    cin >> n >> x;

    if (to_string(x).size() == n) {
        cout << 0 << '\n';
        return 0;
    }

    queue<long long> q;
    unordered_map<long long, int> dist;
    q.push(x);
    dist[x] = 0;

    while (!q.empty()) {
        long long cur = q.front();
        q.pop();
        int d = dist[cur];
        string s = to_string(cur);
        if ((int)s.size() == n) {
            cout << d << '\n';
            return 0;
        }
        vector<bool> used(10, false);
        for (char c : s) {
            int digit = c - '0';
            if (digit > 1 && !used[digit]) {
                used[digit] = true;
                long long nxt = cur * digit;
                if (dist.find(nxt) == dist.end()) {
                    dist[nxt] = d + 1;
                    q.push(nxt);
                }
            }
        }
    }

    cout << -1 << '\n';
    return 0;
}
