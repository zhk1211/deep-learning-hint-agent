#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<ll> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    // dp[i] = minimal damage to reach trap i (0-indexed) having jumped some traps,
    // and we are about to step on trap i (i.e., we do not jump trap i).
    // We use a segment tree over dp[j] + something to query the best previous trap to come from.
    // Let dp[i] = a[i] + min_{j from max(0, i-k-1) to i-1} (dp[j] + (i - j - 1) * something? Wait.
    // Actually, if we jump over some traps between j and i, the bonus damage increases.
    // Let's define dp[i] as the minimal total damage taken up to trap i, and we step on trap i.
    // If we step on trap i, and the last trap we stepped on was j (j < i), then we jumped over (i - j - 1) traps.
    // The damage taken at trap i is a[i] + (i - j - 1) (bonus from jumps after j).
    // But wait: the bonus damage stacks globally: if we jumped over x traps before trap i, damage is a[i] + x.
    // The number of jumps before trap i is exactly the number of traps we skipped among 1..i-1.
    // If we last stepped on trap j, then we skipped (i - j - 1) traps between j and i, plus all skips before j.
    // So total skips before i = skips_before_j + (i - j - 1).
    // Let s_j = number of skips before trap j (i.e., among 1..j-1).
    // Then damage at i = a[i] + s_j + (i - j - 1) = a[i] + i - j - 1 + s_j.
    // Total damage up to i = dp[j] + a[i] + i - j - 1 + s_j.
    // But dp[j] already includes damage up to j, and s_j is the number of skips before j.
    // We need to know s_j. However, s_j = j - (number of traps stepped on up to j).
    // Let cnt_j = number of traps stepped on among 1..j. Since we step on trap j, cnt_j = cnt_{prev} + 1.
    // This is getting complicated. Let's think differently.

    // Alternative DP: Let dp[i][j] = min damage considering first i traps, having jumped exactly j times.
    // But n up to 2e5, k up to n, O(nk) too slow.

    // Hint suggests segment tree. Let's define dp[i] = minimal damage to finish processing first i traps, and we step on trap i.
    // We want to allow up to k jumps total.
    // Let f[i] = minimal damage to finish first i traps, and we step on trap i, with any number of jumps <= k.
    // Transition: f[i] = a[i] + min_{j < i, i - j - 1 <= k - jumps_before_j?} (f[j] + bonus_penalty).
    // The bonus damage on trap i depends on total jumps before i.
    // If we know the number of jumps made before j, say c_j, then jumps before i = c_j + (i - j - 1).
    // So damage at i = a[i] + c_j + i - j - 1.
    // Total damage = f[j] + a[i] + c_j + i - j - 1.
    // But f[j] already includes damage up to j, and c_j is the number of jumps before j.
    // We can store in DP state both min damage and jumps used? That's multi-dimensional.

    // Another perspective: The total damage is sum of a_i for traps we step on, plus for each trap we step on, the number of jumps before it.
    // If we step on traps with indices i_1 < i_2 < ... < i_m, then total damage = sum_{t=1..m} (a_{i_t} + (i_t - t)).
    // Because before trap i_t, we have jumped over (i_t - 1) - (t - 1) = i_t - t traps.
    // Check: total traps before i_t is i_t - 1. Among them, we stepped on t-1 traps, so jumped over (i_t - 1) - (t - 1) = i_t - t.
    // So damage at i_t = a_{i_t} + (i_t - t).
    // Total damage = sum a_{i_t} + sum (i_t - t) = sum a_{i_t} + sum i_t - sum t.
    // sum t = m(m+1)/2.
    // So total damage = sum (a_{i_t} + i_t) - m(m+1)/2.
    // We are allowed to jump over at most k traps, meaning we must step on at least n - k traps.
    // So m >= n - k.
    // We want to choose a subset of indices i_1 < ... < i_m of size m >= n - k to minimize sum (a_i + i) - m(m+1)/2.
    // Since m can be up to n, and we want to minimize, we can think of choosing exactly m traps to step on, for each possible m from n-k to n.
    // For a fixed m, we want to choose m indices to minimize sum (a_i + i). Because -m(m+1)/2 is constant for fixed m.
    // So for each m, we need the minimum sum of (a_i + i) over all increasing subsequences of length m.
    // But we can pick any subset of size m, order is fixed (increasing indices). So it's just picking m indices, sum of (a_i + i).
    // Wait, is there any constraint that we must step on them in order? Yes, we process traps in order, so we just pick a subset of traps to step on, and the order is the natural order. So it's simply choosing m indices out of n to minimize sum of (a_i + i). That's it!
    // Let's verify with sample 2: n=4, k=1, a=[5,10,11,5]. a_i + i: 5+1=6, 10+2=12, 11+3=14, 5+4=9.
    // m >= n-k = 3. So m can be 3 or 4.
    // m=4: sum = 6+12+14+9 = 41. minus 4*5/2=10 -> 31. matches "do not jump any" = 31.
    // m=3: we need to pick 3 indices with min sum of (a_i+i). The smallest three are 6,9,12 sum=27. minus 3*4/2=6 -> 21. matches sample output 21.
    // Sample 3: n=7, k=5, a=[8,2,5,15,11,2,8]. a_i+i: 9,4,8,19,16,8,15.
    // m >= 2. We can pick m=2: min two sum = 4+8=12? Actually 4 (index2) and 8 (index3 or index6). Let's pick 4 and 8 sum=12. minus 2*3/2=3 -> 9. matches sample output 9.
    // Sample 4: n=6, k=3, a=[1,2,3,4,5,6]. a_i+i: 2,4,6,8,10,12. m >= 3. m=3: min three sum = 2+4+6=12. minus 3*4/2=6 -> 6. matches sample output 6.
    // Sample 5: n=1, k=1, a=[7]. a_i+i=8. m >= 0. m=0 gives 0 damage. matches sample output 0.
    // So the problem reduces to: given array b_i = a_i + i, we can choose to skip up to k elements (i.e., we must pick at least n-k elements). We want to minimize sum of picked b_i - m(m+1)/2, where m is the number of picked elements.
    // Since m can vary, we can compute for each possible m from n-k to n the minimal sum of m elements from b. But we can just pick the smallest n-k elements? Not exactly, because m can be larger than n-k, and picking more elements might reduce the total damage due to the -m(m+1)/2 term? Let's check: total damage = sum b_i - m(m+1)/2. As m increases, sum b_i increases (we add more positive numbers), but -m(m+1)/2 decreases (more negative). So there is a trade-off. We need to find m in [n-k, n] that minimizes this.
    // Since b_i are positive (a_i >=1, i>=1 => b_i >=2), sum b_i increases with m. The function f(m) = S(m) - m(m+1)/2, where S(m) is the sum of the smallest m elements of b. We need to find min over m.
    // We can sort b, compute prefix sums, then iterate m from n-k to n and compute f(m) = pref[m] - m*(m+1)/2, take min.
    // But wait: is it always optimal to pick the smallest m elements? Yes, because for a fixed m, we want to minimize sum of picked b_i, so we pick the m smallest b_i. There's no ordering constraint other than picking a subset. So we can just sort b.
    // Let's double-check: The derivation assumed we step on traps in increasing order, and the damage formula sum (a_i + i) - m(m+1)/2 holds regardless of which traps we pick, as long as we process them in order. Since we can pick any subset, the order is just the natural order of indices. So picking the smallest b_i is valid because we can just step on those traps and skip the others. The number of skipped traps will be n - m, which must be <= k, so m >= n-k. That's the only constraint.
    // So the solution is: for each test case, compute b_i = a_i + i (1-indexed). Sort b. Compute prefix sums. Then ans = min_{m = n-k .. n} (pref[m] - m*(m+1)/2). Also consider m=0 if n-k <= 0? But k <= n, so n-k >= 0. If n-k == 0, m can be 0? The problem says "jump over no more than k traps". If we jump over all n traps, we take 0 damage. That corresponds to m=0. But is m=0 allowed? The formula gives 0 for m=0. So we should include m=0 if n-k <= 0, i.e., k >= n. But k <= n, so n-k >= 0. If k == n, then n-k = 0, so m can be 0. So we can just loop m from max(0, n-k) to n. Actually n-k can be 0, so m from 0 to n. But wait, if we pick m=0, we skip all traps, damage=0. That's valid if k >= n. So include m=0.
    // Let's test sample 1: n=4, k=4, a=[8,7,1,4]. b = [9,9,4,8] (1-indexed: 8+1=9, 7+2=9, 1+3=4, 4+4=8). Sorted b: 4,8,9,9. pref: 0,4,12,21,30. m from 0 to 4. f(0)=0, f(1)=4-1=3, f(2)=12-3=9, f(3)=21-6=15, f(4)=30-10=20. min is 0. matches.
    // Sample 2: n=4, k=1, n-k=3. m from 3 to 4. f(3)=27-6=21, f(4)=41-10=31. min=21.
    // Sample 3: n=7, k=5, n-k=2. m from 2 to 7. b sorted: 4,8,8,9,15,16,19. pref: 0,4,12,20,29,44,60,79. f(2)=12-3=9, f(3)=20-6=14, f(4)=29-10=19, f(5)=44-15=29, f(6)=60-21=39, f(7)=79-28=51. min=9.
    // Sample 4: n=6, k=3, n-k=3. m from 3 to 6. b sorted: 2,4,6,8,10,12. pref: 0,2,6,12,20,30,42. f(3)=12-6=6, f(4)=20-10=10, f(5)=30-15=15, f(6)=42-21=21. min=6.
    // Sample 5: n=1, k=1, n-k=0. m from 0 to 1. b=[8]. f(0)=0, f(1)=8-1=7. min=0.
    // All match.

    // Complexity: O(n log n) per test case due to sorting. Sum of n <= 2e5, so fine.

    vector<ll> b(n);
    for (int i = 0; i < n; ++i) {
        b[i] = a[i] + (i + 1);
    }
    sort(b.begin(), b.end());
    vector<ll> pref(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        pref[i + 1] = pref[i] + b[i];
    }
    ll ans = 0; // m=0 gives 0, but we must ensure m=0 is allowed only if k >= n.
    int min_m = max(0, n - k);
    for (int m = min_m; m <= n; ++m) {
        ll cur = pref[m] - (ll)m * (m + 1) / 2;
        if (m == min_m) ans = cur;
        else ans = min(ans, cur);
    }
    // If min_m == 0, we already considered m=0 in the loop? Wait, if min_m = 0, the loop starts at m=0. pref[0]=0, cur = 0 - 0 = 0. So ans will be 0. That's correct.
    // But if min_m > 0, we don't consider m=0, which is correct because we must step on at least n-k traps.
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
