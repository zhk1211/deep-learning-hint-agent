#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    vector<int> n_arr(t), m_arr(t);
    vector<vector<int>> s_arr(t);
    int max_n = 0;
    for (int tc = 0; tc < t; ++tc) {
        cin >> n_arr[tc] >> m_arr[tc];
        max_n = max(max_n, n_arr[tc]);
        s_arr[tc].resize(m_arr[tc]);
        for (int i = 0; i < m_arr[tc]; ++i) {
            cin >> s_arr[tc][i];
        }
    }
    
    vector<int> spf(max_n + 1);
    for (int i = 0; i <= max_n; ++i) spf[i] = i;
    for (int i = 2; i * i <= max_n; ++i) {
        if (spf[i] == i) {
            for (int j = i * i; j <= max_n; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
    vector<int> p(max_n + 1, 0);
    for (int i = 2; i <= max_n; ++i) {
        p[i] = p[i / spf[i]] + 1;
    }
    
    for (int tc = 0; tc < t; ++tc) {
        int n = n_arr[tc];
        int m = m_arr[tc];
        vector<int>& s = s_arr[tc];
        
        int max_p = 0;
        for (int i = 1; i <= n; ++i) {
            if (p[i] > max_p) max_p = p[i];
        }
        if (max_p + 1 > m) {
            cout << "-1\n";
            continue;
        }
        for (int i = 1; i <= n; ++i) {
            cout << s[m - 1 - p[i]] << " \n"[i == n];
        }
    }
    
    return 0;
}
