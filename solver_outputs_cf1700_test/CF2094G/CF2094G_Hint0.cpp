// Hint0
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

        vector<ll> a;
        ll sum = 0, rizz = 0;
        bool reversed = false;
        int shift = 0; // shift for cyclic shift, always in normal orientation

        auto get_idx = [&](int pos) -> int {
            // pos: 0-indexed position in the logical array
            int n = (int)a.size();
            if (!reversed) {
                return (pos - shift + n) % n;
            } else {
                // logical reversed: index 0 corresponds to last element of stored array
                // stored array is a[0..n-1]
                // logical array: a_rev[0] = a[n-1], a_rev[1] = a[n-2], ...
                // after cyclic shift by 'shift' in logical reversed array:
                // logical[i] = a_rev[(i - shift) mod n]
                // a_rev[j] = a[n-1-j]
                // so logical[i] = a[n-1 - ((i - shift) mod n)]
                int idx = ((pos - shift) % n + n) % n;
                return n - 1 - idx;
            }
        };

        while (q--) {
            int s;
            cin >> s;
            if (s == 1) {
                // cyclic shift
                if (!reversed) {
                    shift = (shift + 1) % (int)a.size();
                } else {
                    shift = (shift - 1 + (int)a.size()) % (int)a.size();
                }
                // rizziness update
                // new rizz = old rizz + sum - n * a[last_element]
                int n = (int)a.size();
                if (n == 0) {
                    rizz = 0;
                } else {
                    int last_logical = (n - 1 + shift) % n;
                    int last_idx = get_idx(last_logical);
                    rizz = rizz + sum - (ll)n * a[last_idx];
                }
            } else if (s == 2) {
                // reverse
                reversed = !reversed;
                // rizziness update
                int n = (int)a.size();
                if (n > 0) {
                    rizz = (ll)(n + 1) * sum - rizz;
                }
                // shift adjustment: after reversal, shift direction flips
                // but we keep shift as is; the get_idx handles it
            } else if (s == 3) {
                int k;
                cin >> k;
                int n = (int)a.size();
                // append to the end of logical array
                // logical end is at position n (0-indexed)
                // we need to insert into stored array at appropriate place
                if (!reversed) {
                    // logical end corresponds to stored index (n - shift) % (n+1) ? Actually we just push_back and adjust shift?
                    // Better: maintain stored array such that logical[0] is at stored[shift] (if not reversed)
                    // When appending, logical new element goes to position n.
                    // In stored array, we want to keep the property that logical[0] is at stored[shift].
                    // Currently stored size = n, shift is shift.
                    // After append, size = n+1.
                    // We want new element at logical position n.
                    // In stored array, logical[n] should be at index (n - shift) mod (n+1) if not reversed.
                    // But we can just push_back and then adjust shift if needed? 
                    // Simpler: we can just push_back to the end of stored array, and then rotate? 
                    // Actually, we can maintain that stored array always has logical[0] at index 0, and shift=0, by doing operations differently.
                    // But shift complicates. Let's redesign: we don't store shift in stored array; we store the array in logical order (shift=0) and just track shift logically.
                    // However, appending requires inserting at logical end, which in stored array (if shift != 0) is not simply push_back.
                    // To keep O(1) amortized, we can use deque and push_back/push_front appropriately.
                    // Let's change representation: store elements in a deque, and maintain that the logical array is the deque in some order with shift and reverse.
                    // Actually, we can keep the array in a vector and just do O(1) updates by tracking start index and direction.
                    // For append: we need to insert at logical end. Logical end index = n.
                    // In stored array (size n), logical[i] = stored[(i - shift) mod n] (if not reversed).
                    // After append, size = n+1. We want new element at logical[n].
                    // We can insert it into stored at position (n - shift) mod (n+1). But shift may change? 
                    // We want to keep the same shift relative to the new array? Actually, after append, the logical array's first element (index 0) should still be the same element as before.
                    // So we want logical[0] to remain at stored[shift] (if shift is defined as the index of logical[0] in stored).
                    // Currently, stored size n, logical[0] at stored[shift].
                    // After inserting new element at some position, stored size n+1. We want logical[0] to still be at stored[shift] (if shift < insertion index) or shift+1 (if shift >= insertion index).
                    // To keep shift unchanged, we can insert at the end of stored and then adjust shift? 
                    // Let's do: we maintain stored array such that logical[0] is always at index 0 (shift=0). Then shift is always 0. 
                    // But then cyclic shift would require O(n) to rotate. Not good.
                    // Alternative: use a balanced BST or just recompute rizz from scratch? No, sum q up to 2e5, n can be up to 2e5, O(n) per operation is too slow.
                    // We need O(1) per operation.
                    // Let's use a different approach: don't maintain the array explicitly for rizz calculation. We can maintain sum, rizz, and the first and last elements, and update formulas.
                    // For cyclic shift: we need the element that moves from end to front. We can maintain the array in a deque and actually perform the shift in O(1) using push_front/pop_back or vice versa.
                    // For reverse: we can maintain a boolean reversed flag, and use deque operations accordingly.
                    // For append: we push_back or push_front depending on reversed flag and shift? 
                    // Let's maintain the array in a deque that represents the logical array exactly (after applying all operations). 
                    // But cyclic shift on deque is O(1): if not reversed, we can do push_front(back()); pop_back(); 
                    // If reversed, cyclic shift direction is opposite: we can do push_back(front()); pop_front();
                    // Reverse: just flip reversed flag, and swap front/back for future operations.
                    // Append: always append to the logical end. Logical end depends on reversed flag.
                    // If not reversed, logical end is back of deque; if reversed, logical end is front of deque.
                    // So we can maintain the actual logical array in a deque with O(1) operations.
                    // Then rizziness can be updated incrementally.
                    // Let's do that.

                    // We'll maintain deque<int> dq; // logical array
                    // sum = sum of elements
                    // rizz = current rizziness
                    // reversed flag: if true, the logical array is dq reversed? Actually we can just keep dq as the logical array at all times.
                    // Then cyclic shift: if not reversed, move back to front; if reversed, move front to back.
                    // Reverse: just flip reversed flag. But then the logical array order changes. To keep dq as logical array, we would need to reverse dq, which is O(n). Not good.
                    // So we cannot keep dq as logical array if we flip reversed flag without reversing.
                    // Instead, we keep dq as the stored array in some canonical order, and use shift and reversed to map.
                    // But we need O(1) append. Let's design a representation that supports all operations in O(1) with a deque.

                    // Idea: maintain the array in a deque, but allow it to be "rotated" such that the logical start is at some index. We can do cyclic shift by moving elements between ends.
                    // For reverse, we can maintain a boolean rev flag. When rev is true, the logical order is the reverse of the deque order. 
                    // Then cyclic shift: if not rev, move back to front; if rev, move front to back (because reversing changes direction).
                    // Append: if not rev, push_back; if rev, push_front.
                    // This works! Because we never need to actually reverse the deque. The logical array is either the deque as is (rev=false) or the deque reversed (rev=true).
                    // Let's verify:
                    // Initial: empty deque, rev=false.
                    // Append 1: rev=false -> push_back(1). dq=[1]. logical=[1].
                    // Append 2: push_back(2). dq=[1,2]. logical=[1,2].
                    // Append 3: push_back(3). dq=[1,2,3]. logical=[1,2,3].
                    // Cyclic shift (s=1): rev=false -> move back to front: pop_back() gives 3, push_front(3). dq=[3,1,2]. logical=[3,1,2]. Correct.
                    // Append 4: rev=false -> push_back(4). dq=[3,1,2,4]. logical=[3,1,2,4]. Correct.
                    // Reverse (s=2): rev=true. Now logical is reverse of dq: dq=[3,1,2,4] -> logical=[4,2,1,3]. Correct.
                    // Append 5: rev=true -> push_front(5). dq=[5,3,1,2,4]. logical = reverse of dq = [4,2,1,3,5]. Correct (appended to end of logical).
                    // Cyclic shift (s=1): rev=true -> move front to back: pop_front() gives 5, push_back(5). dq=[3,1,2,4,5]. logical = reverse = [5,4,2,1,3]. 
                    //   Let's check: before shift, logical was [4,2,1,3,5]. Cyclic shift moves last to first: [5,4,2,1,3]. Correct.
                    // This works perfectly!

                    // Now we need to update rizziness in O(1) for each operation.
                    // We maintain sum, rizz, and the deque.
                    // For append:
                    //   new element x. New size n+1.
                    //   If not rev: logical new element is at position n (0-indexed). So rizz += x * (n+1). (since positions are 1-indexed in formula, position = n+1)
                    //   If rev: logical new element is at position n, but it is added to front of dq. In logical (reversed dq), the front of dq becomes the last element of logical.
                    //   So rizz += x * (n+1). Same.
                    //   sum += x.
                    // For reverse:
                    //   rizz = (n+1)*sum - rizz. (as derived)
                    // For cyclic shift:
                    //   We need to know the element that moves. 
                    //   If not rev: we move back to front. The element moved is dq.back(). Its old position was n (1-indexed). Its new position is 1.
                    //   rizz_new = rizz_old - x*n + x*1 + (sum - x)*1? Wait, all other elements shift right by 1, so their position increases by 1.
                    //   Formula: rizz_new = rizz_old + sum - n * x. (where x is the moved element, n is size)
                    //   If rev: we move front to back. The element moved is dq.front(). In logical (reversed), dq.front() is the last element (position n). It moves to position 1.
                    //   So same formula applies: rizz_new = rizz_old + sum - n * x, where x is the moved element (which is dq.front()).
                    //   Then we perform the deque operation.
                    //   Note: size n remains same.
                    // So we can do all updates in O(1).

                    // Let's implement this.
                }

                // We'll use the deque approach described above.
                // But we need to initialize the deque and variables before processing operations.
                // Since we are inside the loop for each test case, we can just declare them here.
                // However, the code structure: we are inside s==3 case, but we need to handle all operations uniformly.
                // Let's restructure: read all operations, but we can process on the fly.
                // We'll put the deque and variables outside the operation loop.
            }
        }
    }
}
