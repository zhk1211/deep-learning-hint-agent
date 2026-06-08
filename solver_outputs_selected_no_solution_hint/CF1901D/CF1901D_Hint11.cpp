#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    if (n == 1) {
        cout << a[0] << '\n';
        return 0;
    }

    // We need to find min x such that for any possible order of hits,
    // all monsters die. The worst-case order for a given start i is
    // that the monster that needs the most damage gets hit as late as possible.
    // The damage sequence is x, x-1, ..., x-(n-1).
    // For a fixed start i, the worst-case assignment of damages to monsters
    // is: the monster at position i gets x, then the rest get decreasing damages
    // in some order. The worst-case is that the monster with the highest
    // requirement a[j] + (distance from i in terms of hit order?) 
    // Actually, the hit order is a permutation that forms a path starting at i
    // and covering all vertices, moving only to adjacent unvisited vertices.
    // This is equivalent to a depth-first traversal of the line graph.
    // The last monster hit will be one of the ends of the line (1 or n) or
    // some other depending on the path. The key observation from hints:
    // The worst-case for a given start i is that the monster that requires
    // the largest damage gets hit as late as possible.
    // The damage a monster at position j receives is x - t_j, where t_j is
    // the time step (0-indexed) it is hit. We need x >= a_j + t_j for all j.
    // So x >= max_j (a_j + t_j). We want to choose start i to minimize
    // the worst-case max over all valid hit sequences starting at i.
    // The worst-case hit sequence for a given i maximizes max_j (a_j + t_j).
    // It turns out the worst-case sequence is: start at i, then go to one end,
    // then go to the other end. The last monster hit will be one of the ends
    // or the farthest from i in the direction opposite to the first move.
    // More precisely, for a fixed start i, the adversary can choose the order
    // to maximize the required x. The maximum over all valid orders of
    // max_j (a_j + t_j) is equal to:
    // max( a[i] + 0,
    //      max_{j < i} (a[j] + (i - j)),
    //      max_{j > i} (a[j] + (j - i)),
    //      a[1] + (i - 1) + (n - i) ? Wait.
    // Actually, the worst-case time for a monster at position j is:
    // if we go left first: for j <= i, t_j = i - j; for j > i, t_j = (i - 1) + (j - 1) = i + j - 2? No.
    // Let's derive carefully.
    // The path is a walk that starts at i, visits all vertices exactly once,
    // moving only to adjacent unvisited vertices. This is exactly a path that
    // goes from i to one end, then back to the other end.
    // There are two possible orders (up to reversal of the second part):
    // Order A: i -> i-1 -> ... -> 1 -> i+1 -> ... -> n
    // Order B: i -> i+1 -> ... -> n -> i-1 -> ... -> 1
    // The adversary will choose the order that gives larger max(a_j + t_j).
    // So for a given i, the worst-case x needed is:
    // max( max_{j<=i} (a[j] + (i - j)),   // from going left first
    //      max_{j>=i} (a[j] + (j - i)),   // from going right first
    //      // but also the monsters on the other side get hit later
    //      // In Order A, for j > i: t_j = (i - 1) + (j - i) = j - 1? Wait:
    //      // start at i (t=0). Then go left: i-1 (t=1), i-2 (t=2), ..., 1 (t=i-1).
    //      // Then go right from i+1: i+1 (t=i), i+2 (t=i+1), ..., n (t = i + (n - i - 1) = n - 1).
    //      // So for j > i: t_j = i + (j - i - 1) = j - 1? No: i + j - i - 1 = j - 1.
    //      // Actually: t_{i+1} = i, t_{i+2} = i+1, ..., t_n = i + (n - i - 1) = n - 1.
    //      // So t_j = j - 1? Check: j = i+1 -> t = i = (i+1)-1 = i. Yes.
    //      // So in Order A, for j > i, t_j = j - 1.
    //      // In Order B, for j < i: t_j = (n - i) + (i - j) = n - j.
    // So the worst-case x for start i is:
    // x_i = max( a[i],
    //            max_{j < i} (a[j] + (i - j)),   // if we go left first, these get hit at time i-j
    //            max_{j > i} (a[j] + (j - i)),   // if we go right first, these get hit at time j-i
    //            max_{j < i} (a[j] + (n - j)),   // if we go right first, left side gets hit later: t_j = n - j
    //            max_{j > i} (a[j] + (j - 1))    // if we go left first, right side gets hit later: t_j = j - 1
    // )
    // But wait, the adversary chooses the order, so they will pick the order that maximizes the max over all j.
    // So x_i = max( max_{j} (a[j] + worst_time(j, i)) ) where worst_time is the maximum over the two orders.
    // For a fixed j, worst_time(j, i) = max( time in Order A, time in Order B ).
    // Let's compute worst_time(j, i):
    // If j <= i:
    //   Order A: t = i - j
    //   Order B: t = (n - i) + (i - j) = n - j
    //   worst = max(i - j, n - j) = n - j (since n - j >= i - j because n >= i)
    // If j >= i:
    //   Order A: t = j - 1
    //   Order B: t = j - i
    //   worst = max(j - 1, j - i) = j - 1 (since j - 1 >= j - i because i >= 1)
    // So worst_time(j, i) = 
    //   if j <= i: n - j
    //   if j >= i: j - 1
    // But wait, is that always achievable? The adversary can choose the order that gives this time for each j independently? No, the adversary chooses one order for the whole sequence. The worst-case x is the maximum over the two orders of the max over j. So x_i = max( max_j (a[j] + t_j^A), max_j (a[j] + t_j^B) ).
    // That is exactly: x_i = max( 
    //   max( a[i], max_{j<i} (a[j] + i - j), max_{j>i} (a[j] + j - 1) ),   // Order A
    //   max( a[i], max_{j<i} (a[j] + n - j), max_{j>i} (a[j] + j - i) )    // Order B
    // )
    // So x_i = max(
    //   a[i],
    //   max_{j<i} (a[j] + i - j),
    //   max_{j>i} (a[j] + j - 1),
    //   max_{j<i} (a[j] + n - j),
    //   max_{j>i} (a[j] + j - i)
    // )
    // We can precompute prefix and suffix maximums to compute this in O(1) per i.
    // Let L1[i] = max_{j <= i} (a[j] - j) -> then max_{j < i} (a[j] + i - j) = i + max_{j < i} (a[j] - j)
    // Let L2[i] = max_{j <= i} (a[j] + n - j) -> then max_{j < i} (a[j] + n - j) = max_{j < i} (a[j] + n - j)
    // Let R1[i] = max_{j >= i} (a[j] + j) -> then max_{j > i} (a[j] + j - 1) = max_{j > i} (a[j] + j) - 1
    // Let R2[i] = max_{j >= i} (a[j] - j) -> then max_{j > i} (a[j] + j - i) = max_{j > i} (a[j] - j) + j? Wait: a[j] + j - i = (a[j] - j) + 2j - i? No.
    // Actually: a[j] + j - i = (a[j] + j) - i. So max_{j > i} (a[j] + j - i) = max_{j > i} (a[j] + j) - i.
    // So we need:
    // pref_max_minus[j] = max_{k <= j} (a[k] - k)
    // pref_max_plus_n_minus[j] = max_{k <= j} (a[k] + n - k)
    // suff_max_plus[j] = max_{k >= j} (a[k] + k)
    // suff_max_minus[j] = max_{k >= j} (a[k] - k)  // maybe not needed
    // Then for each i:
    // val1 = a[i]
    // val2 = (i > 1) ? i + pref_max_minus[i-1] : -1e18
    // val3 = (i < n) ? suff_max_plus[i+1] - 1 : -1e18
    // val4 = (i > 1) ? pref_max_plus_n_minus[i-1] : -1e18
    // val5 = (i < n) ? suff_max_plus[i+1] - i : -1e18
    // x_i = max({val1, val2, val3, val4, val5})
    // Answer = min_i x_i

    vector<long long> pref_minus(n + 1, -1e18);
    vector<long long> pref_plus_n_minus(n + 1, -1e18);
    for (int i = 1; i <= n; ++i) {
        pref_minus[i] = max(pref_minus[i-1], a[i-1] - i);
        pref_plus_n_minus[i] = max(pref_plus_n_minus[i-1], a[i-1] + n - i);
    }

    vector<long long> suff_plus(n + 2, -1e18);
    for (int i = n; i >= 1; --i) {
        suff_plus[i] = max(suff_plus[i+1], a[i-1] + i);
    }

    long long ans = 1e18;
    for (int i = 1; i <= n; ++i) {
        long long cur = a[i-1];
        if (i > 1) {
            cur = max(cur, i + pref_minus[i-1]);
            cur = max(cur, pref_plus_n_minus[i-1]);
        }
        if (i < n) {
            cur = max(cur, suff_plus[i+1] - 1);
            cur = max(cur, suff_plus[i+1] - i);
        }
        ans = min(ans, cur);
    }

    cout << ans << '\n';
    return 0;
}
