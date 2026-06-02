#include <bits/stdc++.h>
using namespace std;

bool can(int k, const string& s) {
    int n = s.size();
    vector<int> add(n + 1, 0);
    int cur = 0;
    for (int i = 0; i < n; ++i) {
        cur += add[i];
        int val = s[i] - '0';
        if ((val + cur) % 2 == 0) {
            if (i + k > n) return false;
            cur++;
            add[i + k]--;
        }
    }
    return true;
}

void solve() {
    int n;
    string s;
    cin >> n >> s;
    for (int k = n; k >= 1; --k) {
        if (can(k, s)) {
            cout << k << '\n';
            return;
        }
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
