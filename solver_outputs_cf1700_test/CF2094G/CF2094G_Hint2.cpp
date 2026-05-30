// Hint2
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int q;
        cin >> q;

        deque<ll> dq;
        ll sum = 0;          // sum of elements
        ll rizz = 0;         // current rizziness
        bool reversed = false;
        int start_offset = 0; // offset of the first element in the original order

        auto get_idx = [&](int pos) -> int {
            // pos is 0-indexed position in the current logical array
            if (!reversed) {
                return (start_offset + pos) % (int)dq.size();
            } else {
                int n = dq.size();
                int logical_last = (start_offset - 1 + n) % n;
                return (logical_last - pos + n) % n;
            }
        };

        auto calc_rizz = [&]() -> ll {
            int n = dq.size();
            if (n == 0) return 0;
            // We can maintain rizziness incrementally instead of recomputing.
            // But for simplicity, we recompute only when needed? Actually we maintain it.
            // We'll maintain rizziness incrementally.
            return rizz;
        };

        // We'll maintain rizziness incrementally.
        // Initially empty.

        while (q--) {
            int op;
            cin >> op;
            if (op == 1) {
                // cyclic shift: move last element to front
                if (!dq.empty()) {
                    int n = dq.size();
                    if (!reversed) {
                        // The last element is at physical index (start_offset - 1 + n) % n
                        int last_idx = (start_offset - 1 + n) % n;
                        ll last_val = dq[last_idx];
                        // New rizziness: each element's position increases by 1, except the last which becomes position 1.
                        // rizz_new = rizz + sum - last_val * n
                        rizz = rizz + sum - last_val * n;
                        // start_offset moves to the last element
                        start_offset = last_idx;
                    } else {
                        // In reversed view, cyclic shift moves the first element to the end.
                        // The first element in logical order is at physical index get_idx(0).
                        int first_idx = get_idx(0);
                        ll first_val = dq[first_idx];
                        // rizz_new = rizz - sum + first_val * n
                        rizz = rizz - sum + first_val * n;
                        // After shift, the logical order: old second becomes first, etc., old first becomes last.
                        // This corresponds to moving start_offset forward by 1 in the physical reversed mapping.
                        // Actually, if reversed, logical order is reversed physical order starting from start_offset-1.
                        // Let's derive: logical array L[0..n-1] = reversed physical from start_offset-1 downwards.
                        // Cyclic shift on L: L' = [L[n-1], L[0], L[1], ..., L[n-2]].
                        // L[n-1] is physical index get_idx(n-1) = (logical_last - (n-1) + n) % n = (logical_last - n + 1 + n) % n = (logical_last + 1) % n.
                        // But logical_last = (start_offset - 1 + n) % n.
                        // So L[n-1] physical = (start_offset) % n.
                        // After shift, new logical first is old last, so new logical first physical = start_offset.
                        // The new logical order is reversed physical starting from new_logical_last = start_offset - 1? Let's recompute mapping.
                        // Better: update start_offset such that the new logical first maps correctly.
                        // Old logical first physical = get_idx(0) = (logical_last - 0 + n)%n = logical_last = (start_offset - 1 + n)%n.
                        // New logical first physical = old logical last physical = get_idx(n-1) = (logical_last - (n-1) + n)%n = (logical_last + 1)%n = start_offset.
                        // So new logical first physical = start_offset.
                        // In reversed mode, logical first physical = (new_logical_last - 0 + n)%n = new_logical_last.
                        // So new_logical_last = start_offset.
                        // But logical_last = (start_offset - 1 + n)%n. So new start_offset = (new_logical_last + 1)%n = (start_offset + 1)%n.
                        start_offset = (start_offset + 1) % n;
                    }
                }
            } else if (op == 2) {
                // reverse
                if (!dq.empty()) {
                    reversed = !reversed;
                    // Rizziness changes: old rizz = sum_{i=1}^n a_i * i.
                    // new rizz = sum_{i=1}^n a_{n-i+1} * i = sum_{j=1}^n a_j * (n - j + 1) = (n+1)*sum - old_rizz.
                    int n = dq.size();
                    rizz = (ll)(n + 1) * sum - rizz;
                }
            } else if (op == 3) {
                ll k;
                cin >> k;
                // Append to the end of logical array.
                // Logical end is at position n (1-indexed) after append.
                int n = dq.size();
                // We need to insert k at the correct physical position.
                if (!reversed) {
                    // Logical end is physical index (start_offset + n) % (n+1) after insertion.
                    // Currently physical array has n elements, start_offset points to logical first.
                    // Logical last is at physical (start_offset + n - 1) % n.
                    // We want to insert after logical last, so at physical index (start_offset + n) % (n+1).
                    // But we can just push_back and adjust start_offset if needed.
                    // Actually, if we maintain dq such that logical order is a contiguous segment starting at start_offset,
                    // then appending to logical end means inserting after the last element.
                    // If not reversed, logical order is dq[start_offset], dq[(start_offset+1)%n], ..., dq[(start_offset+n-1)%n].
                    // The new element should be placed after the last, so at physical index (start_offset + n) % (n+1).
                    // We can achieve this by inserting at position (start_offset + n) % (n+1) in the deque.
                    // But it's easier: just push_back and then maybe rotate? Let's use deque insert.
                    int insert_pos = (start_offset + n) % (n + 1);
                    dq.insert(dq.begin() + insert_pos, k);
                    // If insert_pos <= start_offset, start_offset needs to be incremented because a new element was inserted before it.
                    if (insert_pos <= start_offset) {
                        start_offset = (start_offset + 1) % (n + 1);
                    }
                } else {
                    // Reversed: logical order is reversed physical order starting from logical_last = (start_offset - 1 + n)%n down to start_offset.
                    // Logical last is physical index start_offset (since logical first is logical_last, logical last is logical_first? Wait.)
                    // Let's derive: logical array L[0] = physical[logical_last], L[1] = physical[logical_last-1], ..., L[n-1] = physical[logical_last - (n-1)] = physical[start_offset].
                    // So logical last is physical start_offset.
                    // Appending to logical end means adding after L[n-1], i.e., after physical start_offset.
                    // So we insert at physical index (start_offset + 1) % (n+1)? Actually after start_offset, so at start_offset + 1.
                    // But if start_offset is the last element in the current physical deque, we can push_back.
                    // Let's insert at position (start_offset + 1) % (n+1).
                    int insert_pos = (start_offset + 1) % (n + 1);
                    dq.insert(dq.begin() + insert_pos, k);
                    // If insert_pos <= start_offset, start_offset increments.
                    if (insert_pos <= start_offset) {
                        start_offset = (start_offset + 1) % (n + 1);
                    }
                }
                // Update sum and rizziness
                sum += k;
                // New rizziness: old rizziness + k * (n+1)
                rizz += k * (n + 1);
            }
            cout << rizz << '\n';
        }
    }
    return 0;
}
