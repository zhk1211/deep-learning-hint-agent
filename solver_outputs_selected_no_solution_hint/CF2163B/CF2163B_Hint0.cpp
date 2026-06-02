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

    vector<int> ones;
    for (int i = 0; i < n; ++i) if (x[i] == '1') ones.push_back(i);

    if (ones.empty()) {
        cout << "0\n";
        return;
    }

    int L = ones[0], R = ones.back();
    int min_val = n + 1, max_val = 0;
    for (int i = L; i <= R; ++i) {
        min_val = min(min_val, p[i]);
        max_val = max(max_val, p[i]);
    }

    vector<pair<int, int>> ops;

    auto apply = [&](int l, int r) {
        ops.push_back({l + 1, r + 1});
        int mn = min(p[l], p[r]);
        int mx = max(p[l], p[r]);
        for (int i = l + 1; i < r; ++i) {
            if (mn < p[i] && p[i] < mx) {
                // s[i] becomes 1, but we don't track s explicitly
            }
        }
    };

    // We need to cover all ones in x. The operation sets s_i=1 for i between l and r
    // if p_i is between p_l and p_r. We can think of intervals on values.
    // We can always cover a contiguous segment of indices by choosing l and r as the
    // boundaries of the segment, provided the values inside are between p_l and p_r.
    // But we need to ensure that for every i in ones, there is some operation that covers it.

    // Strategy: Use at most 5 operations. We can cover the whole segment [L,R] if we find
    // l <= L and r >= R such that all values in [L,R] are between p_l and p_r.
    // Actually, if we take l = pos[min_val] and r = pos[max_val], then for any i in [L,R],
    // p_i is between min_val and max_val, so if l < i < r, it will be set.
    // But l and r might not be outside [L,R]. We need l <= L and r >= R.
    // If min_val and max_val are at positions outside [L,R] or at boundaries, we can do it in one op.
    // Otherwise, we might need more ops.

    // Let's find the minimal segment that contains all ones and also contains the positions
    // of the min and max values in that segment.
    // Actually, we can always do it in at most 2 operations:
    // 1. Choose l = L, r = R. This sets all i in (L,R) with p_i between p_L and p_R.
    // 2. If some ones are not covered, we can cover them with additional ops.
    // But we have up to 5 ops, so we can just try to cover each one individually?
    // However, an operation can cover multiple ones.

    // Let's think differently: The operation sets s_i=1 if i is between l and r and p_i is between p_l and p_r.
    // This is equivalent to: if we consider the Cartesian tree, an operation corresponds to taking two nodes
    // and setting all nodes on the path between them? Not exactly.

    // Another perspective: For a fixed i, s_i becomes 1 if there exist l < i < r such that p_l < p_i < p_r or p_r < p_i < p_l.
    // So i is covered if it is not a local extremum with respect to the chosen l,r.
    // We can cover a set of indices by choosing l and r as the boundaries of an interval that contains them,
    // and ensuring that the min and max of the interval are at l and r.
    // If we take l = pos[1] and r = pos[n], then for any i between them, p_i is between 1 and n, so all are covered.
    // But pos[1] and pos[n] might not cover the ones if ones are outside.

    // Let's find the minimal value min_all and maximal value max_all among all ones.
    // Actually, we need to cover all ones. Let's consider the values at ones positions.
    // If we can find l and r such that l <= all ones <= r and p_l <= all p_i (for ones) <= p_r, then one op suffices.
    // We can choose l = pos[min_val_of_ones] and r = pos[max_val_of_ones], but we need l <= min(ones) and r >= max(ones).
    // If that holds, one op.
    // Otherwise, we can use two ops: one to cover left part, one to cover right part.
    // In fact, we can always cover any set of ones with at most 2 ops by choosing l = pos[1], r = pos[n]?
    // If pos[1] and pos[n] are on opposite sides of the ones, then one op covers all.
    // If both are on the same side, we might need two.

    // Let's try to find a solution with at most 2 ops.
    // We can try all pairs of (l, r) from the set of ones boundaries? But n up to 2e5, so we need O(n).

    // Let's analyze the condition for a single operation (l, r) to cover a set S of indices:
    // For all i in S, l < i < r and min(p_l, p_r) < p_i < max(p_l, p_r).
    // This means that the interval (l, r) must contain S, and the values at l and r must be outside the range of values in S.
    // So if we take l = min(S) - 1? Not necessarily, because p_l must be < min(p_i) or > max(p_i).

    // Let's find the minimum and maximum values among all ones: v_min, v_max.
    // Let L = min(ones), R = max(ones).
    // If we can find l <= L and r >= R such that p_l < v_min and p_r > v_max (or vice versa), then one op works.
    // We can just check if there exists an index l <= L with p_l < v_min, and an index r >= R with p_r > v_max.
    // Or l with p_l > v_max and r with p_r < v_min.
    // If yes, we can use that l and r.
    // If not, maybe we can use two ops: one covering left part up to some point, and one covering right part.
    // Since we have 5 ops, we can also just cover each one individually? But an operation requires l < i < r, so we need at least two indices around i. We can cover a single i by choosing l = i-1, r = i+1, provided p_{i-1} and p_{i+1} are on opposite sides of p_i. If not, we might need to choose other l, r.

    // Let's think about the structure: The operation is essentially: pick two indices, and all indices between them that are "between" the two values get set to 1.
    // This is similar to: if we consider the permutation as a sequence, an operation can set a contiguous block of indices to 1 if the endpoints have values that bracket the block.

    // We can always achieve the goal with at most 2 operations:
    // Operation 1: l = pos[1], r = pos[n]. This sets all indices between pos[1] and pos[n] to 1.
    // Operation 2: If there are ones outside [pos[1], pos[n]], we can cover them with another operation using the global min and max? But 1 and n are the global min and max. So pos[1] and pos[n] are the positions of min and max. The ones outside are either all to the left of both or all to the right of both. Suppose ones are to the left of both. Then we can choose l = some index left of ones, r = pos[1] or pos[n]? Actually, if we choose l = pos[1] and r = pos[n], the interval between them is covered. The ones to the left are not covered. To cover them, we can choose l = 0 (if exists) and r = pos[1]? But we need p_l and p_r to bracket the ones. Since ones are to the left of pos[1], their values are >1. So we need an endpoint with value < min(ones) and another with value > max(ones). We have 1 at pos[1], which is < min(ones). We need an endpoint with value > max(ones). That could be n at pos[n], but pos[n] might be to the right. If we choose l = pos[1] and r = pos[n], the interval is (pos[1], pos[n]), which doesn't include the left ones. So we need l to be left of the left ones. We can choose l = leftmost index (0) and r = pos[n]? Then the interval (0, pos[n]) includes the left ones and also pos[1]. But we need p_0 and p_pos[n] to bracket the ones. p_pos[n] = n > max(ones). p_0 is some value. If p_0 < min(ones), then it works. If not, we might need another operation.

    // Actually, we can always do it in at most 2 operations by using the following:
    // Find the minimal value among all ones: v_min. Find its position pos[v_min].
    // Find the maximal value among all ones: v_max. Find its position pos[v_max].
    // If we take l = pos[v_min] and r = pos[v_max], then for any i between them, p_i is between v_min and v_max? Not necessarily, there could be values outside. But the ones have values between v_min and v_max. So if we choose l and r such that p_l = v_min and p_r = v_max, then all ones that lie between l and r will be covered. However, some ones might be outside the interval [l, r]. But v_min and v_max are the min and max of the ones, so all ones have values in [v_min, v_max]. If a one is outside [l, r], its value is still between v_min and v_max. So we can cover the remaining ones by another operation with l and r being the boundaries of the remaining ones, using the same v_min and v_max? But v_min and v_max are already used.

    // Let's consider the following algorithm:
    // We want to cover all ones. We can do:
    // 1. Find the leftmost one L and rightmost one R.
    // 2. Find the minimum value vL among ones, and maximum value vR.
    // 3. Let l1 = pos[vL], r1 = pos[vR].
    // 4. If l1 <= L and r1 >= R, then one operation (l1, r1) covers all ones. (Because all ones are between l1 and r1, and their values are between vL and vR).
    // 5. Otherwise, we can use two operations: one covering the left part and one covering the right part.
    //    Specifically, we can split the ones into two groups: those <= some index, and those >= some index.
    //    But we need to ensure each group can be covered by one operation.

    // Let's test this logic on sample 2: p = [3,4,2,1,5], x = 11111.
    // ones = [0,1,2,3,4], L=0, R=4.
    // vL = min(p[ones]) = 1, vR = max = 5.
    // pos[1] = 3, pos[5] = 4.
    // l1=3, r1=4. L=0, R=4. l1 <= L? 3 <= 0 false. So one op doesn't work.
    // Can we cover with two ops? Let's try to cover left ones [0,1,2] with one op.
    // For left ones, min value = min(3,4,2)=2, max=4. pos[2]=2, pos[4]=1.
    // l=2, r=1? l must be < r. So we need l < r. If we take l=1, r=2? p[1]=4, p[2]=2. min=2, max=4. The ones between 1 and 2: only index 1? Actually l=1, r=2 means i such that 1 < i < 2 -> none. So that doesn't cover index 1 or 2.
    // We need to cover indices 0,1,2. We can choose l=0, r=3? p[0]=3, p[3]=1. min=1, max=3. Indices between: 1,2. p[1]=4 (not between 1 and 3), p[2]=2 (between). So only index 2 gets covered. Not all.
    // It seems impossible, and sample output is -1. So our condition for one op might be necessary but not sufficient? Actually, if l1 <= L and r1 >= R, does it guarantee coverage? Let's check: For any one at index i, we have L <= i <= R, so l1 <= i <= r1. Also vL <= p_i <= vR. Since p_{l1} = vL and p_{r1} = vR, we have min(p_{l1}, p_{r1}) = vL and max = vR. So p_i is between them. Thus i is covered. So condition l1 <= L and r1 >= R is sufficient for one op.

    // What if that condition fails? Can we always do it in 2 ops if it's possible at all?
    // Let's think about the necessary condition for a set of ones to be coverable by one operation.
    // For an operation (l, r) to cover a set S, we need:
    // - l < min(S) and r > max(S)
    // - min(p_l, p_r) < min_{i in S} p_i and max(p_l, p_r) > max_{i in S} p_i.
    // So there must exist an index l < min(S) with p_l < min(S values) or p_l > max(S values), and similarly for r > max(S).
    // Actually, we need both l and r such that one is below the min value and the other is above the max value.
    // So the condition is: there exist indices i <= L and j >= R such that (p_i < v_min and p_j > v_max) or (p_i > v_max and p_j < v_min).
    // If such i, j exist, we can do one op with l=i, r=j.
    // If not, maybe we can split S into S1 and S2 such that each satisfies the condition.
    // Since we have up to 5 ops, we can try to cover each one individually? But an operation needs at least two indices around the one. We can always cover a single index i by choosing l = i-1, r = i+1, provided p_{i-1} and p_{i+1} are on opposite sides of p_i. If they are on the same side, we might need to choose other l, r. But we can always cover a single index if there exist l < i < r with p_l < p_i < p_r or p_r < p_i < p_l. This is true unless p_i is a local extremum with respect to all possible l, r? Actually, if p_i is the global minimum, then there is no p_l < p_i, so we need p_l > p_i and p_r < p_i? But p_r < p_i is impossible if p_i is global min. So if p_i = 1, we cannot cover it unless we have l and r such that one is >1 and the other is <1? But <1 doesn't exist. So 1 can never be covered? Wait, the operation sets s_i=1 if min(p_l, p_r) < p_i < max(p_l, p_r). If p_i = 1, then min(p_l, p_r) < 1 is impossible. So 1 can never be set to 1. Similarly, n can never be set to 1. So if x has a 1 at the position of value 1 or n, it's impossible. Let's check sample 2: p = [3,4,2,1,5], x has 1 at all positions. Value 1 is at index 3, value 5 at index 4. So x_3=1 and x_4=1. But 1 and 5 can never be set to 1. So impossible. That matches.

    // So necessary condition: For every i with x_i = '1', p_i cannot be 1 or n. Because min(p_l, p_r) < p_i < max(p_l, p_r) requires p_i strictly between.
    // Also, if x_i = '1', there must exist some l < i < r with p_l and p_r bracketing p_i. This is equivalent to: i is not a "peak" or "valley" that cannot be bracketed? Actually, any value except 1 and n can be bracketed by choosing l and r appropriately? For any value v (not 1 or n), we can choose l = pos[1] and r = pos[n]? Then if i is between pos[1] and pos[n], it will be covered. But if i is outside, we might need other l, r. However, we can always choose l = i-1 and r = i+1 if they exist and bracket v. If they don't, we can extend. In fact, for any v not 1 or n, there exist indices with values < v and > v. We can pick the nearest such indices to the left and right? Not necessarily, but we can always find some l < i < r with p_l < v < p_r or p_r < v < p_l. Because the set of values less than v is non-empty, and greater than v is non-empty. We can take l as the closest index to the left with value not equal to v? But we need one < v and one > v. They might both be on the same side. For example, p = [2, 3, 1], v=3 at index 1. Values <3: 2 at 0, 1 at 2. Values >3: none. So we can choose l=0 (2<3) and r=2 (1<3) -> min=1, max=2, but 3 is not between 1 and 2. So that fails. We need one <3 and one >3. Since >3 doesn't exist, 3 is the maximum, so it's n. So indeed, only 1 and n are impossible.

    // So if x has 1 at position of 1 or n, output -1.
    // Otherwise, is it always possible with at most 2 operations? Let's test.

    // Consider p = [2, 1, 4, 3], x = 0110 (ones at indices 1 and 2: values 1 and 4). But 1 is impossible, so x can't have 1 there. So x must have 0 at 1 and n.

    // Let's try to construct a solution with at most 2 operations for any valid x.
    // We can use the following:
    // Operation 1: l = pos[1], r = pos[n]. This covers all indices between pos[1] and pos[n] except possibly 1 and n themselves? Actually, for i between pos[1] and pos[n], p_i is between 1 and n, so min=1, max=n, so p_i is strictly between (since p_i != 1, n). So all such i get covered.
    // Operation 2: If there are ones outside [pos[1], pos[n]], they must be all on one side (since pos[1] and pos[n] are the positions of min and max). Suppose they are to the left of both pos[1] and pos[n]. Then we can cover them by choosing l = 0 (or some index left of them) and r = pos[1]? But we need p_l and p_r to bracket the values of those ones. The ones to the left have values >1 (since 1 is at pos[1] to the right). So we need an endpoint with value < their min and another with value > their max. We have 1 at pos[1] which is < their min. We need an endpoint with value > their max. That could be n at pos[n], but pos[n] is to the right. If we choose l = left_index and r = pos[n], then the interval includes pos[1] as well. But we need p_left and p_pos[n] to bracket the left ones. p_pos[n] = n > max. p_left must be < min of left ones. Is there always such an index? The left ones are to the left of pos[1]. Their values are >1. The minimum value among them is some v > 1. Is there an index to the left of them with value < v? Since 1 is to the right, the values to the left might all be > v? Not necessarily. For example, p = [3, 4, 2, 1, 5], ones at indices 0,1 (values 3,4). pos[1]=3, pos[5]=4. Left ones are at 0,1. We need l < 0 and r > 1 such that p_l < 3 and p_r > 4. l can be? There is no index left of 0. So we can't choose l < 0. So we must choose l = 0? But l must be strictly less than the ones? The operation requires l < i < r. So l can be 0, but then i must be >0. So index 0 cannot be covered if l=0. So we need l < 0, which is impossible. Thus, if the leftmost one is at index 0, we cannot cover it with any operation because we need l < 0. Similarly, if the rightmost one is at index n-1, we cannot cover it because we need r > n-1. So if x[0] = '1' or x[n-1] = '1', it's impossible? Let's check: In sample 1, x=010, ones at index 1. n=3. x[0]=0, x[2]=0. So endpoints are not 1. In sample 3, x=001100, ones at indices 2,3. n=6. Endpoints 0 and 5 are 0. In sample 6, x=00100, ones at index 2. Endpoints 0. So it seems endpoints cannot be 1. Is that a necessary condition? If x[0] = '1', then we need l < 0, impossible. So indeed, x[0] and x[n-1] must be '0'. Also, as noted, p_i cannot be 1 or n if x_i='1'.

    // Let's verify: In sample 4, p = [6,2,3,4,5,1], x=110110. n=6. x[0]=1, x[1]=1, x[3]=1, x[4]=1. x[0] is 1. Output is -1. So that matches.

    // So necessary conditions:
    // 1. For all i with x[i]='1', p[i] != 1 and p[i] != n.
    // 2. x[0] == '0' and x[n-1] == '0'.
    // Are these sufficient? Let's test with a case: n=4, p=[2,1,4,3], x=0100. ones at index 1 (value 1). But value 1 is forbidden, so x can't be that. What about x=0010? ones at index 2 (value 4). value n=4 forbidden. So x can't have 1 at 1 or n. What about p=[3,1,4,2], x=0100? ones at index 1 (value 1) forbidden. So any valid x must have ones only at positions with values not 1 or n, and not at endpoints.

    // Let's try to construct a solution for any such x.
    // We can use the following strategy:
    // Find the leftmost one L and rightmost one R. We know 0 < L <= R < n-1.
    // We want to cover all ones in [L, R].
    // Consider the minimum value v_min among all ones, and maximum v_max.
    // Since no one has value 1 or n, we have 1 < v_min <= v_max < n.
    // Let l1 = pos[1], r1 = pos[n].
    // If L > l1 and R < r1, then the interval (l1, r1) covers all ones, because for any i in [L,R], l1 < i < r1 and 1 < p_i < n. So one operation (l1, r1) works.
    // If not, then either L <= l1 or R >= r1 (or both). Since L > 0 and R < n-1, and l1, r1 are somewhere.
    // Case 1: L <= l1. This means the leftmost one is to the left of pos[1] (or at it, but L < l1 because if L = l1, then p[L] = 1, but x[L] cannot be 1 because p[L]=1. So L < l1). So all ones are to the left of pos[1]? Not necessarily, R could be > l1. But if L <= l1, then there is at least one one to the left of pos[1].
    // Similarly, if R >= r1, there is a one to the right of pos[n].
    // Can we cover the left ones with one operation and the right ones with another?
    // Let's consider the left part: ones in [L, l1-1] (since L < l1). We need to cover these. Their values are >1. The maximum value among them is some v_max_left < n. We need an operation (l, r) that covers them. We can choose l = 0? But l must be < L. Since L > 0, we can choose l = 0. We need r > l1-1 such that p_r > v_max_left and p_l < v_min_left. p_l = p[0]. Is p[0] < v_min_left? Not necessarily. But we can choose l = some index with a small value. Since 1 is at l1 > L, the minimum value in the whole array is 1, but it's to the right. The values to the left of L might all be > v_min_left? Actually, v_min_left is the minimum among the left ones. There might be values smaller than v_min_left to the left of L. If there is an index l < L with p_l < v_min_left, we can use that. If not, then all values to the left of L are > v_min_left. But then the minimum value in the prefix up to L is v_min_left? That would mean v_min_left is the minimum of the prefix, but 1 is to the right, so the global minimum is 1. So there must be some value smaller than v_min_left somewhere. It could be to the right. So we can choose r to be an index with a small value? But we need p_r > v_max_left to bracket. So we need one small and one large. We have 1 (small) at l1, and n (large) at r1. If we set l = l1 and r = r1, the interval is (l1, r1), which doesn't cover the left ones. If we set l = 0 and r = r1, then the interval is (0, r1). This covers the left ones if 0 < L. We need p_0 and p_{r1} to bracket the left ones. p_{r1} = n > v_max_left. p_0 must be < v_min_left. If p_0 is not < v_min_left, can we choose a different l? We can choose l to be any index < L. We want p_l < v_min_left. Is there always such an index? Since 1 is the global minimum and is at l1 > L, the minimum of the whole array is 1. The minimum of the prefix [0, L-1] might be > v_min_left. But we can also choose l to be an index with value > v_max_left and r with value < v_min_left? That would also bracket. So we need either (p_l < v_min_left and p_r > v_max_left) or (p_l > v_max_left and p_r < v_min_left). We have n at r1 (which is > v_max_left) and 1 at l1 (which is < v_min_left). So if we can include the left ones between some l and r such that one endpoint is 1 and the other is n, we just need the left ones to be between l and r. If we choose l = l1 and r = something to the left? But l must be < r. If we choose l = something left of L, and r = l1, then the interval is (l, l1). This covers the left ones if l < L and l1 > R_left (which is l1-1). So we need p_l and p_{l1} to bracket the left ones. p_{l1}=1 < v_min_left. So we need p_l > v_max_left. So we need an index l < L with p_l > v_max_left. Is there always such an index? Not necessarily. For example, p = [3, 4, 2, 1, 5], left ones at 0,1 (values 3,4). v_min_left=3, v_max_left=4. l1=3 (value 1). We need l < 0? Impossible. So we can't choose l < L if L=0. But we already excluded L=0. So L > 0. In this example, L=0 is not allowed. Let's modify: p = [5, 3, 4, 2, 1, 6], n=6. ones at indices 1,2 (values 3,4). L=1, R=2. v_min=3, v_max=4. pos[1]=4, pos[6]=5. L=1 < pos[1]=4. We need to cover indices 1,2. Can we do one op? We need l < 1 and r > 2. l can be 0 (value 5). r can be 4 (value 1) or 5 (value 6). If we choose l=0, r=4: p_0=5, p_4=1. min=1, max=5. Indices between: 1,2,3. p_1=3 (between 1 and 5), p_2=4 (between), p_3=2 (between). So all get covered. This works because p_0=5 > v_max=4 and p_4=1 < v_min=3. So we had an l with value > v_max. What if we didn't? Suppose p = [2, 3, 4, 1, 5]. ones at 1,2 (3,4). L=1, pos[1]=3. l can be 0 (value 2). p_0=2 < v_min=3. p_3=1 < v_min. Both are < v_min. We need one > v_max. We have pos[5]=4 (value 5 > 4). So we can choose l=0, r=4: p_0=2, p_4=5. min=2, max=5. Indices 1,2,3: p_1=3 (between), p_2=4 (between), p_3=1 (not between). So indices 1,2 covered. Works.

    // What if both l and r are on the same side of the values? We need one < v_min and one > v_max. Since 1 and n exist, we can always use them as the bracketing values, provided the ones lie between their positions. If the ones are entirely to the left of both pos[1] and pos[n], then we can choose l = some index left of them (or 0) and r = pos[n] (or pos[1] if that gives the right bracket). But we need l < L. Since L > 0, we can choose l = 0. Then we need p_0 and p_r to bracket. p_r can be pos[n] (value n > v_max). We need p_0 < v_min or p_0 > v_max. If p_0 is between v_min and v_max, then it doesn't bracket. But we can choose a different l? We can choose any l < L. Is there always an l < L with p_l outside [v_min, v_max]? Not necessarily. Consider p = [3, 4, 5, 2, 1, 6], ones at indices 1,2 (values 4,5). L=1. v_min=4, v_max=5. Prefix before L: index 0 value 3. 3 < 4, so it's outside (below). So we can use l=0, r=pos[6]=5. p_0=3<4, p_5=6>5. Works.
    // What if the prefix values are all inside [v_min, v_max]? For example, p = [4, 5, 3, 1, 2, 6], ones at 1,2 (5,3? wait, values 5 and 3, so v_min=3, v_max=5). Prefix index 0 value 4. 4 is inside [3,5]. So p_0 is not outside. Can we choose another l? There is no other l < L. So we cannot use l=0. What about r? We can try to use l = pos[1] and r = something? But l must be < L. pos[1]=3 > L, so can't be l. We need l < L. So we must use l=0. But p_0=4 is inside. Then we need p_r to be outside. If we choose r = pos[6]=5 (value 6 >5), then p_0=4, p_5=6. min=4, max=6. The ones values are 5 and 3. 5 is between 4 and 6, but 3 is not (3 < 4). So index 2 (value 3) is not covered. If we choose r = pos[1]=3 (value 1), then p_0=4, p_3=1. min=1, max=4. 5 is not between, 3 is between. So index 1 not covered. So one operation cannot cover both. Can we cover them with two operations? Operation 1: cover index 1 (value 5). We need l < 1 < r with p_l and p_r bracketing 5. We can choose l=0 (4), r=5 (6) -> min=4, max=6, 5 is between. So index 1 covered. Operation 2: cover index 2 (value 3). We need l < 2 < r. We can choose l=0 (4), r=3 (1) -> min=1, max=4, 3 is between. So index 2 covered. So two operations work. So even if one operation can't cover all, we can cover each individually or in groups.

    // Since we have up to 5 operations, we can just cover each one individually? But we need to output at most 5 operations. The number of ones could be up to n. So we can't cover each individually if there are many ones. But we can cover contiguous segments of ones with one operation if we can find appropriate l and r.
