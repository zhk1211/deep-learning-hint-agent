#include <bits/stdc++.h>
using namespace std;

int query(vector<int> &indices) {
    cout << "? " << indices.size();
    for (int idx : indices) cout << " " << idx;
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
    vector<pair<int,int>> pairs;
    
    for (int i = 1; i <= m; i++) {
        if (paired[i]) continue;
        // find j > i such that MAD({i, j}) > 0
        bool found = false;
        for (int j = i + 1; j <= m; j++) {
            if (paired[j]) continue;
            vector<int> q = {i, j};
            int res = query(q);
            if (res > 0) {
                // they are the same number
                a[i] = a[j] = res;
                paired[i] = paired[j] = true;
                pairs.push_back({i, j});
                found = true;
                break;
            }
        }
        if (!found) {
            // i is the second occurrence of some number whose first occurrence is before i
            // but we haven't found it yet. Actually, we will find it when we query with the first occurrence.
            // Since we iterate i from 1 to m, the first occurrence will be found when we reach it.
            // So this case shouldn't happen if we do it correctly.
        }
    }
    
    // Now we have all pairs, but we don't know which number corresponds to which pair.
    // We need to assign numbers 1..n to the pairs.
    // We can use queries of size n to determine the numbers.
    // Actually, we can use a clever method: query all indices except one from each pair.
    // But we need to assign numbers.
    
    // Let's collect one index from each pair.
    vector<int> first_of_pair;
    for (auto &p : pairs) {
        first_of_pair.push_back(p.first);
    }
    
    // We will determine the number for each pair by querying subsets.
    // We know that MAD of a set is the maximum number that appears at least twice.
    // If we take all first indices, we have one copy of each number, so MAD = 0.
    // If we add the second index of a pair, that number appears twice, so MAD becomes that number.
    // So we can find the number for each pair by querying: all first indices + the second index of that pair.
    
    vector<int> base = first_of_pair;
    for (auto &p : pairs) {
        int first_idx = p.first;
        int second_idx = p.second;
        vector<int> q = base;
        q.push_back(second_idx);
        int res = query(q);
        a[first_idx] = a[second_idx] = res;
    }
    
    // Output answer
    cout << "!";
    for (int i = 1; i <= m; i++) {
        cout << " " << a[i];
    }
    cout << endl;
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
