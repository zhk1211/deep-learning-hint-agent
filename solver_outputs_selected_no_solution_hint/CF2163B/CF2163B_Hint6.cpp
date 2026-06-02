#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> p(n);
    for (int i = 0; i < n; ++i) cin >> p[i];
    string x;
    cin >> x;

    // Find positions of 1 and n
    int pos1 = -1, posN = -1;
    for (int i = 0; i < n; ++i) {
        if (p[i] == 1) pos1 = i;
        if (p[i] == n) posN = i;
    }

    // Check if all x are same
    char first = x[0];
    bool all_same = true;
    for (char c : x) if (c != first) { all_same = false; break; }

    if (all_same) {
        if (first == '0') {
            cout << "0\n";
            return;
        }
        // all '1'
        // Need to set all s_i = 1
        // Check if possible: we can set all except possibly ends? Actually operation sets interior.
        // We can set all interior positions between l and r if min< p_i < max.
        // To set all positions, we need to cover all indices.
        // If pos1 and posN are not adjacent? Actually we can use multiple operations.
        // But with 5 operations, can we set all? 
        // If n>=3, we can always set all interior positions using pos1 and posN if they are not at ends?
        // Let's think: we need to set s_i=1 for all i. 
        // Operation: choose l,r, sets s_i=1 for i in (l,r) with p_i between p_l and p_r.
        // If we choose l=pos1, r=posN (or vice versa), then min=1, max=n, so all i strictly between get set.
        // So if pos1 and posN are not adjacent, one operation sets all interior.
        // If they are adjacent, we can't set the one between them because there is none.
        // But we can use another pair? For example, if pos1=0, posN=1, then we can choose l=0, r=2 (if n>=3) to set index 1? 
        // p[0]=1, p[2]=something, min=1, max=p[2], so p[1]=n is not between 1 and p[2] (since n > p[2] unless p[2]=n, but n is at pos1? no). So doesn't work.
        // Actually if pos1 and posN are adjacent, we can't set the other's position directly because operation only sets interior.
        // But we can set the other using a different pair? For example, if pos1=0, posN=1, we need to set s_0 and s_1? Wait x_i=1 means s_i must be 1. But operation only sets s_i for i strictly between l and r. So endpoints l and r are never set by that operation. So we can never set s_l or s_r to 1 using an operation with those endpoints. However, we can set them if they are interior of another operation.
        // So to set all positions, we need every index to be interior of at least one operation.
        // Is it always possible with <=5 operations? Let's analyze.
        // We can cover all indices with intervals. For example, choose l=0, r=n-1: sets all interior (1..n-2). Then we need to set 0 and n-1. We can set 0 by choosing l= something <0? impossible. So we need another operation where 0 is interior: choose l=-1? no. So 0 can only be interior if we choose l<0, which is impossible. So index 0 can never be set! Similarly index n-1 can never be set because we cannot choose r>n-1. Wait, operation bounds are 1<=l<=r<=n. So l can be 1 (0-indexed 0) and r can be n (0-indexed n-1). So endpoints are inclusive. But the condition is l < i < r. So i must be strictly between. Thus indices 0 and n-1 can never be interior of any operation because there is no index less than 0 or greater than n-1. Therefore s_0 and s_{n-1} can never become 1. So if x[0]=='1' or x[n-1]=='1', it's impossible!
        // But wait, the problem statement: "for every i such that l < i < r". So indeed endpoints cannot be set.
        // So if all x are '1', and n>=3, then x[0] and x[n-1] are '1', which is impossible. So output -1.
        // Let's check sample 2: n=5, x=11111, output -1. Yes.
        // So if all x are '1', impossible.
        cout << "-1\n";
        return;
    }

    // Now x is not constant. There is at least one '0' and at least one '1'.
    // We need to set s_i=1 exactly for positions where x_i='1'. We can also set extra 1s (since x_i=0 allows any value).
    // We have at most 5 operations.
    // Hint 4: maximum number of operations needed is small.
    // Let's think about the structure.
    // We can use operations to set intervals of 1s.
    // Key observation: An operation with l,r sets all indices i in (l,r) with p_i between p_l and p_r.
    // This is equivalent to: if we consider the permutation, the set of values between p_l and p_r that lie in the interval (l,r) get set.
    // We can think of the Cartesian tree or something.
    // Another perspective: The operation sets a contiguous range of values? Not exactly.
    // But note that we can always set a contiguous block of indices if we choose l and r appropriately.
    // For example, if we want to set a contiguous segment [L,R] (inclusive) of indices to 1, can we do it in one operation?
    // We need to choose l < L and r > R, and ensure that for all i in [L,R], p_i is between p_l and p_r.
    // So we need p_l < p_i < p_r for all i in [L,R]. That means the minimum of p on [L,R] > p_l and maximum < p_r.
    // So we can pick l such that p_l is less than min in [L,R], and r such that p_r is greater than max in [L,R].
    // Also l and r must be outside [L,R].
    // Since we have the whole permutation, we can often find such l and r if the segment doesn't cover the whole array.
    // But we also need to avoid setting unwanted positions? We don't care if we set extra 1s because x_i=0 allows any value.
    // So we can just set large blocks of 1s.
    // The only restriction is that we cannot set the endpoints (0 and n-1) as mentioned.
    // So if x has 1s only at interior positions, we might be able to set them all in one operation by choosing l=0, r=n-1? But that sets all interior positions where p_i is between p_0 and p_{n-1}. That might not cover all interior positions if some p_i is outside that range. But we can choose different l,r.
    // Actually, we can always set any interior position i by choosing l and r such that l < i < r and p_l < p_i < p_r. Since it's a permutation, we can pick l as the position of some value less than p_i, and r as position of some value greater than p_i. We can do this for each required 1 individually, but we have only 5 operations.
    // We need to cover all 1s with at most 5 operations.
    // Notice that the operation sets a set of indices that is exactly the intersection of the index interval (l,r) and the value interval (p_l, p_r). This is a "rectangle" in the permutation matrix.
    // We can think of the permutation as points (i, p_i). An operation with (l,r) sets all points in the open rectangle (l,r) x (min(p_l,p_r), max(p_l,p_r)).
    // We need to cover all points with x_i=1 using at most 5 such rectangles (with the restriction that the rectangle's x-range is (l,r) and y-range is between the y-values of the endpoints).
    // This is reminiscent of covering points with few "axis-aligned" rectangles defined by two points on the permutation.
    // Since we can choose any l,r, the rectangle's x-bounds are l and r, and y-bounds are p_l and p_r. So the rectangle is exactly the bounding box of the two points, but open on all sides? Actually it's open in x (strictly between) and open in y (strictly between). So it's the interior of the bounding box.
    // We want to cover a set of points (the 1s) with at most 5 such open rectangles.
    // Is it always possible if the set of 1s does not include the endpoints? Not necessarily, but maybe we can always do it with 2 operations? Let's check sample 3: they used 2 operations.
    // Let's analyze the structure of the permutation.
    // Consider the values 1 and n. Their positions are pos1 and posN.
    // If we choose l=pos1, r=posN (assuming pos1 < posN), then the rectangle covers all i between them with p_i between 1 and n, i.e., all interior points. So that one operation sets all indices strictly between pos1 and posN to 1. That's a huge block.
    // Similarly, if we choose l=posN, r=pos1, we cover the other side.
    // So with at most 2 operations (using the 1 and n), we can set all indices except possibly the endpoints and the positions of 1 and n themselves? Wait, the operation sets i strictly between l and r. So if we use l=pos1, r=posN, we set all i in (pos1, posN). That does not include pos1 and posN. But we might need to set pos1 or posN if x has 1 there. However, pos1 and posN are interior? They could be endpoints 0 or n-1. If they are endpoints, we can't set them anyway. If they are interior, we can set them by another operation where they are interior.
    // So maybe we can cover all required 1s with at most 2 operations using 1 and n, plus maybe one more to cover the positions of 1 and n if needed.
    // Let's test this idea.
    // Suppose we have a set of indices I where x_i='1'. We want to cover I with open intervals from operations.
    // Operation 1: l = min(pos1, posN), r = max(pos1, posN). This sets all indices between them.
    // Operation 2: l = max(pos1, posN), r = min(pos1, posN)? That doesn't make sense because l<=r. So we can't do the other side with the same pair if we require l<=r. But we can choose the other pair: if pos1 < posN, then operation 1 covers (pos1, posN). To cover indices outside, we could use another pair. For example, we could use the minimum value and maximum value in the remaining part? But we only have 5 ops.
    // Actually, we can cover everything with 2 operations if we choose l=0, r=pos1? Not necessarily.
    // Let's think differently. The operation sets a set of indices that is exactly the set of indices i such that l < i < r and p_i is between p_l and p_r. This is equivalent to: i is in the open interval (l,r) and p_i is in the open interval between p_l and p_r.
    // If we choose l and r such that p_l = 1 and p_r = n (or vice versa), then the value interval is (1, n), which includes all other values. So the operation sets all indices strictly between l and r. So it sets a contiguous block of indices (excluding endpoints). So we can set any contiguous block of interior indices in one operation by choosing l and r as the positions of 1 and n, provided that block is exactly the interval between them? Wait, if we choose l=pos1, r=posN, we set exactly the indices between pos1 and posN. That's a specific block. We cannot choose an arbitrary block with this pair. But we can choose other pairs to set other blocks.
    // What if we choose l and r such that p_l and p_r are not 1 and n? Then the value interval is smaller, so we set a subset of the block between l and r. That might be useful to avoid setting unwanted 1s? But we don't care about extra 1s.
    // So to set a block of indices, we just need to find l and r outside that block such that the value interval covers all values in the block. The easiest is to use the global min and max (1 and n) as the values. Then any l and r with p_l=1 and p_r=n will set all indices between them. So if we want to set a contiguous segment [L, R] (inclusive) of interior indices, we can do it in one operation if we can find l <= L-1 and r >= R+1 such that p_l=1 and p_r=n (or vice versa). But 1 and n are at fixed positions. So we can only set the block between pos1 and posN (if we use them as l and r). That block might not cover all required 1s.
    // However, we can use multiple operations with different pairs to cover different blocks.
    // Notice that we can also use the positions of 1 and n as interior points of other operations. For example, if we want to set pos1, we can choose l and r such that l < pos1 < r and p_l < 1 < p_r? But 1 is the minimum, so p_l < 1 is impossible. So we can never set the position of 1 using an operation where 1 is the interior value? Wait, the condition is min(p_l, p_r) < p_i < max(p_l, p_r). For i = pos1, p_i = 1. We need min(p_l, p_r) < 1, which is impossible since all values are >=1. So the position of 1 can never be set to 1 by any operation! Similarly, the position of n can never be set because we need max(p_l, p_r) > n, impossible.
    // Therefore, s_{pos1} and s_{posN} will always remain 0. So if x has '1' at pos1 or posN, it's impossible!
    // Let's check: In sample 1, p=[1,2,3], pos1=0, posN=2. x=010. x[0]=0, x[2]=0. So possible.
    // Sample 3: p=[1,3,2,4,6,5], pos1=0, posN=4 (value 6 is at index 4). x=001100. x[0]=0, x[4]=0. Possible.
    // Sample 4: p=[6,2,3,4,5,1], pos1=5, posN=0. x=110110. x[5]=0? x[5] is the last character '0'? x=110110, indices 0..5: 1,1,0,1,1,0. So x[5]=0, x[0]=1. But posN=0 has x[0]=1 -> impossible! Output -1. Matches sample.
    // Sample 6: p=[2,5,3,1,4], pos1=3, posN=1. x=00100. x[3]=0, x[1]=0. Possible.
    // So indeed, if x[pos1]=='1' or x[posN]=='1', it's impossible. Also if x[0]=='1' or x[n-1]=='1', it's impossible because endpoints can never be set. But note that pos1 or posN could be endpoints, which is covered by the endpoint condition. But if pos1 is interior, it still can't be set. So we must check both.
    // So necessary conditions: x[0]=='0', x[n-1]=='0', x[pos1]=='0', x[posN]=='0'. Are these sufficient? Maybe with some additional conditions.
    // Let's test: If these hold, can we always achieve the goal with <=5 operations?
    // We need to set s_i=1 for all i where x_i='1'. These i are interior and not pos1 or posN.
    // We can use operations that set blocks of indices. Since we don't care about extra 1s, we can just try to set all indices that are not forbidden. But we can't set pos1, posN, 0, n-1. So the maximum set we can achieve is all indices except these (up to 4 positions). If x has 1s only in that set, we can just set everything else to 1. Can we set all other indices with a few operations?
    // Consider the set of indices we want to set: all i such that x_i='1'. We can also set extra indices. So we want to cover the required 1s with some operations.
    // Since we can set large blocks, maybe we can cover all required 1s with at most 2 operations using the positions of 1 and n as l and r, but we need to cover indices on both sides of the interval between pos1 and posN.
    // Let's analyze the positions of 1 and n. Without loss of generality, assume pos1 < posN. Then the interval (pos1, posN) can be set by operation l=pos1, r=posN. What about indices < pos1? They are to the left of pos1. Can we set them? We need an operation with l < those indices and r > those indices, and values covering them. Since pos1 is the minimum, any value to the left of pos1 is >1. We could choose l=0 (if 0 is not pos1? but pos1>0 since we assumed pos1<posN and pos1 cannot be 0 if we want to set left? Actually pos1 could be 0. If pos1=0, there is no left side.) If pos1 > 0, we have indices 0..pos1-1. We want to set some of them. We can use an operation with l=0 and r=posN? But l=0 is an endpoint, and we cannot set index 0 anyway. But we can set indices 1..pos1-1 if we choose l=0, r=posN? Then the interval is (0, posN). The values: p_0 is some value >1 (since 1 is at pos1). p_posN = n. So min is p_0? Actually min(p_0, n) = p_0 (since p_0 < n). max = n. So the value interval is (p_0, n). This will set all indices i in (0, posN) with p_i in (p_0, n). This includes all indices except possibly those with p_i <= p_0. But p_0 is some value. Since 1 is at pos1, p_0 > 1. The values in (0, pos1) are all >1 and could be less than or greater than p_0. Some might be less than p_0, so they won't be set. So this operation might not set all of (0, pos1).
    // But we can choose a different r. What if we choose l=0, r=pos1? Then interval (0, pos1). Values: p_0 and 1. min=1, max=p_0. Value interval (1, p_0). This sets all i in (0, pos1) with p_i in (1, p_0). Since all values in (0, pos1) are >1 and <p_0? Not necessarily, some could be >p_0. So again not all.
    // To set a contiguous block of indices, we need the value interval to cover all values in that block. The easiest way is to use the global min and max as the values. But we only have one 1 and one n. So we can only create one interval that covers all values: the interval between pos1 and posN. For any other interval, the value range will be limited by the endpoints' values.
    // However, we can use multiple operations to cover different parts.
    // Notice that the permutation can be decomposed into a Cartesian tree. The operation essentially sets a subtree? Not exactly.
    // Another angle: The operation sets all indices i such that l < i < r and p_i is between p_l and p_r. This is exactly the set of indices that are in the open interval (l,r) and whose values are in the open interval between the endpoints. In terms of the permutation, if we plot points (i, p_i), an operation with (l,r) covers the points strictly inside the rectangle defined by x-range (l,r) and y-range (min(p_l,p_r), max(p_l,p_r)). So it's an open rectangle.
    // We need to cover all points with x_i='1' using at most 5 such rectangles.
    // Since we can choose any l,r, the rectangles are quite flexible. But note that the rectangle's y-range is exactly the interval between the y-values of the two points on the permutation at x=l and x=r. So the rectangle is "anchored" by two points on the permutation.
    // This is similar to: we can select any two indices, and the rectangle between them (open) is covered.
    // We want to cover a given set of points.
    // Is it always possible with 2 operations if the necessary conditions hold? Let's check sample 3: they used 2 operations. Sample 6: they used 1 operation.
    // Let's try to find a general strategy.
    // Consider the set of indices I where x_i='1'. We know 0, n-1, pos1, posN are not in I.
    // We can try to cover I with the following operations:
    // 1. l = pos1, r = posN (if pos1 < posN) or l = posN, r = pos1 (if posN < pos1). This covers all indices between them.
    // 2. For the remaining indices (outside the interval between pos1 and posN), we can use another pair.
    // But note that the remaining indices are on one side if pos1 and posN are not at the extremes? Actually, if pos1 < posN, the interval (pos1, posN) is covered. The left side is [0, pos1) and right side is (posN, n-1]. We need to cover required 1s in these side intervals.
    // Can we cover the left side with one operation? We need to choose l and r such that the open interval (l,r) covers the required indices on the left, and the value interval covers their values. Since all values on the left are >1 (because 1 is at pos1), we could choose l=0 and r=pos1? But then the value interval is (1, p_0) or (p_0, 1) depending on order. Since p_0 > 1, min=1, max=p_0. So it covers values in (1, p_0). But some values on the left might be > p_0. They won't be covered.
    // However, we can choose a different r. What if we choose l=0 and r=posN? Then value interval is (p_0, n) (since p_0 < n). This covers values in (p_0, n). It might cover some left indices but not those with values < p_0.
    // So we might need to split the left side into two parts: those with values < p_0 and those with values > p_0. But we can cover both with two operations? That would total 3 operations.
    // But maybe we can cover the entire left side with one operation by choosing l and r appropriately, not necessarily using 0. For example, we could choose l as the position of the minimum value on the left side, and r as the position of the maximum value on the left side. Then the value interval would cover all values on the left side. But we need l and r to be outside the left side? Actually, if we choose l and r both inside the left side, the open interval (l,r) will only cover indices between them, not the whole left side. To cover the whole left side, we need l <= 0? But l can be 0. If we set l=0, then the left side is (0, r). We want r to be > all left indices, so r >= pos1. If we set r=pos1, the interval is (0, pos1). The value interval is between p_0 and 1. That only covers values between 1 and p_0. To cover all values on the left, we need the value interval to be (1, max_left) or something. But max_left could be large. If we set r to something with a large value, say r=posN (value n), then value interval is (p_0, n). That covers values > p_0. So we miss values between 1 and p_0. So we can't cover both with one operation if there are values on both sides of p_0.
    // But wait, we don't have to cover all left indices, only those with x_i='1'. So if the required 1s on the left all have values > p_0, we can use l=0, r=posN. If they all have values < p_0, we can use l=0, r=pos1. If they are split, we might need two operations for the left side.
    // This suggests we might need up to 3 or 4 operations. But the limit is 5, so it's fine.
    // Let's think if there's a simpler characterization.
    // Consider the permutation as a sequence. The operation sets a contiguous range of indices if the value range covers all values in that range. The only value range that covers all values is (1,n). So the only operation that sets a contiguous block regardless of values is when we use 1 and n as endpoints. That operation sets exactly the indices between pos1 and posN.
    // Any other operation sets a subset of a contiguous block, specifically those indices whose values fall into the value interval.
    // So we can think of the problem as: we have a set of "bad" indices that we cannot set (0, n-1, pos1, posN). All other indices can potentially be set. We want to cover the required 1s.
    // Since we can set extra 1s, we can just try to set all indices except the bad ones. Is it always possible to set all non-bad indices with a few operations?
    // Let's test: Suppose we want to set all indices except 0, n-1, pos1, posN. Can we do it with 2 operations?
    // Operation 1: l=pos1, r=posN (assuming pos1<posN). This sets all indices in (pos1, posN). These are all non-bad? pos1 and posN are bad, but they are endpoints so not set. The interior is set. What about indices < pos1? They are not set. Indices > posN? Not set.
    // We need to set the remaining non-bad indices on the left and right.
    // Consider the left side: indices 1..pos1-1 (since 0 is bad). We want to set them. Can we do it in one operation? We need to choose l and r such that (l,r) covers these indices and the value interval covers their values. Since 1 is at pos1, the values on the left are all >1. The maximum value on the left is some M. If we can find an operation with value interval covering (1, M] or at least covering all values that appear on the left, we can set them. But we need the value interval to be (min(p_l,p_r), max(p_l,p_r)). To cover all left values, we need min(p_l,p_r) <= 1? Actually we need min(p_l,p_r) < p_i for all left i. Since the smallest left value is >1, we need min(p_l,p_r) <= 1. But 1 is the absolute minimum, so min(p_l,p_r) must be 1. That means one of l or r must be pos1. Similarly, we need max(p_l,p_r) > all left values, so it must be at least the maximum left value. So we need one endpoint to be pos1 (value 1) and the other endpoint to have value >= max_left. Also, the interval (l,r) must cover all left indices. If we set l=pos1, then we need r > pos1 to cover left? But left indices are < pos1, so if l=pos1, the interval is (pos1, r) which is to the right of pos1, not left. So we need l < pos1 and r = pos1? Then interval (l, pos1) covers left. Then p_l must be the one with value >= max_left, and p_r=1. So min=1, max=p_l. This works! So we can set the entire left side in one operation by choosing l = position of the maximum value on the left (or any value >= max_left) and r = pos1. But wait, we need l < pos1. The position of the maximum left value is some index in [0, pos1-1]. Let that be Lmax. Then we can set l = Lmax, r = pos1. The interval is (Lmax, pos1). This covers indices strictly between Lmax and pos1. But what about indices to the left of Lmax? They are not covered because they are not > Lmax. So this only covers indices between Lmax and pos1. To cover all left indices, we need l to be as left as possible, ideally 0. But if we set l=0, then p_0 might not be the maximum left value. Then the value interval would be (1, p_0) if p_0 < max_left, missing values > p_0. So we can't cover all left indices with one operation if the maximum left value is not at l=0.
    // However, we can use multiple operations to cover different parts of the left side. For example, we can cover the part with values < p_0 using l=0, r=pos1, and the part with values > p_0 using l=0, r=posN? But l=0, r=posN covers values > p_0, but the interval is (0, posN), which includes the left side and also the middle. That's fine, we can set extra. So we can cover the left side with two operations: one with l=0, r=pos1 (covers values in (1, p_0)) and one with l=0, r=posN (covers values in (p_0, n)). Together they cover all left indices because every left value is either < p_0 or > p_0. (If equal? distinct values, so no equal). So two operations cover the left side completely. Similarly for the right side.
    // But we already used operation l=pos1, r=posN for the middle. So total operations: 1 (middle) + 2 (left) + 2 (right) = 5. That's exactly the limit!
    // Let's verify: We have operations:
    // 1. l=pos1, r=posN (or vice versa) -> covers middle.
    // 2. l=0, r=pos1 -> covers left part with values < p_0.
    // 3. l=0, r=posN -> covers left part with values > p_0 (and also middle again, but that's fine).
    // 4. l=posN, r=n-1 -> covers right part with values < p_{n-1}? Wait, need to cover right side.
    // By symmetry, for the right side (indices > posN), we can use:
    // l=posN, r=n-1 -> covers values in (1, p_{n-1})? Actually p_posN = n, so min=1? No, if we choose l=posN (value n) and r=n-1 (value some v), then min=v, max=n. So value interval (v, n). This covers right indices with values > v.
    // And l=pos1, r=n-1? That would cover values in (1, p_{n-1})? Actually l=pos1 (value 1), r=n-1 (value v), min=1, max=v, covers values < v.
    // So we can cover the right side with two operations: l=posN, r=n-1 and l=pos1, r=n-1.
    // But wait, we need to ensure the intervals cover the indices. For the right side, indices are > posN. If we use l=posN, r=n-1, interval is (posN, n-1). That covers all right indices except possibly n-1 (which is bad anyway). If we use l=pos1, r=n-1, interval is (pos1, n-1). That also covers right indices (since pos1 < posN < right indices). So both cover the right indices. The value intervals: first covers values > p_{n-1}, second covers values < p_{n-1}. Together they cover all values on the right (since p_{n-1} is some value, all other values are either less or greater). So two operations cover the right side.
    // So total operations: 
    // - (pos1, posN) if pos1<posN else (posN, pos1)
    // - (0, pos1)
    // - (0, posN)
    // - (posN, n-1)
    // - (pos1, n-1)
    // That's 5 operations. But note that some of these might be invalid if l >= r? We need l < r because operation requires l < r? Actually condition is 1 <= l <= r <= n. It allows l=r? But if l=r, then l < i < r is empty, so no effect. We want l < r to have an effect. So we should ensure l < r.
    // Check: (0, pos1): 0 < pos1? pos1 is the position of 1. If pos1=0, then 0 < 0 is false. So if pos1=0, we can't use (0, pos1). Similarly, if posN=n-1, (posN, n-1) is invalid. Also (pos1, posN) requires pos1 != posN (they are distinct because 1 != n). So we need to handle edge cases where some of these intervals are empty or invalid.
    // But we don't necessarily need all 5 operations. We only need to cover the required 1s. We can just generate these 5 operations if they are valid (l < r) and that will set a superset of the required 1s (all non-bad indices). Then if the necessary conditions hold, the required 1s will be covered. But wait, does this set all non-bad indices? Let's check:
    // Bad indices: 0, n-1, pos1, posN.
    // Our operations:
    // Op1: (min(pos1,posN), max(pos1,posN)) -> covers indices strictly between them. Does not cover pos1, posN, 0, n-1 (unless they are between, but they are endpoints). So covers middle.
    // Op2: (0, pos1) if 0 < pos1 -> covers indices in (0, pos1). These are left of pos1. Does not cover 0 or pos1.
    // Op3: (0, posN) if 0 < posN -> covers indices in (0, posN). This includes left of pos1 (if pos1<posN) and also between pos1 and posN. Overlaps with Op1.
    // Op4: (posN, n-1) if posN < n-1 -> covers indices in (posN, n
