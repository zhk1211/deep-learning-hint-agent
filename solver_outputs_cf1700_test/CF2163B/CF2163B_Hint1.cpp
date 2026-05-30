// Hint1
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> p(n);
    for (int i = 0; i < n; ++i) cin >> p[i];
    string x;
    cin >> x;

    vector<int> pos1;
    for (int i = 0; i < n; ++i) {
        if (x[i] == '1') pos1.push_back(i);
    }

    if (pos1.empty()) {
        cout << "0\n";
        return;
    }

    // Check impossible condition from hint: if x[0], x[n-1], x[pos of 1], x[pos of n] are all 1
    int pos1_val = -1, posn_val = -1;
    for (int i = 0; i < n; ++i) {
        if (p[i] == 1) pos1_val = i;
        if (p[i] == n) posn_val = i;
    }
    if (x[0] == '1' && x[n-1] == '1' && x[pos1_val] == '1' && x[posn_val] == '1') {
        cout << "-1\n";
        return;
    }

    // We will try to cover all 1s in x using at most 5 operations.
    // Strategy: use intervals that cover ranges of 1s.
    // We can always cover any set of 1s with at most 2 intervals if the impossible condition is not met.
    // But we need to output actual operations.

    // Find the leftmost and rightmost 1 in x.
    int L = pos1[0], R = pos1.back();

    // If L == 0 and R == n-1, we might need special handling.
    // But we already checked impossible condition.

    // We'll try to cover all 1s with one or two intervals.
    // One interval: [L, R] might work if it covers all 1s and doesn't violate anything.
    // But we need to ensure that the operation actually sets the required bits to 1.
    // The operation sets s_i=1 for i strictly between l and r if min(p_l,p_r) < p_i < max(p_l,p_r).
    // So we need to choose l and r such that for every required position i, the condition holds.

    // Observation: If we choose l and r as the positions of 1 and n (or n and 1), then the condition
    // min(p_l,p_r) < p_i < max(p_l,p_r) is true for ALL i strictly between them.
    // Because p_i is between 1 and n, and min is 1, max is n.
    // So if we set l = pos of 1, r = pos of n (or vice versa), then all indices strictly between them become 1.
    // This is a powerful operation.

    // We can use at most 5 operations, so we can cover the required 1s by combining such "full" intervals.

    // Let's find positions of 1 and n.
    int idx1 = pos1_val, idxn = posn_val;

    // If we do operation with l = idx1, r = idxn (order doesn't matter), it sets all indices between them to 1.
    // Similarly, we can do multiple such operations to cover disjoint segments.

    // We need to cover all indices in pos1.
    // If all pos1 are between idx1 and idxn (inclusive? exclusive? operation sets strictly between, so endpoints not set).
    // But endpoints might be 1 in x. If x[idx1] == '1', we need to set it. Operation doesn't set endpoints.
    // So we might need additional operations to set endpoints.

    // Actually, we can set an endpoint by choosing an interval that includes it as an interior point.
    // For example, to set idx1, we can choose l = something left of idx1, r = something right of idx1, such that condition holds.
    // Since p[idx1] = 1, min(p_l,p_r) < 1 < max(p_l,p_r) is impossible because 1 is the minimum.
    // So we can never set the position of 1 to 1 using any operation!
    // Similarly, we can never set the position of n to 1, because n is the maximum.
    // Therefore, if x[idx1] == '1' or x[idxn] == '1', it's impossible unless we can set them? Wait, check:
    // Condition: min(p_l, p_r) < p_i < max(p_l, p_r). For p_i = 1, we need min < 1 < max, but min >= 1, so impossible.
    // For p_i = n, we need min < n < max, but max <= n, so impossible.
    // Thus, positions of 1 and n can NEVER be set to 1.
    // So if x requires them to be 1, it's impossible.
    // That's part of the hint: if x_1, x_n, x_pos1, x_posn are all 1, it's impossible. But actually, if ANY of x_pos1 or x_posn is 1, it's impossible individually.
    // Wait, the hint says if all four are 1, it's impossible. But maybe if only some are 1, it's still impossible? Let's check:
    // If x_pos1 == '1', we can never set it, so impossible. Similarly for x_posn.
    // But the hint says "if x_1 = x_n = x_pos1 = x_posn = 1, it is impossible". It doesn't say it's the only impossible case.
    // However, from the problem, we might be able to set x_1 or x_n even if they are endpoints? Actually, x_1 is index 0, x_n is index n-1.
    // They are not necessarily positions of 1 or n. They are just the first and last elements of the array.
    // The hint specifically mentions x_1 and x_n (the first and last characters of x) and x_pos1, x_posn.
    // So the impossible condition is when all four are 1. That suggests that if only some are 1, it might still be possible.
    // But we just argued that x_pos1 = '1' is impossible because we can never set the position of value 1 to 1.
    // Let's re-read the operation: "for every i such that l < i < r and min(p_l, p_r) < p_i < max(p_l, p_r) hold at the same time, you will set s_i to 1."
    // p_i is the value at position i. If p_i = 1, then min(p_l, p_r) < 1 is false because min is at least 1. So indeed, position of 1 can never be set.
    // So if x has '1' at the position where p_i = 1, it's impossible. Similarly for n.
    // But the hint says only if all four are 1. Why? Maybe because if x_1 or x_n are 1, they might be the positions of 1 or n? Not necessarily.
    // Let's test with sample: In sample 2, n=5, p = [3,4,2,1,5], x = 11111. pos of 1 is index 3 (0-based), pos of n=5 is index 4. x[3]=1, x[4]=1. Also x[0]=1, x[4]=1 (x_n is index 4). So all four are 1, output -1. That matches.
    // What if x_pos1 = 1 but x_1 = 0? Is it possible? According to our reasoning, it's impossible because we can't set pos1. But the hint doesn't say that case is impossible. Let's check if there's any way to set pos1? No, because min < 1 is impossible. So it should be impossible. But maybe the problem allows s_i to be already 1? Initially s is all 0. So we must set it. So indeed impossible.
    // Wait, maybe I misread: "if x_i = 1, then s_i = 1". We start with s all 0. We must make s_i = 1 for those i. So we must be able to set them. So if x_pos1 = 1, impossible. So why does the hint only mention the conjunction? Possibly because the hint is a necessary condition for impossibility, but not sufficient? Or maybe the hint is saying that if all four are 1, it's impossible, but there might be other impossible cases. The hint is just a hint, not a full characterization.
    // Let's check sample 4: n=6, p = [6,2,3,4,5,1], x = 110110. pos of 1 is index 5, pos of 6 is index 0. x[5]=0, x[0]=1. x[0] is 1, x[5] is 0. x_n is index 5, which is 0. So not all four are 1. Output is -1. So indeed, there are other impossible cases.
    // So we need a general algorithm.

    // Let's think differently. We can perform up to 5 operations. Each operation sets a contiguous range of indices (strictly between l and r) to 1, provided their values lie between the endpoints' values.
    // This is equivalent to: we can set to 1 any index i that is between some l and r, and p_i is between p_l and p_r.
    // Since we can choose l and r arbitrarily, we can cover any set of indices that can be expressed as a union of up to 5 such intervals.

    // Notice that if we choose l and r such that p_l = 1 and p_r = n (or vice versa), then the condition min < p_i < max is true for ALL i strictly between them, because all values are between 1 and n. So that operation sets all indices between pos1 and posn to 1.
    // Similarly, if we choose l and r such that p_l = a and p_r = b with a < b, then it sets all indices between them whose values are in (a, b).
    // This is like we can activate a segment of the array based on value range.

    // Another perspective: Consider the Cartesian tree or the permutation's intervals. The operation essentially allows us to set all elements in a subarray that are "between" the endpoints in value.
    // This is similar to the problem "Set All Between Min and Max".

    // Since we have only 5 operations, we can try to cover the required 1s with a few well-chosen intervals.
    // Observe that any index i can be set to 1 by choosing l = i-1, r = i+1 if p_{i-1} and p_{i+1} are such that p_i is between them. But we can also use the global min/max.

    // Let's consider the positions of 1 and n. They are "special" because they can never be set. So if x requires them to be 1, impossible.
    // So first check: if x[pos1] == '1' or x[posn] == '1', output -1.
    // But wait, sample 4: p = [6,2,3,4,5,1], pos1=5, posn=0. x = 110110. x[5]=0, x[0]=1. So x[pos1]=0, x[posn]=1. According to my check, x[posn]==1 would mean impossible, and indeed output is -1. So that matches!
    // Sample 6: n=5, p=[2,5,3,1,4], x=00100. pos1=3, posn=1. x[3]=0, x[1]=0. So possible. Output is 1 operation: 2 4. Let's check: l=2, r=4 (1-based). Indices: 2..4, strictly between: index 3 (value 1). min(p2,p4)=min(5,1)=1, max=5. Condition: 1 < p_3=1 < 5? No, 1 < 1 is false. So that operation would NOT set index 3. But x has 1 at index 2? x=00100, so index 2 (0-based) is 1? Wait, x is 0-indexed in my mind. x=00100 means indices: 0:'0', 1:'0', 2:'1', 3:'0', 4:'0'. So the 1 is at index 2 (value p[2]=3). Operation l=2, r=4 (1-based) means indices 1 and 3 in 0-based? l=2 -> index 1, r=4 -> index 3. Strictly between: index 2. min(p[1],p[3]) = min(5,1)=1, max=5. p[2]=3. 1<3<5 true. So index 2 is set. That works. So my pos1/posn check: pos1=3 (value 1), x[3]=0; posn=1 (value 5), x[1]=0. So allowed.

    // So rule: if x[pos1] == '1' or x[posn] == '1', impossible.
    // But is that sufficient? Sample 2: pos1=3, posn=4, both 1, impossible. Sample 4: posn=0 is 1, impossible. Sample 1: pos1=0? p=[1,2,3], pos1=0, x[0]=0; posn=2, x[2]=0. Possible.
    // Sample 3: p=[1,3,2,4,6,5], pos1=0, x[0]=0; posn=4, x[4]=0. Possible.
    // Sample 5: p=[2,1,4,3,5], pos1=1, x[1]=0; posn=4, x[4]=0. Possible, output 0.
    // So this condition seems necessary. But is it sufficient? Not necessarily, there might be other impossible cases. But we can try to construct a solution whenever this condition holds.

    // Let's try to construct a solution using at most 5 operations.
    // We can always cover all required 1s using at most 2 operations if the required 1s form a contiguous segment? Not exactly.

    // Consider the set of indices that CAN be set to 1. An index i can be set if there exist l < i < r such that p_i is between p_l and p_r.
    // This is equivalent to: i is not a local minimum or maximum in some sense? Actually, any index i that is not the global min or max can be set by choosing l and r as the positions of some smaller and larger values. But we need l and r to be on opposite sides.
    // More precisely, i can be set iff there exist l < i < r with p_l < p_i < p_r or p_r < p_i < p_l.
    // This is true for any i that has an element smaller than p_i to its left and an element larger than p_i to its right, OR an element larger to its left and smaller to its right.
    // In a permutation, for any i except possibly some, this is true. The only indices that might not be settable are those that are "records" from both sides? Let's analyze:
    // If i is such that all elements to its left are greater than p_i, and all elements to its right are also greater than p_i, then p_i is a global minimum? Actually, if all left > p_i and all right > p_i, then p_i is the unique minimum, so p_i=1. So pos1 cannot be set, as we know.
    // Similarly, if all left < p_i and all right < p_i, then p_i=n, so posn cannot be set.
    // What if all left < p_i and all right > p_i? Then p_i is a "left-to-right maximum"? Actually, if all left < p_i, then p_i is greater than all to its left. If all right > p_i, then p_i is smaller than all to its right. Can we set it? We need l < i with p_l < p_i (exists, any left element) and r > i with p_r > p_i (exists, any right element). So we can choose l = some left index, r = some right index. Then min(p_l,p_r) < p_i < max(p_l,p_r) holds because p_l < p_i < p_r. So yes, it can be set.
    // Similarly, if all left > p_i and all right < p_i, we can choose l with p_l > p_i and r with p_r < p_i, then min < p_i < max holds.
    // So the ONLY indices that cannot be set are the global minimum and global maximum positions. Because for any other index, there is at least one element smaller and one element larger somewhere. But we need them on opposite sides? What if all smaller elements are on one side and all larger on the same side? Then we can't have l and r on opposite sides with one smaller and one larger. For example, p = [2, 3, 1]. i=1 (value 3). Left: 2 < 3. Right: 1 < 3. All elements are smaller than 3. There is no element larger than 3. So p_i=3 is the maximum? Actually n=3, so 3 is the maximum. So posn cannot be set. That's covered.
    // What about p = [3, 1, 2]. i=1 (value 1) is minimum. i=0 (value 3): left none, right: 1<3, 2<3. No larger element, so it's maximum. i=2 (value 2): left: 3>2, 1<2. So we have left element larger (3) and left element smaller (1). But we need l < i and r > i. For i=2, r > i doesn't exist. So we can't set it? Wait, i=2 is the last index. Can we set the last index? We need l < i < r, but r <= n-1. So if i = n-1, there is no r > i. So the last index can never be set! Similarly, the first index can never be set because there is no l < i.
    // Ah! The operation requires l < i < r, so i must be strictly between l and r. Therefore, indices 0 and n-1 can NEVER be set, because there is no l < 0 or r > n-1.
    // So x[0] and x[n-1] cannot be 1. That's why the hint includes x_1 and x_n.
    // So we have four special indices: 0, n-1, pos1, posn. None of them can ever be set to 1.
    // Therefore, if x has 1 at any of these four positions, it's impossible.
    // But wait, is it possible that pos1 = 0? Then x[0] and x[pos1] are the same. The hint says if all four are 1, it's impossible. But if pos1=0 and x[0]=1, then x[pos1]=1, so it's impossible. The hint's condition "x_1 = x_n = x_pos1 = x_posn = 1" is just a specific case where all four are 1. But actually, if ANY of them is 1, it's impossible. Why does the hint only mention the conjunction? Maybe because in some cases, you can set them indirectly? No, the operation strictly requires l < i < r, so endpoints of the whole array can never be set. And we already proved pos1 and posn can never be set because of value bounds. So indeed, any of these four being 1 makes it impossible.
    // Let's check sample 4: x = 110110. n=6. x[0]=1 -> impossible. Output -1. Matches.
    // Sample 2: x = 11111. x[0]=1, x[4]=1, pos1=3 -> x[3]=1, posn=4 -> x[4]=1. All four are 1, impossible.
    // Sample 1: x=010. n=3. x[0]=0, x[2]=0, pos1=0 (x[0]=0), posn=2 (x[2]=0). All four are 0. Possible.
    // Sample 3: x=001100. n=6. x[0]=0, x[5]=0, pos1=0 (0), posn=4 (0). Possible.
    // Sample 5: x=00000. All 0. Possible.
    // Sample 6: x=00100. n=5. x[0]=0, x[4]=0, pos1=3 (0), posn=1 (0). Possible.
    // So the necessary and sufficient condition for possibility is: x[0] == '0', x[n-1] == '0', x[pos1] == '0', x[posn] == '0'.
    // Is that sufficient? Can we always construct a solution with at most 5 operations if these four are 0?
    // Let's test: Suppose n=4, p=[2,4,1,3], x=0100. pos1=2, posn=1. x[0]=0, x[3]=0, x[2]=0, x[1]=0. So condition holds. We need to set index 1 (value 4) to 1. But index 1 is posn! Wait, posn=1, x[1]=0 in my example? I set x=0100, so x[1]=1. That violates x[posn]==0. So not allowed.
    // What about x=0010? Then index 2 (value 1) is 1, violates x[pos1]==0.
    // What about x=0000? Trivial.
    // What about x=0100 but with different permutation where posn is not 1? Let's try n=4, p=[3,1,4,2]. pos1=1, posn=2. x=0100 means x[1]=1 -> violates x[pos1]==0.
    // So any 1 in x must be at an index that is not 0, n-1, pos1, posn.
    // Can we always set such an index? Yes, we can set any index i that is not one of those four by using a single operation that covers it.
    // But we might need to set multiple indices. Can we set multiple indices with one operation? Yes, if they all lie between some l and r and their values are between p_l and p_r.
    // Since we have up to 5 operations, we can just set each required index individually? But we can only do 5 operations, and there could be up to n required indices. So we need to cover them efficiently.

    // Observe that if we choose l = pos1 and r = posn (assuming pos1 < posn), then the operation sets ALL indices between pos1 and posn to 1, because all values are between 1 and n. This covers a huge range.
    // Similarly, if posn < pos1, we can swap them.
    // So one operation with l = min(pos1, posn), r = max(pos1, posn) will set all indices strictly between them to 1.
    // This covers all indices except possibly those outside the interval [pos1, posn] and the endpoints themselves.
    // But endpoints pos1 and posn are already 0 in x (by condition), so we don't need to set them.
    // What about indices outside? They are either to the left of min(pos1,posn) or to the right of max(pos1,posn).
    // Can we set those? We might need additional operations.
    // But note: if there is a required 1 outside, say to the left of pos1, then we can use another operation with l = that index's left neighbor? But we need to cover it.
    // Actually, we can use the same trick with other "min" and "max" values? Not exactly, because 1 and n are the absolute extremes. For other values, the range of values covered is smaller.

    // Let's think about the structure of the permutation. The positions of 1 and n divide the array into up to three segments: left of min(pos1,posn), between them, and right of max(pos1,posn).
    // The middle segment can be completely set by one operation.
    // For the left segment, can we set indices there? Suppose we want to set an index i < min(pos1,posn). We need l < i < r with appropriate values. Since i is outside, we can choose r = pos1 or posn? But r must be > i. If we choose r = pos1, then p_r = 1. Then min(p_l, p_r) = min(p_l, 1) = 1 (if p_l > 1) or p_l (if p_l < 1, impossible). So min is at most 1. max is max(p_l, 1). For p_i to be between, we need 1 < p_i < max(p_l,1). But p_i > 1, so min is 1, condition becomes 1 < p_i < max(p_l,1). This can be true if p_l > p_i. So we need an l < i with p_l > p_i. Then we can set i by choosing l and r=pos1.
    // Similarly, for right segment, we can choose l = posn and r > i with p_r < p_i? Wait, if we choose l = posn (value n), then min(p_l, p_r) = min(n, p_r) = p_r, max = n. Condition: p_r < p_i < n. So we need r > i with p_r < p_i.
    // So to set an index i, we need a "witness" on the other side.

    // Actually, we can always set any index i (not special) using at most two operations? But we have only 5 total.

    // Let's try to construct a universal strategy that uses at most 2 operations.
    // Claim: If the four special positions are 0 in x, we can always satisfy the condition with at most 2 operations.
    // Proof idea: Use one operation to cover everything between pos1 and posn. Then use another operation to cover the remaining required 1s on the left and right? But one operation can only cover a contiguous range of indices (strictly between l and r) with values in some range. It might not cover both left and right simultaneously unless they are connected.

    // Alternative: We can choose l and r such that the interval covers the whole array? But we can't set endpoints. If we choose l=0, r=n-1, then we set all indices between 0 and n-1 (i.e., 1..n-2) to 1 if their values are between p_0 and p_{n-1}. But p_0 and p_{n-1} might not be 1 and n, so the value range might not cover all required indices.
    // However, we can choose l and r to be the positions of the minimum and maximum of the whole array? That is pos1 and posn. That operation covers the middle segment.

    // What if all required 1s are in the middle segment? Then one operation suffices.
    // What if there are required 1s in the left segment? Then we need to cover them. Can we cover the left segment with one operation? We can choose l = 0? But l must be < i, so if i is in left segment, we can choose l = 0 (if 0 is not special? But 0 is special and x[0] must be 0, so we don't need to set it. We can use l=0 as a boundary. But l=0 is index 0, which is an endpoint of the array. The operation allows l=0. Then we need r > i. We can choose r = pos1? Then the interval is (0, pos1). The values: p_0 and p_1? p_0 is some value, p_pos1 = 1. min = 1, max = p_0 (if p_0 > 1). Then condition: 1 < p_i < p_0. So this operation sets all indices between 0 and pos1 whose values are less than p_0. But we need to set specific indices. We can choose l=0, r=pos1, but that only sets those with p_i < p_0. What if some required index has p_i > p_0? Then it won't be set.
    // We could instead choose l and r such that the value range covers everything. The only way to cover all values is to use 1 and n. So if we want to cover the left segment entirely, we need an operation with min=1 and max=n. But that requires the endpoints to be pos1 and posn. If we set l = something left of the left segment? But pos1 is the only 1. So to get min=1, we must involve pos1. So any operation that covers indices left of pos1 must have r = pos1 (to include 1) and some l < those indices. Then min=1, max=p_l. This sets all indices between l and pos1 with p_i < p_l. To set all required indices in the left segment, we need p_l to be greater than all their values. So we can choose l such that p_l is the maximum value in the left segment? But l must be to the left of all required indices. So we can choose l = 0 if p_0 is large enough, or we can choose l = some index further left? But there is nothing left of 0. So if the leftmost required index is at index i, we can choose l = i-1? But i-1 might not have a large enough value.
    // This suggests that we might need more operations.

    // Let's look at the problem constraints: at most 5 operations. So we can afford a few.
    // Maybe we can set each required index individually if there are at most 5? But n can be large, and sum n is 2e5, so there could be many 1s. But we only have 5 operations, so we must cover many 1s per operation.

    // Let's re-read the problem statement carefully: "You may do the following operation at most 5 times." So we can do up to 5 operations. The examples show using 1 or 2 operations. The hint says if all four are 1, impossible. So likely, if the four special positions are 0, it's always possible with at most 2 operations. Let's try to prove or find a counterexample.

    // Consider n=5, p=[3,5,2,4,1], x=01010. pos1=4, posn=1. Special positions: 0,4,4,1 -> x[0]=0, x[4]=0, x[4]=0, x[1]=1? Wait x=01010: indices: 0:0, 1:1, 2:0, 3:1, 4:0. So x[1]=1, but posn=1, so x[posn]=1 -> impossible by our condition. So not allowed.
    // Try x=00100. pos1=4, posn=1. x[0]=0, x[4]=0, x[4]=0, x[1]=0. Required 1 at index 2 (value 2). Can we set it? Index 2 is between posn=1 and pos1=4? posn=1, pos1=4, so indices between are 2,3. So one operation with l=1, r=4 sets indices 2 and 3 to 1. That sets index 2. So 1 operation works.
    // What if required 1 is at index 0? Not allowed.
    // What if required 1 is at index 3? Also between.
    // What if required 1 is at index 1? Not allowed (posn).
    // So all allowed 1s are between pos1 and posn? Is that always true?
    // Suppose pos1 and posn are such that there are indices outside the interval between them. For example, p=[2,1,4,3], n=4. pos1=1, posn=2. Interval between is empty (indices strictly between 1 and 2: none). So the middle segment is empty. What if x has a 1 at index 0? Not allowed (x[0] must be 0). Index 3? Not allowed (x[3] must be 0). So no 1s allowed outside? Actually, index 0 and 3 are special (endpoints). So they can't be 1. So the only possible 1s are at indices 1 and 2, but those are pos1 and posn, which also can't be 1. So in this permutation, no 1s can be set at all! So if x has any 1, it's impossible. But our condition says x[pos1]=0, x[posn]=0, x[0]=0, x[3]=0. So x could be all 0, which is possible (0 operations). If x has a 1 at index 1, that's pos1, so condition fails. So indeed, if condition holds, x has no 1s in this case. So it's possible (0 operations).
    // What if pos1 and posn are at the ends? p=[1,3,2,4], n=4. pos1=0, posn=3. Special: 0,3,0,3. x[0]=0, x[3]=0. Allowed 1s: indices 1,2. They are between pos1 and posn. So one operation covers them.
    // What if pos1 and posn are not at ends, but there is a required 1 outside the interval? For example, p=[3,1,4,2,5], n=5. pos1=1, posn=4. Interval between: 2,3. Outside left: index 0. Outside right: none (index 4 is posn). Can we have a required 1 at index 0? x[0] must be 0 by condition. So no.
    // What if n=6, p=[4,1,5,2,6,3]. pos1=1, posn=4. Interval between: 2,3. Left: index 0. Right: index 5. x[0] and x[5] must be 0. So no 1s outside.
    // It seems that any index not between pos1 and posn is either an endpoint (0 or n-1) or is outside but not endpoint? Wait, if pos1=2, posn=4, then left of pos1: indices 0,1. Right of posn: index 5. Are these endpoints? 0 is endpoint, 5 is endpoint if n=6. So they are endpoints. So indeed, the only indices not strictly between pos1 and posn are the endpoints 0 and n-1, and possibly pos1 and posn themselves if they are not endpoints. But wait, if pos1 > 0, then indices 0 to pos1-1 are left of pos1. Are they all endpoints? Only 0 is an endpoint. The others (1 to pos1-1) are not endpoints. For example, n=6, pos1=3, posn=4. Left of pos1: indices 0,1,2. 0 is endpoint, 1 and 2 are not endpoints. So there can be non-endpoint indices outside the interval!
    // Let's construct: n=6, p=[2,3,1,5,6,4]. pos1=2, posn=4. Interval between: index 3. Left: 0,1. Right: 5. x[0]=0, x[5]=0 by condition. But x[1] could be 1? Is that allowed? x[1] is not an endpoint, not pos1, not posn. So condition allows x[1]=1. Can we set index 1?
    // Index 1 has value 3. We need l < 1 < r with
