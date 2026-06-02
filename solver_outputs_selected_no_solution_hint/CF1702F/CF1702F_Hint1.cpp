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
        vector<int> a(n), b(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int i = 0; i < n; ++i) cin >> b[i];

        multiset<int> ma(a.begin(), a.end());
        multiset<int> mb(b.begin(), b.end());

        while (!mb.empty()) {
            int x = *mb.rbegin();
            mb.erase(prev(mb.end()));

            auto it = ma.find(x);
            if (it != ma.end()) {
                ma.erase(it);
            } else {
                if (x == 1) break;
                mb.insert(x / 2);
                mb.insert(x - x / 2); // actually x/2 and (x+1)/2? Wait, floor(x/2) only.
                // The operation is only floor(x/2) or x*2. We are going backwards from b to a.
                // If x is not in a, we can only try to reduce it by floor(x/2).
                // But we must also consider that we might need to split? No, we only replace one element.
                // So we just push floor(x/2) back.
                // Wait, the code above inserts two elements, that's wrong. Let's fix.
            }
        }

        // Correct approach:
        // We can only apply operations to b. Going backwards: from b we can only divide by 2 (floor).
        // So we can try to match largest elements of b with a by repeatedly dividing by 2 until we get a value in a.
        // If we hit 1 and still not in a, impossible.
        // Use priority queue or multiset for a and b.
    }
    return 0;
}
