// Hint2
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    vector<int> order(m);
    iota(order.begin(), order.end(), 0);
    
    // Find the weight of each edge using queries
    vector<int> weight(m);
    string base(m, '0');
    for (int i = 0; i < m; i++) {
        string q = base;
        q[i] = '1';
        cout << "? " << q << endl;
        cin >> weight[i];
    }
    
    // Sort edges by weight
    sort(order.begin(), order.end(), [&](int a, int b) {
        return weight[a] < weight[b];
    });
    
    // Kruskal-like process
    string cur(m, '0');
    int prev = 0;
    for (int i : order) {
        string nxt = cur;
        nxt[i] = '1';
        cout << "? " << nxt << endl;
        int resp;
        cin >> resp;
        if (resp == prev + weight[i]) {
            cur = nxt;
            prev = resp;
        }
    }
    
    cout << "! " << prev << endl;
    
    return 0;
}
