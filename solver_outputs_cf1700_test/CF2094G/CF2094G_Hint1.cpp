// Hint1
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int q;
        cin >> q;
        vector<long long> a;
        long long sum = 0, rizz = 0;
        bool rev = false;
        int shift = 0;
        int len = 0;

        auto get_idx = [&](int pos) -> int {
            if (!rev) {
                return (shift + pos) % len;
            } else {
                return (shift - pos + len) % len;
            }
        };

        while (q--) {
            int s;
            cin >> s;
            if (s == 1) {
                if (!rev) {
                    shift = (shift + 1) % len;
                } else {
                    shift = (shift - 1 + len) % len;
                }
                rizz = rizz + sum - (long long)len * a[get_idx(len - 1)];
            } else if (s == 2) {
                rev = !rev;
                rizz = (long long)(len + 1) * sum - rizz;
            } else {
                int k;
                cin >> k;
                a.push_back(k);
                sum += k;
                if (!rev) {
                    int pos = (shift + len) % len;
                    a.insert(a.begin() + pos, k);
                } else {
                    int pos = (shift - len + len) % len;
                    a.insert(a.begin() + pos + 1, k);
                }
                len++;
                rizz = 0;
                for (int i = 0; i < len; i++) {
                    rizz += (long long)a[i] * (i + 1);
                }
            }
            cout << rizz << '\n';
        }
    }
    return 0;
}
