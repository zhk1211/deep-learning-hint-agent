// Hint1
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, d;
        cin >> n >> d;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        
        // Check if all zeros initially
        bool all_zero = true;
        for (int x : a) {
            if (x == 1) {
                all_zero = false;
                break;
            }
        }
        if (all_zero) {
            cout << 0 << '\n';
            continue;
        }
        
        // We will simulate the process by tracking distances to the nearest 0
        // in the "shifted" direction. The process is equivalent to:
        // Each 1 becomes 0 in the next step if the element d positions to the left is 0.
        // This is because a_i & a_{(i-d) mod n} = 1 only if both are 1.
        // So a 1 survives k steps if there is a contiguous block of 1s of length > k
        // along the "backward" cycle with step d.
        
        // We can decompose the indices into gcd(n, d) independent cycles.
        int g = gcd(n, d);
        int max_steps = 0;
        vector<bool> visited(n, false);
        
        for (int start = 0; start < g; ++start) {
            if (visited[start]) continue;
            // Traverse the cycle
            vector<int> cycle;
            int cur = start;
            while (!visited[cur]) {
                visited[cur] = true;
                cycle.push_back(a[cur]);
                cur = (cur + d) % n;
            }
            // In this cycle, we need to find the maximum number of consecutive 1s
            // when the cycle is considered circularly. But note: the process stops
            // when all become 0. If the cycle is all 1s, it never becomes 0 -> answer -1.
            bool all_ones = true;
            for (int x : cycle) {
                if (x == 0) {
                    all_ones = false;
                    break;
                }
            }
            if (all_ones) {
                max_steps = -1;
                break;
            }
            // Duplicate the cycle to handle circularity
            int sz = cycle.size();
            vector<int> doubled = cycle;
            doubled.insert(doubled.end(), cycle.begin(), cycle.end());
            // Find max consecutive 1s in doubled, but limited to sz (since a block cannot exceed sz)
            int cur_len = 0, max_len = 0;
            for (int i = 0; i < 2 * sz; ++i) {
                if (doubled[i] == 1) {
                    cur_len++;
                    if (cur_len > max_len) max_len = cur_len;
                } else {
                    cur_len = 0;
                }
            }
            // The number of steps needed for this cycle is max_len
            // (because a block of L consecutive 1s takes L steps to clear)
            max_steps = max(max_steps, max_len);
        }
        
        cout << max_steps << '\n';
    }
    
    return 0;
}
