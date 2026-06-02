#include <bits/stdc++.h>
using namespace std;

const int MAXM = 200005;
const int LOG = 18;

int a[MAXM];
int st[MAXM][LOG];
int lg[MAXM];

void build(int m) {
    lg[1] = 0;
    for (int i = 2; i <= m; i++) {
        lg[i] = lg[i / 2] + 1;
    }
    for (int i = 1; i <= m; i++) {
        st[i][0] = a[i];
    }
    for (int j = 1; j < LOG; j++) {
        for (int i = 1; i + (1 << j) - 1 <= m; i++) {
            st[i][j] = max(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);
        }
    }
}

int query_max(int l, int r) {
    if (l > r) swap(l, r);
    int j = lg[r - l + 1];
    return max(st[l][j], st[r - (1 << j) + 1][j]);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= m; i++) {
        cin >> a[i];
    }
    build(m);

    int q;
    cin >> q;
    while (q--) {
        int xs, ys, xf, yf, k;
        cin >> xs >> ys >> xf >> yf >> k;

        if ((xs - xf) % k != 0 || (ys - yf) % k != 0) {
            cout << "NO\n";
            continue;
        }

        int max_blocked = query_max(ys, yf);
        int max_row = xs + (n - xs) / k * k;
        if (max_blocked >= max_row) {
            cout << "NO\n";
        } else {
            cout << "YES\n";
        }
    }

    return 0;
}
