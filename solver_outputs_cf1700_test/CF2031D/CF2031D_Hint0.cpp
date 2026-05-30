// Hint0
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 500005;
int a[MAXN], ans[MAXN];
int L[MAXN], R[MAXN], stk[MAXN];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        for (int i = 1; i <= n; i++) {
            cin >> a[i];
        }
        // nearest greater to left
        int top = 0;
        for (int i = 1; i <= n; i++) {
            while (top && a[stk[top]] <= a[i]) top--;
            L[i] = top ? stk[top] : 0;
            stk[++top] = i;
        }
        // nearest greater to right
        top = 0;
        for (int i = n; i >= 1; i--) {
            while (top && a[stk[top]] <= a[i]) top--;
            R[i] = top ? stk[top] : n + 1;
            stk[++top] = i;
        }
        // nearest smaller to left
        vector<int> nsl(n + 1, 0);
        top = 0;
        for (int i = 1; i <= n; i++) {
            while (top && a[stk[top]] >= a[i]) top--;
            nsl[i] = top ? stk[top] : 0;
            stk[++top] = i;
        }
        // nearest smaller to right
        vector<int> nsr(n + 1, n + 1);
        top = 0;
        for (int i = n; i >= 1; i--) {
            while (top && a[stk[top]] >= a[i]) top--;
            nsr[i] = top ? stk[top] : n + 1;
            stk[++top] = i;
        }
        // compute max reachable height
        vector<int> dp(n + 1, 0);
        for (int i = 1; i <= n; i++) dp[i] = a[i];
        // forward pass: from left to right
        vector<int> seg(4 * n + 5, 0);
        function<void(int, int, int, int, int)> update = [&](int idx, int l, int r, int pos, int val) {
            if (l == r) {
                seg[idx] = max(seg[idx], val);
                return;
            }
            int mid = (l + r) / 2;
            if (pos <= mid) update(idx * 2, l, mid, pos, val);
            else update(idx * 2 + 1, mid + 1, r, pos, val);
            seg[idx] = max(seg[idx * 2], seg[idx * 2 + 1]);
        };
        function<int(int, int, int, int, int)> query = [&](int idx, int l, int r, int ql, int qr) {
            if (ql > r || qr < l) return 0;
            if (ql <= l && r <= qr) return seg[idx];
            int mid = (l + r) / 2;
            return max(query(idx * 2, l, mid, ql, qr), query(idx * 2 + 1, mid + 1, r, ql, qr));
        };
        // forward
        for (int i = 1; i <= n; i++) {
            // from left smaller
            int l = nsl[i] + 1;
            if (l <= i - 1) {
                int mx = query(1, 1, n, l, i - 1);
                dp[i] = max(dp[i], mx);
            }
            update(1, 1, n, i, dp[i]);
        }
        // backward pass
        fill(seg.begin(), seg.end(), 0);
        for (int i = n; i >= 1; i--) {
            int r = nsr[i] - 1;
            if (i + 1 <= r) {
                int mx = query(1, 1, n, i + 1, r);
                dp[i] = max(dp[i], mx);
            }
            update(1, 1, n, i, dp[i]);
        }
        // final answer: max over reachable components
        // Actually dp[i] is max height reachable from i considering monotonic jumps.
        // But we need to consider that from i we can go to L[i] or R[i] (greater) and then continue.
        // The above dp already propagates through smaller neighbors.
        // However, we also need to consider jumps to greater elements.
        // Let's compute reachable maximum via greater elements.
        vector<int> dp2 = dp;
        // forward using greater
        fill(seg.begin(), seg.end(), 0);
        for (int i = 1; i <= n; i++) {
            if (L[i] != 0) {
                dp2[i] = max(dp2[i], query(1, 1, n, L[i], L[i]));
            }
            update(1, 1, n, i, dp2[i]);
        }
        // backward using greater
        fill(seg.begin(), seg.end(), 0);
        for (int i = n; i >= 1; i--) {
            if (R[i] != n + 1) {
                dp2[i] = max(dp2[i], query(1, 1, n, R[i], R[i]));
            }
            update(1, 1, n, i, dp2[i]);
        }
        // Now dp2[i] is max height reachable from i.
        // But we also need to consider that from a greater element we can go to smaller ones, which we already have in dp.
        // Actually the above dp already includes going to smaller and then to greater? Let's combine.
        // We can do a BFS-like propagation using segment tree with max queries over intervals.
        // Simpler: The reachable set from i is the connected component in a graph where edges are between i and j if (i < j and a_i > a_j) or (i > j and a_i < a_j).
        // This graph is the comparability graph of the permutation? Not exactly.
        // We can compute the maximum in each connected component using union-find or by processing in order of values.
        // Since a_i are up to n, we can process values from largest to smallest.
        vector<int> order(n);
        iota(order.begin(), order.end(), 1);
        sort(order.begin(), order.end(), [&](int x, int y) { return a[x] > a[y]; });
        vector<int> vis(n + 1, 0);
        vector<int> comp_max(n + 1, 0);
        vector<int> parent(n + 1);
        iota(parent.begin(), parent.end(), 0);
        function<int(int)> find = [&](int x) {
            return parent[x] == x ? x : parent[x] = find(parent[x]);
        };
        auto unite = [&](int x, int y) {
            x = find(x); y = find(y);
            if (x != y) {
                parent[y] = x;
                comp_max[x] = max(comp_max[x], comp_max[y]);
            }
        };
        for (int i = 1; i <= n; i++) comp_max[i] = a[i];
        set<int> active;
        for (int idx : order) {
            // connect to active neighbors
            auto it = active.lower_bound(idx);
            if (it != active.end()) {
                unite(idx, *it);
            }
            if (it != active.begin()) {
                unite(idx, *prev(it));
            }
            active.insert(idx);
        }
        for (int i = 1; i <= n; i++) {
            ans[i] = comp_max[find(i)];
        }
        for (int i = 1; i <= n; i++) {
            cout << ans[i] << " \n"[i == n];
        }
    }
    return 0;
}
