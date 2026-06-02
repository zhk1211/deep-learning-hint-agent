#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll INF = 1e18;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<ll> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    // dp[i] = minimal damage to reach trap i (0-indexed) and go through it
    // we consider we have already taken damage from trap i
    // dp[i] = a[i] + min_{j = i-k-1 ... i-1} (dp[j] + (number of jumps after j) * (i - j - 1)?)
    // Better: let dp[i] be minimal damage to finish processing first i traps, and we go through trap i.
    // If we go through trap i, and previous went through trap j (j < i), then we jumped over i-j-1 traps between them.
    // The bonus damage on trap i is (total jumps before i) = (total jumps before j) + (i-j-1).
    // But we don't know total jumps before j directly from dp[j].
    // Alternative: Let dp[i] = minimal damage to finish first i traps, and we go through trap i.
    // Let jumps[i] = number of jumps used before trap i in that optimal solution.
    // This is not directly representable.

    // Better approach: Let dp[i] = minimal damage to process first i traps, and we go through trap i.
    // The bonus damage on trap i depends on how many traps we jumped over before i.
    // If we jumped over x traps before i, damage = a[i] + x.
    // If we came from trap j (j < i), then x = (jumps before j) + (i - j - 1).
    // So damage = a[i] + (jumps before j) + (i - j - 1) = (a[i] + i - 1) + (jumps before j - j).
    // Let value[j] = dp[j] - j? Not exactly.
    // dp[i] = a[i] + (jumps before i) + dp[j]? No, dp[j] already includes damage of trap j.
    // Total damage = dp[j] + damage of trap i = dp[j] + a[i] + (jumps before j) + (i - j - 1).
    // We need to know jumps before j. Let cnt[j] = number of jumps used in optimal solution for dp[j].
    // Then dp[i] = dp[j] + a[i] + cnt[j] + (i - j - 1) = dp[j] + cnt[j] - j + a[i] + i - 1.
    // So if we maintain dp[j] + cnt[j] - j, we can query minimum over j in [i - k - 1, i - 1].
    // But cnt[j] depends on the path. This suggests we need to store both dp and cnt in DP state.

    // Alternative DP: Let dp[i][j] = minimal damage to process first i traps, using exactly j jumps, and we go through trap i.
    // Too slow.

    // Hint 4: segment tree. Maybe we can use a greedy or DP with segment tree over values (a[i] - i) or something.
    // Let's think differently: If we decide which traps to jump over, the total damage is sum of (a[i] + number of jumped traps before i) for i not jumped.
    // Let S be the set of indices we go through. Total damage = sum_{i in S} a[i] + sum_{i in S} (number of jumped traps before i).
    // The second term is sum_{i in S} ( (i - 1) - (number of went through traps before i) )? Actually number of jumped traps before i = (i - 1) - (number of went through traps before i).
    // So total damage = sum_{i in S} a[i] + sum_{i in S} (i - 1) - sum_{i in S} (number of went through traps before i).
    // The last term: if we order S = {s1 < s2 < ... < sm}, then number of went through traps before s_j is j-1.
    // So sum_{i in S} (number of went through traps before i) = sum_{j=1..m} (j-1) = m*(m-1)/2.
    // Thus total damage = sum_{i in S} (a[i] + i - 1) - m*(m-1)/2.
    // We must choose S of size m = n - (number of jumps) >= n - k.
    // We want to minimize sum_{i in S} (a[i] + i - 1) - m*(m-1)/2.
    // Since m is fixed once we choose how many jumps we use, we can try all possible m from n-k to n.
    // For a fixed m, we need to choose m indices to go through, minimizing sum of (a[i] + i - 1).
    // But wait, the term -m*(m-1)/2 is constant for fixed m. So we just need to pick m indices with smallest (a[i] + i - 1).
    // However, we must also ensure that we can actually achieve exactly m went-through traps with at most k jumps? 
    // If we pick any m indices, we can just jump over the rest. The number of jumps = n - m. We need n - m <= k, i.e., m >= n - k.
    // So for each m in [n-k, n], we can pick the m smallest values of (a[i] + i - 1). But is that always valid? 
    // The order of traps is fixed; we go through them in order. If we pick a set S of indices to go through, we just go through them in increasing order, jumping over others. That's always valid. There's no constraint that we can't jump over consecutive traps. So any subset S of size m is achievable by jumping over the complement. The number of jumps is exactly n - m. So as long as n - m <= k, it's valid.
    // So the problem reduces to: For each m from n-k to n, compute the sum of the m smallest values of (a[i] + i - 1), then subtract m*(m-1)/2, and take the minimum over m.
    // But wait, is that correct? Let's test with sample 2: n=4, k=1, a=[5,10,11,5].
    // a[i]+i-1: i=1:5+0=5; i=2:10+1=11; i=3:11+2=13; i=4:5+3=8.
    // Sorted: 5,8,11,13.
    // m can be 3 or 4 (since n-k=3).
    // m=4: sum=5+8+11+13=37, minus 4*3/2=6 => 31. (jump 0)
    // m=3: smallest 3 sum=5+8+11=24, minus 3*2/2=3 => 21. (jump 1)
    // Minimum is 21. Matches sample.
    // Sample 3: n=7, k=5, a=[8,2,5,15,11,2,8].
    // a[i]+i-1: i=1:8; i=2:2+1=3; i=3:5+2=7; i=4:15+3=18; i=5:11+4=15; i=6:2+5=7; i=7:8+6=14.
    // Sorted: 3,7,7,8,14,15,18.
    // m from n-k=2 to 7.
    // m=2: sum=3+7=10, minus 1 =9.
    // m=3: sum=3+7+7=17, minus 3=14.
    // m=4: sum=3+7+7+8=25, minus 6=19.
    // m=5: sum=3+7+7+8+14=39, minus 10=29.
    // m=6: sum=+15=54, minus 15=39.
    // m=7: sum=+18=72, minus 21=51.
    // Minimum is 9. Matches sample.
    // Sample 4: n=6, k=3, a=[1,2,3,4,5,6].
    // a[i]+i-1: 1, 3, 5, 7, 9, 11.
    // m from 3 to 6.
    // m=3: sum=1+3+5=9, minus 3=6.
    // m=4: sum=1+3+5+7=16, minus 6=10.
    // m=5: sum=+9=25, minus 10=15.
    // m=6: sum=+11=36, minus 15=21.
    // Min=6. Matches sample.
    // Sample 5: n=1, k=1, a=[7]. a[1]+0=7. m from 0 to 1? n-k=0. m=0: sum=0, minus 0=0. m=1: sum=7, minus 0=7. Min=0. Matches sample.
    // So the solution is simply: compute b[i] = a[i] + i (0-indexed: a[i] + i). Sort b. Compute prefix sums. For m from n-k to n, ans = min(ans, pref[m] - m*(m-1)/2). Note: if m=0, damage=0.
    // Wait, in my formula I had a[i] + i - 1 for 1-indexed. With 0-indexed: a[i] + i. Then total damage = sum_{i in S} (a[i] + i) - m*(m-1)/2. Let's verify with 0-indexed.
    // Sample 2 0-indexed: a=[5,10,11,5], i=0..3. b = [5,11,13,8]. Sorted: 5,8,11,13. m=3: sum=24, minus 3*2/2=3 =>21. Correct.
    // So algorithm: O(n log n) per test case, sum n <= 2e5, fine.

    vector<ll> b(n);
    for (int i = 0; i < n; ++i) {
        b[i] = a[i] + i;
    }
    sort(b.begin(), b.end());
    vector<ll> pref(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        pref[i + 1] = pref[i] + b[i];
    }
    ll ans = INF;
    for (int m = n - k; m <= n; ++m) {
        ll cur = pref[m] - (ll)m * (m - 1) / 2;
        if (cur < ans) ans = cur;
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
