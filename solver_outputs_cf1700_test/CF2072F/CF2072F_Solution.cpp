#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1000000;
int c[MAXN + 1];

void precompute() {
    c[0] = c[1] = 0;
    for (int i = 2; i <= MAXN; ++i) {
        int f = 0;
        int x = i;
        while ((x & 1) == 0) {
            f++;
            x >>= 1;
        }
        c[i] = c[i - 1] + f;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    precompute();
    int t;
    cin >> t;
    while (t--) {
        int n;
        unsigned int k;
        cin >> n >> k;
        vector<unsigned int> row(n);
        for (int r = 0; r < n; ++r) {
            unsigned int val = 0;
            for (int bit = 0; bit < 31; ++bit) {
                if ((k >> bit) & 1) {
                    int cn = c[n - 1];
                    int cr = c[r];
                    int cnr = c[n - 1 - r];
                    if (cn == cr + cnr) {
                        val |= (1u << bit);
                    }
                }
            }
            row[r] = val;
        }
        for (int i = 0; i < n; ++i) {
            if (i > 0) cout << ' ';
            cout << row[i];
        }
        cout << '\n';
    }
    return 0;
}
