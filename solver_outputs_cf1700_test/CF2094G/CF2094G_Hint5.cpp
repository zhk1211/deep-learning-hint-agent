// Hint5
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
        vector<long long> a;
        long long sum = 0;          // sum of elements
        long long rizz = 0;         // current rizziness
        long long rizz_rev = 0;     // rizziness if reversed
        bool reversed = false;      // whether array is logically reversed
        int shift = 0;              // number of cyclic shifts modulo size

        auto get_index = [&](int pos) -> int {
            // pos is 0-based logical index in current view
            int n = (int)a.size();
            if (n == 0) return -1;
            if (!reversed) {
                return (pos - shift + n) % n;
            } else {
                // reversed: logical index 0 corresponds to last element of original
                // original index = (n - 1 - pos - shift) mod n
                return ((n - 1 - pos) - shift + n) % n;
            }
        };

        while (q--) {
            int op;
            cin >> op;
            if (op == 1) {
                // cyclic shift
                if (!a.empty()) {
                    int n = (int)a.size();
                    if (!reversed) {
                        // shift right by 1: last element moves to front
                        // rizz changes: each element's multiplier increases by 1, except last which becomes 1
                        // new_rizz = rizz + sum - n * last_element
                        int last_idx = get_index(n - 1);
                        long long last_val = a[last_idx];
                        rizz = rizz + sum - (long long)n * last_val;
                        // rizz_rev: reverse of shifted array = reverse then shift left? 
                        // Actually, we maintain rizz_rev as rizziness of reversed array.
                        // When we cyclic shift the array, the reversed array also changes.
                        // Reversed array after shift: original reversed array shifted left by 1.
                        // Shift left: first element moves to end.
                        // new_rizz_rev = rizz_rev - sum + n * first_element_of_rev
                        // first element of reversed array = last element of current array
                        int first_rev_idx = get_index(0); // logical first of reversed = logical last of current
                        long long first_rev_val = a[first_rev_idx];
                        rizz_rev = rizz_rev - sum + (long long)n * first_rev_val;
                        shift = (shift + 1) % n;
                    } else {
                        // reversed array: cyclic shift means shift left in original? 
                        // Let's think: array is reversed. Cyclic shift operation is applied to the current array.
                        // Current array = reversed original. Cyclic shift moves last to front.
                        // So reversed array becomes: last element of reversed array moves to front.
                        // That corresponds to original: first element moves to end? 
                        // We can just update rizz and rizz_rev similarly but with reversed logic.
                        // Actually, we can treat reversed flag as just a view. 
                        // We'll compute new rizz and rizz_rev directly using formulas.
                        // Current rizz is for current array. After shift right:
                        // new_rizz = rizz + sum - n * last_val
                        int last_idx = get_index(n - 1);
                        long long last_val = a[last_idx];
                        rizz = rizz + sum - (long long)n * last_val;
                        // rizz_rev: reverse of new array = shift left of old reversed array?
                        // Old reversed array is the array before shift but reversed.
                        // After shift right on current array, its reverse is shift left on old reversed array.
                        // Shift left: new_rizz_rev = rizz_rev - sum + n * first_val_of_old_rev
                        // first_val_of_old_rev = last_val_of_current = last_val
                        rizz_rev = rizz_rev - sum + (long long)n * last_val;
                        // update shift: we are in reversed mode, shift means something else.
                        // Instead of tracking shift and reversed separately, we can just update shift accordingly.
                        // When reversed, a cyclic shift right on reversed array corresponds to shift left on original.
                        shift = (shift - 1 + n) % n;
                    }
                }
            } else if (op == 2) {
                // reverse
                if (!a.empty()) {
                    reversed = !reversed;
                    swap(rizz, rizz_rev);
                }
            } else if (op == 3) {
                long long k;
                cin >> k;
                int n = (int)a.size();
                // Append to the end of current array.
                // Current array end: logical index n (0-based) before append.
                // We need to insert into original array at correct position.
                if (!reversed) {
                    // end of current array = after shift, index (n - shift) mod n? Actually end is after last element.
                    // Original array: elements in order of current view shifted by 'shift'.
                    // The new element should be placed at the end of current view.
                    // In original array, the end of current view corresponds to position (n - shift) % n? 
                    // Let's find where to insert in original vector so that logical order is maintained.
                    // We can just insert at position that represents the end of current array.
                    // Current array elements: original indices: (0 - shift) mod n, (1 - shift) mod n, ..., (n-1 - shift) mod n.
                    // The end is after the last element. The new element will have logical index n.
                    // Its original index should be such that when we apply shift, it becomes the last.
                    // Original index = (n - shift) mod n? Actually, if we insert at position (n - shift) % n in original array,
                    // then after shift it will be at the end? Let's test: n=3, shift=1, original [a,b,c], current [c,a,b].
                    // End of current is after b. Insert k: current becomes [c,a,b,k]. Original should become [c,a,b,k] with shift=1?
                    // Original: [c,a,b,k] with shift=1 gives [k,c,a,b] - wrong.
                    // Better: maintain original array as the sequence of elements in the order they were appended,
                    // but we track logical order via shift and reversed.
                    // When appending, we just push_back to original array, but we need to adjust rizz and rizz_rev.
                    // The new element is appended to the end of the current logical array.
                    // Its logical index = n (0-based). Its contribution to rizz: k * (n+1) because 1-based index = n+1.
                    // But we also need to update sum.
                    // However, we must place it in the original array such that the logical mapping works.
                    // If we always push_back to original vector, then the original order is the order of appends.
                    // But with shifts and reverses, the logical order changes.
                    // We can just store elements in a deque or list? But we need O(1) append and shift.
                    // Actually, we can maintain the array in a vector and never physically shift or reverse.
                    // We just keep track of shift and reversed, and compute rizz and rizz_rev mathematically.
                    // When appending, we need to know the value of the new element and its position in the logical array.
                    // Its logical index is n (0-based) before append, so after append it's at the end.
                    // We can just add its contribution to rizz and rizz_rev.
                    // But we also need to store it for future shift operations (to know last element value).
                    // We can store all elements in a deque and physically perform operations? 
                    // q up to 2e5, total q 2e5, so O(q) physical operations might be okay if we use deque.
                    // However, cyclic shift on deque is O(1) if we move front/back.
                    // Reverse can be handled with a flag.
                    // Let's do that: maintain a deque and a reversed flag.
                    // But we need to output rizziness after each operation quickly.
                    // We can maintain rizz and rizz_rev with formulas.
                    // Let's use a deque to represent the current logical array.
                    // Actually, we can just maintain the array in a vector and never change it, but we need to know the last element for shift.
                    // The last element of current logical array depends on reversed and shift.
                    // If we store elements in original order (the order they were appended), we can compute the last element.
                    // Let's define original array as the sequence of appends in the order they were performed, ignoring shifts and reverses.
                    // But shifts and reverses change the logical order. If we never physically reorder, we need to map logical indices to original indices.
                    // This mapping is: logical index i -> original index = (i - shift) mod n if not reversed, else (n - 1 - i - shift) mod n.
                    // When we append, the new element is placed at logical index n. Its original index would be (n - shift) mod n if not reversed, etc.
                    // But if we just push_back to original vector, its original index is n. That doesn't match the mapping unless shift=0.
                    // So we would need to insert at the correct position. That's O(n) if we use vector.
                    // To avoid O(n), we can use a deque and physically maintain the logical order.
                    // Since total operations 2e5, O(1) per operation with deque is fine.
                    // Let's implement with deque.
                }
                // Let's switch to a simpler approach: maintain the array in a deque, and keep track of rizz and rizz_rev.
                // We'll physically perform operations on deque, but reverse is just a flag.
                // For cyclic shift: if not reversed, move back to front; if reversed, move front to back? 
                // Actually, cyclic shift on current array: [a1, a2, ..., an] -> [an, a1, ..., a_{n-1}].
                // If we maintain a deque representing the current array, we can do:
                // if not reversed: pop_back, push_front.
                // if reversed: the current array is reversed. Cyclic shift on it: last becomes first.
                // But our deque stores the current array in its logical order. So we can just do pop_back and push_front regardless of reversed flag? 
                // Wait, if we have a reversed flag, the deque might store the array in a certain order. 
                // Let's not use a reversed flag for the deque; instead, always keep the deque in the current logical order.
                // Then reverse operation would need to physically reverse the deque, which is O(n). Not good.
                // So we keep a flag. But then cyclic shift depends on the flag.
                // If flag is false: deque is in logical order. Cyclic shift: move last to first.
                // If flag is true: deque is in reversed logical order? Actually, we can store the deque always in the order of appends, and use shift and reversed to compute logical order.
                // But we need to know the last element for shift. We can compute it using the mapping.
                // Let's stick to the mapping approach with vector, but we need to append efficiently.
                // When appending, we can just push_back to the vector, but we need to adjust shift so that the new element appears at the end of logical array.
                // If we always push_back, the new element's original index is n. We want its logical index to be n.
                // Logical index n corresponds to original index = (n - shift) % n if not reversed.
                // For this to equal n, we need (n - shift) % n = n, which is impossible since n < n? Actually n is the new size, so original index n is out of bounds of old array.
                // We are inserting a new element, so the array size becomes n+1. The new element should be at logical index n (the last position).
                // If we just push_back, its original index is n. We can then set shift such that logical index n maps to original index n.
                // For not reversed: logical index i maps to (i - shift) mod (n+1). We want (n - shift) mod (n+1) = n.
                // This implies shift = 0. So if we always reset shift to 0 after append? But then we lose previous shifts.
                // Alternatively, we can insert the new element at the correct position in the vector to maintain the mapping with current shift.
                // That position is: we want the new element to be at logical index n. Its original index should be such that when we apply shift and reversed, it becomes logical index n.
                // Original index = (n - shift) mod (n+1) if not reversed.
                // So we can insert at that index in the vector. But inserting in middle of vector is O(n). Total O(n^2) worst case.
                // However, we can use a deque and maintain it in logical order, but handle reverse with a flag and swap front/back for operations.
                // Let's design a data structure: we maintain a deque, but we also have a flag 'reversed'.
                // The deque always stores the array in the order: if not reversed, front is first element, back is last element.
                // If reversed, front is last element, back is first element? That would be confusing.
                // Better: store the array in a fixed order (say, the order of appends) and just compute rizz and rizz_rev mathematically.
                // For shift, we just update shift variable. For reverse, we flip reversed flag and swap rizz/rizz_rev.
                // For append, we need to add the new element to our storage and update rizz/rizz_rev.
                // The new element is appended to the end of the current logical array.
                // Its logical index is n (0-based). Its contribution to rizz is k * (n+1).
                // Its contribution to rizz_rev: in the reversed array, it would be at the beginning, so its 1-based index is 1, contribution k * 1.
                // But we also need to update the contributions of existing elements? No, their multipliers don't change because we just append at the end.
                // Wait, when we append, the length increases, so the multipliers for existing elements in rizz change? 
                // Rizziness is sum of element * (index). If we append at the end, the existing elements keep their positions, so their multipliers remain the same.
                // Only the new element adds k * (n+1). So rizz increases by k*(n+1).
                // For rizz_rev: the reversed array will have the new element at the front, so existing elements shift right by 1.
                // Their multipliers increase by 1. So rizz_rev becomes rizz_rev + sum + k * 1.
                // Let's verify: original array A of length n, rizz = sum_{i=1}^n A[i]*i.
                // Append k: new array A' = [A[1], ..., A[n], k]. rizz' = rizz + k*(n+1).
                // Reversed array of A': rev(A') = [k, A[n], ..., A[1]].
                // rizz_rev' = k*1 + sum_{i=1}^n A[i]*(i+1) = k + sum_{i=1}^n A[i]*i + sum_{i=1}^n A[i] = k + rizz + sum.
                // But wait, rizz_rev was the rizziness of rev(A). rev(A) = [A[n], ..., A[1]].
                // rizz_rev = sum_{i=1}^n A[n+1-i]*i.
                // After append, rev(A') = [k, A[n], ..., A[1]].
                // rizz_rev' = k*1 + sum_{i=1}^n A[n+1-i]*(i+1) = k + sum_{i=1}^n A[n+1-i]*i + sum_{i=1}^n A[n+1-i] = k + rizz_rev + sum.
                // Yes! So we can update:
                // rizz += k * (n+1)
                // rizz_rev += k + sum
                // sum += k
                // Then we need to store k somewhere to be able to compute last element for future shifts.
                // We need to know the last element of the current logical array for shift operation.
                // If we don't store the array, how do we know the last element?
                // We can store the elements in a deque in logical order, but we don't physically reverse, we just use reversed flag.
                // Let's maintain a deque that always represents the logical array (current view).
                // When we append, we push_back to deque.
                // When we cyclic shift, if not reversed: we move back to front. If reversed: we move front to back? 
                // Actually, if we maintain the deque in logical order, then reversed flag is not needed for the deque; we just physically perform operations on deque.
                // But reverse operation would require reversing the deque, which is O(n). We can avoid that by keeping a flag and interpreting front/back accordingly.
                // Let's keep a deque and a boolean 'rev'. The deque stores the array in the order: if rev=false, deque[0] is first element, deque.back() is last.
                // If rev=true, deque[0] is last element, deque.back() is first.
                // Then:
                // - Append: we want to add to the end of logical array. If rev=false, end is back, so push_back. If rev=true, end is front, so push_front.
                // - Cyclic shift: move last to first. If rev=false, last is back, first is front: pop_back, push_front. If rev=true, last is front, first is back: pop_front, push_back.
                // - Reverse: just flip rev.
                // This gives O(1) per operation. We also need to maintain rizz and rizz_rev.
                // We can update rizz and rizz_rev using formulas without iterating.
                // For append: we already have formulas.
                // For cyclic shift: we need to know the last element value. We can get it from deque: if rev=false, last = deque.back(); if rev=true, last = deque.front().
                // Then update rizz and rizz_rev as derived earlier.
                // For reverse: just swap rizz and rizz_rev.
                // This is perfect.

                // Let's implement this approach.
                // We'll restart the logic.
            }
        }
    }
    return 0;
}
