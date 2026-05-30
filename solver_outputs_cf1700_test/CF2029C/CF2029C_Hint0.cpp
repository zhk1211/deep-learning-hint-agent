// Hint0
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    
    // Simulate the process without skipping to get base rating and prefix ratings
    vector<int> pref(n + 1, 0);
    int x = 0;
    for (int i = 0; i < n; ++i) {
        if (a[i] > x) x++;
        else if (a[i] < x) x--;
        pref[i + 1] = x;
    }
    
    // We need to find max possible rating after skipping some interval [l, r]
    // Let f(i) = rating after i contests without skipping
    // If we skip [l, r], final rating = f(l-1) + (f(n) - f(r))? Not exactly, because rating after r depends on f(l-1)
    // Actually, the process is path-dependent.
    // Let's think differently: we want to maximize final rating.
    // Consider the effect of skipping interval [l, r].
    // Let base = rating before l (after l-1 contests).
    // Then we skip l..r, so rating remains base.
    // Then we process r+1..n starting from base.
    // Let g(start, from, to) be the rating after processing contests from..to starting with rating start.
    // We want max over l<=r of g(pref[l-1], r+1, n).
    // But n up to 3e5, so O(n^2) is too slow.
    
    // Alternative: We can think of the process as: rating = number of "up" steps minus "down" steps, but only when a_i > current or < current.
    // This is equivalent to: rating after processing sequence = count of i such that a_i > current rating at that moment, minus count of i such that a_i < current rating.
    // This is like a "self-adjusting" process.
    
    // Key observation: The final rating without skipping is f(n). Skipping an interval can only increase or keep the same? Actually, it can increase.
    // We need to find max possible.
    
    // Let's define for each position i, the rating if we start from 0 and process up to i: pref[i].
    // Now, if we skip [l, r], the rating after r is still pref[l-1].
    // Then we process from r+1 to n starting from pref[l-1].
    // Let's define suffix simulation: for each i, and each possible starting rating? Too many.
    
    // Another perspective: The rating change only depends on relative order of a_i and current rating.
    // Notice that rating x changes by +1 if a_i > x, -1 if a_i < x, 0 if a_i == x.
    // This is exactly the process of maintaining a value that tries to "match" the sequence.
    // There is known technique: we can think of the process as: x is the number of "up" moves minus "down" moves, but with a floor/ceiling? Not exactly.
    
    // Let's consider the difference: final rating without skip = f(n).
    // If we skip some interval, we might avoid some decreases or miss some increases.
    // We want to maximize final rating.
    
    // Let's define for each i, the "potential" if we start with rating 0 and go to i: pref[i].
    // Now, if we skip [l, r], we start from pref[l-1] at position r+1.
    // Let's define suffix DP: for each i, and for each possible starting rating s, what is the final rating after processing i..n?
    // But s can be up to n, so O(n^2) states.
    
    // However, note that the process is monotonic in starting rating? If we start with higher rating, do we always end with higher or equal rating?
    // Let's test: Suppose sequence is [5]. Start 0 -> 1. Start 10 -> 9. So higher start can lead to lower end. Not monotonic.
    
    // We need a different approach.
    
    // Let's analyze the effect of skipping a single element. Then we can extend to intervals? Not directly.
    
    // Consider the "rating graph" over time. Rating changes by +1, -1, 0.
    // Skipping an interval means we cut out that segment and connect the rating before to the rating after, but the rating after is the same as before (since skip means no change).
    // So we are essentially removing a subsegment of the rating path and replacing it with a horizontal line.
    // We want to maximize the final rating.
    
    // Let's define the rating path without skips: r_0=0, r_1, ..., r_n.
    // If we skip [l, r], the new path is: r_0, r_1, ..., r_{l-1}, then r_{l-1} repeated (r-l+1) times, then we continue from r_{l-1} but with the remaining contests.
    // The continuation from r_{l-1} on contests r+1..n is not the same as the original r_{r+1}..r_n because the starting rating is different.
    // So we need to compute, for each possible start rating s at position i, the final rating after processing i..n.
    
    // Let's define a function F(i, s) = final rating after processing contests i..n starting with rating s.
    // We want max_{l, r} F(r+1, pref[l-1]).
    // Note that F(i, s) can be computed recursively: F(i, s) = F(i+1, s') where s' = s + (a_i > s ? 1 : (a_i < s ? -1 : 0)).
    // Base: F(n+1, s) = s.
    
    // We need to answer queries: for each i and s, what is F(i, s)? But we only need max over l,r of F(r+1, pref[l-1]).
    // Notice that pref[l-1] is the rating after l-1 contests without skip. So we are considering starting ratings that actually appear in the original process.
    // There are at most n distinct values of pref[i].
    
    // Can we compute F(i, s) efficiently for all i and all s that appear?
    // Since s only takes values that are reachable, maybe we can process backwards.
    
    // Let's process from right to left. For each position i, we want to know for each possible starting rating s, what is the final rating.
    // But s can be many. However, note that the function s -> F(i, s) is piecewise linear? Actually, it's like: F(i, s) = F(i+1, s + delta(s, a_i)).
    // This is a composition of functions. Each step is a function f_i(s) = s + (a_i > s ? 1 : (a_i < s ? -1 : 0)).
    // f_i(s) is a simple function: it's s+1 for s < a_i, s for s = a_i, s-1 for s > a_i.
    // So f_i is non-decreasing? Let's check: if s1 < s2, then f_i(s1) vs f_i(s2). 
    // If both < a_i: s1+1 < s2+1, so increasing.
    // If s1 < a_i <= s2: s1+1 vs s2 (if s2 > a_i, then s2-1; if s2 = a_i, then s2). In all cases, s1+1 <= s2? Since s1 < a_i <= s2, s1+1 <= a_i <= s2, so s1+1 <= s2. And f_i(s2) >= s2-1. So f_i(s1) <= f_i(s2) is not guaranteed if s2 = a_i and s1 = a_i-1: f_i(a_i-1)=a_i, f_i(a_i)=a_i, equal. If s2 > a_i, f_i(s2)=s2-1, and s1+1 <= a_i <= s2-1? Not necessarily: s1 = a_i-1, s2 = a_i+1: f_i(s1)=a_i, f_i(s2)=a_i, equal. So it seems f_i is non-decreasing. Let's verify: For s1 < s2, is f_i(s1) <= f_i(s2)? 
    // Case 1: s1 < s2 < a_i: f_i(s1)=s1+1, f_i(s2)=s2+1, so s1+1 < s2+1.
    // Case 2: s1 < s2 = a_i: f_i(s1)=s1+1, f_i(s2)=a_i. Since s1 < a_i, s1+1 <= a_i, so f_i(s1) <= f_i(s2).
    // Case 3: s1 < a_i < s2: f_i(s1)=s1+1, f_i(s2)=s2-1. We need s1+1 <= s2-1 => s1+2 <= s2. But s1 < a_i < s2, so s2 >= a_i+1, s1 <= a_i-1, so s1+2 <= a_i+1 <= s2? Actually, s1+2 <= a_i+1, and s2 >= a_i+1, so s1+2 <= s2 is true. So f_i(s1) <= f_i(s2).
    // Case 4: s1 = a_i < s2: f_i(s1)=a_i, f_i(s2)=s2-1. Since s2 > a_i, s2-1 >= a_i, so f_i(s1) <= f_i(s2).
    // Case 5: a_i < s1 < s2: f_i(s1)=s1-1, f_i(s2)=s2-1, so s1-1 < s2-1.
    // So indeed f_i is non-decreasing. Therefore, the composition F(i, s) = f_n(f_{n-1}(... f_i(s)...)) is also non-decreasing in s.
    
    // This is crucial: F(i, s) is non-decreasing in s. So for a fixed i, higher starting rating gives higher or equal final rating.
    // Therefore, to maximize F(r+1, pref[l-1]), we want to maximize pref[l-1] for a given r? Or we can choose l to maximize pref[l-1] subject to l <= r+1? Actually l <= r, so l-1 <= r-1. So for a fixed r, we can choose any l <= r, so we can take the maximum pref[l-1] among l-1 <= r-1, i.e., max_{0 <= k <= r-1} pref[k] (with pref[0]=0). Let M[r] = max_{0 <= k <= r} pref[k]. Then for a fixed r, the best starting rating at position r+1 is M[r-1] (if r>=1) or 0 if r=0? But r>=1 since l>=1, so r>=1. So we can achieve starting rating s = max_{0 <= k <= r-1} pref[k] at position r+1.
    // Then the final rating would be F(r+1, s). We want max over r from 1 to n of F(r+1, M[r-1]).
    // Note: r can be n, then r+1 = n+1, F(n+1, s) = s. So we also consider skipping suffix.
    // Also, we must skip at least one contest, so interval length >=1, so r >= l, so r >= 1.
    
    // So the problem reduces to: compute for each i from 1 to n+1, and for a given s_i = M[i-2] (with M[-1]=0? Let's define M[i] = max_{0<=k<=i} pref[k], with pref[0]=0. Then for r, s = M[r-1]. For r=1, s = M[0] = max(pref[0]) = 0. For r=n, s = M[n-1]. Then we need F(r+1, s). We need max over r=1..n of F(r+1, M[r-1]).
    // Additionally, we could skip a prefix? That is l=1, then we start with rating 0 at position r+1. That is covered by r>=1, s = M[r-1] which includes 0.
    
    // Now we need to compute F(i, s) for various i and s. Since F(i, s) is non-decreasing in s, and we only need to evaluate it at specific s values (which are M[i-2]), we can try to compute F(i, s) efficiently.
    // We can process from right to left, maintaining the function F(i, ·) as a piecewise linear function? But s only up to n, and we only need to query at specific points.
    // Since n sum <= 3e5, we might be able to compute F(i, s) for all i and all s that appear? But s can be up to n, and there are n positions, so O(n^2) is too much.
    
    // However, note that F(i, s) is just the result of applying a sequence of functions f_i, each of which is simple: it shifts values above a_i down by 1, below a_i up by 1, and leaves a_i unchanged.
    // This is similar to the "rating" process itself. In fact, if we reverse the sequence, we might find a dual process.
    
    // Let's think about the effect of applying f_i from right to left. Define a sequence of functions. We want to evaluate F(i, s) = f_n(f_{n-1}(... f_i(s)...)).
    // This is equivalent to: starting from s, we process a_i, a_{i+1}, ..., a_n.
    // So F(i, s) is just the final rating if we start with rating s and process contests i..n.
    // So we need to answer: for each i, and a given starting rating s_i, what is the final rating after processing i..n?
    // We can compute this by simulating the process for each i? That would be O(n^2).
    
    // But we can use the fact that the process is "self-adjusting" and maybe we can compute the final rating for all starting ratings at once using a data structure.
    // Alternatively, we can observe that the function F(i, s) has a simple form: it is s plus some offset that depends on how many times s is "above" or "below" the sequence.
    // Actually, consider the process: we have a threshold. For a fixed starting rating s, the rating changes only when a_i > current or < current.
    // This is equivalent to: the rating at the end is s + (number of times a_i > current) - (number of times a_i < current).
    // This is not simply a function of s and the sequence, because the current rating changes.
    
    // Another angle: The rating process can be seen as: x = 0; for each a_i: if a_i > x, x++; elif a_i < x, x--.
    // This is exactly the process of maintaining the "median" or something? No.
    
    // Let's try to find an invariant. Consider the difference a_i - x. When a_i > x, x increases, so the difference decreases by 1? Actually, new x = x+1, new difference = a_i - (x+1) = (a_i - x) - 1. When a_i < x, new x = x-1, new difference = a_i - (x-1) = (a_i - x) + 1. When equal, difference becomes 0. So the absolute difference |a_i - x| decreases by 1 if a_i != x, and becomes 0 if a_i = x. So the process tries to reduce the distance between x and a_i.
    
    // This is reminiscent of the "water level" or "potential" process.
    
    // Let's consider the final rating without skips: f(n). We want to see if skipping can increase it.
    // Suppose we skip an interval. The effect is that we freeze the rating during that interval, then continue. This might allow us to avoid some decreases that would have happened if we had processed those contests.
    // So we want to skip a segment that would have caused a net decrease in rating, relative to the rating we enter with.
    
    // Let's define for each position i, the "value" of starting with rating s and processing i..n. We want to compute this for s = M[i-1]? Actually for r, we start at r+1 with s = M[r-1].
    // Let's denote s_i = M[i-1] for i from 1 to n (i = r+1, so r = i-1). Then we want max_{i=1..n} F(i, s_{i-1})? Wait: r from 1 to n, i = r+1 from 2 to n+1. So we need F(i, M[i-2]) for i=2..n+1. Also we can consider i=1? That would mean skipping nothing? But we must skip at least one, so r>=1, so i>=2. However, we could also skip a prefix? That is l=1, r>=1, then we start at r+1 with rating 0. That is covered by M[i-2] for i>=2: M[0]=0, so for i=2, s=0. So it's fine.
    
    // So we need to compute for each i from 2 to n+1: F(i, M[i-2]).
    // And also we could skip the whole array? That is r=n, then i=n+1, F(n+1, s) = s. So that's just M[n-1].
    // So the answer is max( M[n-1], max_{i=2..n} F(i, M[i-2]) ).
    
    // Now, how to compute F(i, s) for all i efficiently?
    // Since s = M[i-2] is non-decreasing with i? M is prefix maximum of pref, so M is non-decreasing. So s_i is non-decreasing.
    // Also, note that F(i, s) is the final rating after processing i..n starting from s.
    // We can compute F(i, s) by simulating backwards? Or we can use the fact that the process is reversible? Not exactly.
    
    // Let's think about the suffix process. Define for each i, the function g_i(s) = F(i, s). We want to evaluate g_i at s = M[i-2].
    // We can compute g_i from g_{i+1}: g_i(s) = g_{i+1}(f_i(s)), where f_i(s) = s + (a_i > s ? 1 : (a_i < s ? -1 : 0)).
    // So if we know g_{i+1} as a function, we can compose with f_i to get g_i.
    // Since f_i is a simple piecewise linear function with slopes 1, and g_{i+1} is non-decreasing, the composition might be manageable.
    // In fact, g_i(s) is just the result of applying a sequence of such functions. The composition of such functions is also a function of the form: g(s) = s + c, but only for s in certain intervals? Let's test.
    
    // Consider a simple sequence: a = [3]. f_1(s): s<3 -> s+1; s=3 -> 3; s>3 -> s-1.
    // g_1(s) = f_1(s). So it's not just s + constant.
    
    // But maybe we can represent g_i(s) as: g_i(s) = s + h_i(s), where h_i(s) is the net change. And h_i(s) only depends on the relative order of s and the sequence elements.
    // Actually, the net change is the number of times the current rating is less than a_j minus the number of times it is greater. This is path-dependent.
    
    // Alternative approach: Since n sum is 3e5, maybe we can compute F(i, s) for all i by simulating the process for each i, but using the fact that s_i is non-decreasing and we can reuse computation? 
    // If we process from right to left, we could maintain the function g_i(s) for all s? But s can be up to n.
    
    // Wait, the rating x never exceeds n? Actually, a_i <= n, and x starts at 0, and each step x changes by at most 1, so x is always between 0 and n. So s is in [0, n].
    // So we could, for each i, compute an array of size n+1? That would be O(n^2) total.
    
    // But we only need g_i(s) for one specific s per i. So we need to answer n queries: for each i, given s_i, what is g_i(s_i)?
    // We can answer these queries offline by processing from right to left and maintaining a data structure that can apply f_i to all elements? 
    // f_i is: for all s < a_i, s -> s+1; for s = a_i, s -> a_i; for s > a_i, s -> s-1.
    // This is like: we have a set of values, and we apply this transformation. If we maintain the values of interest (the s_i for i >= current), we can update them.
    // But note that the queries are for specific i, and the s_i for that i is fixed. If we process from right to left, at step i, we have already applied f_{i+1}..f_n to some values. We want to apply f_i to the value s_i to get g_i(s_i)? Actually, g_i(s_i) = g_{i+1}(f_i(s_i)). So if we know g_{i+1} as a function, we can compute g_i(s_i) by first applying f_i to s_i, then applying g_{i+1} to the result.
    // But we don't have g_{i+1} explicitly. However, we can think of it as: we have a set of "active" starting values that we are tracking. Initially, at i = n+1, g_{n+1}(s) = s. So for each i, we could insert s_i into a data structure that maintains the values after applying the suffix functions.
    // But the order of applying functions is from left to right: g_i = f_n ∘ ... ∘ f_i. So if we process from right to left, we are building the composition. At step i (from n down to 1), we have the function G = f_n ∘ ... ∘ f_i. We want to evaluate G at s_{i-1}? Wait, careful.
    
    // Let's index: contests 1..n. f_i is the function for contest i.
    // g_i = f_n ∘ f_{n-1} ∘ ... ∘ f_i. So g_i(s) = final rating starting from s at position i.
    // We want to compute g_i(s_{i-1}) for i=2..n+1, where s_{i-1} = M[i-2] (since i = r+1, r = i-1, s = M[r-1] = M[i-2]).
    // For i = n+1, g_{n+1}(s) = s, so g_{n+1}(s_n) = s_n = M[n-1].
    // For i = n, g_n(s) = f_n(s). So g_n(s_{n-1}) = f_n(s_{n-1}).
    // For i = n-1, g_{n-1}(s) = f_n(f_{n-1}(s)).
    // So if we process from right to left, we can maintain the composed function G = f_n ∘ ... ∘ f_i. At step i, we want to evaluate G at s_{i-1}. But s_{i-1} is known from prefix maxima.
    // So we can just compute G(s) for the specific s values we need, if we can efficiently apply the composed function to a value.
    // But the composed function is just the result of simulating the process on the suffix starting from s. So if we can quickly compute the final rating for a given starting rating on a suffix, we can answer each query in O(log n) or something.
    // Is there a way to compute the final rating for a given starting rating on a suffix faster than O(length)? Maybe we can preprocess something.
    
    // Notice that the process is deterministic and the rating only depends on the sequence. For a fixed suffix, the function G(s) is non-decreasing and piecewise linear with slopes 0 or 1? Actually, from the properties of f_i, the composition might have slopes only 0 or 1? Let's check: f_i has slopes: 1 for s < a_i, 0 for s = a_i? Actually, at s = a_i, it's a point. For s > a_i, slope 1? f_i(s) = s-1, so slope 1. So f_i has slope 1 everywhere except at the point s=a_i where it's not differentiable, but the function is continuous? f_i(a_i) = a_i, left limit a_i, right limit a_i-1? Wait: for s > a_i, f_i(s) = s-1, so as s -> a_i+, f_i(s) -> a_i-1. But f_i(a_i) = a_i. So there is a jump discontinuity? Let's check: s = a_i + epsilon -> s-1 = a_i + epsilon -1. As epsilon -> 0+, it's a_i - 1. But f_i(a_i) = a_i. So the function jumps down by 1 at s = a_i? Actually, for s = a_i, it's a_i. For s just above a_i, it's s-1 which is just below a_i. So the function is not continuous: it has a drop of 1 at a_i. But wait, is that correct? Let's re-evaluate: f_i(s) = s + 1 if s < a_i; s if s = a_i; s - 1 if s > a_i. So at s = a_i, f_i(a_i) = a_i. For s = a_i + 0.1, f_i = a_i - 0.9. So indeed, there is a discontinuity: the value jumps from a_i (at s=a_i) to a_i - 1 (just above). So the function is not non-decreasing? Earlier we proved it is non-decreasing. Let's check: s1 = a_i, s2 = a_i + epsilon. f_i(s1)=a_i, f_i(s2)=a_i+epsilon-1. For epsilon small, a_i+epsilon-1 < a_i, so f_i(s1) > f_i(s2). That violates non-decreasing! So my earlier proof was flawed. Let's re-check case 4: s1 = a_i < s2. f_i(s1)=a_i, f_i(s2)=s2-1. Since s2 > a_i, s2-1 >= a_i? Not necessarily: if s2 = a_i + 0.5, s2-1 = a_i - 0.5 < a_i. So f_i(s1) > f_i(s2). So f_i is NOT non-decreasing. It decreases at the point a_i. So my earlier conclusion was wrong.
    
    // Let's re-analyze f_i(s):
    // For s < a_i: f_i(s) = s+1. This is increasing.
    // At s = a_i: f_i(a_i) = a_i.
    // For s > a_i: f_i(s) = s-1. This is increasing.
    // So the function increases for s < a_i, then at s = a_i it drops to a_i (from a_i? Actually, as s approaches a_i from below, f_i(s) approaches a_i. So left limit is a_i. At s=a_i, value is a_i. So it's continuous from the left. Then for s > a_i, f_i(s) = s-1, which at s just above a_i is a_i - epsilon. So there is a drop of 1 at a_i. So the function is not monotonic. It has a local peak at a_i? Actually, it increases to a_i, then drops to a_i-1, then increases again. So it's not monotonic.
    
    // This complicates things. The composition of such functions can be complex.
    
    // Let's think differently. The rating process is equivalent to: x = 0; for each a_i: if a_i > x, x++; elif a_i < x, x--.
    // This is exactly the process of maintaining a value that tries to "match" the sequence. There is a known concept: the rating after processing the whole sequence is the number of elements greater than the "median" or something? Not exactly.
    
    // Let's simulate the process on the sequence and observe the rating path. The rating changes by +1, -1, 0. It never exceeds n. 
    // If we skip an interval, we are essentially removing a segment of the path and replacing it with a constant segment at the rating before the skip. Then we continue from that rating.
    // We want to maximize the final rating.
    
    // Consider the original rating path r_0, r_1, ..., r_n. If we skip [l, r], the new path is: r_0..r_{l-1}, then constant r_{l-1} for l..r, then a new path from r+1 to n starting from r_{l-1}. This new path is not the same as the original r_{r+1}..r_n because the starting point is different.
    // However, note that the process from r+1 to n starting from r_{l-1} is exactly the same as if we had a different history. 
    
    // Maybe we can compute the final rating for any starting rating on any suffix using a segment tree? Since the function f_i is simple, we can compose functions. Each function f_i can be represented as a mapping from [0, n] to [0, n]. We can compose two such functions. The composition of two such functions might be representable as a piecewise linear function with a small number of pieces? Let's see.
    
    // f_i(s) = 
    //   s+1 if s < a_i
    //   a_i  if s = a_i
    //   s-1 if s > a_i
    // This can be written as: f_i(s) = s + 1 - 2 * I(s > a_i) - I(s = a_i)? Not exactly.
    // Actually, it's: f_i(s) = s + 1 if s < a_i; s if s = a_i; s - 1 if s > a_i.
    // So the change is +1 for s < a_i, 0 for s = a_i, -1 for s > a_i.
    // This is like: f_i(s) = s + sign(a_i - s) with sign(0)=0.
    
    // If we compose many such functions, what is the resulting function? It might be that the function is of the form: G(s) = s + c for s in some intervals, but with possible jumps? Actually, each f_i is a bijection? Let's check if f_i is injective: For s1 != s2, can f_i(s1) = f_i(s2)? 
    // If both < a_i: s1+1 = s2+1 => s1=s2.
    // If one < a_i and one = a_i: s1+1 = a_i => s1 = a_i-1. So f_i(a_i-1) = a_i, and f_i(a_i) = a_i. So f_i(a_i-1) = f_i(a_i). So not injective.
    // So information is lost. This means the process is not reversible.
    
    // Given the complexity, maybe there is a simpler observation. Let's look at the examples and try to find a pattern.
    
    // Example 1: 1 2 3 4 5 6. Without skip: 0->1->2->3->4->5->6? Wait: start 0, a1=1>0 ->1; a2=2>1 ->2; a3=3>2 ->3; a4=4>3 ->4; a5=5>4 ->5; a6=6>5 ->6. So final 6. But output is 5. Because we must skip at least one contest. If we skip one, we lose one increase, so final 5. So max is 5.
    
    // Example 2: 1 2 1 1 1 3 4. Without skip: 0->1->2->1->1->1->2->3? Let's simulate: start 0; a1=1>0 ->1; a2=2>1 ->2; a3=1<2 ->1; a4=1=1 ->1; a5=1=1 ->1; a6=3>1 ->2; a7=4>2 ->3. Final 3. But output is 4. They skipped [3,5] (the three 1s). Then: 0->1->2->skip->skip->skip->2->3->4. So final 4. So skipping the 1s avoided the decrease from 2 to 1, and then the subsequent increases from 2 gave higher final.
    
    // Example 3: 1. Must skip, final 0.
    
    // Example 4: 9 9 8 2 4 4 3 5 3. Output 4.
    
    // Example 5: 1 2 3 4 1 3 2 1 1 10. Output 5.
    
    // Let's think about the effect of skipping. The rating only changes when a_i != current rating. If we skip a contest, we avoid that change. So we want to skip contests that would decrease our rating, and we want to skip them before they decrease it, so that we stay at a higher rating for future contests that might increase it.
    // In example 2, the sequence of 1s would have decreased the rating from 2 to 1. By skipping them, we kept rating at 2, then the 3 and 4 increased it to 4.
    
    // So the optimal strategy is to find a point where the rating is high, then skip a segment that would bring it down, and then continue.
    // More precisely, we can choose l such that pref[l-1] is high, and then choose r such that the suffix from r+1 starting from that high rating yields a high final rating.
    // We already had the idea: max over r of F(r+1, M[r-1]).
    // But we need to compute F(i, s) efficiently.
    
    // Since n is up to 3e5, maybe we can compute F(i, s) for all i and all s that are prefix maxima? There are at most n distinct (i, s) pairs? Actually, i from 2 to n+1, and s = M[i-2]. So there are n pairs. We need to compute F(i, s) for each.
    // If we can compute F(i, s) in O(log n) or O(1) amortized, we can solve it.
    
    // Let's consider the suffix process. For a fixed suffix i..n, we want to compute the final rating for a given start s. This is equivalent to simulating the process on that suffix. Can we preprocess the suffix to answer such queries quickly?
    // Notice that the process on a sequence is similar to the original process but with a different starting value. If we reverse the sequence, maybe we can use the same logic? 
    
    // Define a reversed process: start with rating 0 at the end and go backwards? Not sure.
    
    // Another idea: The rating process can be seen as: x is the number of "up
