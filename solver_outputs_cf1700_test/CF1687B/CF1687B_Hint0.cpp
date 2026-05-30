// Hint0
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> u(m), v(m), w(m);
    // We don't know the edges yet, but we can query.

    // First, find the weight of each edge by querying with only that edge active.
    vector<long long> edge_weight(m);
    for (int i = 0; i < m; ++i) {
        string s(m, '0');
        s[i] = '1';
        cout << "? " << s << endl;
        long long resp;
        cin >> resp;
        edge_weight[i] = resp;
    }

    // Sort edges by weight.
    vector<int> order(m);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) {
        return edge_weight[i] < edge_weight[j];
    });

    // Kruskal-like: add edges one by one, keep track of current max capacity.
    long long current_max = 0;
    string active(m, '0');
    for (int idx : order) {
        // Try adding this edge.
        string test = active;
        test[idx] = '1';
        cout << "? " << test << endl;
        long long resp;
        cin >> resp;

        // If the max capacity increased by exactly edge_weight[idx], it's part of the minimum spanning forest.
        if (resp == current_max + edge_weight[idx]) {
            active[idx] = '1';
            current_max = resp;
        }
        // Otherwise, adding it would create a cycle where it's not the maximum edge, so skip.
    }

    cout << "! " << current_max << endl;
    return 0;
}
