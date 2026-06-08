#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        ll c;
        cin >> n >> c;
        vector<ll> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        // We need to check if we can connect all nodes.
        // Key idea: sort by index, but we can always connect to the component containing node 1
        // if we process nodes in increasing order of (i*c - a[i]).
        // Actually, we can start with component containing node 1.
        // We maintain sum of a in the component.
        // We can add an edge between component (containing 1) and node i if sum + a[i] >= 1 * i * c.
        // But we can also connect other nodes first? The optimal strategy is to always connect
        // to the component containing node 1, because it has the smallest index (1) and thus
        // the product i*j*c is minimized when one endpoint is 1.
        // However, we might need to connect some other nodes first to increase sum.
        // The condition to connect component S (containing 1) and node i is:
        // sum_S + a[i] >= i * c   (since j=1 for component containing 1)
        // So we can greedily connect nodes that satisfy this, and then update sum.
        // But what if no node satisfies it initially? We might need to connect two other nodes
        // to form a larger component, then connect to 1.
        // However, note that any component not containing 1 has some minimum index j.
        // The product i*j*c is at least i*c (since j>=1). So connecting to 1 is always easiest.
        // Thus we should always try to connect directly to 1's component.
        // If we cannot, we might need to merge some other nodes first.
        // Consider sorting other nodes by i*c - a[i] (the "deficit").
        // We can merge them if we have enough sum.
        // Actually, we can process nodes 2..n in increasing order of i*c - a[i].
        // Maintain current sum = a[1]. For each node, if sum + a[i] >= i*c, we add a[i] to sum.
        // Otherwise, we cannot connect it now, but maybe later after adding other nodes?
        // If we sort by i*c - a[i], we are effectively trying to connect the "easiest" nodes first.
        // This greedy works: if we can connect all nodes in this sorted order, then YES, else NO.
        // Proof: Suppose there is a valid sequence. Consider the first time we connect a node
        // that is not the one with smallest i*c - a[i] among remaining. We can swap the order
        // because connecting an easier node first only increases sum, making it easier to connect others.
        
        vector<pair<ll, int>> nodes;
        for (int i = 1; i < n; ++i) {
            nodes.push_back({1LL * (i + 1) * c - a[i], i});
        }
        sort(nodes.begin(), nodes.end());
        
        ll sum = a[0];
        bool ok = true;
        for (auto &p : nodes) {
            int idx = p.second;
            if (sum + a[idx] >= 1LL * (idx + 1) * c) {
                sum += a[idx];
            } else {
                ok = false;
                break;
            }
        }
        cout << (ok ? "YES" : "NO") << '\n';
    }
    return 0;
}
