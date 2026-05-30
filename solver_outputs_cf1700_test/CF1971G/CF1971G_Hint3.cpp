// Hint3
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        // Group numbers by their value shifted right by 2 (i.e., removing last 2 bits)
        // Key: a[i] >> 2, Value: list of original numbers
        unordered_map<int, vector<int>> groups;
        for (int x : a) {
            groups[x >> 2].push_back(x);
        }

        // Sort each group in descending order so we can pop from the back
        for (auto& [key, vec] : groups) {
            sort(vec.begin(), vec.end(), greater<int>());
        }

        // Reconstruct the array: for each position, take the smallest available from its group
        for (int i = 0; i < n; ++i) {
            int key = a[i] >> 2;
            auto& vec = groups[key];
            a[i] = vec.back();
            vec.pop_back();
        }

        for (int i = 0; i < n; ++i) {
            cout << a[i] << " \n"[i == n - 1];
        }
    }

    return 0;
}
