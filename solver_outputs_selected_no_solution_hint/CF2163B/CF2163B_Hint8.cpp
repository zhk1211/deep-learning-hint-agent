#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> p(n);
    for (int i = 0; i < n; ++i) cin >> p[i];
    string x;
    cin >> x;

    vector<int> pos(n + 1);
    for (int i = 0; i < n; ++i) pos[p[i]] = i;

    // Check if x is all same
    char first = x[0];
    bool all_same = true;
    for (char c : x) if (c != first) { all_same = false; break; }

    if (all_same) {
        if (first == '0') {
            cout << "0\n";
            return;
        }
        // x all '1'
        // Check if permutation is sorted
        bool sorted = true;
        for (int i = 0; i < n; ++i) if (p[i] != i + 1) { sorted = false; break; }
        if (sorted) {
            cout << "-1\n";
            return;
        }
        // Need to cover all indices 1..n-2 (0-indexed 1..n-2)
        // Find l,r such that min(p[l],p[r]) < p[i] < max(p[l],p[r]) for all i in (l,r)
        // We can use l=pos[1], r=pos[n] if they are not adjacent and cover all?
        // Actually we need to cover all positions except possibly ends.
        // We can do operation with l=pos[1], r=pos[n] if pos[1] < pos[n] and pos[n]-pos[1] > 1
        // But this only covers indices between them. We need to cover all indices 1..n-2.
        // If pos[1] == 0 and pos[n] == n-1, then operation (0, n-1) covers all interior.
        // But if not, we might need two operations.
        // However, we can always do it in at most 2 operations if not sorted.
        // Let's find two operations that cover all required indices.
        // Required set: all i where x[i]=='1', but here all are '1', so all i from 1 to n-2 (0-indexed 1..n-2).
        // We can cover all interior indices by using l=pos[1], r=pos[n] if they are not adjacent and cover all?
        // Actually we can just output any valid sequence.
        // Since n>=3, we can try to cover all interior indices with one operation if possible.
        // One operation covers indices between l and r where value is between p[l] and p[r].
        // To cover all interior indices, we need l and r such that all other indices have values between p[l] and p[r].
        // That means p[l] and p[r] must be 1 and n (or n and 1) and l,r must be the two ends of the array.
        // If pos[1] and pos[n] are the two ends, one operation suffices.
        // Otherwise, we can use two operations: (pos[1], pos[n]) and something else?
        // Actually we can always cover all interior indices with two operations:
        // Operation 1: (pos[1], pos[n]) covers indices between them with values between 1 and n (all).
        // Operation 2: (pos[n], pos[1]) if pos[1] > pos[n]? But we need l<=r.
        // Let's think differently: we can cover all indices except possibly the ends by using two operations that together cover everything.
        // Since we only need to cover indices 1..n-2, we can do:
        // If pos[1] < pos[n]:
        //   Op1: (pos[1], pos[n]) covers all indices between them.
        //   Indices outside [pos[1], pos[n]] are not covered. But we only need to cover indices 1..n-2.
        //   If pos[1] > 0, then index 0 is not covered, but we don't need to cover 0 if x[0]=='1'? Wait x is all '1', so we need to cover index 0 as well? No, condition: if x_i=1 then s_i=1. x[0]='1', so we need s[0]=1. But operation only sets s_i for l < i < r. So ends l and r are never set. So we cannot set s[0] or s[n-1] using operations. Thus if x[0]=='1' or x[n-1]=='1', it's impossible unless we can set them? Wait, the operation sets s_i for l < i < r. So s_l and s_r remain 0. So if x[0]=='1', we must have s[0]=1, but we can never set s[0] because it's an endpoint. So if x[0]=='1' or x[n-1]=='1', it's impossible? But sample 1: n=3, x=010, operation (1,3) sets s_2=1, s_1 and s_3 remain 0. x[0]='0', x[2]='0'. So endpoints can be 0. In sample 2: x=11111, n=5, output -1. So indeed, if x has '1' at endpoints, it might be impossible. But wait, sample 3: x=001100, n=6, endpoints are 0. Sample 4: x=110110, n=6, endpoints: x[0]='1', x[5]='0'? Actually x=110110, indices 0..5: 1,1,0,1,1,0. So x[0]='1', but output is -1. So endpoints matter.
        // Actually, we can never set s[0] or s[n-1] to 1 because they are never in (l,r). So if x[0]=='1' or x[n-1]=='1', it's impossible. Thus for all same '1', if n>=3, endpoints are '1', so impossible. But wait, if n=3, x=111, endpoints are 1, impossible. Sample 2: n=5, x=11111, endpoints 1, impossible. So if all same '1', it's always impossible because endpoints are '1' and cannot be set. Except if n<3? But n>=3. So output -1.
        cout << "-1\n";
        return;
    }

    // x is not constant
    // We can always achieve in at most 2 operations? Let's see hints: "If x is not constant, what is the maximum number of operations you need to perform?" Hint 4: probably 2.
    // We need to set s_i=1 for all i where x_i='1'. We can never set endpoints. So if x[0]=='1' or x[n-1]=='1', impossible.
    if (x[0] == '1' || x[n-1] == '1') {
        cout << "-1\n";
        return;
    }

    // Now we need to cover all indices i (1<=i<=n-2) where x[i]=='1'.
    // We can use operations that set ranges of indices.
    // Observation: An operation (l,r) sets s_i=1 for i in (l,r) if p_i is between p_l and p_r.
    // This is equivalent to: if we consider the Cartesian tree? Or we can think of intervals.
    // We can always cover a set of indices if we can find l,r such that the values at l and r bracket the values at the target indices.
    // Since we can do up to 5 operations, we can try to cover all '1's with two operations.
    // Idea: Use the minimum and maximum values in the array to cover everything between them.
    // Let L = pos[1], R = pos[n] (or vice versa). If L < R, then operation (L, R) covers all indices between L and R because all values are between 1 and n.
    // So that operation sets s_i=1 for all i in (L, R). If L > R, then (R, L) covers all between R and L.
    // So one operation can cover the entire interval between the positions of 1 and n.
    // The uncovered parts are outside [min(L,R), max(L,R)].
    // We can cover the left part by an operation involving the left endpoint and something else, and similarly for the right part.
    // Since we only need to cover indices where x_i='1', we can check if the '1's are all inside the interval between pos[1] and pos[n]. If so, one operation suffices.
    // Otherwise, we might need two operations: one to cover the left part and one to cover the right part.
    // But we can also cover the left part by choosing l=0 and r=pos[1] (if pos[1] > 0) and the operation will cover indices between them with values between p[0] and 1? Wait, p[0] is some value. To cover indices between 0 and pos[1], we need values between p[0] and p[pos[1]]=1. Since 1 is the minimum, p[0] > 1, so values between them are those >1 and <p[0]. That might not cover all indices. Better: use the global min and max.
    // Actually, we can always cover any set of '1's with at most 2 operations by using the following:
    // Let a = pos[1], b = pos[n]. Assume a < b (if not, swap).
    // Operation 1: (a, b) covers all i in (a, b).
    // Now we need to cover '1's in [1, a-1] and [b+1, n-2].
    // To cover left part, we can use operation (0, a). This covers i in (0, a) with values between p[0] and 1. Since 1 is min, values between are those < p[0]. But we need to cover all '1's there. Is it always possible? Not necessarily, because some values might be > p[0] and thus not covered.
    // Alternative: Use operation (0, b) or (a, n-1)? 
    // Let's think differently: We can choose l and r such that the interval of values covers exactly the required range.
    // Since we can do up to 5 operations, we can just try to cover each contiguous block of '1's with one operation.
    // But maybe we can always do it in 2 operations.
    // Consider the set of indices where x_i='1'. They form contiguous segments (since we only care about '1's, but they might not be contiguous). However, we can cover multiple segments with one operation if they lie between l and r and their values are between p_l and p_r.
    // Notice that if we take l = pos[1] and r = pos[n] (assuming pos[1] < pos[n]), then all indices between them are covered. So any '1' in (pos[1], pos[n]) is covered.
    // The only '1's not covered are those outside this interval. They must be either to the left of pos[1] or to the right of pos[n].
    // To cover a '1' at index i < pos[1], we need an operation (l, r) with l <= i < r and p_i between p_l and p_r.
    // If we choose l = i, r = pos[1], then p_l = p_i, p_r = 1. Since 1 is min, the condition min(p_l,p_r) < p_j < max(p_l,p_r) becomes 1 < p_j < p_i. This covers indices between i and pos[1] with values between 1 and p_i. But we only need to cover i? Wait, operation sets s_j for l < j < r. So if l=i, r=pos[1], then i is not covered (since l < j < r). So we cannot cover i itself if we use it as an endpoint. So to cover i, we need l < i < r. So we need an operation where i is strictly inside.
    // Thus, to cover a '1' at index i, we need to find l < i < r such that p_i is between p_l and p_r.
    // This is always possible if we can find l and r with values that bracket p_i. For example, we can take l = pos[1] (if pos[1] < i) and r = pos[n] (if pos[n] > i). Then p_l=1, p_r=n, so p_i is between. So if i is between pos[1] and pos[n], it's covered by (pos[1], pos[n]).
    // If i is to the left of pos[1], then we need r > i such that p_r > p_i. We can take r = pos[1] (since p_r=1 < p_i? No, 1 < p_i, so min=1, max=p_i, so p_i is NOT between 1 and p_i because it's equal to max. Condition: min < p_i < max. So p_i must be strictly between. So if p_r = 1, then max = p_i, min = 1, and p_i is not < max. So it fails. So we need p_l and p_r such that one is less than p_i and one is greater than p_i.
    // So to cover i < pos[1], we need an endpoint with value > p_i. We can use r = pos[n] if pos[n] > i. But if pos[n] < i? Then i is to the right of pos[n]? Actually if i < pos[1], then pos[n] could be anywhere. If pos[n] > i, then we can use l = something with value < p_i, r = pos[n] (value n > p_i). Then p_i is between. What can we use for l? We need l < i with p_l < p_i. Since i < pos[1], and pos[1] is the position of 1, there might not be any element with value < p_i to the left of i. But we can use l = 0? p[0] might be > p_i or < p_i. We can always find some l? Not necessarily.
    // This suggests that covering arbitrary '1's might require more operations.
    // Let's look at the hints: "Can you somehow get the answer for x using the answer for x+1?" and "What happens when x is constant and p is not sorted?" 
    // Hint 8: "How would you solve the problem if x is constant and p is not sorted?" We already saw that if x is all '1', it's impossible because endpoints can't be set. But if x is all '1' except endpoints? That is, x[0]='0', x[n-1]='0', and all others '1'. Is it always possible? Let's test: n=4, p=[2,1,4,3], x=0110. Can we set s_1 and s_2 to 1? We need operations. Try (1,4) (1-indexed): l=1, r=4, p1=2, p4=3. min=2, max=3. Indices between: 2,3. p2=1 not between, p3=4 not between. So no. (1,3): p1=2, p3=4, min=2, max=4. p2=1 not between. (2,4): p2=1, p4=3, min=1, max=3. p3=4 not between. So we cannot cover both. Maybe two operations? (1,3) and (2,4)? (1,3) covers nothing. (2,4) covers nothing. So impossible? But maybe we can do (1,2)? l=1, r=2, no interior. So impossible. So not always possible.
    // So we need a more systematic approach.

    // Let's think about the structure. The operation sets s_i=1 if i is between l and r and p_i is between p_l and p_r. This is equivalent to: in the permutation, if we consider the interval [l,r], the set of values that get set are those that are not the minimum or maximum of that interval? Actually, for any subarray, the operation sets all elements that are not the minimum or maximum in that subarray? Wait: min(p_l, p_r) < p_i < max(p_l, p_r). This is exactly the condition that p_i is strictly between the two endpoints. So it sets all elements in (l,r) that are not the global min or max of the subarray? No, only relative to the endpoints. If the subarray has min and max inside, they might still be set if they are between the endpoint values. For example, subarray [2,5,1,4], l=1, r=4, endpoints 2 and 4. min=2, max=4. The interior elements 5 and 1 are not between 2 and 4, so not set. So it only sets elements that lie in the open interval between the endpoint values.

    // Another perspective: Consider the Cartesian tree of the permutation. The operation (l,r) sets all nodes in the open interval (l,r) that are not ancestors of l or r? Not exactly.

    // Let's consider the problem from the perspective of intervals on the value line. We want to cover certain positions. Each operation chooses two positions l and r, and covers all positions i in (l,r) such that p_i is between p_l and p_r. This is like: we have points (i, p_i). An operation picks two points and covers all points in the vertical strip between them that also lie in the horizontal strip between their y-coordinates.

    // Notice that the operation is symmetric: if we swap l and r, the condition is the same. So we can assume p_l < p_r without loss of generality (by swapping l and r if needed). Then the operation covers i in (l,r) with p_l < p_i < p_r.

    // So each operation covers a set of points that lie in the rectangle (l, r) x (p_l, p_r) excluding the boundaries.

    // We need to cover all points with x_i='1'. We can do up to 5 operations.

    // Since n is up to 2e5, we need an efficient check.

    // Let's analyze the necessary and sufficient conditions for a set of '1's to be coverable by one operation.
    // A set S of indices can be covered by a single operation (l,r) with p_l < p_r if and only if there exist l < r such that:
    // - For all i in S, l < i < r and p_l < p_i < p_r.
    // - l and r can be any indices, not necessarily in S.
    // This means S must be contained in some open interval (l,r) and all values in S must lie strictly between p_l and p_r.
    // Equivalently, if we let L = min(S), R = max(S) (in terms of indices), and V_min = min_{i in S} p_i, V_max = max_{i in S} p_i, then we need to find l <= L-1 and r >= R+1 such that p_l < V_min and p_r > V_max (or vice versa if we swap). Also we need l < r.
    // So we need an element to the left of S with value less than all values in S, and an element to the right of S with value greater than all values in S (or left with greater and right with smaller).
    // Actually, we could also have p_l > p_r, then condition is p_r < p_i < p_l. So we need one endpoint with value < all values in S and the other with value > all values in S, and they must be on opposite sides of S (one left, one right).
    // So S is coverable by one operation iff there exists an index l < min(S) and an index r > max(S) such that one of p_l, p_r is < min_{i in S} p_i and the other is > max_{i in S} p_i.
    // This is a nice characterization.

    // Now, we have multiple '1's. They might be split into several connected components? But we can cover multiple components with one operation if they all lie between l and r and their values are between p_l and p_r.
    // So we can think of the whole set of '1's as a set of indices T = {i | x_i='1'}. We want to cover T with at most 5 operations.
    // Since 5 is small, we can try to cover T with a few operations by partitioning T into at most 5 groups, each coverable by one operation.
    // But we need to find any valid sequence.

    // Let's think about the structure of the permutation. The operation is closely related to the concept of "frames" in the permutation. If we consider the minimum and maximum of the whole array, they are at some positions. Any operation that uses these two positions will cover everything between them. So if T is entirely contained between pos[1] and pos[n] (assuming pos[1] < pos[n]), then one operation (pos[1], pos[n]) covers all of T. If T has elements outside, we might need additional operations.

    // In fact, we can always cover T with at most 2 operations if T does not contain the endpoints? Let's test the earlier counterexample: n=4, p=[2,1,4,3], x=0110. T = {1,2} (0-indexed). pos[1]=1, pos[4]=2. So pos[1] < pos[4]. T is between them? min(T)=1, max(T)=2. pos[1]=1, pos[4]=2. So T is exactly the interior between pos[1] and pos[4]? Actually (pos[1], pos[4]) = (1,2) which is empty interior. So operation (1,2) covers nothing. So T is not covered. But can we cover T with two operations? We need to cover index 1 and index 2. Can we cover index 1 with one operation? We need l < 1 < r and p_l < p_1 < p_r or p_r < p_1 < p_l. p_1 = 1. So we need one endpoint with value <1 (impossible) or we need p_l >1 and p_r <1? But 1 is minimum, so no value <1. So we cannot cover index 1 at all! Because any operation requires p_i to be strictly between two values. Since p_1=1 is the global minimum, there is no value less than 1. So p_1 cannot be strictly between any two values. Thus s_1 can never be set to 1. So if x_1='1' and p_1=1, it's impossible. Similarly, if p_i=n, it can never be set because no value >n. So any '1' at the position of the global minimum or global maximum is impossible to satisfy.
    // This is a crucial observation: The global minimum and global maximum can never be set to 1 because they cannot be strictly between any two values. So if x[pos[1]]='1' or x[pos[n]]='1', it's impossible.
    // In the sample 4: n=6, p=[6,2,3,4,5,1], x=110110. pos[1]=5, pos[6]=0. x[0]='1' (pos[6]), x[5]='0' (pos[1]). So x[pos[6]]='1' -> impossible. Output -1. Matches!
    // Sample 2: p=[3,4,2,1,5], pos[1]=3, pos[5]=4. x=11111. x[3]='1' (pos[1]), so impossible.
    // Sample 1: p=[1,2,3], pos[1]=0, pos[3]=2. x=010. x[0]='0', x[2]='0'. So possible.
    // Sample 3: p=[1,3,2,4,6,5], pos[1]=0, pos[6]=4. x=001100. x[0]='0', x[4]='0'. Possible.
    // Sample 5: p=[2,1,4,3,5], pos[1]=1, pos[5]=4. x=00000. All zero, possible (0 operations).
    // Sample 6: p=[2,5,3,1,4], pos[1]=3, pos[5]=1. x=00100. x[3]='0', x[1]='0'? x[1] is index 1: x=00100 -> indices: 0:0, 1:0, 2:1, 3:0, 4:0. So x[pos[1]]=x[3]='0', x[pos[5]]=x[1]='0'. Possible. Output: 1 operation (2,4) (1-indexed). Let's check: l=2, r=4 (1-indexed) -> 0-indexed: l=1, r=3. p[1]=5, p[3]=1. min=1, max=5. Interior: index 2. p[2]=3. 1<3<5, so s_2=1. x[2]='1'. Works.

    // So necessary condition: For all i where x_i='1', p_i cannot be 1 or n. So if x[pos[1]]=='1' or x[pos[n]]=='1', output -1.

    // Now, what about other values? Can we always cover the remaining '1's with a few operations?
    // Let's consider the set T of indices to cover. We know 1 and n are not in T. So the values in T are between 2 and n-1.
    // We can try to cover T with operations that use the positions of 1 and n as endpoints? But we saw that if T is between pos[1] and pos[n], then (pos[1], pos[n]) covers all interior. However, if T has elements outside, we need more.
    // But note: If we use an operation with l = pos[1] and r = some index with value > all values in a part of T, we can cover that part.
    // Actually, we can always cover T with at most 2 operations? Let's test the counterexample: n=4, p=[2,1,4,3], T={1,2} (indices 1 and 2). pos[1]=1, pos[4]=2. T is not between them. Can we cover T with 2 ops? We need to cover index 1 (value 1) and index 2 (value 4). But value 1 is global min, so impossible. So if T contains 1 or n, impossible. What if T does not contain 1 or n? Let's modify: p=[3,1,4,2], n=4. pos[1]=1, pos[4]=2. x=0110 -> T={1,2}. p[1]=1 (min) -> impossible. So any T containing the position of 1 or n is impossible. So assume T avoids pos[1] and pos[n]. Then can we always cover T with 2 operations?
    // Consider p=[2,4,1,3], n=4. pos[1]=2, pos[4]=1. x=0100? T={1}? Actually x=0100 means T={1} (0-indexed). p[1]=4 (max) -> impossible. So avoid max.
    // Let's try p=[3,2,4,1], n=4. pos[1]=3, pos[4]=2. T={0,1}? x=1100 -> T={0,1}. p[0]=3, p[1]=2. Neither 1 nor 4. Can we cover? We need s[0]=1, s[1]=1. Operation (l,r) with l<0<r impossible because l>=0. So index 0 cannot be covered because it's the first element? Wait, l can be 0? But operation sets l < i < r. So if i=0, we need l < 0, impossible. So index 0 can never be covered! Similarly, index n-1 can never be covered. So endpoints of the array can never be set to 1. Thus if x[0]=='1' or x[n-1]=='1', it's impossible. We already had this condition. So T must be a subset of {1,2,...,n-2}.
    // So necessary conditions:
    // 1. x[0] == '0' and x[n-1] == '0'.
    // 2. x[pos[1]] == '0' and x[pos[n]] == '0'.
    // Are these sufficient? Let's test.
    // Consider n=5, p=[2,5,3,1,4]. pos[1]=3, pos[5]=1. x=00100 -> T={2}. x[0]=0, x[4]=0. x[pos[1]]=x[3]=0, x[pos[5]]=x[1]=0. T={2}. Can we cover index 2? We need l<2<r and p_l < p_2 < p_r or p_r < p_2 < p_l. p_2=3. We can choose l=1, r=3: p[1]=5, p[3]=1. min=1, max=5. 1<3<5, so covers index 2. Works. So one operation.
    // What about a case where T has multiple elements not between pos[1] and pos[n]? Let's try to construct a case where T requires 2 operations.
    // We want T to have elements on both sides of the interval between pos[1] and pos[n]. Suppose pos[1] < pos[n]. Then the interval (pos[1], pos[n]) is covered by one operation. The left side is [1, pos[1]-1] and right side [pos[n]+1, n-2]. To cover left side, we need an operation that covers some of those indices. Can we cover the entire left side with one operation? We need l < min(left T) and r > max(left T) with appropriate values. Since left side is to the left of pos[1], we can try to use l = 0? But l=0 is endpoint, we can use it. p[0] is some value. We need r > max(left T) such that p_r is on the other side of the values in left T. Since all values in left T are >1 (because 1 is at pos[1] which is to the right), we need one endpoint with value < min value in left T and one with value > max value in left T. We can use r = pos[1] (value 1) which is < all values in left T. Then we need l with value > max value in left T. We can choose l = 0 if p[0] > max. If not, we might need to choose a different l. But we can also use r = pos[n] (value n) which is > all values. Then we need l with value < min value in left T. We can use l = 0 if p[0] < min. So if p[0] is either less than min or greater than max, we can cover the left side with one operation using l=0 and r=pos[1] or pos[n]. What if p[0] is between min and max? Then we cannot use l=0 as the bracketing endpoint. But we could use some other l < min(left T)? There is no index before 0. So if p[0] is between the min and max of left T, then we cannot cover the left T with an operation that has l=0? Wait, we need l < i for all i in left T. The only possible l is 0 (since left T starts at 1). So l must be 0. Then p_l = p[0]. We need r > max(left T) such that either p_r < min(left T) and p_l > max(left T), or p_r > max(left T) and p_l < min(left T). So we need p[0] to be either less than min(left T) or greater than max(left T). If p[0] is in between, then no operation can cover the left T because the only available left endpoint is 0 and its value doesn't bracket the values. So in that case, covering left T might be impossible? But we could cover some elements of left T with one operation and the rest with another? But we only have 5 operations, so we could cover each element individually? But each operation must cover at least one element, and we can do multiple. However, if an element cannot be covered by any operation, it's impossible.
    // So the condition for an index i to be coverable at all is: there exist l < i < r such that p_l and p_r bracket p_i. This is equivalent to: i is not the leftmost or rightmost index, and p_i is not the global min or max, AND there is some element to the left with value on one side of p_i and some element to the right with value on the other side. Actually, since we can choose l and r anywhere, the only obstruction is if all elements to the left of i have values on the same side of p_i as all elements to the right? More precisely, if all elements to the left are < p_i and all elements to the right are also < p_i, then we cannot bracket p_i because we need one < and one >. But we can use l from left and r from right. So we need at least one element to the left that is > p_i and at least one to the right that is < p_i, OR one to the left < p_i and one to the right > p_i. In other words, p_i must not be a "record" from both sides? Actually, if p_i is greater than all elements to its left, then all left are < p_i. To bracket, we need a right element > p_i. So if there is some right element > p_i, we can use that as r and any left as l (since left are < p_i). So it's coverable. Similarly, if p_i is less than all left, we need a right element < p_i. So the only way p_i is uncoverable is if it is the global min or max, or if it is the leftmost or rightmost index, OR if all elements to the left and right are on the same side? But if all left are < p_i and all right are < p_i, then p_i is the global maximum? Because if all elements are < p_i, then p_i is the maximum. So that's the global max case. Similarly for min. So actually, any index i that is not an endpoint and not the position of 1 or n is coverable by some operation! Because we can always find l and r. For example, take l = i-1, r = i+1. If p[i-1] and p[i+1] bracket p_i, done. If not, they are both < p_i or both > p_i. If both < p_i, then p_i is greater than both neighbors. Since p_i is not the global max, there exists some j with p_j > p_i. If j > i, we can take r = j and l = i-1. Then p_l < p_i < p_r. If j < i, take l = j and r = i+1. So yes, any non-endpoint, non-min/max position is coverable individually.
    // So the only impossible cases are when x has '1' at endpoints or at min/max positions.

    // Now, we need to cover all '1's with at most 5 operations. Since each '1' is individually coverable, can we always cover the whole set with a few operations? We can try to cover them with one operation if they all lie in some interval that can be bracketed. If not, we can split into at most 5 groups. Since 5 is quite large, we can probably always do it in 2 or 3 operations.
    // Let's think about a strategy using the positions of 1 and n.
    // Let L = pos[1], R = pos[n]. Assume L < R (if L > R
