// Hint1
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    vector<int> order(m);
    vector<int> len(m);
    
    // First, find the length of each edge using m queries
    for (int i = 0; i < m; i++) {
        string s(m, '0');
        s[i] = '1';
        cout << "? " << s << endl;
        cin >> len[i];
        order[i] = i;
    }
    
    // Sort edges by length (Kruskal's algorithm)
    sort(order.begin(), order.end(), [&](int a, int b) {
        return len[a] < len[b];
    });
    
    // Now build MST step by step
    string cur(m, '0');
    int prev_max = 0;
    
    for (int idx : order) {
        cur[idx] = '1';
        cout << "? " << cur << endl;
        int cur_max;
        cin >> cur_max;
        
        // If adding this edge increases max capacity by exactly its length,
        // it's part of the MST (or at least needed for connectivity)
        // Otherwise, it forms a cycle and we remove it
        if (cur_max - prev_max != len[idx]) {
            cur[idx] = '0';
        } else {
            prev_max = cur_max;
        }
    }
    
    cout << "! " << prev_max << endl;
    
    return 0;
}
