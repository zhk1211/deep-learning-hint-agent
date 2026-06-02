#include <bits/stdc++.h>
using namespace std;

struct Node {
    long long sum, pref, suff, best;
    int pref_pos, suff_pos, best_l, best_r;
};

Node combine(const Node& left, const Node& right) {
    Node res;
    res.sum = left.sum + right.sum;
    
    if (left.pref >= left.sum + right.pref) {
        res.pref = left.pref;
        res.pref_pos = left.pref_pos;
    } else {
        res.pref = left.sum + right.pref;
        res.pref_pos = right.pref_pos;
    }
    
    if (right.suff >= right.sum + left.suff) {
        res.suff = right.suff;
        res.suff_pos = right.suff_pos;
    } else {
        res.suff = right.sum + left.suff;
        res.suff_pos = left.suff_pos;
    }
    
    if (left.best >= right.best) {
        res.best = left.best;
        res.best_l = left.best_l;
        res.best_r = left.best_r;
    } else {
        res.best = right.best;
        res.best_l = right.best_l;
        res.best_r = right.best_r;
    }
    if (left.suff + right.pref > res.best) {
        res.best = left.suff + right.pref;
        res.best_l = left.suff_pos;
        res.best_r = right.pref_pos;
    }
    return res;
}

Node make_leaf(int pos, int val) {
    Node leaf;
    leaf.sum = val;
    leaf.pref = leaf.suff = leaf.best = val;
    leaf.pref_pos = leaf.suff_pos = leaf.best_l = leaf.best_r = pos;
    return leaf;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
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
        
        long long best_global = -1e18;
        int best_a = 0, best_l = 0, best_r = 0;
        
        int size = 1;
        while (size < n) size <<= 1;
        vector<Node> tree(2 * size);
        for (int i = 0; i < n; ++i) {
            tree[size + i] = make_leaf(i, -1);
        }
        for (int i = n; i < size; ++i) {
            tree[size + i] = make_leaf(i, -1);
            tree[size + i].sum = 0;
            tree[size + i].pref = tree[size + i].suff = tree[size + i].best = -1e9;
        }
        for (int i = size - 1; i >= 1; --i) {
            tree[i] = combine(tree[2*i], tree[2*i+1]);
        }
        
        for (auto& [val, pos] : positions) {
            for (int p : pos) {
                int idx = size + p;
                tree[idx] = make_leaf(p, 1);
                idx >>= 1;
                while (idx) {
                    tree[idx] = combine(tree[2*idx], tree[2*idx+1]);
                    idx >>= 1;
                }
            }
            
            Node root = tree[1];
            if (root.best > best_global) {
                best_global = root.best;
                best_a = val;
                best_l = root.best_l + 1;
                best_r = root.best_r + 1;
            }
            
            for (int p : pos) {
                int idx = size + p;
                tree[idx] = make_leaf(p, -1);
                idx >>= 1;
                while (idx) {
                    tree[idx] = combine(tree[2*idx], tree[2*idx+1]);
                    idx >>= 1;
                }
            }
        }
        
        cout << best_a << ' ' << best_l << ' ' << best_r << '\n';
    }
    return 0;
}
