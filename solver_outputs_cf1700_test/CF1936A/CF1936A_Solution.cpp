#include <bits/stdc++.h>
using namespace std;

int ask(int a, int b, int c, int d) {
    cout << "? " << a << " " << b << " " << c << " " << d << endl;
    char res;
    cin >> res;
    if (res == '<') return -1;
    if (res == '=') return 0;
    return 1;
}

void solve() {
    int n;
    cin >> n;
    
    // Step 1: find index of maximum element (n-1)
    int max_idx = 0;
    for (int i = 1; i < n; ++i) {
        int cmp = ask(max_idx, max_idx, i, i);
        if (cmp < 0) {
            max_idx = i;
        }
    }
    
    // Step 2: find all indices k such that p_max | p_k is maximum
    vector<int> candidates;
    candidates.push_back(max_idx == 0 ? 1 : 0);
    for (int i = 0; i < n; ++i) {
        if (i == max_idx) continue;
        if (i == candidates[0]) continue;
        int cmp = ask(max_idx, candidates[0], max_idx, i);
        if (cmp < 0) {
            candidates.clear();
            candidates.push_back(i);
        } else if (cmp == 0) {
            candidates.push_back(i);
        }
    }
    
    // Step 3: among candidates, find the one with minimum value
    int min_idx = candidates[0];
    for (int i = 1; i < (int)candidates.size(); ++i) {
        int cmp = ask(min_idx, min_idx, candidates[i], candidates[i]);
        if (cmp > 0) {
            min_idx = candidates[i];
        }
    }
    
    cout << "! " << max_idx << " " << min_idx << endl;
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
