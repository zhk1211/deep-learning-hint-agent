// Hint2
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
        vector<int> c(n);
        for (int i = 0; i < n; ++i) {
            cin >> c[i];
        }
        
        // Check condition 1: there must be exactly one position with c_i == 1
        int cnt1 = 0;
        for (int x : c) {
            if (x == 1) cnt1++;
        }
        if (cnt1 != 1) {
            cout << "NO\n";
            continue;
        }
        
        // Rotate c so that it starts with 1
        // Find the index of 1
        int pos = -1;
        for (int i = 0; i < n; ++i) {
            if (c[i] == 1) {
                pos = i;
                break;
            }
        }
        vector<int> a;
        for (int i = 0; i < n; ++i) {
            a.push_back(c[(pos + i) % n]);
        }
        
        // Now check that for all i from 0 to n-2, a[i+1] - a[i] <= 1
        bool ok = true;
        for (int i = 0; i < n - 1; ++i) {
            if (a[i+1] - a[i] > 1) {
                ok = false;
                break;
            }
        }
        // Also check that a[n-1] (which is c[pos-1 mod n]) is not > a[0] + 1? 
        // Actually the condition a[i+1] - a[i] <= 1 for all adjacent pairs in the rotated array is sufficient.
        // The last element a[n-1] is the one before 1 in the original array, and a[0]=1.
        // The condition a[n-1] - a[0] <= 1 is automatically satisfied because a[n-1] <= n and a[0]=1, but we need to check it.
        // Actually we already checked all adjacent pairs including the wrap-around? No, we didn't check the wrap-around.
        // But the condition for the wrap-around is between the element before 1 and 1 itself.
        // In the rotated array, that's between a[n-1] and a[0]. We need to check a[0] - a[n-1]? No, the condition is c_{i+1} - c_i <= 1 for all i.
        // For the original array, the pair (c[pos-1], c[pos]) is (a[n-1], a[0]=1). So we need a[0] - a[n-1] <= 1? Wait, c_{i+1} - c_i <= 1.
        // If we set i = pos-1 (mod n), then c_{pos} - c_{pos-1} <= 1 => 1 - a[n-1] <= 1 => -a[n-1] <= 0 => a[n-1] >= 0, always true.
        // The other direction: c_{pos} - c_{pos-1} could be negative, that's fine. The condition only restricts increases > 1.
        // So we don't need to check the wrap-around.
        
        cout << (ok ? "YES" : "NO") << '\n';
    }
    return 0;
}
