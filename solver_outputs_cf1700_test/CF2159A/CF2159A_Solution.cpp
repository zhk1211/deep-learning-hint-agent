// Solution
#include <bits/stdc++.h>
using namespace std;

int query(const vector<int>& idx) {
    cout << "? " << idx.size();
    for (int i : idx) cout << " " << i;
    cout << endl;
    int res;
    cin >> res;
    return res;
}

void solve() {
    int n;
    cin >> n;
    int m = 2 * n;
    vector<int> a(m + 1, 0);
    vector<int> S;
    vector<int> unknown;
    
    // First pass: find second occurrences
    for (int i = 1; i <= m; ++i) {
        vector<int> q = S;
        q.push_back(i);
        int x = query(q);
        if (x != 0) {
            a[i] = x;
        } else {
            S.push_back(i);
        }
    }
    
    // S now contains one copy of each number 1..n
    // The other indices are the ones we haven't found yet
    vector<bool> inS(m + 1, false);
    for (int idx : S) inS[idx] = true;
    for (int i = 1; i <= m; ++i) {
        if (a[i] == 0) unknown.push_back(i);
    }
    
    // Second pass: find first occurrences using S as the zero set
    // But we need a new zero set: the complement of S
    // Actually we can just query S (which has MAD=0) plus each unknown index
    for (int i : unknown) {
        vector<int> q = S;
        q.push_back(i);
        int x = query(q);
        a[i] = x;
    }
    
    cout << "!";
    for (int i = 1; i <= m; ++i) {
        cout << " " << a[i];
    }
    cout << endl;
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
