// Hint4
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int q;
        cin >> q;
        deque<int> dq;
        long long sum = 0;       // sum of elements
        long long rizz = 0;      // current rizziness
        bool reversed = false;   // whether the array is logically reversed
        int n = 0;               // current size

        while (q--) {
            int s;
            cin >> s;
            if (s == 1) {
                // cyclic shift
                if (n <= 1) {
                    // nothing changes
                } else {
                    if (!reversed) {
                        // normal order: move last to front
                        int last = dq.back();
                        dq.pop_back();
                        dq.push_front(last);
                        // rizziness change: old sum of positions: sum_{i=1..n} a_i * i
                        // new: last moves from n to 1, others shift right by 1
                        // delta = last*1 + sum_{others} a_i*(i+1) - old
                        // simpler: rizz = rizz - last*n + sum (since all others increase index by 1)
                        // but sum includes last, so sum_others = sum - last
                        // delta = (sum - last) - last*(n-1) = sum - last*n
                        rizz += sum - 1LL * last * n;
                    } else {
                        // reversed order: move front to back
                        int first = dq.front();
                        dq.pop_front();
                        dq.push_back(first);
                        // In reversed view, front is last element, back is first element.
                        // Cyclic shift on reversed array: move last (which is front of dq) to front (back of dq).
                        // Equivalent to moving front to back in dq.
                        // Rizziness change: similar formula but with reversed indexing.
                        // Actually, we can compute delta using same logic: element at position n moves to 1.
                        // In reversed array, the element at logical position n is dq.front().
                        // So delta = sum - first * n.
                        rizz += sum - 1LL * first * n;
                    }
                }
            } else if (s == 2) {
                // reverse
                reversed = !reversed;
                // rizziness changes: new_rizz = (n+1)*sum - old_rizz
                rizz = 1LL * (n + 1) * sum - rizz;
            } else {
                // append k
                int k;
                cin >> k;
                ++n;
                sum += k;
                if (!reversed) {
                    dq.push_back(k);
                    // new element at position n
                    rizz += 1LL * k * n;
                } else {
                    dq.push_front(k);
                    // In reversed view, new element is at position 1 (front of logical array)
                    // But wait: if reversed, the logical array is dq reversed.
                    // Appending to the end of logical array means adding to the front of dq.
                    // The new element's logical position is n (since it's appended at the end).
                    // So rizz += k * n.
                    rizz += 1LL * k * n;
                }
            }
            cout << rizz << '\n';
        }
    }
    return 0;
}
