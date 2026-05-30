// Hint1
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<long long> s(n);
        for (int i = 0; i < n; ++i) {
            cin >> s[i];
        }

        vector<long long> extra(n + 1, 0);
        long long ans = 0;

        for (int i = 0; i < n; ++i) {
            // carry over extra passes from previous trampolines
            if (i > 0) {
                extra[i] += extra[i - 1];
            }

            long long cur = s[i] - 1; // needed passes to reduce to 1
            long long have = extra[i];

            if (have < cur) {
                // we need to start new passes here
                long long need = cur - have;
                ans += need;
                have += need;
            }

            // extra passes beyond what's needed to reduce to 1
            long long pass_on = have - cur;
            // distribute extra passes to subsequent trampolines
            if (i + 1 < n) {
                extra[i + 1] += pass_on;
                // remove the effect from the range that these passes would have covered
                // each pass from i jumps to i + s[i], but s[i] decreases
                // Actually, we simulate the effect of passes starting at i.
                // For each pass, it will affect positions i+2 .. i+s[i] (since s[i] decreases)
                // But we already accounted for the needed passes to reduce s[i] to 1.
                // The extra passes (pass_on) will also jump from i, but s[i] is already 1,
                // so they jump to i+1. So they just move to i+1.
                // Wait, careful: The process of reducing s[i] to 1 involves passes that jump
                // to i + s[i], i + s[i] - 1, ..., i + 2. The extra passes (beyond reducing to 1)
                // will have s[i] = 1, so they jump to i+1.
                // So extra passes just go to i+1.
                // But we also need to account for the effect of the needed passes on later trampolines.
                // For each needed pass that started at i (or earlier and landed here),
                // when s[i] was >1, it launched to i + s[i] (original s[i] decreasing).
                // We can simulate the effect by adding 1 to extra for positions i+2 .. i+s[i].
                // However, doing this naively is O(n^2). We can use a difference array.
                // But we already have extra array. Let's rethink.
            }
        }

        // The above logic is incomplete. Let's use a different approach.
        // We'll simulate the process greedily from left to right.
        // For each position i, we maintain how many passes are currently "active" that will land on i.
        // We can use a difference array to add passes to ranges.
        // When we are at i, we know how many passes land here (cnt).
        // If cnt < s[i] - 1, we need to start (s[i] - 1 - cnt) new passes at i.
        // Each new pass from i will affect positions i+2 to i+s[i] (since s[i] decreases by 1 each time,
        // but the first jump uses original s[i], then s[i]-1, etc. Actually, if we start a pass at i,
        // it will jump to i + s[i], then from there with s[i]-1, etc. But we only care about how many
        // times each later position gets visited. A pass starting at i will visit positions:
        // i, then i+s[i], then i+s[i]+(s[i]-1), ... until out of bounds.
        // This is complex. Instead, we can simulate the process by just doing the jumps naively,
        // but using the fact that we only need to reduce to 1. We can use a greedy approach:
        // For i from 1 to n, we need to reduce s[i] to 1. The extra passes that come from left
        // will help. We can maintain an array `cnt` of how many passes are currently at each position.
        // When we process i, we have cnt[i] passes landing here. If cnt[i] < s[i] - 1, we need to
        // start new passes. Each new pass will jump to i + s[i] (with current s[i]), but s[i] decreases.
        // Actually, if we start a pass at i, it will use the current s[i] value, then s[i] decreases.
        // But we are processing left to right, and we might have already modified s[i] due to previous passes?
        // No, s[i] is only modified when a pass lands on it. The passes from left land on i and reduce s[i].
        // So when we are at i, s[i] might have been reduced by previous passes. But we don't track that.
        // Alternative: We can simulate the process by maintaining an array `b` where b[i] is the number
        // of passes that will land on i from previous jumps. We process i from 1 to n.
        // At step i, we have b[i] passes landing here. The current strength is s[i] (original).
        // But wait, the passes that landed here before we start new passes might have reduced s[i].
        // Actually, the order matters: all passes that land on i (from left) will reduce s[i] by 1 each time,
        // down to max(1, ...). So if b[i] >= s[i] - 1, then s[i] becomes 1, and the extra passes
        // (b[i] - (s[i] - 1)) will just jump to i+1 (since s[i]=1). If b[i] < s[i] - 1, we need to
        // start new passes at i. Each new pass will use the current s[i] (which is original s[i] minus
        // the number of passes that already landed here? No, the passes that landed here already reduced
        // s[i]. So the current s[i] is original s[i] - min(b[i], original s[i] - 1). But we don't need
        // to track the exact current s[i] for the new passes? Actually, if we start a new pass at i,
        // it will jump to i + current_s[i]. But current_s[i] depends on how many passes have already
        // landed here. This is getting messy.

        // Let's use the standard solution: We can simulate the process by maintaining an array `cnt`
        // of how many times each trampoline is visited. We start with cnt[i] = 0. We iterate i from 1 to n.
        // If cnt[i] < s[i] - 1, we need to add (s[i] - 1 - cnt[i]) passes starting at i.
        // Each pass starting at i will jump to i + s[i], then i + s[i] + (s[i]-1), etc.
        // But we can just simulate the jumps for each new pass? That would be O(n^2) worst case.
        // However, we can use a difference array to add the effect of these new passes efficiently.
        // For a pass starting at i, it will visit a sequence of positions. But we only care about
        // how many times each position is visited. We can add 1 to cnt for each position in the sequence.
        // The sequence is: i, i+s[i], i+s[i]+(s[i]-1), i+s[i]+(s[i]-1)+(s[i]-2), ...
        // This is an arithmetic progression of decreasing step sizes. It's not a simple range.
        // But note that s[i] can be up to 1e9, but n is only 5000. So many jumps will go out of bounds.
        // We can just simulate the jumps for each new pass, but only up to n. Since each pass will
        // visit at most n positions (actually, the number of jumps is at most n because each jump
        // increases the index by at least 1). So simulating each pass naively is O(n) per pass.
        // Total passes could be up to sum(s[i]) which is huge. So we cannot simulate each pass individually.

        // We need a more efficient way. Observe that we only need to reduce all to 1. The process
        // is equivalent to: we can start passes at any positions. Each pass starting at i will
        // decrease s[i] by 1 (if >1) and then move to i+s[i], etc. We want to minimize the number
        // of passes started. This is a known problem: we can process from left to right, and for each i,
        // we need to ensure that the number of times we land on i is at least s[i]-1. The passes
        // that land on i come from previous positions. We can maintain an array `extra` that tells
        // how many passes will land on each position from previous jumps. When we are at i, we have
        // extra[i] passes landing here. If extra[i] < s[i] - 1, we need to start new passes at i.
        // Each new pass at i will jump to i + s[i] (with the current s[i] value). But wait, the
        // current s[i] is original s[i] minus the number of passes that have already landed here?
        // Actually, the passes that land here (extra[i]) will reduce s[i] by extra[i] (down to 1).
        // So the current s[i] when we start a new pass is original s[i] - extra[i] (but not less than 1).
        // However, if we start a new pass, it will use the current s[i] at the moment of the jump.
        // But we are starting multiple new passes. The first new pass will use current s[i] = original s[i] - extra[i].
        // After that, s[i] decreases by 1, so the next new pass will use s[i]-1, etc.
        // So the sequence of jump targets for the new passes is: 
        // i + (original s[i] - extra[i]), i + (original s[i] - extra[i] - 1), ..., i + 2.
        // (The last new pass will have s[i]=2, so it jumps to i+2; after that s[i]=1 and no more new passes needed.)
        // So the new passes will add 1 to the count of positions: i + (original s[i] - extra[i]), i + (original s[i] - extra[i] - 1), ..., i + 2.
        // This is a contiguous range from i+2 to i + (original s[i] - extra[i]).
        // We can add 1 to this entire range using a difference array.
        // Then, after processing i, we also need to consider the extra passes that were already landing here
        // beyond what was needed to reduce s[i] to 1. Those extra passes will have s[i]=1, so they jump to i+1.
        // So we add (extra[i] - (original s[i] - 1)) to position i+1, but only if extra[i] > original s[i] - 1.
        // Actually, extra[i] is the total passes landing here. We need original s[i] - 1 passes to reduce to 1.
        // If extra[i] >= original s[i] - 1, then the excess (extra[i] - (original s[i] - 1)) passes will
        // jump to i+1. If extra[i] < original s[i] - 1, we start new passes, and after starting them,
        // the total passes landing here becomes original s[i] - 1 (since we add exactly the deficit).
        // So there is no excess in that case.
        // So the algorithm:
        // ans = 0
        // extra array of size n+2 initialized to 0
        // for i = 0 to n-1:
        //   extra[i] += extra[i-1] (if i>0)  // but we can just accumulate
        //   if extra[i] < s[i] - 1:
        //       deficit = s[i] - 1 - extra[i]
        //       ans += deficit
        //       // these deficit passes start at i, and they will jump to i+2 .. i + (s[i] - extra[i])
        //       // because current s[i] = original s[i] - extra[i]
        //       // the first jump target: i + (s[i] - extra[i])
        //       // the last jump target: i + 2
        //       // so range is [i+2, i + s[i] - extra[i]]
        //       // we add 1 to this range in extra
        //       // we can use a difference array: diff[i+2] += 1, diff[i + s[i] - extra[i] + 1] -= 1
        //       // but we need to apply this for each deficit? Actually, we are adding deficit passes,
        //       // each adds 1 to the range. So we add deficit to the range.
        //       // So diff[i+2] += deficit, diff[i + s[i] - extra[i] + 1] -= deficit
        //       // Then extra[i] becomes s[i] - 1 (we don't really need to update extra[i] because we are done with i)
        //   else:
        //       // extra[i] >= s[i] - 1
        //       // excess = extra[i] - (s[i] - 1)
        //       // these excess passes jump to i+1
        //       // so we add excess to extra[i+1]
        //       // but we also need to consider the effect of the passes that reduced s[i] to 1.
        //       // The first s[i]-1 passes that landed here (out of extra[i]) would have jumped to
        //       // various positions depending on the original s[i] and the order.
        //       // Actually, the passes that land here come from previous jumps. They are not necessarily
        //       // starting at i. They are just landing here. When they land, they reduce s[i] and then
        //       // jump to i + current_s[i]. The current_s[i] decreases with each landing.
        //       // So the first landing uses s[i], jumps to i+s[i]; the second uses s[i]-1, jumps to i+s[i]-1;
        //       // ... the (s[i]-1)-th landing uses 2, jumps to i+2.
        //       // So these landings add 1 to positions i+s[i], i+s[i]-1, ..., i+2.
        //       // That is exactly the range [i+2, i+s[i]].
        //       // So regardless of whether we start new passes or not, the first (s[i]-1) landings
        //       // always add 1 to the range [i+2, i+s[i]].
        //       // The excess landings (beyond s[i]-1) jump to i+1.
        //       // So we can unify:
        //       // Let landings = extra[i] (total passes landing here).
        //       // Let needed = s[i] - 1.
        //       // If landings < needed, we start new passes: ans += needed - landings, and landings = needed.
        //       // Now we have exactly 'needed' landings that will reduce s[i] to 1, and possibly some excess.
        //       // The first 'needed' landings add 1 to range [i+2, i+s[i]].
        //       // The excess (landings - needed) add to i+1.
        //       // So we can just do:
        //       // if landings < needed: ans += needed - landings; landings = needed;
        //       // add 1 to range [i+2, i+s[i]] for 'needed' times? Wait, the range addition is for each of the 'needed' landings.
        //       // But each of the 'needed' landings adds 1 to a specific position in that range.
        //       // The total effect is that each position in [i+2, i+s[i]] gets exactly 1 from these 'needed' landings.
        //       // So we can just add 1 to the entire range [i+2, i+s[i]].
        //       // Then, if landings > needed, add (landings - needed) to i+1.
        //       // This is much simpler!
        //       // Let's verify: Suppose s[i]=4, and we have 2 landings. needed=3. landings < needed.
        //       // We start 1 new pass. Then total landings = 3. The three landings will jump to i+4, i+3, i+2.
        //       // So range [i+2, i+4] gets +1 each. Correct.
        //       // Suppose we have 5 landings. needed=3. landings > needed.
        //       // The first 3 landings jump to i+4, i+3, i+2. The excess 2 landings jump to i+1 (twice).
        //       // So range [i+2, i+4] gets +1, and i+1 gets +2. Correct.
        //       // So the algorithm is:
        //       // for i = 1..n:
        //       //   landings = extra[i]
        //       //   needed = s[i] - 1
        //       //   if landings < needed:
        //       //       ans += needed - landings
        //       //       landings = needed
        //       //   // now landings >= needed
        //       //   // add 1 to range [i+2, i+s[i]]  (if i+2 <= i+s[i])
        //       //   // add (landings - needed) to i+1
        //       //   // we can use a difference array for range additions.
        //       //   diff[i+2] += 1
        //       //   diff[i+s[i]+1] -= 1
        //       //   extra[i+1] += landings - needed
        //       // But wait: extra[i] is the number of landings on i. We need to maintain extra as we go.
        //       // We can just keep a variable 'carry' that we add to extra[i] from previous range additions.
        //       // Actually, we can maintain an array 'cnt' that is the number of landings at each position.
        //       // We can process left to right, and maintain a running sum of the difference array.
        //       // Let's do:
        //       // vector<long long> cnt(n+2, 0);
        //       // long long ans = 0;
        //       // long long cur = 0; // running sum of diff
        //       // for i = 0..n-1:
        //       //   cur += diff[i]  // diff is the difference array for range additions
        //       //   cnt[i] = cur + extra_landings_from_excess? 
        //       // Actually, the range additions and the excess additions are both part of the landings.
        //       // We can just use a single array 'cnt' that we update directly.
        //       // But range additions are efficient with difference array.
        //       // Let's maintain an array 'add' that is the difference array for the range additions.
        //       // And we also have direct additions to i+1 from excess.
        //       // We can just keep an array 'cnt' and for each i, we do:
        //       // cnt[i] += cnt[i-1] (if we use prefix sums) but we need to add the range additions.
        //       // Better: maintain a variable 'cur' that is the current number of landings from range additions.
        //       // And we have an array 'excess' that we add directly.
        //       // Actually, the excess just adds to i+1, which is a single position. We can just add it to a variable
        //       // that we will include in cnt[i+1].
        //       // Let's define an array 'cnt' of size n+2, initialized to 0.
        //       // We'll also have a difference array 'd' for the range [i+2, i+s[i]] additions.
        //       // We'll process i from 0 to n-1.
        //       // At step i, we first apply d[i] to a running sum 'range_add'.
        //       // Then cnt[i] += range_add.
        //       // Then we have landings = cnt[i].
        //       // needed = s[i] - 1.
        //       // if landings < needed: ans += needed - landings; landings = needed.
        //       // Now, we need to add 1 to range [i+2, i+s[i]].
        //       // So we do: d[i+2] += 1; if i+s[i]+1 <= n: d[i+s[i]+1] -= 1;
        //       // And we add (landings - needed) to cnt[i+1].
        //       // Then move to next i.
        //       // This works because cnt[i] already includes all landings from previous steps.
        //       // Let's test with sample 1: n=7, s=[1,4,2,2,2,2,2]
        //       // Initialize cnt[0..8]=0, d[0..8]=0, range_add=0.
        //       // i=0: s[0]=1. range_add += d[0]=0. cnt[0]=0. landings=0. needed=0.
        //       //   landings >= needed. range [2, 1]? i+2=2, i+s[i]=1 -> empty. excess=0. cnt[1]+=0.
        //       // i=1: s[1]=4. range_add += d[1]=0. cnt[1]=0. landings=0. needed=3.
        //       //   landings < needed: ans += 3, landings=3.
        //       //   range [3, 5]: d[3]+=1, d[6]-=1.
        //       //   excess = 0. cnt[2]+=0.
        //       // i=2: s[2]=2. range_add += d[2]=0. cnt[2]=0. landings=0. needed=1.
        //       //   landings < needed: ans += 1, landings=1. (ans=4)
        //       //   range [4, 4]: d[4]+=1, d[5]-=1.
        //       //   excess=0. cnt[3]+=0.
        //       // i=3: s[3]=2. range_add += d[3]=1. cnt[3]=1. landings=1. needed=1.
        //       //   landings >= needed. range [5, 5]: d[5]+=1, d[6]-=1.
        //       //   excess=0. cnt[4]+=0.
        //       // i=4: s[4]=2. range_add += d[4]=1. cnt[4]=1. landings=1. needed=1.
        //       //   landings >= needed. range [6, 6]: d[6]+=1, d[7]-=1.
        //       //   excess=0. cnt[5]+=0.
        //       // i=5: s[5]=2. range_add += d[5] = (d[5] was -1 from i=1, +1 from i=3? Wait, let's track d carefully.
        //       //   d initially all 0.
        //       //   i=1: d[3]+=1, d[6]-=1.
        //       //   i=2: d[4]+=1, d[5]-=1.
        //       //   i=3: d[5]+=1, d[6]-=1.
        //       //   i=4: d[6]+=1, d[7]-=1.
        //       //   Now i=5: range_add += d[5]. d[5] = -1 + 1 = 0. So range_add remains? Let's compute range_add step by step.
        //       //   range_add starts 0.
        //       //   i=0: +d[0]=0 -> 0
        //       //   i=1: +d[1]=0 -> 0
        //       //   i=2: +d[2]=0 -> 0
        //       //   i=3: +d[3]=1 -> 1
        //       //   i=4: +d[4]=1 -> 2
        //       //   i=5: +d[5]=0 -> 2
        //       //   So cnt[5] = 2? But we also have cnt[5] from previous direct additions? cnt[5] was 0 initially, we never added directly to cnt[5] except through range_add? Actually, we set cnt[i] = range_add before processing? In my logic, I said cnt[i] += range_add. But cnt[i] might already have direct additions from excess of previous steps. So we need to maintain cnt[i] as the total landings. Let's do: at start of loop for i, we do range_add += d[i]; then cnt[i] += range_add; then landings = cnt[i]; then process.
        //       //   But we also need to add direct excess to cnt[i+1]. So we should have an array cnt that we update directly.
        //       //   Let's re-initialize and simulate properly.
        //       // We'll use vector<long long> cnt(n+2, 0), d(n+2, 0).
        //       // long long range_add = 0;
        //       // for i=0..n-1:
        //       //   range_add += d[i];
        //       //   cnt[i] += range_add;
        //       //   long long landings = cnt[i];
        //       //   long long needed = s[i] - 1;
        //       //   if (landings < needed) {
        //       //       ans += needed - landings;
        //       //       landings = needed;
        //       //   }
        //       //   // range addition
        //       //   if (i + 2 <= i + s[i]) { // i+2 <= i+s[i] => s[i] >= 2
        //       //       int L = i + 2;
        //       //       int R = i + s[i];
        //       //       if (L < n) {
        //       //           d[L] += 1;
        //       //           if (R + 1 < n) d[R + 1] -= 1;
        //       //           else if (R + 1 == n) // do nothing, out of bounds
        //       //       }
        //       //   }
        //       //   // excess
        //       //   long long excess = landings - needed;
        //       //   if (i + 1 < n) cnt[i + 1] += excess;
        //       // 
        //       // Let's test this on sample 1.
        //       // n=7, s=[1,4,2,2,2,2,2]
        //       // cnt = [0,0,0,0,0,0,0,0,0], d same.
        //       // i=0: range_add += d[0]=0. cnt[0]=0. landings=0, needed=0. landings>=needed.
        //       //   s[0]=1, so i+2=2, i+s[0]=1 -> L>R, no range addition.
        //       //   excess=0. cnt[1]+=0.
        //       // i=1: range_add += d[1]=0. cnt[1]=0. landings=0, needed=3. landings<needed -> ans+=3, landings=3.
        //       //   s[1]=4, L=3, R=5. d[3]+=1, d[6]-=1.
        //       //   excess=0. cnt[2]+=0.
        //       // i=2: range_add += d[2]=0. cnt[2]=0. landings=0, needed=1. ans+=1 (ans=4), landings=1.
        //       //   s[2]=2, L=4, R=4. d[4]+=1, d[5]-=1.
        //       //   excess=0. cnt[3]+=0.
        //       // i=3: range_add += d[3]=1. cnt[3]=1. landings=1, needed=1. landings>=needed.
        //       //   s[3]=2, L=5, R=5. d[5]+=1, d[6]-=1.
        //       //   excess=0. cnt[4]+=0.
        //       // i=4: range_add += d[4]=1. cnt[4]=1. landings=1, needed=1.
        //       //   s[4]=2, L=6, R=6. d[6]+=1, d[7]-=1.
        //       //   excess=0. cnt[5]+=0.
        //       // i=5: range_add += d[5]. d[5] was -1 from i=2? Wait, i=2: d[5]-=1. i=3: d[5]+=1. So d[5]=0. range_add = previous range_add (was 2) + 0 = 2.
        //       //   cnt[5] = 2. landings=2, needed=1. landings>=needed.
        //       //   s[5]=2, L=7, R=7. L=7 >= n? n=7, indices 0..6. L=7 is out of bounds. So we should only add if L < n. So no range addition.
        //       //   excess = 2-1=1. cnt[6] += 1.
        //       // i=6: range_add += d[6]. d[6] was -1 (i=1) then -1 (i=3) then +1 (i=4) = -1. range_add = 2 + (-1) = 1.
        //       //   cnt[6] = 1 + (excess from previous? cnt[6] was 0, then we added 1 from i=5, so cnt[6]=1 before range_add? Wait, we do cnt[i] += range_add. So cnt[6] initially 1, then +1 = 2. landings=2, needed=1 (s[6]=2).
        //       //   landings>=needed. L=8, R=8 out of bounds. excess=1. cnt[7] += 1 (out of bounds, ignore).
        //       // ans=4. Correct.
        //       // Sample 2: n=2, s=[2,3]
        //       // i=0: s[0]=2. cnt[0]=0. needed=1. ans+=1, landings=1. L=2, R=2. L=2 >= n? n=2, indices 0,1. L=2 out of bounds. excess=0. cnt[1]+=0.
        //       // i=1: range_add=0. cnt[1]=0. s[1]=3, needed=2. ans+=2 (total 3). landings=2. L=3, R=4 out of bounds. excess=0.
        //       // ans=3. Correct.
        //       // Sample 3: all 1s -> ans=0. Correct.
        //       // This seems to work!

        // Let's implement this.
        vector<long long> cnt(n + 2, 0), d(n + 2, 0);
        long long ans = 0;
        long long range_add = 0;
        for (int i = 0; i < n; ++i) {
            range_add += d[i];
            cnt[i] += range_add;
            long long landings = cnt[i];
            long long needed = s[i] - 1;
            if (landings < needed) {
                ans += needed - landings;
                landings = needed;
            }
            // range addition for the needed landings
            if (s[i] >= 2) {
                int L = i + 2;
                int R = i + s[i];
                if (L < n) {
                    d[L] += 1;
                    if (R + 1 < n) {
                        d[R + 1] -= 1;
                    }
                    // if R+1 >= n, we don't need to subtract because it's out of bounds
                }
            }
            // excess
            long long excess = landings - needed;
            if (excess > 0 && i + 1 < n) {
                cnt[i + 1] += excess;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
