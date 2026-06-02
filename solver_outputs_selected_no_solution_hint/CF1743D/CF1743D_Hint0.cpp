#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string s;
    cin >> n >> s;

    int first_one = -1;
    for (int i = 0; i < n; ++i) {
        if (s[i] == '1') {
            first_one = i;
            break;
        }
    }

    if (first_one == -1) {
        cout << "0\n";
        return 0;
    }

    string t = s.substr(first_one);
    int m = t.size();

    string best = t;
    int second_one = -1;
    for (int i = 1; i < m; ++i) {
        if (t[i] == '1') {
            second_one = i;
            break;
        }
    }

    if (second_one != -1) {
        int len = m - second_one;
        string candidate = t;
        for (int start = 0; start <= second_one; ++start) {
            string tmp = t;
            for (int i = 0; i < len; ++i) {
                if (t[start + i] == '1') {
                    tmp[second_one + i] = '1';
                }
            }
            if (tmp > best) {
                best = tmp;
            }
        }
    }

    cout << best << "\n";
    return 0;
}
