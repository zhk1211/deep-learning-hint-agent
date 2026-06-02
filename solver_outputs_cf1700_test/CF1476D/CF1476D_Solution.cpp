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
        vector<int> left(n + 1), right(n + 1);
        left[0] = 0;
        for (int i = 1; i <= n; ++i) {
            if (s[i - 1] == 'L') {
                if (i >= 2 && s[i - 2] == 'R') {
                    left[i] = left[i - 2];
                } else {
                    left[i] = i - 1;
                }
            } else {
                left[i] = i;
            }
        }
        right[n] = n;
        for (int i = n - 1; i >= 0; --i) {
            if (s[i] == 'R') {
                if (i + 2 <= n && s[i + 1] == 'L') {
                    right[i] = right[i + 2];
                } else {
                    right[i] = i + 1;
                }
            } else {
                right[i] = i;
            }
        }
        for (int i = 0; i <= n; ++i) {
            cout << right[i] - left[i] + 1 << " \n"[i == n];
        }
    }
    return 0;
}
