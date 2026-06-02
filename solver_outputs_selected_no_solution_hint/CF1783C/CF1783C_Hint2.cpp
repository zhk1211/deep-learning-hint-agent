#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    
    // We want to find the minimum possible place.
    // Place = 1 + number of participants with strictly more wins than us.
    // Let w be our number of wins.
    // For each opponent i, their wins without us: i-1 (since they beat all j < i).
    // If we beat opponent i, they get i-1 wins; if we lose, they get i wins.
    // So opponent i's wins = i - 1 + (1 if we lose to i else 0).
    // We want to maximize our wins w subject to sum of a_i for beaten opponents <= m.
    // Then we compute place = 1 + count of i such that opponent i's wins > w.
    
    // Sort opponents by a_i, but we need to keep track of original indices to compute their wins.
    // Actually, we can just sort a copy with indices.
    vector<pair<int,int>> b(n);
    for (int i = 0; i < n; ++i) {
        b[i] = {a[i], i};
    }
    sort(b.begin(), b.end());
    
    // Compute maximum wins w we can get.
    int w = 0;
    long long sum = 0;
    for (int i = 0; i < n; ++i) {
        if (sum + b[i].first <= m) {
            sum += b[i].first;
            ++w;
        } else {
            break;
        }
    }
    
    // If we can beat all, place is 1.
    if (w == n) {
        cout << 1 << '\n';
        return;
    }
    
    // Now we need to compute place.
    // Opponent i's wins = i + (1 if we lose to i else 0).
    // We want count of i with wins > w.
    // For i from 0 to n-1 (0-indexed), opponent i's index is i+1 in 1-indexed.
    // Their wins if we beat them: i (since they beat i opponents among 0..i-1).
    // Their wins if we lose: i+1.
    // So if we beat opponent i, they have i wins; if we lose, they have i+1 wins.
    // We need to decide which opponents we beat to maximize w, but also we can choose which ones to beat to minimize place.
    // Actually, we can choose any set of size w with sum <= m.
    // To minimize place, we might want to beat opponents with high index i, because beating them reduces their wins from i+1 to i, which might drop them below or equal to w.
    // The place is determined by the number of opponents with wins > w.
    // Let's think: For each opponent i, if we beat them, their wins = i; if we lose, their wins = i+1.
    // We want to minimize the count of i such that (i+1 if lose else i) > w.
    // Equivalently, we want to maximize the number of i with wins <= w.
    // For a given w, which opponents can have wins <= w?
    // If we beat i: i <= w  -> i <= w.
    // If we lose to i: i+1 <= w -> i <= w-1.
    // So opponents with i <= w-1 will have wins <= w regardless of whether we beat them or not.
    // Opponents with i = w: if we beat them, wins = w (<= w); if we lose, wins = w+1 (> w).
    // Opponents with i >= w+1: if we beat them, wins = i >= w+1 (> w); if we lose, wins = i+1 > w.
    // So the only opponent whose status affects whether they have > w wins is the one with index i = w.
    // Wait, check: i is 0-indexed? Let's use 0-indexed i.
    // Opponent i (0-indexed) beats all j < i, so they have i wins from other opponents.
    // If we beat them, total wins = i.
    // If we lose, total wins = i+1.
    // We want wins > w.
    // For i < w: i <= w-1. If we beat: i <= w-1 <= w. If we lose: i+1 <= w. So always <= w.
    // For i = w: if beat: w; if lose: w+1. So lose -> > w.
    // For i > w: if beat: i > w; if lose: i+1 > w. So always > w.
    // Therefore, the number of opponents with wins > w is exactly:
    // (n - 1 - w) + (1 if we lose to opponent w else 0) = n - w - (1 if we beat opponent w else 0).
    // Wait, count of i > w is n - 1 - w (since indices 0..n-1, i > w means i = w+1 .. n-1, count = n-1 - (w+1) + 1 = n-1-w).
    // Plus possibly opponent w if we lose.
    // So total with > w = (n - 1 - w) + (1 if lose to w else 0) = n - w - 1 + (1 if lose to w else 0).
    // If we beat w: n - w - 1.
    // If we lose to w: n - w.
    // Then place = 1 + that count.
    // So place = n - w if we lose to w, or n - w + 1 if we beat w? Let's compute:
    // If beat w: count = n - w - 1, place = n - w.
    // If lose to w: count = n - w, place = n - w + 1.
    // But wait, we also have ourselves. Our wins = w. Opponents with > w are those counted. Place = 1 + count.
    // So place = n - w + 1 if we lose to w, else n - w.
    // We want to minimize place, so we want to beat opponent w if possible.
    // But we already chose a set of w opponents to beat to maximize w. Can we always choose a set that includes opponent w?
    // We need to check if there exists a set of w opponents with sum <= m that includes opponent w.
    // If yes, place = n - w.
    // If not, place = n - w + 1.
    // But wait, is it always optimal to maximize w first? Yes, because place decreases as w increases.
    // Let's verify: place = n - w or n - w + 1. As w increases, n - w decreases. So maximizing w is primary.
    // After maximizing w, we try to include opponent w in the set if possible.
    // However, we must be careful: The set of w opponents we beat must have sum <= m. We found the maximum w by taking the w smallest a_i.
    // But maybe we can achieve the same w with a different set that includes opponent w, even if opponent w's a_i is not among the w smallest.
    // So we need to check: Is there a set of size w with sum <= m that includes opponent w?
    // We can check by considering the sum of the w smallest a_i, and see if we can swap one of them with opponent w without exceeding m.
    // Specifically, let S be the sum of the w smallest a_i. If we want to include opponent w, we can take the w-1 smallest among all except opponent w, plus opponent w.
    // Let that sum be S'. If S' <= m, then we can include opponent w.
    // But we also need to ensure that we can actually achieve w wins. The maximum w is already determined by the greedy approach.
    // If the greedy w is maximal, then any set of size w+1 has sum > m. So we cannot increase w.
    // So we just need to check if we can achieve w wins while beating opponent w.
    // How to compute S'? We can precompute prefix sums of sorted a.
    // Let sorted array be sorted_a.
    // We have w = max wins.
    // We need to find the sum of the w-1 smallest a_i excluding opponent w, plus a[w].
    // We can find the rank of a[w] in the sorted array.
    // Let pos = index of opponent w in the sorted array (by value, then by original index to break ties? Actually we only care about value).
    // But careful: The sorted array we used for greedy might have multiple elements with same value. We need to pick w-1 smallest excluding opponent w.
    // We can just compute the sum of the first w elements in sorted array, then if opponent w is among them, we can try to replace the largest among them (which is the w-th element) with opponent w? Actually, if opponent w is already in the first w, then we can just use the greedy set and we are good (we beat opponent w). So place = n - w.
    // If opponent w is not in the first w, then we need to include opponent w and drop one of the first w. To minimize sum, we should drop the largest among the first w, i.e., the w-th element (0-indexed w-1). So new sum = prefix_sum[w-1] + a[w] (if w > 0). If w == 0, then we cannot beat anyone, so opponent w is opponent 0. If we beat opponent 0, we need sum <= m, which is just a[0] <= m. But if w=0, it means even the smallest a_i > m, so a[0] > m, so we cannot beat opponent 0. So place = n - 0 + 1 = n+1? Wait, if w=0, we beat nobody. Then opponent 0 has wins = 1 (since we lose). Opponents >0 have wins >=1. So all n opponents have >=1 win, we have 0. Place = n+1. Formula: n - w + 1 = n+1. Correct.
    // So algorithm:
    // 1. Find w: maximum number of wins by taking smallest a_i.
    // 2. If w == n: place = 1.
    // 3. Else, check if we can beat opponent w (0-indexed w). Opponent w is the one with original index w? Wait, opponent indices are 1..n. In 0-indexed, opponent i has index i. The opponent with i = w is the one with original index w (0-indexed). So we need a[w] (the preparation time for opponent w).
    // 4. We need to see if there is a set of size w with sum <= m that includes opponent w.
    //    - If w == 0: cannot include anyone, so place = n + 1.
    //    - Else: Let sorted array of pairs (a_i, i). We have prefix sums.
    //      Find the position of opponent w in the sorted array. Since we only care about value, we can just find how many of the first w elements have value <= a[w]? Actually, we need to know if opponent w is among the first w elements of the sorted array. But the sorted array is sorted by a_i, and we might have ties. We need to know if we can pick a set of size w including opponent w with sum <= m.
    //      We can compute the sum of the w smallest elements excluding opponent w, plus a[w]. To do this efficiently:
    //      Let sorted_a be the sorted values of a. We can also keep original indices.
    //      We can find the number of elements in the first w that are strictly less than a[w], or equal? Since we can choose any w elements, if there are ties, we might be able to include opponent w without increasing the sum if there is an element with the same a_i in the first w. Actually, if opponent w's a_i is equal to the w-th smallest value, we can just swap them without changing sum. So we only fail if a[w] is strictly greater than the w-th smallest value, and even then we might fail if the new sum exceeds m.
    //      Let's formalize: We have sorted array of pairs. Let the first w elements be the "cheapest" w. Their sum is S. If opponent w is among them, we can achieve w wins including opponent w, so place = n - w.
    //      If opponent w is not among them, then a[w] >= max of the first w (with tie-breaking by index? Actually, if a[w] equals the w-th value, it might not be included if its index is larger? But we can just choose it instead of the one with the same value. So if a[w] <= the w-th value, we can include it without increasing sum. So we only have a problem if a[w] > the w-th value.
    //      So condition: if a[w] <= sorted_a[w-1] (the w-th smallest value), then we can include opponent w without increasing sum beyond S? Wait, if a[w] == sorted_a[w-1], then we can swap opponent w with the element at position w-1 (or any with that value) and sum remains S. So yes, we can achieve sum S.
    //      If a[w] > sorted_a[w-1], then including opponent w forces us to drop the largest of the first w, which is sorted_a[w-1]. New sum = S - sorted_a[w-1] + a[w]. If this new sum <= m, then we can include opponent w. Otherwise, we cannot.
    //      But wait: What if w = 0? Then we don't have sorted_a[w-1]. Handled separately.
    //      Also, we need to be careful: The greedy w is the maximum possible. If we can achieve w with opponent w, place = n - w. Else place = n - w + 1.
    //      But is it possible that by dropping opponent w we could achieve w+1 wins? No, because w is maximal.
    //      So this logic seems correct.
    
    // Let's test with examples.
    // Example 1: n=4, m=401, a=[100,100,200,1]. Sorted: [1,100,100,200]. w=4 (sum=401 <= m). w=n -> place=1. Correct.
    // Example 2: n=3, m=2, a=[1,2,3]. Sorted: [1,2,3]. w: sum 1 <=2, sum 1+2=3>2 -> w=1. Opponent w = opponent 1 (0-indexed 1) has a[1]=2. Sorted_a[w-1] = sorted_a[0]=1. a[1]=2 > 1. New sum = S - sorted_a[0] + a[1] = 1 - 1 + 2 = 2 <= m=2. So we can include opponent 1. Place = n - w = 3 - 1 = 2. Correct.
    // Example 3: n=5, m=0, a=[1,1,1,1,1]. Sorted: [1,1,1,1,1]. w=0 (since 1>0). w=0 -> place = n+1 = 6. Correct.
    // Example 4: n=4, m=0, a=[0,1,1,1]. Sorted: [0,1,1,1]. w: sum 0 <=0 -> w=1. Opponent w = opponent 1 (0-indexed 1) has a[1]=1. Sorted_a[w-1] = sorted_a[0]=0. a[1]=1 > 0. New sum = 0 - 0 + 1 = 1 > m=0. So cannot include opponent 1. Place = n - w + 1 = 4 - 1 + 1 = 4. Correct.
    // Example 5: n=4, m=4, a=[1,2,2,1]. Sorted: [1,1,2,2]. w: sum 1+1=2 <=4, sum 1+1+2=4 <=4, sum 1+1+2+2=6>4 -> w=3. Opponent w = opponent 3 (0-indexed 3) has a[3]=1. Sorted_a[w-1] = sorted_a[2]=2. a[3]=1 <= 2, so we can include opponent 3. Place = n - w = 4 - 3 = 1. Correct.
    
    // So algorithm is correct.
    
    // Implementation details:
    // We need sorted_a values. We can just sort a copy.
    vector<int> sorted_a = a;
    sort(sorted_a.begin(), sorted_a.end());
    
    // Compute prefix sums.
    vector<long long> pref(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        pref[i+1] = pref[i] + sorted_a[i];
    }
    
    // Find max w.
    w = 0;
    while (w < n && pref[w+1] <= m) {
        ++w;
    }
    
    if (w == n) {
        cout << 1 << '\n';
        return;
    }
    
    if (w == 0) {
        cout << n + 1 << '\n';
        return;
    }
    
    // Check if we can include opponent w.
    int opponent_w_val = a[w];
    if (opponent_w_val <= sorted_a[w-1]) {
        // Can include without increasing sum beyond S.
        cout << n - w << '\n';
    } else {
        // Need to swap with the largest of the first w.
        long long new_sum = pref[w-1] + opponent_w_val; // pref[w-1] is sum of first w-1 elements.
        if (new_sum <= m) {
            cout << n - w << '\n';
        } else {
            cout << n - w + 1 << '\n';
        }
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
