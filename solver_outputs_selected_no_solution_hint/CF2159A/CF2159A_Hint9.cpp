#include <bits/stdc++.h>
using namespace std;

int query(vector<int> &idx) {
    cout << "? " << idx.size();
    for (int x : idx) cout << " " << x;
    cout << endl;
    int res;
    cin >> res;
    if (res == -1) exit(0);
    return res;
}

void solve() {
    int n;
    cin >> n;
    int m = 2 * n;
    vector<int> a(m + 1, 0);
    
    // Find pairs using queries of size 2
    vector<bool> paired(m + 1, false);
    vector<int> unpaired;
    for (int i = 1; i <= m; i++) unpaired.push_back(i);
    
    while (unpaired.size() > 1) {
        int i = unpaired.back(); unpaired.pop_back();
        int lo = 0, hi = (int)unpaired.size() - 1;
        int found = -1;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            vector<int> q = {i};
            for (int j = lo; j <= mid; j++) q.push_back(unpaired[j]);
            int res = query(q);
            if (res > 0) {
                found = mid;
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        }
        if (found != -1) {
            int j = unpaired[found];
            // find the value
            vector<int> q2 = {i, j};
            int val = query(q2);
            a[i] = a[j] = val;
            paired[i] = paired[j] = true;
            unpaired.erase(unpaired.begin() + found);
        } else {
            // i is part of a pair with some already paired? Actually this shouldn't happen if we process all.
            // But we might have the case where the other copy is already paired? No, each number appears twice.
            // If no match in unpaired, then the other copy is already paired? That can't happen because we haven't paired i.
            // So it must be that the other copy is in unpaired but query returned 0? That means the other copy is not in the range.
            // So we just continue, i will be matched later.
            // Actually we need to put i back? No, we removed it. We'll handle remaining at the end.
        }
    }
    
    // Now we have some unpaired indices (should be 0 or 2? Actually if n is odd/even? Let's see.
    // After the while loop, unpaired might have 1 element left if total unpaired was odd.
    // But total indices is even, and we remove in pairs, so unpaired size should be even.
    // However, the binary search might fail to find a match if the other copy is not in the current unpaired list.
    // This happens if we removed the other copy earlier? No, we only remove when we find a pair.
    // So if we have an index i whose partner is not in unpaired, that means the partner was already paired? Impossible.
    // Actually, the binary search queries all unpaired indices. If no match, then the partner is not in unpaired, meaning it's already paired. But we only pair indices when we find both. So this can't happen.
    // So unpaired should be empty.
    
    // But wait, the above binary search might be O(n log n) queries, which is fine (n<=300, log n ~9, total ~ 2n * log n ~ 5400, but limit is 3n=900. Too many!
    // We need a better approach.
    
    // Let's rethink using hints.
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
