// Hint3
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, a, b;
    cin >> n >> a >> b;
    --a; --b;
    vector<vector<int>> adj(n);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Find the first vertex painted blue: the midpoint on the path from a to b
    // Actually, the first blue vertex is the one where P_A arrives before P_B.
    // The optimal strategy: P_A goes towards b, P_B goes towards a.
    // The first blue vertex is the one on the path a-b that is closest to a
    // such that distance from a to that vertex <= distance from b to that vertex.
    // More precisely, it's the vertex on the path at distance floor(d/2) from a,
    // where d = dist(a,b). But we need the exact vertex.
    
    vector<int> parent(n, -1);
    vector<int> dist_a(n, -1);
    queue<int> q;
    q.push(a);
    dist_a[a] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist_a[v] == -1) {
                dist_a[v] = dist_a[u] + 1;
                parent[v] = u;
                q.push(v);
            }
        }
    }

    // Reconstruct path from a to b
    vector<int> path;
    int cur = b;
    while (cur != -1) {
        path.push_back(cur);
        cur = parent[cur];
    }
    reverse(path.begin(), path.end());
    // path[0] = a, path.back() = b
    int d = (int)path.size() - 1; // distance a-b
    // The first blue vertex is the one at index ceil(d/2) from a?
    // Actually, P_A moves first. They move simultaneously.
    // After k steps, P_A is at distance k from a along the path, P_B at distance k from b.
    // They meet when k + k >= d? The first vertex painted blue is the one where P_A arrives
    // and then P_B arrives later. The first time P_B paints a vertex blue is when it steps
    // on a red vertex. That happens when P_B reaches a vertex that P_A has already visited.
    // The first such vertex is the one at distance ceil(d/2) from a (if a != b).
    // Let's compute: after t steps, P_A has visited vertices up to distance t from a,
    // P_B has visited vertices up to distance t from b. They overlap when t + t >= d.
    // The first overlap vertex is at distance ceil(d/2) from a.
    int mid_idx = (d + 1) / 2; // ceil(d/2)
    int r = path[mid_idx]; // first blue vertex

    // Now we need to compute the minimum steps after r is painted blue.
    // After r is blue, P_A can follow P_B's movements. The total steps = 
    // steps to make r blue + 2 * (n - 1) - max_depth_from(r) ?
    // Actually, from hints: after r is blue, P_A can restore P_B's movements.
    // The total steps = dist(a, r) + 2*(n-1) - max_depth(r)
    // Wait, let's derive carefully.
    // Let t0 = dist(a, r). At time t0, r becomes red (P_A arrives).
    // Then at some later time, P_B arrives at r and paints it blue.
    // Actually, P_B moves after P_A in the same step. So if P_A arrives at r at step t0,
    // then in that same step, P_B moves. If P_B also arrives at r in that step, then
    // r becomes blue at step t0. Otherwise, it becomes blue later.
    // The time when r becomes blue is max(dist(a,r), dist(b,r))? 
    // Since P_A and P_B move simultaneously, the time when both have visited r is 
    // max(dist(a,r), dist(b,r)). But P_A moves first, so if dist(a,r) == dist(b,r),
    // then in step dist(a,r), P_A arrives, then P_B arrives, so r becomes blue at step dist(a,r).
    // If dist(a,r) < dist(b,r), P_A arrives earlier, r is red, then later P_B arrives and paints it blue.
    // So the step when r becomes blue is max(dist(a,r), dist(b,r)).
    // Let's compute dist from a and b to r.
    int da = dist_a[r];
    // Compute dist from b to all vertices
    vector<int> dist_b(n, -1);
    q.push(b);
    dist_b[b] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist_b[v] == -1) {
                dist_b[v] = dist_b[u] + 1;
                q.push(v);
            }
        }
    }
    int db = dist_b[r];
    int t_blue = max(da, db);

    // After r is blue, we can ignore red painting. P_A can mimic P_B.
    // The remaining problem: we have a tree, all vertices white except r is blue.
    // P_B is at some vertex (after t_blue steps, P_B is at r? Actually at step t_blue,
    // P_B arrives at r and paints it blue. So at the end of step t_blue, P_B is at r.
    // P_A is somewhere? At step t_blue, P_A moved to some neighbor. But we can choose
    // the paths optimally. The optimal strategy: after r is blue, P_B will traverse the
    // tree to paint all vertices blue. P_A will follow P_B's path exactly, one step behind,
    // so that when P_B moves to a new vertex, it is already red (painted by P_A in the previous step).
    // This way, every move of P_B paints a new vertex blue.
    // The total number of steps after t_blue is the number of edges P_B needs to traverse
    // to visit all vertices, starting from r. That is 2*(n-1) - max_depth_from(r),
    // because the optimal traversal is a DFS that returns to r? Wait, P_B doesn't need to return.
    // Actually, P_B needs to visit all vertices. The minimum number of steps to visit all vertices
    // starting from r and ending anywhere is 2*(n-1) - max_depth(r), where max_depth(r) is the
    // maximum distance from r to any vertex. This is the length of the shortest walk that visits
    // all vertices. Because you can do a DFS, and the last vertex visited can be left without returning.
    // So steps after t_blue = 2*(n-1) - max_depth(r).
    // Total steps = t_blue + 2*(n-1) - max_depth(r).

    // Compute max depth from r
    vector<int> dist_r(n, -1);
    q.push(r);
    dist_r[r] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist_r[v] == -1) {
                dist_r[v] = dist_r[u] + 1;
                q.push(v);
            }
        }
    }
    int max_d = *max_element(dist_r.begin(), dist_r.end());

    long long ans = t_blue + 2LL * (n - 1) - max_d;
    cout << ans << '\n';
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
