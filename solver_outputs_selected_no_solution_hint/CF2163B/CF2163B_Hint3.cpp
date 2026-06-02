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

    // Find all indices i where x[i] == '1'
    vector<int> ones;
    for (int i = 0; i < n; ++i) {
        if (x[i] == '1') ones.push_back(i);
    }

    if (ones.empty()) {
        cout << "0\n";
        return;
    }

    // Check if all ones are in a single "valley" or "peak" structure
    // We need to find a sequence of operations that covers all ones.
    // Key observation: an operation (l, r) sets s_i = 1 for i in (l, r) if p_i is between p_l and p_r.
    // This is equivalent to: if we consider the Cartesian tree, an operation corresponds to an interval
    // where the endpoints are the min and max in that interval? Actually, it's about values between p_l and p_r.
    // Another view: if we sort indices by value, we can think of intervals.
    
    // Let's find the minimal interval [L, R] that contains all ones.
    int L = *min_element(ones.begin(), ones.end());
    int R = *max_element(ones.begin(), ones.end());

    // We need to cover all ones with at most 5 operations.
    // Each operation can cover a contiguous range of indices that are "between" the endpoints in value.
    // This is similar to covering points with intervals that are "valid" according to p.
    
    // We can try to cover all ones with a single operation if possible.
    // For a single operation (l, r) to cover a set of indices S, we need:
    // For all i in S, l < i < r and min(p_l, p_r) < p_i < max(p_l, p_r).
    // This means that p_l and p_r must be the minimum and maximum of the set {p_l, p_r} union {p_i for i in S}.
    // Actually, if we choose l and r such that p_l is the minimum value among all indices in [l, r] and p_r is the maximum,
    // then all indices between them with values in between will be set to 1.
    // So to cover S, we can try to find l <= min(S) and r >= max(S) such that p_l = min value in [l, r] and p_r = max value in [l, r],
    // and all elements of S are strictly between l and r and have values between p_l and p_r.
    
    // Since we have at most 5 operations, we can try to cover the ones with a few intervals.
    // Notice that the operation is symmetric: if we have an interval [l, r] where p_l and p_r are the min and max,
    // then the operation sets to 1 all indices i in (l, r) with p_i between them.
    // This is exactly the set of indices that are in the same "component" when we consider the Cartesian tree?
    
    // Alternative approach: The operation can be seen as: pick l, r. Let a = min(p_l, p_r), b = max(p_l, p_r).
    // Then for all i in (l, r) with a < p_i < b, set s_i = 1.
    // This is equivalent to: if we consider the permutation, we can "activate" a contiguous range of values (a, b) 
    // that are located between l and r.
    
    // We can think of the problem as: we need to cover all ones with at most 5 such "value intervals" that are contiguous in index.
    // Actually, each operation covers a set of indices that form a contiguous subsequence in the index order? Not necessarily:
    // the indices with values between a and b that lie between l and r might not be contiguous if there are indices with values outside [a, b] in between.
    // But the operation only sets those with values in (a, b). So the set of indices set to 1 is exactly the intersection of (l, r) with the set of indices whose values are in (a, b).
    // This set is not necessarily contiguous.
    
    // Let's re-read the operation: "for every i such that l < i < r and min(p_l, p_r) < p_i < max(p_l, p_r) hold at the same time, you will set s_i to 1."
    // So it's exactly the indices between l and r whose values are strictly between the values at l and r.
    
    // We can think of the permutation as points (i, p_i). An operation with endpoints (l, p_l) and (r, p_r) defines an axis-aligned rectangle 
    // (l, r) x (min(p_l, p_r), max(p_l, p_r)). All points strictly inside this rectangle get set to 1.
    // We need to cover a given set of points (the ones) with at most 5 such rectangles, where the rectangles' left and right boundaries are exactly at the x-coordinates of the endpoints,
    // and the bottom and top boundaries are exactly at the y-coordinates of the endpoints.
    
    // This is a known problem: covering points with "orthogonal rectangles" defined by two points on the permutation.
    // Since the permutation is a set of points with distinct x and y, each rectangle is defined by two points that are its bottom-left and top-right? Not exactly:
    // The rectangle's x-range is (l, r) and y-range is (min, max). The endpoints are on the boundary of the rectangle (on the left and right edges).
    // So the rectangle is "anchored" at two points that lie on its left and right boundaries.
    
    // We can try to cover the ones greedily. Since we have only 5 operations, we can try to find a small number of rectangles.
    // Observe that if we take l = 1 and r = n, then the rectangle covers all points except possibly the min and max? 
    // Actually, if we take l=1, r=n, then min(p_1, p_n) and max(p_1, p_n). The rectangle covers all i in (1, n) with values between them.
    // This might not cover all ones if some ones are at the boundaries or have values outside.
    
    // Another idea: The operation is essentially: pick two indices, and you "activate" all indices between them that are "between" in value.
    // This is similar to the concept of "inversions" or "intervals" in a permutation.
    
    // Let's consider the complement: we need to set s_i = 1 for all i with x_i = 1. We can also set extra ones.
    // So we need to cover the ones.
    
    // Since n can be large, we need an efficient check.
    
    // Let's analyze the structure of a single operation.
    // For a fixed l, r, the set of i set to 1 is: { i | l < i < r, min(p_l, p_r) < p_i < max(p_l, p_r) }.
    // This set is exactly the set of indices that are in the open interval (l, r) and whose values are between the values at l and r.
    // If we consider the permutation as a sequence, this is the set of indices that are "between" l and r in both index and value.
    
    // Notice that if we have an operation (l, r), we can also think of it as: we choose a value interval [a, b] = [min, max], and we set to 1 all indices in (l, r) with values in (a, b).
    // But l and r are exactly the positions of a and b? Not necessarily: p_l could be a or b, and p_r the other.
    // So the endpoints are exactly the positions of the min and max values in that interval? Not necessarily: there could be values outside [a, b] at l and r? No, p_l and p_r are exactly a and b.
    // So the endpoints are the positions of the minimum and maximum values of the set {p_l, p_r} union {p_i for i in (l, r) with values between}? Actually, p_l and p_r are the min and max of the whole interval [l, r]? Not necessarily: there could be values in (l, r) that are less than min or greater than max, but those are not set to 1. However, the operation only cares about p_l and p_r. The values outside [min, max] in (l, r) are ignored.
    
    // So an operation is defined by two indices l < r. The set of affected indices is those between l and r with values between p_l and p_r.
    
    // We can try to cover the ones with a single operation if possible. If not, try two, etc., up to 5.
    // Since 5 is small, we can try to find a set of at most 5 intervals [l_i, r_i] that cover all ones.
    // But how to check if a set of ones can be covered by one operation?
    // For a set S of indices to be coverable by one operation (l, r), we need:
    // - There exist l, r such that l < min(S) and r > max(S) (since all i in S must satisfy l < i < r).
    // - For all i in S, min(p_l, p_r) < p_i < max(p_l, p_r).
    // - Also, we don't care about other indices.
    // This means that the values of S must be strictly between p_l and p_r.
    // So if we choose l and r such that p_l is the minimum value among all indices in [l, r] and p_r is the maximum, then all S will be covered if they lie between.
    // But we can also choose l and r that are not the absolute min and max of the whole interval, as long as the values of S are between them.
    // To maximize the chance of covering S, we should choose l and r such that p_l is as small as possible and p_r as large as possible, while keeping l < min(S) and r > max(S).
    // The best we can do is to take l as the position of the minimum value in the prefix up to min(S)-1, and r as the position of the maximum value in the suffix from max(S)+1.
    // But wait, l and r can be anywhere as long as l < min(S) and r > max(S). We can even take l=1, r=n if that works.
    // However, we also need that for all i in S, p_i is between p_l and p_r. So we need p_l < min_{i in S} p_i and p_r > max_{i in S} p_i.
    // So we need to find l < min(S) and r > max(S) such that p_l < min(p_S) and p_r > max(p_S), where p_S = {p_i : i in S}.
    // Is that sufficient? If we find such l and r, then for any i in S, l < i < r (since i in S implies min(S) <= i <= max(S), and l < min(S) <= i, r > max(S) >= i, so l < i < r holds). And min(p_l, p_r) <= p_l < min(p_S) <= p_i <= max(p_S) < p_r <= max(p_l, p_r). So indeed p_i is strictly between. So the condition is exactly: there exists l < min(S) with p_l < min(p_S), and there exists r > max(S) with p_r > max(p_S). Note that l and r can be the same operation? Yes, we need both in one operation. So we need to find l < min(S) and r > max(S) such that p_l < min(p_S) and p_r > max(p_S). They don't have to be related except l < r, which is automatically true if l < min(S) <= max(S) < r.
    // So a set S can be covered by one operation iff there exists an index l < min(S) with p_l < min(p_S) and an index r > max(S) with p_r > max(p_S).
    // But wait: what if min(p_S) is 1? Then we cannot find p_l < 1. So if min(p_S) == 1, we cannot have p_l < min(p_S). However, we could choose l such that p_l is the minimum? The condition requires min(p_l, p_r) < p_i. If p_i = 1, then we need min(p_l, p_r) < 1, impossible. So any i with p_i = 1 cannot be covered by any operation because the operation requires the value to be strictly greater than the minimum of the endpoints. Similarly, if p_i = n, it cannot be covered because we need p_i < max(p_l, p_r). So if x has a 1 at the position of value 1 or value n, it's impossible? Let's check: In the operation, we set s_i = 1 if min(p_l, p_r) < p_i < max(p_l, p_r). So if p_i = 1, it can never be strictly greater than the min. So s_i can never become 1. Thus if x_i = 1 for the index with p_i = 1, it's impossible. Similarly for p_i = n. So we can immediately output -1 if x has 1 at the position of 1 or n.
    // Let's verify with examples: In sample 2, p = [3,4,2,1,5], x = 11111. The position of 1 is index 4 (1-indexed). x_4 = 1, so impossible. Output -1. Correct.
    // In sample 4: p = [6,2,3,4,5,1], x = 110110. Position of 1 is index 6, x_6 = 0. Position of 6 is index 1, x_1 = 1? Wait, p_1 = 6, x_1 = 1. So value 6 has x=1. That should be impossible because p_i = n cannot be covered. But sample output is -1. Yes.
    // So this condition is necessary.
    
    // Now, if the ones do not include 1 or n, can we always cover them with a single operation? Not necessarily, because we need to find l and r that satisfy the conditions. We can always choose l as the position of 1? But 1 might be to the right of min(S). We need l < min(S). So if the position of 1 is >= min(S), we cannot use it as l. We need some l < min(S) with p_l < min(p_S). Similarly for r.
    // So a set S is coverable by one operation iff there exists l < min(S) with p_l < min(p_S) and r > max(S) with p_r > max(p_S).
    // This is equivalent to: min(p_S) > min_{i < min(S)} p_i? Not exactly, we need existence of at least one such l. So min(p_S) > min_{i < min(S)} p_i? If the minimum value in the prefix before min(S) is less than min(p_S), then we can pick that l. Similarly, we need max(p_S) < max_{i > max(S)} p_i.
    // So we can precompute prefix minimums and suffix maximums.
    
    // Now, we have a set of ones. We want to cover them with at most 5 operations. Each operation can cover a subset of ones that satisfies the above condition.
    // Since 5 is small, we can try to partition the ones into at most 5 groups, each group being a contiguous range of indices? Not necessarily contiguous in index, but the condition for a group S involves min(S) and max(S). If we have multiple groups, they might interleave. But we can always order the groups by their min index. Since operations are independent, we can perform them in any order. The sets of indices covered by different operations can overlap; we just need every one to be covered by at least one operation.
    
    // To simplify, we can try to cover the ones with a single operation if possible. If not, try two, etc. But how to check if a given set of ones can be covered by k operations?
    // Since k <= 5, we can try a greedy approach: repeatedly find the leftmost uncovered one, and try to cover as many as possible with one operation starting from it.
    // But the condition for a group depends on the min and max of the group. If we take a contiguous range of ones (in index order), the min and max of the group are just the min and max of that range. So we can try to cover a prefix of the remaining ones with one operation, and then recurse.
    // However, the optimal grouping might not be contiguous in index order? Suppose ones are at indices 2 and 5, and we can cover them together with one operation if we choose l=1, r=6. That's contiguous in index? The group is {2,5}, min=2, max=5. The condition checks l < 2 and r > 5. It doesn't require the indices between 2 and 5 to be ones. So the group is essentially defined by its min and max index. Any subset of ones that lie within some interval [L, R] can be covered together if there exist l < L and r > R with the value conditions. So if we have a set of ones, we can cover them all with one operation if the condition holds for L = min(ones) and R = max(ones). Because if we cover the whole interval [L, R] of indices, we cover all ones inside. But wait, the operation only covers indices with values between p_l and p_r. So if there is a one inside [L, R] with value outside the range, it won't be covered. But the condition we derived (p_l < min(p_S) and p_r > max(p_S)) ensures that all values in S are between p_l and p_r. So if we take S = all ones, we need p_l < min_{i in S} p_i and p_r > max_{i in S} p_i. This condition is exactly that the minimum value among ones is greater than some value to the left, and maximum value among ones is less than some value to the right. So it's a condition on the whole set of ones.
    
    // Therefore, if the whole set of ones satisfies the condition, we can cover them with one operation. If not, we might need to split them into multiple groups, each group being a contiguous block in index? Not necessarily contiguous, but we can always consider groups that are contiguous in index because if we have a group that is not contiguous, we can just take the min and max of that group and the condition only depends on those. So any group can be represented by its index interval [L, R]. And covering that interval with one operation will cover all ones inside that interval (provided the value condition holds). So we can think of covering the ones with intervals [L_i, R_i] such that each interval satisfies the condition for its own set of ones inside.
    
    // Thus, the problem reduces to: given a set of points (indices of ones), we want to cover them with at most 5 intervals [L, R] such that for each interval, there exists l < L with p_l < min_{i in [L,R] and x_i=1} p_i and r > R with p_r > max_{i in [L,R] and x_i=1} p_i.
    // But note: the condition for an interval [L, R] only depends on the ones inside it. If we take an interval that contains some ones, the min and max values of those ones must satisfy the condition. If we take a larger interval that includes more ones, the min might decrease and max might increase, making the condition harder to satisfy. So it's always better to take intervals that are as tight as possible around the ones they cover.
    
    // Since we want to cover all ones, we can try to partition the sorted list of one indices into at most 5 contiguous segments. For each segment, we check if it can be covered by one operation. If any segment cannot, then that partition fails.
    // Is it sufficient to only consider partitions into contiguous segments of the sorted ones? Suppose we have ones at indices 2, 4, 6. Could we cover {2,6} with one operation and {4} with another? That would mean intervals [2,6] and [4,4]. But [2,6] contains 4, so 4 would be covered twice. That's fine. But if we partition into contiguous segments, we would either group {2,4,6} together, or {2,4} and {6}, or {2} and {4,6}, or all separate. The grouping {2,6} and {4} is not a partition into contiguous segments because {2,6} is not contiguous in the sorted list (it skips 4). But we can just take the interval [2,6] which covers 2,4,6. If [2,6] is coverable, then we can cover all three with one operation. If [2,6] is not coverable, maybe [2,4] and [6,6] are? But if [2,6] is not coverable, it means either the min value condition fails or the max value condition fails. If we split into [2,4] and [6,6], the min value for [2,4] might be higher (since we removed 6 which might have had a low value? Actually, removing indices can only increase the min value or decrease the max value, making the condition easier). So splitting can help. But would we ever need a non-contiguous group like {2,6}? If we take {2,6}, the min index is 2, max is 6. The condition depends on the min and max values among {2,6}. If we instead took the contiguous group {2,4,6}, the min value could be lower (if 4 has a lower value than both 2 and 6) and max value could be higher, making it harder. So it's possible that {2,6} is coverable but {2,4,6} is not. But if {2,6} is coverable, we can just use that operation to cover 2 and 6, and then we still need to cover 4. But the operation covering {2,6} will also cover any index between 2 and 6 that has value between p_l and p_r. It might or might not cover 4. If it doesn't cover 4, we need another operation for 4. But if we use {2,6}, we are using an interval [l, r] with l < 2 and r > 6. This interval will also include 4. Whether 4 gets covered depends on its value. If 4's value is not between p_l and p_r, it won't be covered. So we might need a separate operation for 4. In that case, the groups are {2,6} and {4}. But note that {2,6} is not a contiguous segment of ones; it's two ones with a one in between that is not covered by that operation. So the partition of the set of ones into groups is not necessarily a partition of the index line into intervals; it's a partition of the set of ones into subsets, each covered by one operation. The operations' intervals may overlap.
    
    // However, we can observe that if we have an operation that covers a set S, the interval [min(S), max(S)] is covered by the operation's interval (l, r). Any other one inside that interval will either be covered or not. If it is not covered, it must have a value outside (p_l, p_r). But then we could potentially adjust l and r to include it? Not necessarily.
    
    // Given the complexity, maybe there is a simpler characterization. Let's think about the hints.
    // Hint 1: Think about the case where x is constant.
    // If x is all 1s, we need to set all s_i = 1. But we can never set the positions of 1 and n to 1. So if x is all 1s, it's possible only if n=1? But n>=3. So impossible unless 1 and n are not in the string? But x is all 1s, so it's impossible. That matches sample 2.
    // If x is all 0s, we can do 0 operations.
    
    // Hint 2: How would you solve the problem if you could change p?
    // If we could permute p arbitrarily, we could just place 1 and n at the ends and cover everything in one operation. But we can't.
    
    // Hint 3: Can you use some information about x to make your solution more efficient?
    // Maybe we only need to consider the leftmost and rightmost ones, and the ones with minimum and maximum values?
    
    // Let's analyze the necessary and sufficient conditions for a set of ones to be coverable by k operations.
    // We already know that if any one has value 1 or n, it's impossible.
    // Now, suppose we have a set of ones. We want to cover them with operations. Each operation can be thought of as choosing two indices l, r. The effect is setting s_i = 1 for i in (l, r) with p_i in (min(p_l, p_r), max(p_l, p_r)).
    // This is equivalent to: we can "activate" any index i if we can find l < i < r such that p_l < p_i < p_r. So an index i can be covered if there exist l < i with p_l < p_i and r > i with p_r > p_i. That is, i is not a left-to-right minimum (i.e., there is some j < i with p_j < p_i) and i is not a right-to-left maximum (i.e., there is some k > i with p_k > p_i). Actually, we need both simultaneously in one operation? The operation uses the same l and r. So we need a single pair (l, r) with l < i < r, p_l < p_i < p_r. This is equivalent to: i is not a prefix minimum (there exists l < i with p_l < p_i) AND i is not a suffix maximum (there exists r > i with p_r > p_i). But do they need to be the same operation? Yes, we need one operation that provides both l and r. But we can always choose l as the left one and r as the right one independently? The operation requires choosing both l and r. So we need to find l < i and r > i such that p_l < p_i < p_r. This is possible if and only if there exists some l < i with p_l < p_i AND some r > i with p_r > p_i. Because we can just pick that l and that r in the same operation. They don't have to be related except l < r, which is true since l < i < r. So the condition for a single index i to be coverable by some operation is exactly: i is not a prefix minimum (i.e., min_{j < i} p_j < p_i) and i is not a suffix maximum (i.e., max_{j > i} p_j > p_i). If either condition fails, i can never be covered. So if any one fails this, it's impossible.
    // Let's check: For i to be covered, we need l < i with p_l < p_i. If no such l exists, then p_i is the minimum among all indices <= i. Then any l < i will have p_l > p_i, so min(p_l, p_r) >= p_i, cannot be strictly less. So indeed impossible. Similarly for r.
    // So a necessary condition for each one is that it is not a prefix minimum and not a suffix maximum. In other words, there is a smaller value to its left and a larger value to its right.
    // Is this condition also sufficient for a single index? Yes, we can just pick l as the index of the minimum to the left, and r as the index of the maximum to the right. Then the operation (l, r) will cover i. But wait: we need to ensure that l < i < r. If we pick l as the index of the minimum to the left, it's < i. If we pick r as the index of the maximum to the right, it's > i. So it works.
    // So each individual one can be covered by its own operation if the condition holds. But we have a limit of 5 operations total. So we need to cover multiple ones with the same operation.
    
    // Now, when can a single operation cover a set of indices S? As derived, we need l < min(S) and r > max(S) with p_l < min_{i in S} p_i and p_r > max_{i in S} p_i.
    // This is equivalent to: there exists an index l < min(S) with p_l < min_{i in S} p_i, and an index r > max(S) with p_r > max_{i in S} p_i.
    // So the condition for a set S is exactly that the minimum value in S is greater than the minimum value in the prefix before min(S), and the maximum value in S is less than the maximum value in the suffix after max(S).
    // Let's define L = min(S), R = max(S). Let min_left = min_{j < L} p_j, max_right = max_{j > R} p_j.
    // Then S can be covered by one operation iff min_{i in S} p_i > min_left and max_{i in S} p_i < max_right.
    // Note that min_{i in S} p_i >= min_{i in [L, R]} p_i, but we only care about the ones in S. However, if we take the whole interval [L, R], the condition becomes harder because the min over [L, R] might be smaller. So it's better to only consider the ones in S. But when we perform the operation, we will set s_i = 1 for all i in (l, r) with values between p_l and p_r. This might cover additional ones, which is fine. So we can always choose to cover a set S by taking l and r that satisfy the condition for S. The operation will also cover any other index in (l, r) with value in (p_l, p_r). So if we want to cover a specific set S, we just need to find l, r that satisfy the condition for S. The fact that it might cover more is okay.
    
    // Therefore, to cover all ones with at most 5 operations, we can try to partition the ones into at most 5 groups, where each group G satisfies: let L = min(G), R = max(G). There exists l < L with p_l < min_{i in G} p_i and r > R with p_r > max_{i in G} p_i.
    // Since we want to minimize the number of groups, we can try to cover as many ones as possible with one operation, then recurse.
    // But how to find the maximal group starting from the leftmost one?
    // Suppose we sort the ones by index. Let the ones be at indices i_1 < i_2 < ... < i_m.
    // We want to find the largest k such that the set {i_1, ..., i_k} can be covered by one operation.
    // For this set, L = i_1, R = i_k. The min value among these ones is min_{j=1..k} p_{i_j}, max value is max_{j=1..k} p_{i_j}.
    // We need min_left(i_1) < min_value and max_right(i_k) > max_value.
    // As k increases, L remains i_1, R increases. min_value can only decrease or stay same, max_value can only increase or stay same.
    // So the condition min_left < min_value becomes easier to satisfy? Actually min_left is fixed (depends only on i_1). min_value decreases, so min_left < min_value might become false if min_value drops below min_left. So it can become harder.
    // Similarly, max_right(i_k) might decrease as R increases (since the suffix shrinks), and max_value increases, so max_right > max_value might become false.
    // So the condition is monotonic in the sense that if it fails for some k, it might fail for larger k? Not necessarily monotonic, but we can just check each k from 1 to m and find the largest k that works. Since m can be up to n, and we might need to do this for each group, and we have up to 5 groups, we can do a linear scan.
    
    // But wait: the groups don't have to be contiguous in the sorted ones. Could it be beneficial to skip some ones in the first group to make the condition hold, and cover the skipped ones with later groups? For example, suppose the first few ones have a very small value that makes min_value too small, so the condition fails. If we skip that one, the min_value might be larger, allowing the group to be covered. Then we can cover the skipped one with another operation. So the optimal strategy might involve choosing a subset of the ones to cover with the first operation, not necessarily a prefix.
    // However, note that if we skip an one, it must be covered by some later operation. That later operation will have its own L' and R'. Since the skipped one is to the right of the first group's L, it could be covered by an operation that starts later. But we can always reorder operations. So we can think of it as: we want to select at most 5 intervals [l_j, r_j] (with their associated value ranges) such that every one is covered by at least one interval.
    // This is a covering problem. With only 5 intervals, we can try to find them greedily or via dynamic programming.
    
    // Let's think differently. The operation essentially allows us to "activate" a set of indices that form a "valley" in the permutation? Actually, if we take l and r such that p_l is the minimum and p_r is the maximum in the interval [l, r], then the operation sets all indices between them to 1. This is exactly the set of indices that are not the minimum or maximum of the interval? Not exactly.
    
    // Another perspective: Consider the Cartesian tree of the permutation. The operation (l, r) where p_l and p_r are the min and max of [l, r] corresponds to taking a node in the Cartesian tree? Not sure.
    
    // Let's look at the samples.
    // Sample 3: p = [1,3,2,4,6,5], x = 001100. Ones at indices 3,4 (1-indexed). Values: p_3=2, p_4=4. L=3, R=4. min_left = min(p_1, p_2) = min(1,3)=1. min_value = min(2,4)=2. 1 < 2 ok. max_right = max(p_5, p_6) = max(6,5)=6. max_value = 4. 6 > 4 ok. So one operation (1,5) works? l=1, r=5. p_1=1, p_5=6. min=1, max=6. Indices between: 2,3,4. Values: 3,2,4. All between 1 and 6. So s_2, s_3, s_4 become 1. That covers 3 and 4. But x has 1 at 3 and 4, and also 0 at others. So valid. They also did a second operation (2,6) which didn't change anything. So one operation was enough.
    
    // Sample 6: p = [2,5,3,1,4], x = 00100. Ones at index 3. p_3=3. Check if index 3 can be covered: need l<3 with p_l<3. p_1=2 <3, ok. r>3 with p_r>3. p_5=4>3, ok. So we can do l=1, r=5. Operation (1,5): p_1=2, p_5=4. min=2, max=4. Indices between: 2,3,4. Values: 5,3,1. Those between 2 and 4: only 3. So s_3
