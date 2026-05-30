// Hint4
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
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        // f[i] = rating after processing first i elements (0-indexed, i from 0 to n)
        vector<int> f(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            f[i + 1] = f[i];
            if (a[i] > f[i + 1]) f[i + 1]++;
            else if (a[i] < f[i + 1]) f[i + 1]--;
        }

        // g[i] = rating before processing suffix starting at i (0-indexed, i from 0 to n)
        // g[n] = 0, g[i] is rating after processing a[i..n-1] if we start from 0
        vector<int> g(n + 1, 0);
        for (int i = n - 1; i >= 0; --i) {
            g[i] = g[i + 1];
            if (a[i] > g[i]) g[i]++;
            else if (a[i] < g[i]) g[i]--;
        }

        // We need to find max possible rating after skipping some interval [l, r] (1-indexed)
        // Let l0 = l-1, r0 = r (0-indexed). We skip a[l0..r0-1].
        // Final rating = rating after processing prefix of length l0, then suffix starting at r0.
        // But the suffix processing starts from the rating we had after prefix.
        // Let x = f[l0]. We need to compute result of applying suffix a[r0..n-1] starting from x.
        // We can precompute for each possible starting value? But values are up to n.
        // Observe that the process is monotonic in a certain sense.
        // Actually, we can think of it as: final rating = x + (g[r0] computed from 0) but adjusted because starting from x instead of 0.
        // Let's define h[i][v] = rating after processing suffix i..n-1 starting from v.
        // We can note that h[i][v] = v + (g[i] if we consider relative changes? Not exactly linear.
        // But we can use the fact that the function v -> h[i][v] is non-decreasing and has a simple form.
        // Actually, we can compute the maximum possible final rating by considering all possible l0, r0.
        // We need max over l0 < r0 of h[r0][f[l0]].
        // Since n <= 3e5, we need O(n) or O(n log n) per test case.

        // Let's analyze the function h[i][v].
        // For a fixed suffix starting at i, the process is: start with v, for each element a[j] (j >= i):
        // if a[j] > current, current++; if a[j] < current, current--; else unchanged.
        // This is equivalent to: current = v + (number of times we go up minus number of times we go down).
        // But the decision depends on current value.
        // However, we can observe that if we start from a very large v, we will only go down or stay; if we start from very small v, we only go up or stay.
        // There is a threshold behavior.

        // Alternative approach from hints: binary search on answer.
        // Hint 2: If answer >= k, then there exists interval [l,r] such that f[l-1] >= g[r+1] (with some relation to k?).
        // Let's interpret hint 2 carefully.
        // Actually, the hints suggest: If the answer is >= k (with skipping), then there exists an interval [l,r] such that f_{l-1} >= g_{r+1}.
        // But what are f and g here? Probably f_i is the rating after prefix i if we start from 0, and g_i is the rating after suffix i if we start from k?
        // Let's read hint 4: Try to calculate f_i and g_i.
        // Maybe f_i is the rating after prefix i starting from 0, and g_i is the rating after suffix i starting from 0.
        // Then the condition f_{l-1} >= g_{r+1} might be related to the ability to reach k.
        // Let's think: If we skip [l,r], final rating = process suffix starting from f_{l-1}.
        // We want final >= k.
        // Define g_i(k) = rating after suffix i starting from k.
        // Then we need f_{l-1} such that g_{r+1}(f_{l-1}) >= k.
        // Since g_i(v) is non-decreasing in v, we need f_{l-1} >= some threshold T(r+1, k).
        // Maybe T(r+1, k) is exactly the value such that if we start from that value, we end up at k.
        // Let's define for each suffix i, the minimum starting value needed to achieve at least k at the end.
        // Let min_start[i][k] be that value. Then we need f_{l-1} >= min_start[r+1][k].
        // But k can be up to n, so we can't precompute for all k.
        // However, we can binary search k, and for a fixed k, we can compute min_start[i] for all i in O(n).
        // Then we need to check if there exists l < r+1 such that f[l-1] >= min_start[r+1].
        // This is equivalent to: max_{l < i} f[l-1] >= min_start[i] for some i.
        // We can precompute prefix maximums of f and check in O(n).

        // Let's verify this approach.
        // For a fixed target k, we want to know if we can achieve final rating >= k.
        // We process the array from right to left to compute min_start[i]: the minimum initial rating x such that processing a[i..n-1] yields final rating >= k.
        // Base: min_start[n] = k (to end with >= k, we need to start with at least k, since no more contests).
        // For i from n-1 down to 0:
        // We have a[i]. We want to find the minimum x such that after processing a[i], the new rating y satisfies that from i+1 we can achieve >= k.
        // That is, we need y >= min_start[i+1].
        // The transition from x to y:
        // if a[i] > x: y = x + 1
        // if a[i] == x: y = x
        // if a[i] < x: y = x - 1
        // We need y >= min_start[i+1].
        // So we need to find the smallest x satisfying this.
        // Since y is a function of x, we can analyze:
        // If x < a[i]: y = x+1. Condition: x+1 >= min_start[i+1] => x >= min_start[i+1] - 1.
        // If x == a[i]: y = a[i]. Condition: a[i] >= min_start[i+1].
        // If x > a[i]: y = x-1. Condition: x-1 >= min_start[i+1] => x >= min_start[i+1] + 1.
        // We want the minimum x overall.
        // We can consider three cases for the minimum x:
        // 1. x < a[i]: the smallest x in this region satisfying x >= min_start[i+1] - 1 is max(min_start[i+1] - 1, ...). But also x < a[i].
        // So if min_start[i+1] - 1 < a[i], then candidate x = max(min_start[i+1] - 1, -infinity? Actually x can be negative? Ratings can be negative? The problem says rating starts at 0, but during process it can go negative? "If a_i < x, rating x will decrease by 1". It doesn't say it can't go negative. But a_i >= 1. So x can become negative. However, we are looking for minimum starting value, which could be negative. But f[l-1] is always >= 0? f is computed from 0, and a_i >= 1, so f stays >= 0? Actually if a_i < x and x=0, a_i >=1 so a_i > 0? Wait, if x=0, a_i >=1, so a_i > x, so x increases. So x never decreases from 0 because a_i >=1. So f[i] >= 0 always. So we only care about x >= 0. But min_start could be negative? If k is large, we might need negative start? But we only have f >= 0. So we can clamp to 0.
        // Let's just compute min_start[i] as an integer, possibly negative, but we will later check f >= min_start.
        // To find min x:
        // We can try x = a[i] - 1 (the largest x < a[i]). Then y = a[i]. Condition: a[i] >= min_start[i+1]. If true, then x = a[i]-1 works, but maybe smaller x works? If x < a[i]-1, then y = x+1 < a[i]. We need x+1 >= min_start[i+1] => x >= min_start[i+1]-1. So the smallest x < a[i] is max(min_start[i+1]-1, -inf). But we also need x < a[i]. So if min_start[i+1]-1 < a[i], then the smallest x in this region is min_start[i+1]-1 (if that is < a[i]). But wait, if min_start[i+1]-1 is very small, it's valid. So candidate1 = min_start[i+1]-1, provided min_start[i+1]-1 < a[i].
        // 2. x = a[i]: y = a[i]. Condition: a[i] >= min_start[i+1]. Candidate2 = a[i].
        // 3. x > a[i]: the smallest x in this region is max(a[i]+1, min_start[i+1]+1). Candidate3 = max(a[i]+1, min_start[i+1]+1).
        // Then min_start[i] = min of valid candidates.
        // But we must ensure the candidate actually satisfies the condition for its region.
        // Let's formalize:
        // For region x < a[i]: condition is x >= min_start[i+1] - 1. The smallest x in this region is max(min_start[i+1] - 1, -INF). But we also need x < a[i]. So if max(min_start[i+1] - 1, -INF) < a[i], then this candidate is valid and equals max(min_start[i+1] - 1, -INF). Otherwise no valid x in this region.
        // For region x = a[i]: condition a[i] >= min_start[i+1]. Candidate = a[i].
        // For region x > a[i]: condition x >= min_start[i+1] + 1. Smallest x is max(a[i]+1, min_start[i+1]+1). Always valid because x > a[i] is satisfied.
        // So we can compute min_start[i] as the minimum among valid candidates.

        // However, we need to consider that x can be negative. f is non-negative. So when checking f[l-1] >= min_start[i], if min_start[i] < 0, it's automatically true for all f >= 0. But we can just compute min_start as described.

        // Let's test this logic with an example.
        // Suppose a = [1], k=1.
        // min_start[1] = 1.
        // i=0: a[0]=1.
        // Region x<1: min_start[1]-1 = 0. 0 < 1, so candidate1 = 0. Check: x=0 -> y=1 >=1. Valid.
        // x=1: candidate2 = 1. y=1 >=1. Valid.
        // x>1: candidate3 = max(2, 2)=2. Valid.
        // min_start[0] = 0.
        // f[0] = 0 (prefix of length 0). f[0] >= min_start[0]? 0 >= 0 true. So we can achieve >=1 by skipping nothing? But we must skip at least one contest. So we need l < r+1, i.e., skip non-empty interval. So we need to check existence of l < i such that f[l-1] >= min_start[i]. Here i can be 1 (r+1 = 1 => r=0, l <= 0, but l>=1, impossible). i can be 0? r+1=0 => r=-1 impossible. So we need to consider i from 1 to n (r+1 from 1 to n). l-1 from 0 to i-2. So we need max_{0 <= j <= i-2} f[j] >= min_start[i].
        // In this case, n=1, i can only be 1. j from 0 to -1: empty. So no valid skip. So answer <1. Correct, answer 0.

        // So the check function for a given k:
        // Compute min_start array of size n+1.
        // min_start[n] = k.
        // For i = n-1 down to 0:
        //   candidates = []
        //   // region x < a[i]
        //   cand1 = min_start[i+1] - 1;
        //   if (cand1 < a[i]) candidates.push_back(cand1);
        //   // region x == a[i]
        //   if (a[i] >= min_start[i+1]) candidates.push_back(a[i]);
        //   // region x > a[i]
        //   cand3 = max(a[i] + 1, min_start[i+1] + 1);
        //   candidates.push_back(cand3);
        //   min_start[i] = *min_element(candidates.begin(), candidates.end());
        // Then we need to check if there exists i from 1 to n such that max_{0 <= j <= i-2} f[j] >= min_start[i].
        // We can precompute pref_max_f where pref_max_f[j] = max(f[0..j]).
        // For i=1, we need max over empty set -> -INF. So we can just check for i>=2: pref_max_f[i-2] >= min_start[i].
        // If any such i exists, then k is achievable.

        // But wait: Is min_start[i] monotonic in some way? Not needed.
        // Complexity: O(n) per check. Binary search k from 0 to n. O(n log n) per test case. Sum n <= 3e5, so total O(N log N) which is fine.

        // Let's test with sample 2: n=7, a = [1,2,1,1,1,3,4]. Answer 4.
        // We'll implement and test mentally? We'll trust.

        // Edge cases: k can be up to n? Actually rating can increase at most n times, so max rating <= n. So binary search 0..n.

        // Let's refine the check function.
        // We need to compute f array (prefix ratings from 0).
        // f[0] = 0.
        // for i=0..n-1: f[i+1] = f[i] + (a[i] > f[i] ? 1 : (a[i] < f[i] ? -1 : 0)).

        auto check = [&](int k) -> bool {
            if (k == 0) return true; // always possible? Actually if we skip everything, rating remains 0. But we must skip at least one contest, so if n>=1, we can skip all and get 0. So 0 is always achievable.
            vector<int> min_start(n + 1);
            min_start[n] = k;
            for (int i = n - 1; i >= 0; --i) {
                int cand = INT_MAX;
                // region x < a[i]
                int cand1 = min_start[i+1] - 1;
                if (cand1 < a[i]) cand = min(cand, cand1);
                // region x == a[i]
                if (a[i] >= min_start[i+1]) cand = min(cand, a[i]);
                // region x > a[i]
                int cand3 = max(a[i] + 1, min_start[i+1] + 1);
                cand = min(cand, cand3);
                min_start[i] = cand;
            }
            // pref_max_f[j] = max(f[0..j])
            vector<int> pref_max_f(n + 1);
            pref_max_f[0] = f[0];
            for (int i = 1; i <= n; ++i) {
                pref_max_f[i] = max(pref_max_f[i-1], f[i]);
            }
            // We need to skip at least one contest: interval [l, r] with l <= r.
            // l-1 = j, r+1 = i. We need 0 <= j < i-1 <= n-1? Actually i can be from 1 to n.
            // j from 0 to i-2.
            // So for i from 2 to n:
            for (int i = 2; i <= n; ++i) {
                if (pref_max_f[i-2] >= min_start[i]) {
                    return true;
                }
            }
            return false;
        };

        // Binary search
        int low = 0, high = n, ans = 0;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (check(mid)) {
                ans = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
