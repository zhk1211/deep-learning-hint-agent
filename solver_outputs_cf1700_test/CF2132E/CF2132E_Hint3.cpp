// Hint3
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve() {
    int n, m, q;
    cin >> n >> m >> q;
    vector<ll> a(n), b(m);
    for (int i = 0; i < n; ++i) cin >> a[i];
    for (int i = 0; i < m; ++i) cin >> b[i];
    sort(a.rbegin(), a.rend());
    sort(b.rbegin(), b.rend());
    vector<ll> prefA(n + 1, 0), prefB(m + 1, 0);
    for (int i = 0; i < n; ++i) prefA[i + 1] = prefA[i] + a[i];
    for (int i = 0; i < m; ++i) prefB[i + 1] = prefB[i] + b[i];

    // best[k] = maximum sum using exactly k cards without restrictions
    // We can precompute for all k up to n+m, but n+m can be up to 4e5, sum over test cases 2e5 each, so total 4e5 per test case, but sum of n+m over test cases <= 4e5, so it's fine.
    int total = n + m;
    vector<ll> best(total + 1, 0);
    // We'll fill best by considering all possible splits between A and B.
    // Since arrays are sorted descending, for a fixed k, the optimal is to take some i from A and k-i from B.
    // The function f(i) = prefA[i] + prefB[k-i] is concave? Actually prefA and prefB are concave (differences decreasing).
    // We can use two pointers or ternary search, but n+m is small enough that we can just iterate over possible i from max(0, k-m) to min(n, k).
    // However total sum of n+m over test cases is 4e5, and total sum of (n+m)^2 could be large if we do O((n+m)^2) per test case.
    // But we can compute best[k] for all k in O(n+m) using the fact that the optimal i is monotonic in k? Let's think.
    // Actually we don't need best for all k, we only need to answer queries. But queries ask for exactly z cards with limits x,y.
    // We can answer each query in O(log n) or O(1) by precomputing something.
    // Let's precompute best[k] for all k from 0 to total. Since total sum of total over test cases <= 4e5, we can afford O(total) per test case if we compute best efficiently.
    // We can compute best using a greedy approach: we have two sorted lists, we want to pick k elements with max sum. This is equivalent to merging the two sorted arrays and taking top k. But we need to respect the fact that we can't take more than n from A or m from B. But if we just take top k from the merged list, it automatically respects the counts because we can't take more than available. So best[k] is simply the sum of the largest k elements from the combined multiset of a and b.
    // Wait, is that true? If we take top k from combined, we might take more than n from A? But A only has n elements, so we can't take more than n. The combined multiset has exactly n+m elements. Taking top k means we pick the k largest among all. Since there are only n elements from A, we can't pick more than n from A. So it's automatically satisfied. Similarly for B. So best[k] is just the sum of the k largest elements overall.
    // So we can just merge a and b, sort descending, and compute prefix sums. That's O((n+m) log (n+m)) per test case. Sum of n+m <= 4e5, so total O(4e5 log 4e5) is fine.
    vector<ll> all;
    all.reserve(total);
    for (ll x : a) all.push_back(x);
    for (ll x : b) all.push_back(x);
    sort(all.rbegin(), all.rend());
    vector<ll> prefAll(total + 1, 0);
    for (int i = 0; i < total; ++i) prefAll[i + 1] = prefAll[i] + all[i];
    // best[k] = prefAll[k]

    // Now for each query (x, y, z), we need max sum of exactly z cards with at most x from A and at most y from B.
    // Let i be number taken from A, j = z - i from B. Constraints: 0 <= i <= x, 0 <= j <= y, i <= n, j <= m, i+j = z.
    // So i in [max(0, z - y), min(x, z, n)] and also i <= n, j <= m automatically if i <= n and z-i <= m.
    // We want max over i of prefA[i] + prefB[z-i].
    // Since prefA and prefB are concave (differences decreasing), the function g(i) = prefA[i] + prefB[z-i] is concave in i (sum of concave functions). So we can find the maximum by ternary search or by looking at the point where the marginal gain of taking one more from A vs one more from B changes sign.
    // The unconstrained optimal i (ignoring x,y) is the one that maximizes g(i) over [max(0, z-m), min(n, z)].
    // Let L0 = max(0, z - m), R0 = min(n, z). The unconstrained optimal i* is the one where a[i] >= b[z-i] and a[i+1] <= b[z-i-1] (if we think of 1-indexed). Actually we want the largest i such that a[i] >= b[z-i+1]? Let's derive.
    // Consider moving from i to i+1: we gain a[i+1] and lose b[z-i]. So we want to increase i while a[i+1] >= b[z-i].
    // So the optimal i is the number of elements from A that are >= the corresponding elements from B in the merged top z.
    // This is exactly the number of A elements in the top z of the combined sorted list.
    // Let cntA = number of A elements among the top z overall. Then the unconstrained optimal i is cntA.
    // Because if we take top z from combined, we get some number from A and some from B. That split maximizes the sum for exactly z elements without any limits. And since the function is concave, the maximum over the continuous interval is at that point.
    // So i* = number of A elements in the top z of the combined sorted array.
    // We can precompute for each z the optimal i* (unconstrained). But we need to handle limits x and y.
    // With limits, the feasible interval is [L, R] where L = max(0, z - y) and R = min(x, z, n). Also we must have L <= R, otherwise impossible? But problem guarantees z <= x+y, so L <= R always.
    // Since g(i) is concave, the maximum over [L, R] is attained at the point closest to i* within the interval. So answer = g(clamp(i*, L, R)).
    // So we just need to compute i* for each z quickly.
    // How to compute i* for a given z? i* = number of A elements in top z of combined sorted array.
    // We can precompute an array posA of size total+1? Or we can binary search on the value threshold.
    // Since we have the combined sorted array all, we can also precompute for each prefix of length z, how many A elements are there. But we don't know which elements are A in the combined array unless we store their origin.
    // We can merge a and b while keeping track of origin, sort, and then compute prefix counts of A.
    // Let merged be vector of pairs (value, isA). Sort descending. Then prefCntA[z] = number of A in first z.
    // Then i* = prefCntA[z].
    // Then for query, L = max(0, z - y), R = min({x, z, n}). i_opt = clamp(prefCntA[z], L, R).
    // Answer = prefA[i_opt] + prefB[z - i_opt].
    // This is O(1) per query after O((n+m) log (n+m)) preprocessing.
    // Total time: sum of (n+m) log (n+m) <= 4e5 log 4e5, fine.

    vector<pair<ll, int>> merged; // value, 0 for A, 1 for B
    merged.reserve(total);
    for (ll x : a) merged.emplace_back(x, 0);
    for (ll x : b) merged.emplace_back(x, 1);
    sort(merged.begin(), merged.end(), [](const pair<ll,int>& p1, const pair<ll,int>& p2) {
        return p1.first > p2.first;
    });
    vector<int> cntA(total + 1, 0);
    for (int i = 0; i < total; ++i) {
        cntA[i+1] = cntA[i] + (merged[i].second == 0 ? 1 : 0);
    }

    while (q--) {
        int x, y, z;
        cin >> x >> y >> z;
        int L = max(0, z - y);
        int R = min({x, z, n});
        int i_star = cntA[z];
        int i_opt = max(L, min(R, i_star));
        ll ans = prefA[i_opt] + prefB[z - i_opt];
        cout << ans << '\n';
    }
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
