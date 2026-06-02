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

    // If x has no '1', we can do 0 operations
    if (x.find('1') == string::npos) {
        cout << "0\n";
        return;
    }

    // Find the range of indices that must be covered
    int L = n, R = -1;
    for (int i = 0; i < n; ++i) {
        if (x[i] == '1') {
            L = min(L, i);
            R = max(R, i);
        }
    }

    // Check if it's possible to cover all required positions with one operation
    // We need to find l, r such that all positions i with x[i]=='1' satisfy:
    // l < i < r and min(p[l], p[r]) < p[i] < max(p[l], p[r])
    // Also l and r can be any indices, not necessarily outside L..R
    // But we can try to set l = L, r = R? Not necessarily, because p[L] and p[R] might not bracket all required values.
    // Actually we can choose any l <= L and r >= R, but the condition must hold for all required i.
    // Let's find the min and max values among required positions.
    int min_val = n + 1, max_val = -1;
    for (int i = 0; i < n; ++i) {
        if (x[i] == '1') {
            min_val = min(min_val, p[i]);
            max_val = max(max_val, p[i]);
        }
    }

    // We need to find l and r such that:
    // l < L, r > R (strictly? Actually condition says l < i < r, so l must be < L and r > R)
    // and min(p[l], p[r]) < min_val and max(p[l], p[r]) > max_val.
    // Also l and r can be any indices, not necessarily outside the range, but if l is inside, then l < i fails for i = l? No, condition is l < i < r, so l must be strictly less than all required i, so l < L. Similarly r > R.
    // So we need to find an index l < L and an index r > R such that the interval (min(p[l], p[r]), max(p[l], p[r])) contains all required values.
    // Since we can do up to 5 operations, maybe we can cover each required position individually? But 5 is small.
    // Let's think differently: The operation sets s_i = 1 for i strictly between l and r if p_i is between p[l] and p[r].
    // This is like drawing a rectangle on the permutation matrix? Actually it's an interval in both index and value.
    // We can cover a set of points (i, p[i]) with a rectangle defined by l, r, p[l], p[r].
    // The rectangle is (l, r) x (min(p[l],p[r]), max(p[l],p[r])).
    // We need to cover all points (i, p[i]) where x[i] = '1'.
    // We have up to 5 rectangles.
    // Since n up to 2e5, we need an efficient check.

    // Key observation: The operation can only set bits to 1, never to 0. So we just need to cover all 1s in x.
    // With 5 rectangles, we can cover a lot. But is there a simple necessary and sufficient condition?
    // Let's analyze the structure of the permutation.
    // Consider the Cartesian tree of the permutation? Or just the fact that any interval [l, r] corresponds to a contiguous range of values? Not exactly.
    // Actually, if we choose l and r, the set of indices i with l < i < r and min(p[l],p[r]) < p[i] < max(p[l],p[r]) is exactly the set of indices between l and r whose values lie between the two endpoints.
    // This is a "block" in the permutation.
    // With multiple operations, we can cover unions of such blocks.
    // Since we only need to cover the 1s, maybe we can always do it with 2 operations? The sample shows 2 operations for some cases.
    // Let's think about the structure of the required set.
    // If the required set is empty, 0 ops.
    // If the required set can be covered by one rectangle, 1 op.
    // Otherwise, can we always cover with 2 ops? Sample 2 says -1, so not always.
    // What makes it impossible? In sample 2: p = [3,4,2,1,5], x = 11111. We need to set all bits to 1. Can we?
    // With one operation: we need l < 1? impossible since l>=1. So we need l=1, r=5? Then l < i < r means i=2,3,4. We can't cover i=1 and i=5. So one op can't cover endpoints.
    // With two ops: we could cover 1 and 5 by choosing l=1,r=something? Actually to cover i=1, we need l < 1, impossible. So we can never set s_1 or s_n to 1 because the operation only sets indices strictly between l and r. So if x has '1' at index 1 or n, it's impossible! Wait, sample 1: x=010, index 1 is 0, index 3 is 0. Sample 3: x=001100, indices 1,2,5,6 are 0. Sample 4: x=110110, indices 1 and 6 are 1? Let's check: n=6, x=110110, so indices 1 and 2 are 1, 4 and 5 are 1. Index 1 is 1! But output is -1. So indeed, if x[0] == '1' or x[n-1] == '1', it's impossible because we can never set the first or last element. Because operation requires l < i < r, so i cannot be 1 or n. So if x[0] == '1' or x[n-1] == '1', output -1.
    // Check sample 4: x=110110, n=6, x[0]='1', x[5]='0'. So impossible. Sample 2: x=11111, n=5, x[0]='1', impossible. Sample 5: x=00000, possible (0 ops). Sample 6: x=00100, n=5, x[0]='0', x[4]='0', possible with 1 op.
    // So first condition: if x[0] == '1' or x[n-1] == '1', then impossible.

    if (x[0] == '1' || x[n-1] == '1') {
        cout << "-1\n";
        return;
    }

    // Now we only need to cover internal positions.
    // Can we always cover any set of internal positions with at most 2 operations? Let's test.
    // Consider we want to cover a set S of indices (all between 2 and n-1).
    // We can choose l=1, r=n. Then we cover all i with min(p[1],p[n]) < p[i] < max(p[1],p[n]).
    // This covers a contiguous range of values between the endpoints.
    // If the required values are not all in that range, we might need another operation.
    // But we can also choose other l, r.
    // Actually, we can cover any single index i by choosing l and r such that l < i < r and p[l] and p[r] bracket p[i]. For example, we can choose l = i-1, r = i+1. Then min(p[i-1], p[i+1]) < p[i] < max(...) might not hold. But we can choose l and r further away to bracket the value.
    // Since it's a permutation, for any i, there exist l < i < r such that p[l] < p[i] < p[r] or p[r] < p[i] < p[l]? Not necessarily. For example, if p[i] is the global maximum, we can't find l,r with p[l] > p[i] and p[r] > p[i] because p[i] is max. So we can't bracket it from above. But we can bracket it from below: we need min(p[l],p[r]) < p[i] < max(p[l],p[r]). If p[i] is max, we need max(p[l],p[r]) > p[i], impossible. So we can never cover the global maximum or global minimum if they are internal? Wait, if p[i] is the maximum, then for any l,r, max(p[l],p[r]) <= p[i], so the condition max(p[l],p[r]) > p[i] fails. So we can never set s_i = 1 for the index of the maximum value. Similarly for the minimum value. So if x[i] == '1' for the index of the maximum or minimum, it's impossible.
    // Let's check: In sample 3, p = [1,3,2,4,6,5], max is 6 at index 5 (1-based), min is 1 at index 1. x = 001100, so indices 3 and 4 are 1. Index 5 is 0, index 1 is 0. So max and min are not required. In sample 6, p = [2,5,3,1,4], max 5 at index 2, min 1 at index 4. x = 00100, index 3 is 1. So max and min are not required.
    // So another condition: if the position of value 1 or value n has x == '1', impossible.
    int pos1 = pos[1], posn = pos[n];
    if (x[pos1] == '1' || x[posn] == '1') {
        cout << "-1\n";
        return;
    }

    // Now, is it always possible with at most 2 operations if the above conditions hold?
    // Let's think: We need to cover all required indices. We can use the operation with l = pos1, r = posn? But pos1 and posn might not be in order. Let's assume pos1 < posn. Then we can choose l = pos1, r = posn. Then we cover all i between them with values between 1 and n, i.e., all values! Because min(p[l],p[r]) = 1, max = n. So this operation would set all s_i for l < i < r to 1. That covers everything between pos1 and posn. What about indices outside? They are either < pos1 or > posn. But we already know x[0] and x[n-1] are 0, so the only outside indices are between 1 and pos1-1, or between posn+1 and n-1. Can we cover those with another operation?
    // If pos1 > posn, we can choose l = posn, r = pos1, same effect.
    // So one operation with l = min(pos1, posn), r = max(pos1, posn) covers all indices strictly between them. That's a huge range.
    // The remaining uncovered indices are the prefixes before the min and suffixes after the max.
    // But note: the endpoints of that operation are pos1 and posn themselves, which are not covered. But we don't need to cover them because x[pos1] and x[posn] are 0.
    // So if all required indices lie strictly between pos1 and posn, then one operation suffices.
    // What if some required indices are outside? For example, required index i < pos1. Since pos1 is the position of 1, p[i] > 1. Can we cover i with another operation? We need l < i < r and min(p[l],p[r]) < p[i] < max(p[l],p[r]). Since p[i] > 1, we can try to use the minimum value 1 as one endpoint to bracket it. But 1 is at pos1, which is > i. So we can choose l = i-1? Not necessarily. We can choose l = something, r = pos1. Then l < i < pos1, and we need min(p[l], 1) < p[i] < max(p[l], 1). Since 1 is the minimum, min is 1, max is p[l]. So we need 1 < p[i] < p[l]. So we need to find an index l < i such that p[l] > p[i]. If such l exists, we can cover i with operation (l, pos1). Similarly, for a required index j > posn, we need an index r > j such that p[r] > p[j], and use operation (posn, r).
    // But what if no such l exists? That means for all l < i, p[l] < p[i]. So p[i] is a prefix maximum. Similarly, if no such r exists, p[j] is a suffix maximum.
    // Can we cover a prefix maximum? Suppose i is a prefix maximum, so p[i] > all p[1..i-1]. We need to cover i. We need l < i < r with min(p[l],p[r]) < p[i] < max(p[l],p[r]). Since p[i] is large, we need max(p[l],p[r]) > p[i]. That means either p[l] > p[i] or p[r] > p[i]. p[l] > p[i] is impossible because l < i and p[i] is prefix max. So we need p[r] > p[i] for some r > i. If such r exists, we can choose l = something with p[l] < p[i] (e.g., l = i-1, p[i-1] < p[i]), and r with p[r] > p[i]. Then min < p[i] < max holds. So we need an element to the right that is larger than p[i]. If no such element exists, then p[i] is the global maximum, which we already excluded. So for any prefix maximum that is not global max, there is a larger element to the right. So we can cover it with one operation: choose l = i-1 (or any index left of i with smaller value), and r = the index of some larger element to the right. But we need to ensure that this operation doesn't mess up other things? It only sets additional bits to 1, which is fine.
    // So it seems we can cover any single required index with one operation, as long as it's not the global min or max, and not the endpoints.
    // But we have only 5 operations total. We can cover multiple indices with one operation if they are in the same "bracketed" interval.
    // Actually, we can cover all required indices with at most 2 operations: one using (pos1, posn) to cover the middle, and then one for the left side and one for the right side? But we can combine left and right into one operation? Not necessarily, because they are on opposite sides.
    // Let's think systematically: We want to cover a set S of indices. We can always do:
    // Operation 1: l = min(pos1, posn), r = max(pos1, posn). This covers all indices strictly between them.
    // Now, any remaining uncovered required indices are either < min(pos1, posn) or > max(pos1, posn).
    // Let L = min(pos1, posn), R = max(pos1, posn).
    // For an index i < L, we need to cover it. Since i < L <= pos1 and posn, we have i < pos1 and i < posn. We can try to cover all such left indices with one operation: choose l = 0? But l must be >=1. We can choose l = 1? But we need l < i. If i > 1, we can choose l = 1. But we need to bracket the values. We can choose r = L (which is either pos1 or posn). Then we cover all i between 1 and L with values between min(p[1], p[L]) and max(p[1], p[L]). Will this cover all required left indices? Not necessarily, because some required left index might have a value that is not in that range. But we can choose a different r.
    // Actually, we can cover the entire left side with one operation if we choose l = 1 and r = L? Let's test: We need for each required i < L: min(p[1], p[L]) < p[i] < max(p[1], p[L]). Is this always true? Not necessarily. For example, p[1] could be very large, and p[L] is 1 or n. If p[L] = 1, then min = 1, max = p[1]. We need p[i] < p[1]. If some required i has p[i] > p[1], it won't be covered. But we can choose a different l. We can choose l to be the index of the maximum value on the left? But we don't know.
    // Let's think differently: We can cover any index i by an operation (i-1, i+1) if p[i-1] and p[i+1] bracket p[i]. But they might not.
    // However, we can always cover i by finding the nearest larger elements to the left and right. In a permutation, for any element that is not the global max, there is a larger element somewhere. We can find the closest larger element to the left (if any) and to the right (if any). If we have both, we can use them as l and r. If only one side has a larger element, we can use that side and the other side can be any element with a smaller value? Wait, we need min < p[i] < max. So we need one endpoint with value > p[i] and the other with value < p[i]. Since p[i] is not global min or max, there exists at least one element > p[i] and at least one < p[i]. But they might not be on opposite sides of i. They could both be on the same side. If both are on the left, then we can choose l as the smaller value, r as the larger value, both left of i? But then r < i, so we need l < i < r, which fails because r < i. So we need the larger value to be on the right and the smaller on the left, or vice versa. So we need a larger element on one side and a smaller element on the other side. Is that always true for any i that is not an endpoint and not global min/max? Not necessarily. Consider permutation: 3, 1, 2. i=2 (value 1). Left: 3 (>1), right: 2 (>1). Both sides have larger elements. No smaller element except itself. But we need a smaller element to be min. We can't use 1 itself because it's the value at i. So we need an element with value < 1, which doesn't exist. So value 1 is the global minimum. We already excluded global min. So for any non-global-min, there is a smaller element somewhere. But it might be on the same side as the larger element? For i=2 in 2, 3, 1. Value 3. Left: 2 (<3), right: 1 (<3). Both smaller. No larger element except global max? Actually 3 is not global max if n>3. But here n=3, 3 is global max. Excluded. So for non-global-max, there is a larger element. It could be that all larger elements are on one side and all smaller on the same side? Let's try to construct: i with value v. Suppose all elements > v are to the left, and all elements < v are also to the left. Then the right side has only elements... but there are elements on the right, and they must be either >v or <v. If all right elements are >v, then there is a larger element on the right. If all right elements are <v, then there is a smaller element on the right. So it's impossible for all larger and all smaller to be only on the left, because the right side is non-empty (since i is not the last element). So there is at least one element on the right. That element is either >v or <v. If it's >v, then we have a larger on right. We still need a smaller on left. Is there a smaller on left? Not necessarily. For example: p = [5, 3, 4, 2, 1], i=2 (value 3). Left: 5 (>3). Right: 4 (>3), 2 (<3), 1 (<3). So we have larger on left and both larger and smaller on right. We can choose l=1 (value 5) and r=4 (value 2). Then min=2, max=5, and 2 < 3 < 5. Works.
    // What if left has only larger, and right has only larger? Then v is smaller than all other elements? That means v is the global minimum. Excluded.
    // What if left has only smaller, and right has only smaller? Then v is global max. Excluded.
    // What if left has both, right has only larger? Then we can use a smaller from left and a larger from right. Works.
    // What if left has both, right has only smaller? Use larger from left and smaller from right. Works.
    // So it seems for any internal index that is not global min or max, we can find l < i < r that bracket the value. So we can cover any single index with one operation.
    // But we need to cover a set of indices with at most 5 operations. Since we can cover each individually, we can just do one operation per required index? But there could be up to n required indices, and we only have 5 ops. So we need to group them.
    // Can we always cover any set of internal indices (excluding global min/max positions) with at most 2 operations? Let's test with a tricky case.
    // Suppose required indices are all over the place. We can use the (pos1, posn) operation to cover everything between them. Then the remaining are left of min(pos1,posn) and right of max(pos1,posn). Can we cover the left part with one operation and the right part with another? That would be 3 operations total. But maybe we can cover both left and right with one operation? Unlikely because they are separated by the middle which is already covered.
    // Let's analyze the left part. Let L = min(pos1, posn). The left part is indices 2..L-1 (since index 1 is 0). We need to cover some subset of these. Can we cover all of them with one operation? We can choose l = 1, r = L. Then we cover all i in (1, L) with values between p[1] and p[L]. Will this cover all required left indices? Not necessarily, because some required index might have value outside that range. But we can choose a different l and r. We can choose l = 1, and r = some index > L that has a very large value, to make the max large. But then we might also cover some of the middle again, which is fine. Actually, we can choose l = 1, r = n? But then we cover everything between 1 and n, which includes all internal indices! Wait, if we choose l=1, r=n, we cover all i with 1 < i < n and min(p[1], p[n]) < p[i] < max(p[1], p[n]). This covers all indices whose values are strictly between p[1] and p[n]. What about indices with values outside that range? If p[1] and p[n] are not 1 and n, then some values might be outside. But we can choose l and r to be the positions of 1 and n? That is exactly the operation we already considered. But if we choose l=1, r=n, we might not cover everything.
    // However, we have the freedom to choose l and r arbitrarily. We can cover the entire array (except endpoints) with one operation if we can find l and r such that min(p[l],p[r]) = 1 and max(p[l],p[r]) = n. That is, if we can find two indices that are the positions of 1 and n, and we set l = min(pos1, posn), r = max(pos1, posn). Then we cover all indices between them. The only uncovered indices are those outside the interval [pos1, posn]. So if the required set is a subset of the indices between pos1 and posn, one operation suffices.
    // If there are required indices outside, we need additional operations.
    // Can we always cover the outside parts with one additional operation? Let's consider the left outside part: indices < min(pos1, posn). We want to cover some of them. We can do an operation with l = 1, r = min(pos1, posn). This covers indices between 1 and min(pos1, posn). Will it cover all required ones? It covers those with values between p[1] and p[min]. If some required index has value outside, we might need another operation. But maybe we can choose a different r that is further right to include larger values. For example, if we need to cover a left index with a very large value, we can choose r to be the position of that large value? But r must be > i, so if the large value is to the right of i, we can use it. But if the large value is also on the left? Then it's not > i. So we need to find an element to the right of all left required indices that has a value large enough to bracket them.
    // Actually, we can think of the problem as: we have a set of points (i, p[i]) that we need to cover with rectangles of the form (l, r) x (min(p[l],p[r]), max(p[l],p[r])). We can use up to 5 rectangles.
    // This is similar to covering points with "bracketed" rectangles. There is a known result: any set of points not containing the global min or max and not containing endpoints can be covered by at most 2 rectangles? Let's check the sample 3: required indices 3 and 4 (1-based: 3,4). p = [1,3,2,4,6,5]. pos1=1, posn=5 (value 6). min(pos1,posn)=1, max=5. The required indices 3,4 are between 1 and 5. So one operation (1,5) covers them? Let's check: l=1, r=5. min(p[1],p[5])=min(1,6)=1, max=6. So all values between 1 and 6 are covered. Indices between 1 and 5 are 2,3,4. Their values: 3,2,4. All are between 1 and 6. So they all get set to 1. So s becomes 011100? Wait, index 2 is also set to 1, but x[2] was 0, that's fine. So one operation suffices. But the sample output uses 2 operations: (1,5) and (2,6). Why? Maybe because (1,5) alone would set s_2=1, which is fine, but maybe they wanted to show another solution. Actually, the sample output for case 3 is:
    // 2
    // 1 5
    // 2 6
    // So they used two operations. But could they have used one? Let's test: (1,5) covers indices 2,3,4. x has 1 at indices 3,4 (0-based: 2,3). So s_2 and s_3 become 1. s_1 and s_4? Wait, 0-based: indices: 0:1, 1:3, 2:2, 3:4, 4:6, 5:5. x=001100 -> indices 2 and 3 are '1'. Operation (1,5) in 1-based means l=1, r=5. Indices strictly between: 2,3,4. Their values: 3,2,4. All between 1 and 6. So s[2], s[3], s[4] become 1. x[2] and x[3] are 1, x[4] is 0. So it works. So one operation is enough. But the sample used two. So maybe my reasoning is correct: one operation suffices if all required are between pos1 and posn.
    // What if there are required indices outside? Consider a case where pos1 and posn are at the ends? But endpoints can't be required. So pos1 and posn are internal or at ends? They could be at ends. If pos1=1, then x[1] must be 0. Then min(pos1,posn)=1. The left part is empty. So all required are to the right of 1. If posn is also at n, then all required are between 1 and n, so one operation (1,n) covers everything. If posn is internal, then right part might have required indices > posn. We need to cover them.
    // Let's try to construct a case where two operations are not enough. Suppose n=7, p = [2, 7, 1, 6, 3, 5, 4]. pos1=3, posn=2 (value 7). min=2, max=3. The interval between them is empty (since l=2, r=3, no integer between). So operation (2,3) covers nothing. Required indices: say we need to cover index 4 (value 6) and index 6 (value 5). They are outside the interval. Can we cover both with one operation? We need l < 4 and r > 6? Actually we can cover them separately or together. Let's see: To cover index 4, we need l<4<r and bracket value 6. We can choose l=3 (value 1), r=5 (value 3)? min=1, max=3, 6 is not between. Need max > 6. So r must have value >6. The only value >6 is 7 at index 2. But r must be >4, so index 2 is not >4. So we cannot use 7 as r. What about l having value >6? l must be <4. The only value >6 is 7 at index 2, which is <4. So we can set l=2 (value 7). Then we need r such that min(7, p[r]) < 6 < max(7, p[r]). Since 7 > 6, we need min < 6. So we need p[r] < 6. We can choose r=5 (value 3). Then l=2, r=5: indices between are 3,4. Values: 1,6. min=1, max=7. 1<6<7, so index 4 is covered! Also index 3 is covered but not required. So operation (2,5) covers index 4. What about index 6? We need to cover index 6 (value 5). Can we cover it with the same operation? (2,5) only goes up to 5, so index 6 is not between. We need r > 6. Let's try to cover both 4 and 6 with one operation: we need l < 4 and r > 6. l can be 2 (value 7). r must be >6, so r=7 (value 4). Then operation (2,7): indices 3,4,5,6. Values: 1,6,3,5. min=min(7,4)=4, max=7. So we cover values between 4 and 7, i.e., 5 and 6. So indices with values 5 and 6 are covered: index 4 (6) and index 6 (5). Also index 5 (3) is not covered because 3 < 4. So this operation covers both required indices! So one operation suffices for the outside parts.
    // Can we always cover all required indices with at most 2 operations? Let's think: We can always use the operation (pos1, posn) to cover the middle. Then for the left part, we can try to cover it with an operation that uses the global max or min to bracket. But what if the left part has required indices that are very spread out in value? We might need to cover a left index with a small value and another with a large value. Can one operation cover both? Suppose left part has indices i and j with i < j < L. We want to cover both with one operation (l, r) where l < i and r > j (and r <= L maybe, or r could be > L). We need the value interval to contain both p[i] and p[j]. So we need min(p[l], p[r]) < min(p[i], p[j]) and max(p[l], p[r]) > max(p[i], p[j]). We can choose l to be something with a very small value (like 1) and r to be something with a very large value (like n). But 1 and n might not be positioned correctly. If pos1 is to the left of i, we can use l = pos1. If posn is to the right of j, we can use r = posn. But posn might be to the left of j? If posn < j, then we can't use it as r because r must be > j. So we need a large value to the right of all left required indices. Is there always a large value to the right? The global maximum is somewhere. If the global max is to the right of all left required indices, we can use it as r. If the global max is to the left, then the left part contains the global max. But we already excluded the global max from being required. So the global max is not required, but it could be in the left part. If it's in the left part, then for any required index in the left part, its value is less than the global max. So we can use the global max as l? But l must be < i. If the global max is at index M, and M < i, then we can use l = M. Then we need an r > j with a value smaller than the required values? Actually we need min(p[l], p[r]) < p[i] and max > p[i]. If we use l = M (global max), then max is n. We need min < p[i]. So we need p[r] < p[i]. We can choose r to be any index > j with a value smaller than p[i]. Does such an r always exist? Since p[i] is not the global minimum, there is some element with value < p[i]. But it might be to the left of i. If all elements with value < p[i] are to the left of i, then we can't use them as r because r > j >= i. So we need a smaller value to the right. Is it guaranteed that there is a smaller value to the right of any left required index? Not necessarily. Consider the left part: all elements to the left of L. Suppose the left part consists of a decreasing sequence? For example, p = [5,4,3,2,1, ...] but 1 is at pos5, so L=5. Left part is indices 1..4. Values: 5,4,3,2. Required index i=2 (value 4). We need to cover it. Global max is 5 at index 1. So l=1 works. We need r > i with p[r] < 4. The only smaller values are 3,2,1. 3 and 2 are to the right of i? i=2, so indices 3,4 have 3,2. So we can choose r=3. Then operation (1,3) covers index 2? l=1, r=3, indices between: 2. Values: 4. min=min(5,
