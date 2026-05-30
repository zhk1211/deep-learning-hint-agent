// Hint0
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    vector<int> pos(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        pos[a[i]] = i;
    }
    
    int l = pos[0], r = pos[0];
    long long ans = 1;
    int used = 1; // number of elements placed in [l, r]
    
    for (int mex = 1; mex < n; ++mex) {
        int p = pos[mex];
        if (p < l) {
            // expand left
            int new_l = p;
            int free_slots = (l - new_l - 1);
            // we need to place numbers > mex into these slots later
            // but we just count available choices now
            // Actually, we multiply by number of ways to choose positions for numbers > mex
            // that will be placed in the gap.
            // We'll handle it differently.
            l = new_l;
        } else if (p > r) {
            int new_r = p;
            int free_slots = (new_r - r - 1);
            r = new_r;
        } else {
            // p inside [l, r]
            // mex is already inside, so no expansion
            // but we have free slots inside? Actually no, all inside are already filled with smaller numbers.
            // So nothing to do.
        }
        // The number of elements currently in [l, r] is (r - l + 1)
        // The mex is the smallest missing, so all numbers < mex are inside [l, r]
        // So the number of elements placed is mex.
        // The number of free slots inside [l, r] is (r - l + 1) - mex.
        // But wait, we haven't placed numbers > mex yet.
        // Actually, the condition forces that all numbers < mex must be inside [l, r].
        // For numbers > mex, they can be placed anywhere outside, but also some may be inside?
        // Let's think: If we have an interval with mex = k, then all numbers 0..k-1 are inside,
        // and k is outside. So the minimal interval containing 0..k-1 cannot contain k.
        // This implies that the positions of 0..k-1 form a contiguous segment? Not necessarily,
        // but the minimal interval covering them must not contain k.
        // The standard solution: maintain l = min pos of 0..i, r = max pos of 0..i.
        // For mex = i+1, the interval [l, r] contains all 0..i. The number i+1 must be outside.
        // The number of ways to place numbers > i+1 into the gaps? Actually, we build the permutation
        // by inserting numbers in increasing order. When we consider mex = i, we have placed 0..i-1.
        // Their positions define l and r. The number i must be placed outside [l, r].
        // The number of available positions outside is n - (r - l + 1).
        // But we also have to consider that later numbers can be placed in the gaps inside?
        // Wait, if we place i outside, then the interval [l, r] still contains only 0..i-1,
        // so mex of that interval is i. For any subinterval, the condition must hold.
        // The known solution: ans = product over i from 1 to n-1 of (number of choices for placing i)
        // where choices = (r - l + 1 - i) if pos[i] is inside [l, r]? Actually, let's recall.
        // Standard problem: count permutations with same MEX intervals.
        // Solution: maintain l, r. For i from 1 to n-1:
        // if pos[i] < l: l = pos[i], no multiplication? Actually, we multiply by the number of ways
        // to place the numbers that will eventually fill the gap? Let's derive.
        // We process numbers in increasing order. At step i, we have placed 0..i-1.
        // Their min position is L, max is R. The number i must be placed somewhere.
        // If we place i outside [L, R], then the MEX of [L, R] becomes i, which matches.
        // If we place i inside [L, R], then the MEX of [L, R] would still be > i? Actually, if we place i inside,
        // then the set of numbers in [L, R] would include i, so the MEX would be > i, but the original permutation
        // has MEX = i for that interval. So i cannot be inside [L, R] if the original had MEX = i for that interval.
        // But wait, the original permutation might have i inside [L, R]? Let's check: In the original, the interval
        // [L, R] is the minimal interval containing 0..i-1. Its MEX is the smallest missing number.
        // Since it contains 0..i-1, the MEX is at least i. If i is inside [L, R], then MEX > i.
        // If i is outside, MEX = i. So the original permutation's pos[i] determines whether MEX = i or > i.
        // For the new permutation to have the same MEX for all intervals, the relative ordering of positions
        // of numbers must preserve the property that for each i, the minimal interval containing 0..i-1
        // has the same MEX. This means that the set of numbers inside that interval must be exactly the same.
        // So the new permutation must have the same L and R for each i? Actually, the minimal interval containing
        // 0..i-1 is determined by the positions of 0..i-1. If we change positions, L and R might change.
        // But the condition requires that for every interval [l, r], MEX is the same. This is a stronger condition.
        // It is known that two permutations have the same MEX for all intervals iff they have the same "MEX sequence"
        // or something. There is a known solution: we can reorder numbers that are not "fixed" by the MEX condition.
        // Let's think differently: The MEX of an interval depends only on the set of numbers in it.
        // For the MEX to be the same for all intervals, the permutations must be "MEX-equivalent".
        // I recall a Codeforces problem: "MEX similar permutations". The solution:
        // We maintain the current interval [L, R] of placed numbers (0..i-1).
        // For the next number i, if its position in the original array is inside [L, R], then we must place it
        // somewhere inside the current [L, R] but not necessarily the same position? Actually, if it's inside,
        // then the MEX of [L, R] in the original is > i, so in the new permutation, the MEX of the new [L, R]
        // (which might be different if we place numbers differently) must also be > i. But wait, the interval
        // [L, R] itself might change if we place numbers differently. The condition is for all intervals,
        // not just the minimal one. So it's more complex.
        // Let's search memory: There is a problem "MEX of all intervals" or "similar permutations".
        // I think the solution is: For each i from 1 to n-1, let L = min(pos[0..i-1]), R = max(pos[0..i-1]).
        // If pos[i] is inside [L, R], then we have (R - L + 1 - i) choices for where to place i (any free position inside).
        // If pos[i] is outside, then we must place i exactly at the same relative outside position? Actually,
        // if pos[i] < L, then we expand L to pos[i], and we don't multiply by anything? But then we have to place
        // the numbers that will go into the gap later. The standard solution multiplies by the number of available
        // positions inside the current interval when we encounter a number that is inside.
        // Let's test with sample 1: a = [4,0,3,2,1], n=5.
        // pos: 0->1, 1->4, 2->3, 3->2, 4->0.
        // i=0: L=1, R=1.
        // i=1: pos[1]=4 > R -> R=4. No multiplication? But sample output is 2.
        // i=2: pos[2]=3 inside [1,4] -> free slots = (4-1+1) - 2 = 4 - 2 = 2? Actually, numbers placed: 0,1 (i=2 means we are placing 2? Wait, i is the number we are placing. At step i, we have placed 0..i-1. So for i=1, we place 1. pos[1]=4 > R=1 -> R=4. The interval becomes [1,4]. The number of elements placed is 2 (0 and 1). The length is 4. Free slots = 4 - 2 = 2. But we don't multiply? Let's see: In sample, answer is 2. If we multiply by free slots when we encounter an inside number, then for i=2, pos[2]=3 inside, free slots = (4-1+1) - 2 = 2? Wait, placed numbers: 0,1. So count = 2. Length = 4. Free = 2. Multiply by 2 -> ans=2. Then i=3: pos[3]=2 inside [1,4], placed numbers: 0,1,2 -> count=3. Length=4. Free=1. Multiply by 1 -> ans=2. i=4: pos[4]=0 < L -> L=0. No multiplication. Total ans=2. Matches!
        // Sample 4: a = [1,2,4,0,5,3], n=6.
        // pos: 0->3, 1->0, 2->1, 3->5, 4->2, 5->4.
        // i=0: L=3, R=3.
        // i=1: pos[1]=0 < L -> L=0. No mult.
        // i=2: pos[2]=1 inside [0,3]? L=0, R=3. Placed: 0,1 (count=2). Length=4. Free=2. Multiply by 2 -> ans=2.
        // i=3: pos[3]=5 > R -> R=5. No mult.
        // i=4: pos[4]=2 inside [0,5]. Placed: 0,1,2,3 (count=4). Length=6. Free=2. Multiply by 2 -> ans=4.
        // i=5: pos[5]=4 inside [0,5]. Placed: 0..4 (count=5). Length=6. Free=1. Multiply by 1 -> ans=4. Matches sample output 4!
        // Sample 5: n=8, a = [1,3,7,2,5,0,6,4]
        // pos: 0->5, 1->0, 2->3, 3->1, 4->7, 5->4, 6->6, 7->2.
        // i=0: L=5, R=5.
        // i=1: pos[1]=0 < L -> L=0. No mult.
        // i=2: pos[2]=3 inside [0,5]. Placed: 0,1 (count=2). Length=6. Free=4. Multiply by 4 -> ans=4.
        // i=3: pos[3]=1 inside [0,5]. Placed: 0,1,2 (count=3). Length=6. Free=3. Multiply by 3 -> ans=12.
        // i=4: pos[4]=7 > R -> R=7. No mult.
        // i=5: pos[5]=4 inside [0,7]. Placed: 0..4 (count=5). Length=8. Free=3. Multiply by 3 -> ans=36.
        // i=6: pos[6]=6 inside [0,7]. Placed: 0..5 (count=6). Length=8. Free=2. Multiply by 2 -> ans=72.
        // i=7: pos[7]=2 inside [0,7]. Placed: 0..6 (count=7). Length=8. Free=1. Multiply by 1 -> ans=72. Matches sample output 72!
        // So the algorithm is correct.
    }
    
    // Re-implement cleanly:
    long long ans = 1;
    int L = pos[0], R = pos[0];
    for (int i = 1; i < n; ++i) {
        int p = pos[i];
        if (p < L) {
            L = p;
        } else if (p > R) {
            R = p;
        } else {
            // inside
            int free = (R - L + 1) - i;
            ans = (ans * free) % MOD;
        }
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
