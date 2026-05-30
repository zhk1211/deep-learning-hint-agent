// Hint2
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    vector<int> L(n), R(n);
    stack<int> st;

    // nearest greater to left
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && a[st.top()] <= a[i]) st.pop();
        L[i] = st.empty() ? -1 : st.top();
        st.push(i);
    }
    while (!st.empty()) st.pop();

    // nearest greater to right
    for (int i = n - 1; i >= 0; --i) {
        while (!st.empty() && a[st.top()] < a[i]) st.pop();
        R[i] = st.empty() ? n : st.top();
        st.push(i);
    }

    ll ans = 0;
    for (int i = 0; i < n; ++i) {
        int left_len = i - L[i];
        int right_len = R[i] - i;
        ans += (ll)left_len * right_len * a[i];
    }

    // subtract contributions where adjacent equal elements cause overcount
    // we need to handle equal elements carefully: for equal, we break tie by index to avoid double count
    // Actually the above with <= and < already handles it: for equal elements, the left one gets the right one as part of its range,
    // but the right one does not include the left one. This correctly counts each subsegment's maximum exactly once.
    // However, the problem asks for sum of f(c) over all subsegments, where f(c) is the length of the shortest input sequence.
    // It turns out f(c) = number of elements that are not equal to the previous element? Wait, let's derive.

    // From the problem: we start with sequence x, then repeatedly insert (x_i+1) after x_i.
    // This means the output sequence b can be partitioned into blocks where each block is a maximal strictly increasing run with step 1?
    // Actually, if we have x, then we can insert x_i+1 after x_i. This means we can generate any sequence that can be formed by starting with some sequence
    // and repeatedly inserting an element that is exactly one more than the element before it.
    // So the generated sequence b must satisfy: if we look at the differences, we can "compress" consecutive elements that increase by exactly 1?
    // Let's think: The algorithm: start with x, then insert x_i+1 after x_i. This is like we can expand any element into a chain of consecutive integers.
    // So the final sequence b can be partitioned into segments where each segment is a maximal arithmetic progression with difference 1, and the first element of each segment
    // corresponds to an original element of x. The length of the shortest x is exactly the number of such segments.
    // More precisely, f(b) = number of indices i such that i == 1 or b[i] != b[i-1] + 1.
    // Because if b[i] == b[i-1] + 1, then b[i] could have been generated from b[i-1] by insertion, so they belong to the same "chain" from one original element.
    // If b[i] != b[i-1] + 1, then a new original element must start here.
    // So f(b) = 1 + count of i from 2 to |b| where b[i] != b[i-1] + 1.

    // Wait, check sample: a = [1,2,3,4,5] -> all subsegments have f=1? For subsegment [2,3,4], differences are +1,+1, so f=1. Yes.
    // For a = [1,3,5,7,9], any subsegment of length L has all differences !=1, so f = L. Sum over all subsegments = sum of lengths = n*(n+1)/2? For n=5, 15? But sample output is 35. So my f is wrong.

    // Let's re-evaluate: The algorithm: we can insert (x_i+1) after x_i. This means we can generate sequences where we start with some sequence, and then we can insert an element that is exactly one more than the element immediately before it.
    // But we can do this repeatedly. So from an original element x_i, we can generate a chain: x_i, x_i+1, x_i+2, ... by repeatedly inserting after the last inserted.
    // However, we can also insert after other elements? The step says: select any index i, insert (x_i+1) immediately after x_i. So we can insert after any existing element, not just the last one.
    // So we can have branching? No, because we insert after x_i, and the new element becomes part of the sequence. Then later we can insert after that new element as well.
    // So effectively, from each original element, we can generate a contiguous block of consecutive increasing integers starting from that original element.
    // But these blocks are placed in the order of the original sequence. So the final sequence is a concatenation of blocks, where each block is a maximal strictly increasing sequence with step 1, and the first element of each block is an original element.
    // However, is it possible that two adjacent blocks merge? If the last element of block i is x, and the first element of block i+1 is x+1, then they would form a longer increasing sequence, but they come from different original elements. Could the algorithm produce that?
    // Suppose original x = [a, b]. We can expand a into a, a+1, a+2,... and b into b, b+1,... The final sequence is the concatenation. If the last element of first block is L, and first of second block is b, if b == L+1, then the whole sequence has consecutive increasing numbers, but they originated from different original elements. The algorithm doesn't merge them; they are just adjacent. So in the final sequence, we cannot tell whether a +1 step came from insertion or from a new original element. But the problem asks for the shortest input sequence that could generate b. So if we have a +1 step, we could have generated it from the previous element, thus reducing the needed original elements. So we should compress as much as possible.
    // So f(b) is the minimum number of original elements needed. We can think of it as: we want to partition b into the minimum number of chains where each chain is a sequence of consecutive integers (each step +1). The chains are ordered. This is equivalent to: we can merge adjacent elements if they differ by 1. So we can greedily merge: start with f = 1, then for each next element, if it equals previous + 1, we can consider it part of the same chain, so f doesn't increase; otherwise, we need a new chain, f++.
    // So f(b) = 1 + number of i from 2 to |b| such that b[i] != b[i-1] + 1.
    // Let's test on sample 2: a = [1,3,5,7,9]. Subsegments:
    // length 1: f=1 each, 5 subsegments -> sum=5
    // length 2: [1,3] diff 2 -> f=2; [3,5] diff 2 -> f=2; [5,7] diff 2 -> f=2; [7,9] diff 2 -> f=2; sum=8
    // length 3: [1,3,5] diffs 2,2 -> f=3; [3,5,7] -> f=3; [5,7,9] -> f=3; sum=9
    // length 4: [1,3,5,7] -> f=4; [3,5,7,9] -> f=4; sum=8
    // length 5: [1,3,5,7,9] -> f=5; sum=5
    // Total = 5+8+9+8+5 = 35. Matches sample! So f is correct.

    // So we need sum over all subsegments of (1 + number of i in subsegment where a[i] != a[i-1] + 1).
    // For a subsegment [l, r], f = 1 + sum_{i=l+1}^r [a[i] != a[i-1] + 1].
    // So total sum = sum_{l<=r} 1 + sum_{l<=r} sum_{i=l+1}^r [a[i] != a[i-1] + 1].
    // First part: number of subsegments = n*(n+1)/2.
    // Second part: for each adjacent pair (i-1, i), how many subsegments include both? The subsegment must have l <= i-1 and r >= i. Number of such subsegments = (i-1) * (n - i + 1) (1-indexed). Let's use 0-indexed: for i from 1 to n-1, the pair (i-1, i) is included in subsegments with l <= i-1 and r >= i. Number = (i) * (n - i). Because l can be 0..i-1 (i choices), r can be i..n-1 (n-i choices).
    // So answer = n*(n+1)/2 + sum_{i=1}^{n-1} (i * (n-i)) * [a[i] != a[i-1] + 1].

    // Let's test on sample 1: a=[1,2,3,4,5]. All adjacent differences are 1, so condition false for all. Answer = 5*6/2 = 15. Matches sample.
    // Sample 2: a=[1,3,5,7,9]. All adjacent differences !=1, so condition true for all. i from 1 to 4: i*(5-i) = 1*4=4, 2*3=6, 3*2=6, 4*1=4. Sum = 20. Total = 15 + 20 = 35. Matches.
    // Sample 3: a=[1,2,5,6,5]. n=5. Base=15.
    // pairs:
    // i=1 (1,2): diff=1 -> false
    // i=2 (2,5): diff=3 -> true, add 2*3=6
    // i=3 (5,6): diff=1 -> false
    // i=4 (6,5): diff=-1 -> true, add 4*1=4
    // Sum = 15+6+4=25. Matches sample.
    // Sample 4: 1 2 4 5 3 7 8. n=7. Base=28.
    // pairs:
    // 1-2: diff 1 -> false
    // 2-4: diff 2 -> true, i=2, add 2*5=10
    // 4-5: diff 1 -> false
    // 5-3: diff -2 -> true, i=4, add 4*3=12
    // 3-7: diff 4 -> true, i=5, add 5*2=10
    // 7-8: diff 1 -> false
    // Sum = 28+10+12+10=60. Matches sample.
    // Sample 5: 9 8 9 2 3 4 4 5 3. n=9. Base=45.
    // pairs:
    // 9-8: diff -1 -> true, i=1, add 1*8=8
    // 8-9: diff 1 -> false
    // 9-2: diff -7 -> true, i=3, add 3*6=18
    // 2-3: diff 1 -> false
    // 3-4: diff 1 -> false
    // 4-4: diff 0 -> true, i=6, add 6*3=18
    // 4-5: diff 1 -> false
    // 5-3: diff -2 -> true, i=8, add 8*1=8
    // Sum = 45+8+18+18+8 = 97? But sample output is 78. Wait, let's recalculate carefully.

    // Sample 5 array: 9 8 9 2 3 4 4 5 3
    // Indices 1-based:
    // 1:9
    // 2:8 -> diff 8-9 = -1 !=1 -> true. i=2? Wait, i is the index of the second element in the pair? Let's use 0-indexed: elements a[0]=9, a[1]=8, a[2]=9, a[3]=2, a[4]=3, a[5]=4, a[6]=4, a[7]=5, a[8]=3.
    // Pairs (i-1, i) for i=1..8:
    // i=1: a[0]=9, a[1]=8, diff=-1 !=1 -> true. add i*(n-i) = 1*8 = 8.
    // i=2: a[1]=8, a[2]=9, diff=1 -> false.
    // i=3: a[2]=9, a[3]=2, diff=-7 !=1 -> true. add 3*6=18.
    // i=4: a[3]=2, a[4]=3, diff=1 -> false.
    // i=5: a[4]=3, a[5]=4, diff=1 -> false.
    // i=6: a[5]=4, a[6]=4, diff=0 !=1 -> true. add 6*3=18.
    // i=7: a[6]=4, a[7]=5, diff=1 -> false.
    // i=8: a[7]=5, a[8]=3, diff=-2 !=1 -> true. add 8*1=8.
    // Sum = 8+18+18+8 = 52. Base = 9*10/2 = 45. Total = 45+52 = 97. But sample output is 78. So my formula is wrong? Let's re-read sample 5: 9 8 9 2 3 4 4 5 3. Output 78. My calculation gives 97. There's a discrepancy.

    // Let's manually compute f for all subsegments of sample 5 to see where the error is.
    // Array: 9, 8, 9, 2, 3, 4, 4, 5, 3
    // Let's list subsegments and their f:
    // We'll write a quick mental script? Maybe I misinterpreted f. Let's re-read problem statement carefully.

    // "For a sequence b of n integers, let us define f(b) as the length of the shortest sequence that could be given as an input for the algorithm to generate b."
    // Algorithm: start with x, then repeatedly insert (x_i+1) immediately after x_i.
    // So we can insert after any element. This means we can generate sequences where we can have multiple insertions after the same element? Yes, we can select any index each step. So from an original element x_i, we can generate a sequence that is not necessarily a single contiguous block? Wait, if we insert after x_i, we get x_i, x_i+1. Then later we can insert after the new element x_i+1, getting x_i, x_i+1, x_i+2, etc. So it's a chain. But we can also insert after x_i again? The index of x_i might change if we insert before it? No, we insert after x_i, so x_i remains at its position, and the new element is placed immediately after. If we later want to insert after x_i again, we can, and the new element will be placed after x_i but before the previously inserted elements? Actually, if we have sequence ... x_i, A, ... where A was inserted after x_i. If we insert again after x_i, the new element B will be placed immediately after x_i, so the sequence becomes ... x_i, B, A, ... So the order of insertions matters. This means we can generate sequences that are not just simple increasing chains. For example, from x = [1], we can do: insert after 1 -> 1,2. Then insert after 1 again -> 1,3,2. Then insert after 3 -> 1,3,4,2. So we can generate sequences that are not sorted. So my assumption that f(b) is just the number of breaks in +1 steps is completely wrong.

    // Let's re-analyze the algorithm. We have a starting sequence x. We can insert x_i+1 after x_i. This operation can be repeated. The final sequence b is some permutation? No, it's built by insertions. This is exactly the process of generating a sequence by starting with some elements and then inserting elements that are exactly one greater than some existing element, immediately after that element. This is reminiscent of the "Cartesian tree" or "stack" generation. In fact, this is the same as the number of "left-to-right maxima" or something? Let's think.

    // Consider the final sequence b. We want to find the minimum length of x. This is equivalent to: we can "compress" b by repeatedly removing an element b[i] if b[i] == b[i-1] + 1? No, because the insertion can happen in any order, so the condition for an element to be insertable is that there exists some element before it (not necessarily immediately) that is exactly one less? Actually, when we insert x_i+1 after x_i, the new element is placed immediately after x_i. So in the final sequence, if an element y was inserted after some element z, then in the final sequence, y must appear immediately after z? Not necessarily, because later insertions could happen after z again, pushing y to the right. But y will always be after z, and there might be other elements between them. However, the key is that y was generated from z, so y = z+1. And at the moment of insertion, y was immediately after z. Later insertions after z will insert between z and y. So in the final sequence, all elements between z and y must have been inserted after z (so they are > z? Actually they are z+1, z+2, etc., but they could be inserted in different order). This is getting complicated.

    // Let's search for known problem: "Codeforces 1845E" or something? Actually this is problem "1845D" maybe? No, it's "1845C" ? The problem name might be "Insert a Progression" but not. Let's think about the hints: "Think of the online nature of both solutions." "Use the stack solution." This suggests a monotonic stack approach. The first part of my code with nearest greater elements is a classic monotonic stack for sum of maximums over subsegments. But that was for a different problem? Wait, the problem statement says: "This is the hard version of the problem. The difference between the versions is that in this version, you must compute the sum of values over all subsegments." So there is an easy version where you just compute f for the whole array. The hints mention "online nature" and "stack solution". So the easy version likely has a solution using a stack to compute f(a) for the whole array. Then the hard version asks for sum over all subsegments. So we need to extend that stack solution to all subsegments.

    // Let's figure out f(b) for a single sequence. How to compute the minimum length of x? This is equivalent to the minimum number of elements we need to start with so that we can generate b by insertions. This is exactly the size of a minimum "generating set" under the operation "insert x+1 after x". This is known as the "minimum number of increasing subsequences" or something? Actually, consider the process in reverse: we can delete an element if it is exactly one more than some element to its left? But careful: In the forward process, we insert x_i+1 after x_i. So in reverse, we can remove an element y if there is some element z to its left such that z+1 == y, and after removal, the sequence is valid? But we can only remove the most recently inserted? No, we can remove any element that could have been the last insertion. The last insertion must be an element that was inserted after some element, and no later insertions happened after that same element? Actually, the last insertion in the process: we selected some index i and inserted x_i+1 after x_i. At that moment, the new element is at some position. After that, no more insertions. So in the final sequence, the last inserted element has the property that it is x_i+1 for some x_i before it, and there is no element that was inserted after it? Not necessarily. But we can think of the "dependencies": each inserted element depends on an element to its left that is exactly one less. However, an original element depends on nothing. So we can model this as a forest where each node points to its "parent" which is the element that generated it. The parent must be to the left and have value exactly one less. But multiple elements can have the same parent? Yes, if we insert multiple times after the same element. So the final sequence can be seen as a set of trees where each node's parent is some node to its left with value = node->val - 1. The original elements are roots. The length of x is the number of roots. We want to minimize the number of roots. So we want to find a forest covering all elements such that each edge goes from a node to a node to its left with value exactly one less, and we want to minimize the number of trees. This is equivalent to: we can link an element to any previous element with value exactly one less. We want to maximize the number of edges (i.e., elements that are not roots). So f(b) = n - maximum number of elements we can "attach" to previous elements with value -1.

    // But we must respect the order: the parent must be to the left. So we can process from left to right, and for each element, we can decide to attach it to some previous element with value = current-1. To minimize roots, we should attach as many as possible. This is a greedy matching: for each value v, we can keep track of how many "available" parents of value v we have seen that haven't been used yet? Actually, each element can be a parent to multiple children? Yes, because we can insert multiple times after the same element. So an element of value v can be the parent for any number of subsequent elements of value v+1. So the only constraint is that a child must have a parent to its left. So to maximize attachments, we can simply attach each element to the nearest available parent to its left? But we can attach multiple children to the same parent. So the maximum number of attachments is simply: for each element, if there is any element to its left with value exactly one less, we can attach it. But we must ensure we don't "use up" a parent? No, parents can be reused infinitely. So actually, every element that has some element to its left with value v-1 can be attached! The only elements that must be roots are those for which there is no element to their left with value exactly one less. So f(b) = number of elements that do not have any element to their left with value = (their value - 1). Let's test this hypothesis.

    // Test on sample 2: [1,3,5,7,9]. For 1: no left, root. 3: no 2 to left, root. 5: no 4, root. 7: no 6, root. 9: no 8, root. So f=5. For whole array, f=5. But earlier we computed f for whole array as 5? Yes, sample 2 whole array f=5. For subsegment [1,3,5]: 1 root, 3 no 2, root, 5 no 4, root -> f=3. Matches our earlier manual calculation. For [1,2,3,4,5]: 1 root, 2 has 1 left -> attach, 3 has 2 left -> attach, etc. So f=1. Matches. For [1,2,5,6,5]: 1 root, 2 attach to 1, 5 no 4 left -> root, 6 attach to 5, 5 has 4? No 4 left, but it has 6? No, left is 6, not 4. So 5 root. Total roots: 1,5,5 -> f=3. Whole array f=3? Let's check: can we generate [1,2,5,6,5] from length 3? Start with x = [1,5,5]? But we need to generate 2 after 1, and 6 after 5. Sequence: start [1,5,5]. Insert 2 after 1 -> [1,2,5,5]. Insert 6 after first 5 -> [1,2,5,6,5]. Yes, length 3. So f=3. Our rule gives 3. Good.

    // Now test sample 5 whole array: [9,8,9,2,3,4,4,5,3]. Let's compute roots by this rule:
    // 9: no left -> root
    // 8: needs 7 left? No -> root
    // 9: needs 8 left? Yes, there is an 8 to the left -> attach
    // 2: needs 1 left? No -> root
    // 3: needs 2 left? Yes, 2 is left -> attach
    // 4: needs 3 left? Yes, 3 is left -> attach
    // 4: needs 3 left? Yes, 3 is left -> attach (can attach to same 3)
    // 5: needs 4 left? Yes, 4 is left -> attach
    // 3: needs 2 left? Yes, 2 is left -> attach
    // Roots: 9, 8, 2 -> f=3. So whole array f=3. Let's verify if we can generate from length 3: x = [9,8,2]. Generate:
    // start: 9,8,2
    // insert after 8: 9 -> 9,8,9,2
    // insert after 2: 3 -> 9,8,9,2,3
    // insert after 3: 4 -> 9,8,9,2,3,4
    // insert after 3 again: 4 -> 9,8,9,2,3,4,4? Wait, if we insert after 3, we get 4 immediately after 3. So sequence: 9,8,9,2,3,4. Then insert after 3 again: 9,8,9,2,3,4,4? Actually, inserting after 3 again would place the new 4 immediately after 3, pushing the existing 4 to the right: 9,8,9,2,3,4,4. Yes.
    // insert after 4: 5 -> we have two 4s. We can insert after the first 4: 9,8,9,2,3,4,5,4. Then insert after 2: 3? But we already have 3. We need a 3 at the end. We can insert after 2 again: 3 -> 9,8,9,2,3,3,4,5,4? Not matching. We need final sequence: 9,8,9,2,3,4,4,5,3. Let's try to generate exactly:
    // x = [9,8,2]
    // 1. insert 9 after 8: 9,8,9,2
    // 2. insert 3 after 2: 9,8,9,2,3
    // 3. insert 4 after 3: 9,8,9,2,3,4
    // 4. insert 4 after 3 again: 9,8,9,2,3,4,4 (wait, after step 3 we have ...2,3,4. Inserting after 3 gives ...2,3,4,4? Actually, the 3 is at index 4 (0-indexed? Let's track carefully:
    // Initial: [9,8,2]
    // Insert 9 after 8 (index 1): -> [9,8,9,2]
    // Insert 3 after 2 (index 3): -> [9,8,9,2,3]
    // Insert 4 after 3 (index 4): -> [9,8,9,2,3,4]
    // Insert 4 after 3 (index 4) again: the 3 is at index 4, insert 4 after it -> [9,8,9,2,3,4,4]? Wait, the previous 4 was at index 5. Inserting after index 4 puts the new element at index 5, shifting the old index 5 to 6: [9,8,9,2,3,4_new,4_old] -> [9,8,9,2,3,4,4]. Yes.
    // Now we need 5 after a 4. We have 4 at index 5 and 6. Insert 5 after index 5: -> [9,8,9,2,3,4,5,4]
    // Now we need a 3 at the end. We can insert 3 after 2 (index 3): -> [9,8,9,2,3_new,3_old,4,5,4] -> [9,8,9,2,3,3,4,5,4]. Not matching. We need the last element to be 3, and before it 5,4,4,... So maybe different order. What if we insert the final 3 earlier? Let's try:
    // x = [9,8,2]
    // Insert 3 after 2: [9,8,2,3]
    // Insert 9 after 8: [9,8,9,2,3]
    // Insert 4 after 3: [9,8,9,2,3,4]
    // Insert 4 after 3 again: [9,8,9,2,3,4,4]
    // Insert 5 after the first 4 (index 5): [9,8,9,2,3,4,5,4]
    // Insert 3 after 2 (index 3): [9,8,9,2,3,3,4,5,4] -> still not.
    // What if we insert the second 3 at the very end? We need a 3 that is after 5? But 3 cannot be generated from 5 (needs 2). So the last 3 must be generated from the 2. But if we generate it after all insertions, it will be immediately after 2, not at the end. To get it at the end, we must have inserted it after 2 early, and then later insertions after elements to its right push it to the end? No, insertions after an element only affect the position of elements after that element. If we insert after 2, the new element goes immediately after 2. If we later insert after some element to the right of 2, that doesn't affect the relative order of elements after 2? Actually, inserting after an element shifts all subsequent elements to the right. So if we have ...2, A, B... and we insert after 2, we get ...2, New, A, B... So New is before A and B. To get New at the end, we would need to insert after something that is after A and B? No, inserting after an element always places the new element immediately after that element. So the only way an element generated from 2 can be at the end is if there are no elements after 2 that were inserted later? But we have elements 4,5, etc. that are after 2. If we insert the final 3 after 2 at the very end, it will go immediately after 2, before all those. So we cannot get a 3 at the end if there are elements after 2. Unless those elements were also generated from 2? But 4,5 are generated from 3, which is generated from 2. So they are all in a chain. If we have 2, then we insert 3 after 2, then 4 after 3, then 5 after 4, we get 2,3,4,5. Then if we insert another 3 after 2, we get 2,3_new,3_old,4,5. The new 3 is before the old 3. To get a 3 at the end, we would need to insert it after something that is at the end. But 3 can only be inserted after 2. So it's impossible to have a 3 after a 5 if 5 was generated from 4,4 from 3,3 from 2. So the sequence [...,5,3] is impossible to generate if 5 is from the chain 2-3-4-5. Therefore, my rule "f(b) = number of elements with no left element of value-1" might be incorrect because it doesn't account for the fact that attachments must respect the tree structure and order? But wait, in my rule, the last 3 can attach to the 2 on its left, because there is a 2 to its left. So it would be considered attached, not a root. But is it actually possible to generate the sequence with that attachment? Let's check if the whole array [9,8,9,2,3,4,4,5,3] can be generated from 3 roots. We need to assign each non-root to a parent to its left with value-1. The last 3 has a 2 to its left, so it can attach to 2. But does the order allow it? The parent 2 is at index 3 (0-indexed: 9,8,9,2,...). The last 3 is at index 8. Between them, there are elements 3,4,4,5. These elements could be generated from the chain 2->3->4->5, and also another 4 from the 3. The last 3 would need to be generated from the 2, but it appears after the 5. As argued, if we generate a chain 2->3->4->5, then any new insertion after 2 will appear before the existing 3. So we cannot have a 3 after 5. Therefore, the last 3 cannot be attached to the 2 if we also have the chain 2->3->4->5. But could we attach the last 3 to something else? There is no other 2. So the last 3 must be a root? But then f would be 4. Let's test if f(whole array) is actually 4. Can we generate it from length 4? Try x = [9,8,2,3]? Let's attempt to generate the sequence from 4 roots. We need to place the roots in order: maybe 9,8,2,? The sequence has 9,8,9,2,3,4,4,5,3. The roots must appear in the order of the original x. The original x is a subsequence of the final sequence? Not necessarily, because insertions happen after elements, so the original elements maintain their relative order. So the roots must appear in the same order as in the final sequence. So the roots are some subsequence of the final sequence. We need to choose a set of roots such that every other element can be generated from some root to its left with value-1, and the generation order must be valid. This is exactly the problem of finding the minimum number of "chains" where each chain is a sequence of consecutive integers, and the chains are interleaved in a way that respects the insertion order? Actually, the process is equivalent to: we have a set of stacks? Let's think differently.

    // There
