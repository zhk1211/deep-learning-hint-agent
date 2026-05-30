// Hint2
#include <bits/stdc++.h>
using namespace std;

struct Node {
    int best_l, best_r;
    long long best_sum;
    long long pref_sum, pref_l;
    long long suff_sum, suff_r;
    long long total_sum;
};

Node combine(const Node& left, const Node& right) {
    Node res;
    res.total_sum = left.total_sum + right.total_sum;
    
    if (left.pref_sum >= left.total_sum + right.pref_sum) {
        res.pref_sum = left.pref_sum;
        res.pref_l = left.pref_l;
    } else {
        res.pref_sum = left.total_sum + right.pref_sum;
        res.pref_l = right.pref_l;
    }
    
    if (right.suff_sum >= right.total_sum + left.suff_sum) {
        res.suff_sum = right.suff_sum;
        res.suff_r = right.suff_r;
    } else {
        res.suff_sum = right.total_sum + left.suff_sum;
        res.suff_r = left.suff_r;
    }
    
    if (left.best_sum >= right.best_sum && left.best_sum >= left.suff_sum + right.pref_sum) {
        res.best_sum = left.best_sum;
        res.best_l = left.best_l;
        res.best_r = left.best_r;
    } else if (right.best_sum >= left.best_sum && right.best_sum >= left.suff_sum + right.pref_sum) {
        res.best_sum = right.best_sum;
        res.best_l = right.best_l;
        res.best_r = right.best_r;
    } else {
        res.best_sum = left.suff_sum + right.pref_sum;
        res.best_l = left.suff_r;
        res.best_r = right.pref_l;
    }
    return res;
}

Node make_node(int pos, long long val) {
    Node node;
    node.total_sum = val;
    if (val >= 0) {
        node.pref_sum = val;
        node.pref_l = pos;
        node.suff_sum = val;
        node.suff_r = pos;
        node.best_sum = val;
        node.best_l = pos;
        node.best_r = pos;
    } else {
        node.pref_sum = 0;
        node.pref_l = pos + 1;
        node.suff_sum = 0;
        node.suff_r = pos - 1;
        node.best_sum = 0;
        node.best_l = pos;
        node.best_r = pos - 1;
    }
    return node;
}

void solve() {
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> x(n);
        for (int i = 0; i < n; ++i) cin >> x[i];
        
        map<int, vector<int>> positions;
        for (int i = 0; i < n; ++i) {
            positions[x[i]].push_back(i);
        }
        
        long long global_best = -1;
        int best_a = -1, best_l = -1, best_r = -1;
        
        for (auto& [val, pos] : positions) {
            vector<int> arr;
            for (int p : pos) arr.push_back(p);
            int m = arr.size();
            vector<long long> b(m);
            for (int i = 0; i < m; ++i) {
                b[i] = 1;
            }
            vector<long long> gaps(m - 1);
            for (int i = 0; i < m - 1; ++i) {
                gaps[i] = -(arr[i + 1] - arr[i] - 1);
            }
            
            int size_seg = m + (m - 1);
            vector<Node> seg(size_seg);
            for (int i = 0; i < m; ++i) {
                seg[2 * i] = make_node(arr[i], b[i]);
            }
            for (int i = 0; i < m - 1; ++i) {
                seg[2 * i + 1] = make_node(-1, gaps[i]);
            }
            
            function<Node(int, int)> build = [&](int l, int r) -> Node {
                if (l == r) return seg[l];
                int mid = (l + r) / 2;
                return combine(build(l, mid), build(mid + 1, r));
            };
            
            if (size_seg == 0) continue;
            Node res = build(0, size_seg - 1);
            long long cand = res.best_sum;
            if (cand > global_best) {
                global_best = cand;
                best_a = val;
                best_l = res.best_l + 1;
                best_r = res.best_r + 1;
            }
        }
        
        if (global_best <= 0) {
            best_a = x[0];
            best_l = 1;
            best_r = 1;
        }
        
        cout << best_a << " " << best_l << " " << best_r << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
