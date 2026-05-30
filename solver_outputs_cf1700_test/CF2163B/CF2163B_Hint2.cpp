// Hint2
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> p(n);
    for (int i = 0; i < n; ++i) cin >> p[i];
    string x;
    cin >> x;

    int pos1 = -1, posn = -1;
    for (int i = 0; i < n; ++i) {
        if (p[i] == 1) pos1 = i;
        if (p[i] == n) posn = i;
    }

    // Check impossible condition from Hint 1
    if (x[0] == '1' && x[n-1] == '1' && x[pos1] == '1' && x[posn] == '1') {
        cout << "-1\n";
        return;
    }

    vector<pair<int,int>> ops;

    // We'll use at most 5 operations to set required 1s.
    // Strategy: cover all required positions using intervals that set them to 1.
    // We can always do it if the impossible condition is not met.

    // Collect indices where x[i] == '1'
    vector<int> req;
    for (int i = 0; i < n; ++i) {
        if (x[i] == '1') req.push_back(i);
    }

    if (req.empty()) {
        cout << "0\n";
        return;
    }

    // We'll try to cover all required positions with operations.
    // An operation (l, r) sets s_i=1 for i in (l, r) if min(p_l,p_r) < p_i < max(p_l,p_r).
    // We can use endpoints that are extremes to cover ranges.

    // Simple approach: use operations that cover contiguous segments of required 1s.
    // We can always cover a required position i by choosing l and r such that
    // p_i is between p_l and p_r, and l < i < r.
    // We can use l = 0 or r = n-1 or positions of 1 and n.

    // We'll construct operations greedily.
    // First, if x[0] == '1', we need to set it. But operation cannot set endpoints.
    // So x[0] can only be 1 if it's already 1? No, s starts all 0. So x[0] must be 0 unless impossible?
    // Wait: operation sets s_i for l < i < r. Endpoints are never set.
    // So if x[0] == '1', it's impossible to set s[0] = 1. Similarly for x[n-1] == '1'.
    // But the problem says "if x_i = 1 then s_i = 1". So if x[0] == '1', we must have s[0] = 1.
    // Since we cannot set endpoints, it's impossible if x[0] == '1' or x[n-1] == '1'.
    // However, the sample has x = "010" with n=3, x[0]='0', x[2]='0'. So that's fine.
    // Check: is it always impossible if x[0]=='1' or x[n-1]=='1'? Yes, because operations never change s[0] or s[n-1].
    // But wait: the impossible condition in Hint 1 includes x_1 and x_n (1-indexed). So indeed if x[0]=='1' or x[n-1]=='1', it's impossible.
    // Let's add that check.

    if (x[0] == '1' || x[n-1] == '1') {
        // But sample 3 has x = "001100", x[0]='0', x[5]='0'. OK.
        // Actually, is it always impossible? Yes, because s[0] and s[n-1] start 0 and never change.
        cout << "-1\n";
        return;
    }

    // Now we know endpoints are 0. We need to set internal positions.
    // We can use operations that cover all required positions.
    // Since we have at most 5 operations, we can just cover each contiguous block of 1s with one operation?
    // But an operation might set extra 1s, which is allowed (x_i=0 can have s_i=1).
    // We need to ensure we can set all required 1s.

    // Let's think: we can always set a position i to 1 by choosing l and r such that
    // p_l and p_r are the min and max of some range that includes p_i.
    // For example, choose l = 0, r = n-1. Then for any i with 1 < p_i < n, s_i becomes 1.
    // That sets all internal positions except those with p_i = 1 or p_i = n.
    // So if we do (0, n-1), we set all internal positions except pos1 and posn.
    // Then if x[pos1] == '1' or x[posn] == '1', we need additional operations to set those.
    // But we can set pos1 by choosing l and r such that p_l < 1 < p_r? No, 1 is min, so cannot be between.
    // Actually, to set s_i=1, we need min(p_l,p_r) < p_i < max(p_l,p_r).
    // For p_i = 1, there is no value less than 1, so it can never be set.
    // Similarly, p_i = n can never be set because no value > n.
    // Therefore, if x[pos1] == '1' or x[posn] == '1', it's impossible!
    // That matches Hint 1: if x_1, x_n, x_{pos1}, x_{posn} are all 1, it's impossible.
    // But actually, even if only one of them is 1, it's impossible because we can't set it.
    // Wait, Hint 1 says if all four are 1, it's impossible. It doesn't say if any of them is 1 it's impossible.
    // Let's re-read: "If x_1 = x_n = x_{pos_1} = x_{pos_n} = 1, it is impossible."
    // That suggests if some are 0, it might be possible? But we just argued that endpoints and 1/n values cannot be set.
    // However, x_1 and x_n are endpoints, so they can't be set. So if x_1=1 or x_n=1, it's impossible.
    // For pos1 and posn, they are internal positions (unless 1 or n is at endpoints, but then they are endpoints).
    // If 1 is at an internal position, can we set s at that position? We need min < 1 < max, impossible because 1 is minimum.
    // So indeed, if x[pos1] == '1', it's impossible. Similarly for x[posn] == '1'.
    // So why does Hint 1 only say if all four are 1? Maybe because if some are 0, we don't need to set them, so it's fine.
    // But if x[pos1] == '1', we must set it, which is impossible. So it should be impossible.
    // Let's check sample 2: p = [3,4,2,1,5], x = "11111". pos1=3 (0-indexed), posn=4. x[0]='1', x[4]='1', x[3]='1', x[4]='1' (posn=4 is endpoint). All four are 1. Output -1.
    // Sample 4: p = [6,2,3,4,5,1], x = "110110". pos1=5, posn=0. x[0]='1' (posn), x[5]='1' (pos1), x[1]='1', x[4]='1'? Actually x = "110110": indices 0:1,1:1,2:0,3:1,4:1,5:0. So x[0]=1 (posn), x[5]=0 (pos1). So not all four are 1. Output -1 anyway.
    // So indeed, if any of the four is 1, it might be impossible? But sample 4 output is -1. Let's see: x[0]=1 (endpoint) -> impossible. So yes.
    // So the condition should be: if x[0]=='1' or x[n-1]=='1' or x[pos1]=='1' or x[posn]=='1', it's impossible.
    // But wait, what if pos1 == 0? Then x[0] is both endpoint and pos1. Still impossible if '1'.
    // So we can just check: if any of these four positions has x == '1', output -1.
    // Let's test with sample 1: p=[1,2,3], x="010". pos1=0, posn=2. x[0]='0', x[2]='0', x[pos1]='0', x[posn]='0'. OK.
    // Sample 3: p=[1,3,2,4,6,5], x="001100". pos1=0, posn=4. x[0]='0', x[5]='0', x[0]='0', x[4]='0'. OK.
    // Sample 5: p=[2,1,4,3,5], x="00000". pos1=1, posn=4. x[1]='0', x[4]='0'. OK.
    // Sample 6: p=[2,5,3,1,4], x="00100". pos1=3, posn=1. x[3]='0', x[1]='0'. x[0]='0', x[4]='0'. OK.
    // So the condition is: if x[0]=='1' or x[n-1]=='1' or x[pos1]=='1' or x[posn]=='1', impossible.
    // But Hint 1 says "If x_1 = x_n = x_{pos_1} = x_{pos_n} = 1, it is impossible." That is a subset of our condition. Our condition is stronger. Let's trust our logic: we cannot set those positions, so if they are required, impossible.

    if (x[0] == '1' || x[n-1] == '1' || x[pos1] == '1' || x[posn] == '1') {
        cout << "-1\n";
        return;
    }

    // Now we know all required positions are internal and not 1 or n.
    // We can set all internal positions except 1 and n with one operation (0, n-1).
    // But we might not want to set extra 1s? It's allowed.
    // So we can just do one operation (1, n) in 1-indexed? That sets all i with 1 < p_i < n.
    // That covers all required positions because none of them are 1 or n.
    // But wait: what if there are required positions that are 1 or n? We already excluded them.
    // So one operation (1, n) (1-indexed) should suffice.
    // However, sample 3 uses two operations: (1,5) and (2,6). Why not just (1,6)?
    // p = [1,3,2,4,6,5], x = "001100". Required positions: 2 and 3 (0-indexed). p[2]=2, p[3]=4.
    // (1,6) would set all i with 1 < p_i < 6. That includes 2,3,4,5. So s becomes 011110? Actually p[1]=3, p[2]=2, p[3]=4, p[4]=6 (n), p[5]=5. So (1,6) sets indices 1,2,3,5? Wait, check: l=0, r=5. min(p0,p5)=min(1,5)=1, max=5. So p_i between 1 and 5: p1=3, p2=2, p3=4, p5=5? 5 is not <5, so not set. So s = 0 1 1 1 0 0? That's 011100. That matches required 001100? Required are indices 2,3 (0-indexed) which are set. So one operation works.
    // But sample output uses two operations. It says "If there are multiple correct solutions, output any of them." So one operation (1,6) would be accepted? Probably yes.
    // Let's check if there's any case where one operation (1,n) doesn't work. What if required positions include pos1 or posn? We already excluded. So it should always work.
    // But wait: what if n is small? n>=3. (1,n) is valid.
    // However, we must output at most 5 operations. One is fine.
    // But is it always true that (1,n) sets all internal positions except 1 and n? Yes, because min(p1,pn) = 1, max = n, so all p_i with 1 < p_i < n are set. That includes all positions except those with value 1 or n.
    // So if x has no 1 at endpoints or at pos1 or posn, then (1,n) works.
    // But wait: what if pos1 or posn are internal but x[pos1]='0' and x[posn]='0', we don't need to set them. So (1,n) sets them anyway, which is allowed.
    // So answer is just 1 operation: 1 n.
    // Let's test sample 6: p=[2,5,3,1,4], x="00100". Required: index 2 (p=3). (1,5): min=2, max=4? Wait p0=2, p4=4, min=2, max=4. So p_i between 2 and 4: p1=5 (no), p2=3 (yes), p3=1 (no). So s2 becomes 1. That works. But sample output is "1\n2 4". That is l=2, r=4 (1-indexed). p1=5? Wait 1-indexed: p2=5, p4=1. min=1, max=5. Then all internal between 2 and 4: index 3 has p3=3, set. So also works.
    // So our simple solution of (1,n) might not always work if p1 and pn are not 1 and n? Wait, in sample 6, p1=2, pn=4. min=2, max=4. Then p_i between 2 and 4: only 3. That sets index 2. But what if required position has value 5? It wouldn't be set because 5 is not between 2 and 4. But we already excluded posn (value n) from required. So if required position has value 5, that's posn, which we excluded. So any required position has value not 1 and not n. But could it be that p_i is not between p1 and pn? Yes, if p1 and pn are not 1 and n. For example, p = [2, 3, 1, 5, 4], n=5. p1=2, p5=4. min=2, max=4. Values between: 3. So only value 3 is set. Value 5 is not set, but value 5 is n, which we don't need to set. Value 1 is not set, which we don't need. So all required positions (which are not 1 or n) must have values between 2 and 4? Not necessarily: what if required position has value 6? n=5, so max value is 5. So all values except 1 and n are between 1 and n. But they might not be between p1 and pn if p1 and pn are not 1 and n. For example, p = [4, 1, 3, 2, 5], n=5. p1=4, p5=5. min=4, max=5. Values between: none. So (1,5) sets nothing. But required positions might be index with value 3. That is not set. So (1,n) doesn't always work.
    // So we need a more robust method.

    // We need to set all required positions. Each required position i has value v = p[i]. We need to find l < i < r such that min(p_l,p_r) < v < max(p_l,p_r).
    // We can use the positions of 1 and n as endpoints because they provide the widest range.
    // If we use l = pos1, r = posn (or vice versa), then min=1, max=n, so all internal positions between them with values between 1 and n are set. That sets everything except endpoints and 1,n themselves. But if pos1 and posn are not the ends, the range between them might not cover all indices. However, we can do multiple operations.

    // Hint 2 says: "The problem is always solvable for any binary string x such that x_1 = x_n = x_{pos_1} = x_{pos_n} = 0, within 5 operations."
    // So we can always do it in 5 ops. Let's design a constructive algorithm.

    // We have four special positions: 0, n-1, pos1, posn. They all have x=0.
    // We can use these as endpoints to cover required positions.
    // Consider the permutation. The values 1 and n are at pos1 and posn.
    // Any other value v is between 1 and n. So if we choose l and r such that one endpoint is pos1 or posn, and the other is some position, we can cover v if v is between the endpoint values.
    // For example, if we choose l = pos1, r = i, then min=1, max=p_i. This sets all positions between l and r with values between 1 and p_i.
    // Similarly, l = i, r = posn sets values between p_i and n.
    // We can cover all required positions by a few such intervals.

    // Since we have at most 5 operations, we can just cover the array in segments.
    // A known solution from editorial: 
    // If x[0]=x[n-1]=x[pos1]=x[posn]=0, we can do:
    // 1. (pos1, posn) if pos1 < posn else (posn, pos1) -> sets everything between them except 1 and n.
    // 2. If there are required positions outside [pos1, posn], we can cover them with additional operations using endpoints 0 or n-1.
    // Actually, we can always cover everything with at most 4 operations:
    // - (0, pos1) if pos1 > 0, sets some
    // - (pos1, n-1) if pos1 < n-1
    // - (0, posn) if posn > 0
    // - (posn, n-1) if posn < n-1
    // But we need to ensure we don't set pos1 or posn if they are not required? It's fine to set them because x there is 0, so setting them to 1 is allowed.
    // Wait, we cannot set pos1 or posn because they are value 1 and n, which cannot be between min and max. So they never get set. So it's safe.
    // Let's think systematically.

    // We can always set all required positions using at most 4 operations:
    // Let a = pos1, b = posn. Assume a < b (swap if needed).
    // The array is divided into: [0, a-1], [a], [a+1, b-1], [b], [b+1, n-1].
    // We can set the middle segment (a+1, b-1) with one operation (a, b) (using 1-indexed? careful).
    // For the left segment [0, a-1], we can use (0, a) if a > 0. This sets positions between 0 and a with values between p_0 and 1? min(p_0,1)=1, max=p_0. So values between 1 and p_0. That might not cover all required positions in that segment if they have values > p_0. But we can also use (0, b) or something.
    // Actually, we can use the fact that we have 5 operations. We can just do:
    // 1. (1, n) in 1-indexed? That's (0, n-1). This sets all internal positions with values between p_0 and p_{n-1}. Not guaranteed to cover all.
    // Better: use the four operations:
    // (0, pos1), (pos1, n-1), (0, posn), (posn, n-1). But some may be invalid if endpoints coincide.
    // Let's test on sample 6: p=[2,5,3,1,4], n=5. pos1=3, posn=1. a=1, b=3 (0-indexed). So a=1, b=3.
    // Operations: (0,1): l=0,r=1. min(p0,p1)=min(2,5)=2, max=5. No internal positions.
    // (1,4): l=1,r=4. min(5,4)=4, max=5. Internal: indices 2,3. p[2]=3 (between 4 and 5? No, 3<4). So not set. p[3]=1 (no). So sets nothing.
    // (0,3): l=0,r=3. min(2,1)=1, max=2. Internal: 1,2. p[1]=5 (no), p[2]=3 (no). Nothing.
    // (3,4): l=3,r=4. min(1,4)=1, max=4. Internal: none.
    // So that doesn't work.

    // Let's look at sample 3: p=[1,3,2,4,6,5], n=6. pos1=0, posn=4. a=0, b=4.
    // Required: indices 2,3 (0-indexed). They are in (a,b). Operation (0,4): l=0,r=4. min(1,6)=1, max=6. Internal: 1,2,3. p[1]=3, p[2]=2, p[3]=4. All between 1 and 6, so all set. That works.
    // Sample 6: required index 2. It is in (a,b) = (1,3). Operation (1,3): l=1,r=3. min(5,1)=1, max=5. Internal: index 2. p[2]=3, between 1 and 5, set! So (1,3) works. But sample output used (2,4) which is (1,3) in 0-indexed? Wait, sample output: "2 4" in 1-indexed means l=2, r=4 -> 0-indexed: l=1, r=3. Yes! So they did exactly (posn, pos1) since posn=1, pos1=3. So (1,3) works.
    // So the operation (min(pos1,posn), max(pos1,posn)) sets all internal positions between them with values between 1 and n, i.e., all values except 1 and n. So it sets every internal position in that range except possibly if 1 or n are inside? But 1 and n are at the endpoints of this interval, so they are not internal. So it sets all positions strictly between pos1 and posn.
    // Therefore, if all required positions lie strictly between pos1 and posn, one operation suffices.
    // What if some required positions are outside? Then we need additional operations.

    // Suppose a = pos1, b = posn, a < b.
    // The required positions can be in [0, a-1] or [b+1, n-1].
    // We can set the left part using an operation with l=0, r=a (if a>0). This sets positions between 0 and a with values between p_0 and 1. Since p_0 is some value >1 (because 1 is at a), min=1, max=p_0. So it sets all positions in (0,a) with values < p_0. But what if a required position in (0,a) has value > p_0? Then it won't be set. However, we can also use l=0, r=b. That sets values between p_0 and n. That might cover larger values.
    // Actually, we can cover the left segment by using the fact that we can choose endpoints to create any range.
    // A systematic way: we can always set any required position i by choosing l and r as the nearest positions to the left and right that have values outside the range of p_i? Not exactly.

    // Let's think differently. We have 5 operations. We can just do:
    // 1. (0, pos1)
    // 2. (pos1, n-1)
    // 3. (0, posn)
    // 4. (posn, n-1)
    // 5. (pos1, posn) (or (posn, pos1))
    // Will this set all possible positions that can be set? We need to check if any required position remains unset.
    // A position i is set if there exists an operation (l,r) with l < i < r and min(p_l,p_r) < p_i < max(p_l,p_r).
    // With these 5 operations, the endpoints used are 0, n-1, pos1, posn.
    // For any i, if p_i is not 1 or n, can we guarantee it's covered? Not necessarily, because the min/max might not bracket p_i.
    // For example, p = [5, 3, 1, 4, 2], n=5. pos1=2, posn=0. a=0, b=2. Required: index 4 (value 2). It's outside [a,b]. Operations:
    // (0,2): l=0,r=2. min(5,1)=1, max=5. Internal: index 1. p[1]=3, set.
    // (2,4): l=2,r=4. min(1,2)=1, max=2. Internal: index 3. p[3]=4 (no).
    // (0,0) invalid.
    // (0,4): l=0,r=4. min(5,2)=2, max=5. Internal: 1,2,3. p[1]=3 (yes), p[2]=1 (no), p[3]=4 (yes). So index 3 set.
    // (0,0) invalid.
    // (0,2) already.
    // So index 4 is not set because it's an endpoint in all operations? Actually index 4 is never internal because it's n-1. But we already excluded endpoints. So required positions are internal. In this example, index 4 is n-1, so x[4] must be 0. So no problem.
    // What about an internal position outside [a,b]? Say index 1 in this example (value 3). It is set by (0,2) and (0,4). So it's covered.
    // It seems that using these operations, any internal position (not 0, n-1, pos1, posn) will be covered by at least one? Let's test a tricky case: p = [3, 1, 4, 2, 5], n=5. pos1=1, posn=4. a=1, b=4. Required: index 2 (value 4) is between a and b? Actually a=1, b=4, so index 2 is between. (1,4): l=1,r=4. min(1,5)=1, max=5. Internal: 2,3. p[2]=4, p[3]=2. Both set. So fine.
    // What if required is index 0? excluded. Index 3? between. So all internal are between a and b? Not necessarily: if a > b, then the interval [b,a] covers some, and the outside is [0,b-1] and [a+1,n-1]. But since a and b are pos1 and posn, one of them might be at 0 or n-1. The only internal positions not between min(a,b) and max(a,b) are those outside. Can they be covered? Let's test: p = [2, 4, 1, 5, 3], n=5. pos1=2, posn=3. a=2, b=3. Required: index 1 (value 4). It's outside [2,3]. Operations:
    // (0,2): l=0,r=2. min(2,1)=1, max=2. Internal: index 1. p[1]=4 (no).
    // (2,4): l=2,r=4. min(1,3)=1, max=3. Internal: index 3. p[3]=5 (no).
    // (0,3): l=0,r=3. min(2,5)=2, max=5. Internal: 1,2. p[1]=4 (yes), p[2]=1 (no). So index 1 is set!
    // So (0, posn) set it.
    // What about index 4? n-1, excluded.
    // So it seems the set of operations: (0, pos1), (pos1, n-1), (0, posn), (posn, n-1), and maybe (pos1, posn) covers all internal positions except possibly some? Let's try to find a counterexample.
    // We need an internal position i (not 0, n-1, pos1, posn) such that for all these operations, either i is not internal or the value condition fails.
    // Consider operation (0, pos1). i is internal if 0 < i < pos1. Value condition: min(p_0, 1) < p_i < max(p_0, 1). Since p_pos1 = 1, min is 1, max is p_0. So condition: 1 < p_i < p_0. So it sets positions in (0, pos1) with values less than p_0.
    // Operation (pos1, n-1): internal if pos1 < i < n-1. Condition: min(1, p_{n-1}) < p_i < max(1, p_{n-1}) => 1 < p_i < p_{n-1} (if p_{n-1} > 1). So sets positions in (pos1, n-1) with values less than p_{n-1}.
    // Operation (0, posn): internal if 0 < i < posn. Condition: min(p_0, n) < p_i < max(p_0, n) => p_0 < p_i < n (since p_0 < n typically). So sets positions in (0, posn) with values greater than p_0.
    // Operation (posn, n-1): internal if posn < i < n-1. Condition: min(n, p_{n-1}) < p_i < max(n, p_{n-1}) => p_{n-1} < p_i < n. So sets positions in (posn, n-1) with values greater than p_{n-1}.
    // Operation (pos1, posn) (assuming pos1 < posn): internal if pos1 < i < posn. Condition: 1 < p_i < n. Sets all values except 1 and n in that interval.
    // So overall, for any internal position i (not endpoints, not pos1, posn), we have:
    // - If i is between pos1 and posn, it's covered by (pos1, posn) because p_i is not 1 or n.
    // - If i < pos1 and i < posn: then i is in (0, pos1) and also in (0, posn). It will be covered if either p_i < p_0 (from first op) or p_i > p_0 (from third op). Since p_i is not 1 or n, it's either < p_0 or > p_0. So it's covered! (Unless p_i == p_0, but permutation has distinct values, so p_i != p_0). So always covered.
    // - If i > pos1 and i > posn: symmetric, covered by (pos1, n-1) or (posn, n-1).
    // - If pos1 < i < posn? Already covered.
    // - If posn < i < pos1? Then i is between them, covered by (posn, pos1) similarly.
    // So indeed, the four operations (0, pos1), (pos1, n-1), (0, posn), (posn, n-1) cover all internal positions except possibly those that are exactly at the endpoints? But we only need to cover required positions, which are not 0, n-1, pos1, posn. So all required positions are covered by at least one of these four operations!
    // Wait, check the case where pos1 or posn is at 0 or n-1. Then some operations might be invalid (l < r required, but if pos1=0, (0, pos1) is invalid). But we can just skip invalid operations. The remaining will still cover everything? Let's test: if pos1=0, then (0, pos1) is invalid. The other three: (pos1, n-1) = (0, n-1) covers all internal positions with values < p_{n-1}. (0, posn) = (0, posn) covers values > p_0. (posn, n-1) covers values > p_{n-1}. Do these cover all internal positions? For i not 0, n-1, posn, we need to cover it. If i < posn, it's in (0, posn). It's covered if p_i > p_0. What if p_i < p_0? Then it's not covered by (0, posn). Is it covered by (0, n-1)? (0, n-1) covers i if p_i < p_{n-1}. Could it be that p_i < p_0 and p_i > p_{n-1}? That would require p_{n-1} < p_i < p_0. Is that possible? Yes. For example, p = [1, 3, 2, 5, 4], n=5. pos1=0, posn=3. p_0=1, p_{n-1}=4. Required: index 1 (value 3). i=1 < posn=3. p_i=3. p_0=1, so p_i > p_0, covered by (0, posn). What about index 2 (value 2)? p_i=2 > p_0, covered. What if p = [1, 4, 2, 5, 3], pos1=0, posn=3. p_0=1, p_{n-1}=3. Required: index 1 (value 4). p_i=4 > p_0, covered by (0, posn). Index 2 (value 2): p_i=2 > p_0, covered. So seems okay. But can we have p_i < p_0? p_0 is 1, so no value is less than 1. So if pos1=0, p_0=1, then all other values are >1, so (0, posn) covers
