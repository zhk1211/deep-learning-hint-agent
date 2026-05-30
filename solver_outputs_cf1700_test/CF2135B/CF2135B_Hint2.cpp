// Hint2
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll ask(char dir, ll k) {
    cout << "? " << dir << " " << k << endl;
    ll res;
    cin >> res;
    if (res == -1) exit(0);
    return res;
}

void answer(ll X, ll Y) {
    cout << "! " << X << " " << Y << endl;
}

void solve() {
    int n;
    cin >> n;
    vector<pair<ll, ll>> pts(n);
    for (int i = 0; i < n; ++i) {
        cin >> pts[i].first >> pts[i].second;
    }

    const ll V = 2000000000LL;

    // Move left by V, then left by V again (total left 2V)
    ll d1 = ask('L', V);
    ll d2 = ask('L', V);

    // Move up by V, then up by V again (total up 2V)
    ll d3 = ask('U', V);
    ll d4 = ask('U', V);

    // After these moves, the robot is at (X - 2V, Y + 2V)
    // The distances returned correspond to min Manhattan distance to anchors from that point.
    // Let f(x, y) = min_i (|x_i - x| + |y_i - y|)
    // We have:
    // d1 = f(X - V, Y)
    // d2 = f(X - 2V, Y)
    // d3 = f(X - 2V, Y + V)
    // d4 = f(X - 2V, Y + 2V)

    // We can compute the coordinates of the closest anchor point to (X - 2V, Y + 2V)
    // Because V is huge, the closest anchor point is the one that minimizes |x_i - (X - 2V)| + |y_i - (Y + 2V)|
    // Since V is large, the term |x_i - X + 2V| is dominated by 2V, and similarly for y.
    // Actually, for any anchor point, the distance is approximately 2V - x_i + X + 2V + y_i - Y? Wait.
    // Let's analyze: For large V, X - 2V is very negative, Y + 2V is very positive.
    // So for any anchor point (x_i, y_i), |x_i - (X - 2V)| = (X - 2V) - x_i? No, x_i is between -1e9 and 1e9, X - 2V is about -2e9, so x_i > X - 2V, so |x_i - (X - 2V)| = x_i - (X - 2V) = x_i - X + 2V.
    // Similarly, Y + 2V is about 2e9, y_i <= 1e9, so y_i < Y + 2V, so |y_i - (Y + 2V)| = (Y + 2V) - y_i = Y - y_i + 2V.
    // So distance = (x_i - X + 2V) + (Y - y_i + 2V) = (x_i - y_i) - X + Y + 4V.
    // Thus the anchor point that minimizes this distance is the one that minimizes (x_i - y_i).
    // Let that minimum value be M = min_i (x_i - y_i). The distance d4 = M - X + Y + 4V.
    // Similarly, we can get expressions for d1, d2, d3.
    // But we can also find the specific anchor point that achieves the minimum at the final position.
    // Actually, we can find the anchor point that gives d4 by checking which one minimizes (x_i - y_i).
    // However, there might be multiple with the same (x_i - y_i). But we can still determine X and Y.

    // Let's derive formulas:
    // For any query, the returned distance is min_i (|x_i - curr_x| + |y_i - curr_y|).
    // Because V is huge, the absolute values simplify as above for all queries.
    // Let's check:
    // Query 1: move left by V -> curr = (X - V, Y)
    // For any anchor (x_i, y_i):
    // |x_i - (X - V)| = x_i - X + V (since x_i >= -1e9, X - V <= -1e9? V=2e9, X up to 1e9, so X - V <= -1e9, x_i >= -1e9, so x_i >= X - V? Actually X - V could be -3e9 if X=-1e9, V=2e9. x_i >= -1e9 > -3e9, so yes x_i > X - V. So |x_i - (X - V)| = x_i - X + V.
    // |y_i - Y|: Y is between -1e9 and 1e9, y_i also. This could be either sign. So we cannot simplify this one.
    // So d1 = min_i (x_i - X + V + |y_i - Y|).
    // Similarly, d2: curr = (X - 2V, Y) -> distance = x_i - X + 2V + |y_i - Y|.
    // d3: curr = (X - 2V, Y + V) -> distance = x_i - X + 2V + |y_i - (Y + V)|.
    // d4: curr = (X - 2V, Y + 2V) -> distance = x_i - X + 2V + |y_i - (Y + 2V)|.
    // For d4, since Y + 2V >= 1e9? Y up to 1e9, V=2e9, so Y+2V >= 3e9? Actually Y min -1e9, V=2e9 -> Y+2V >= 3e9? Wait: Y >= -1e9, V=2e9 => Y+2V >= 3e9? No: -1e9 + 4e9 = 3e9. y_i <= 1e9, so y_i < Y+2V always. So |y_i - (Y+2V)| = Y+2V - y_i.
    // So d4 = min_i (x_i - X + 2V + Y + 2V - y_i) = min_i (x_i - y_i) - X + Y + 4V.
    // Let M = min_i (x_i - y_i). Then d4 = M - X + Y + 4V.   (1)

    // For d3: curr = (X - 2V, Y + V). Y+V could be between -1e9 and 3e9? Y min -1e9, V=2e9 -> Y+V >= 1e9. y_i <= 1e9, so y_i <= Y+V? Actually Y+V >= 1e9, y_i <= 1e9, so y_i <= Y+V always. So |y_i - (Y+V)| = Y+V - y_i.
    // So d3 = min_i (x_i - X + 2V + Y + V - y_i) = min_i (x_i - y_i) - X + Y + 3V = M - X + Y + 3V.   (2)

    // For d2: curr = (X - 2V, Y). Y is between -1e9 and 1e9, y_i also. So |y_i - Y| could be either sign. We cannot simplify directly.
    // But we can use d1 and d2 to find something about Y.
    // d1 = min_i (x_i - X + V + |y_i - Y|).
    // d2 = min_i (x_i - X + 2V + |y_i - Y|).
    // Notice that d2 - d1 = V? Not necessarily, because the anchor point that minimizes d1 might be different from the one that minimizes d2.
    // However, we can consider the function g(y) = min_i (x_i + |y_i - y|). Then d1 = g(Y) - X + V, d2 = g(Y) - X + 2V.
    // So d2 - d1 = V, regardless of which anchor point achieves the minimum? Wait: g(Y) is the same in both if the same anchor point minimizes both. But is it guaranteed that the same anchor point minimizes both? Since the term x_i - X + V + |y_i - Y| and x_i - X + 2V + |y_i - Y| differ only by V, the anchor point that minimizes the first also minimizes the second. So yes, the same anchor point achieves the minimum for both. Therefore d2 - d1 = V exactly. Let's verify: d1 = min_i (x_i - X + V + |y_i - Y|) = V + min_i (x_i - X + |y_i - Y|). d2 = 2V + min_i (x_i - X + |y_i - Y|). So d2 - d1 = V. This is a key observation!

    // So we have:
    // d2 - d1 = V. We can check this, but we already know V.
    // Actually we can use this to verify, but we don't need to.

    // Now, from (1) and (2): d4 - d3 = V. Also d3 - d2? Not necessarily, because d2 involves |y_i - Y|, while d3 involves |y_i - (Y+V)|. But we can find Y by considering the change from d2 to d3.

    // Let's analyze d2 and d3.
    // d2 = min_i (x_i - X + 2V + |y_i - Y|)
    // d3 = min_i (x_i - X + 2V + |y_i - (Y+V)|)
    // Let h(y) = min_i (x_i + |y_i - y|). Then d2 = h(Y) - X + 2V, d3 = h(Y+V) - X + 2V.
    // So d3 - d2 = h(Y+V) - h(Y).
    // We know h(y) is a convex function. We can compute h(y) for any y if we know the anchor points? But we don't know Y.
    // However, we can find the anchor point that achieves the minimum in d4. That anchor point has the minimum (x_i - y_i). Let's call it p = (x_p, y_p). Then from (1): d4 = (x_p - y_p) - X + Y + 4V.
    // Also, for this anchor point, what is its contribution to d2? d2 <= x_p - X + 2V + |y_p - Y|.
    // But we don't know if it's the minimizer for d2.

    // Alternative approach: We can find the exact coordinates of the anchor point that is closest to the final position (X - 2V, Y + 2V). Since V is huge, the closest anchor point is the one with minimum (x_i - y_i). If there are multiple, we can disambiguate? Actually, we can find X and Y without knowing which anchor point it is, by using the values of d1, d2, d3, d4.

    // Let's derive equations:
    // Let A = min_i (x_i - y_i). We can compute A from the input.
    // Then d4 = A - X + Y + 4V  =>  Y - X = d4 - A - 4V.   (Eq1)

    // Now we need another equation to find X and Y individually.
    // Consider d1 and d2: d1 = V + min_i (x_i - X + |y_i - Y|), d2 = 2V + min_i (x_i - X + |y_i - Y|).
    // Let B = min_i (x_i - X + |y_i - Y|). Then d1 = V + B, d2 = 2V + B. So B = d1 - V.
    // Also B = min_i (x_i - X + |y_i - Y|).
    // We can rewrite B = min_i ( (x_i + y_i) - X - Y? No, |y_i - Y| is not linear.
    // But we can consider the transformation: Let u = X + Y, v = X - Y? Not sure.

    // Another idea: We can find the anchor point that minimizes d2. How? We can query something else? We have only 10 moves, we used 4. We can use more moves to pinpoint.
    // But the problem says we can find initial coordinates in no more than 10 moves. The hints suggest using 4 moves: (L, V), (L, V), (U, V), (U, V). So maybe we can determine X and Y just from these 4 responses and the anchor points.

    // Let's analyze the function f(x, y) = min_i (|x_i - x| + |y_i - y|). We have f(X - V, Y) = d1, f(X - 2V, Y) = d2, f(X - 2V, Y + V) = d3, f(X - 2V, Y + 2V) = d4.
    // Since V is huge, the region we are probing is far to the left and up. The closest anchor point to all these positions will be the one that is "bottom-left" in some sense? Actually, because we moved far left and far up, the closest anchor point is the one that is most "bottom-right"? Wait: We moved left (negative x) and up (positive y). So the robot is at very negative x and very positive y. The anchor points are in [-1e9, 1e9] x [-1e9, 1e9]. The closest anchor point in Manhattan distance is the one that maximizes x and minimizes y? Let's check: distance = |x_i - curr_x| + |y_i - curr_y|. curr_x is very negative, so |x_i - curr_x| = x_i - curr_x (since x_i > curr_x). curr_y is very positive, so |y_i - curr_y| = curr_y - y_i. So distance = (x_i - curr_x) + (curr_y - y_i) = (x_i - y_i) + (curr_y - curr_x). Since curr_y - curr_x is constant for a given query, minimizing distance is equivalent to minimizing (x_i - y_i). So indeed, the anchor point with the smallest (x_i - y_i) is the closest. If there are multiple with the same minimum (x_i - y_i), then all of them give the same distance, and the distance is exactly that minimum plus (curr_y - curr_x). So d4 = min_i (x_i - y_i) + (Y + 2V) - (X - 2V) = M + Y - X + 4V, where M = min_i (x_i - y_i). This matches our earlier Eq1.

    // Now, what about d1, d2, d3? For d1: curr = (X - V, Y). curr_x is very negative, curr_y is Y (which could be anything). So |x_i - curr_x| = x_i - curr_x, but |y_i - curr_y| is not necessarily curr_y - y_i because curr_y might not be larger than y_i. So the closest anchor point might not be the one with minimum (x_i - y_i). It could be a different one.
    // However, we can consider the function g(y) = min_i (x_i + |y_i - y|). Then d1 = g(Y) - (X - V) = g(Y) - X + V. d2 = g(Y) - X + 2V. So g(Y) = d1 + X - V = d2 + X - 2V.
    // We don't know X, but we can find g(Y) from d1 and d2? Actually, from d1 and d2 we can find g(Y) if we know X? No.

    // But we can find the anchor point that minimizes g(Y). How? We can query at different Y positions? We already have d3 and d4 which are at Y+V and Y+2V. For those, curr_y is large, so the closest anchor point is again the one with minimum (x_i - y_i). So d3 = M - X + Y + 3V, d4 = M - X + Y + 4V. So d4 - d3 = V, which is consistent.

    // Now, we need to find X and Y. We have Y - X = d4 - M - 4V. (Eq1)
    // We need another relation. Consider d2 and d3. d2 = min_i (x_i - X + 2V + |y_i - Y|). d3 = min_i (x_i - X + 2V + |y_i - (Y+V)|). Let's denote h(y) = min_i (x_i + |y_i - y|). Then d2 = h(Y) - X + 2V, d3 = h(Y+V) - X + 2V. So d3 - d2 = h(Y+V) - h(Y).
    // We can compute h(y) for any y if we know the anchor points. But we don't know Y. However, we can find Y by solving h(Y+V) - h(Y) = d3 - d2.
    // Since h is a piecewise linear convex function, and we can compute its value at any point, we can find Y by checking the anchor points? Actually, h(y) is determined by the anchor points. The function h(y) = min_i (x_i + |y_i - y|). This is the lower envelope of V-shaped functions. The minimum of h(y) occurs at some y, and the slope changes at each y_i.
    // We can compute the entire function h(y) easily because n <= 100. We can evaluate h(y) for any y in O(n). We need to find Y such that h(Y+V) - h(Y) = d3 - d2.
    // Since V is huge (2e9), Y+V is far to the right. For large y, h(y) behaves like min_i (x_i + y - y_i) = y + min_i (x_i - y_i) = y + M. So for y >= max(y_i), h(y) = y + M. Similarly, for y <= min(y_i), h(y) = -y + min_i (x_i + y_i)? Let's check: if y <= all y_i, then |y_i - y| = y_i - y, so h(y) = min_i (x_i + y_i - y) = -y + min_i (x_i + y_i). So h is linear with slope -1 for y <= min(y_i), and slope +1 for y >= max(y_i). In between, it's piecewise linear with slopes between -1 and 1.
    // Y is between -1e9 and 1e9. Y+V is at least 1e9 (since V=2e9, Y >= -1e9). max(y_i) <= 1e9. So Y+V >= 1e9 >= max(y_i). Therefore, for y = Y+V, we are in the region where h(y) = y + M. So h(Y+V) = (Y+V) + M.
    // What about h(Y)? Y is between -1e9 and 1e9, so it could be anywhere. But we can compute h(Y) from d2: h(Y) = d2 + X - 2V. We don't know X.
    // But we have d3 = h(Y+V) - X + 2V = (Y+V+M) - X + 2V = M - X + Y + 3V, which matches our earlier d3.
    // So d3 - d2 = (M - X + Y + 3V) - (h(Y) - X + 2V) = M + Y + V - h(Y).
    // Thus h(Y) = M + Y + V - (d3 - d2).
    // But we also have from d2: h(Y) = d2 + X - 2V.
    // And from Eq1: Y - X = d4 - M - 4V => X = Y - d4 + M + 4V.
    // Substitute X into h(Y):
    // h(Y) = d2 + (Y - d4 + M + 4V) - 2V = d2 + Y - d4 + M + 2V.
    // Equate the two expressions for h(Y):
    // M + Y + V - (d3 - d2) = d2 + Y - d4 + M + 2V
    // Cancel M + Y:
    // V - d3 + d2 = d2 - d4 + 2V
    // => -d3 = -d4 + V
    // => d4 - d3 = V.
    // This is an identity, so it doesn't give new information. We need another equation.

    // We need to find Y independently. Notice that h(Y) is a function we can compute if we know Y. But we don't know Y. However, we can find Y by using the fact that h(Y) is piecewise linear and we know its value at Y+V (which is Y+V+M) and we know the difference h(Y+V) - h(Y) = d3 - d2. So we know h(Y) = h(Y+V) - (d3 - d2) = Y + V + M - (d3 - d2).
    // So we have an expression for h(Y) in terms of Y. But we also know that h(Y) = min_i (x_i + |y_i - Y|). So we need to find Y such that min_i (x_i + |y_i - Y|) = Y + V + M - (d3 - d2).
    // This is an equation in Y. Since the left side is a known function (we can compute it for any Y), we can solve for Y. Because n is small, we can find Y by considering the breakpoints of h(Y). The function h(Y) is convex and piecewise linear with breakpoints at y_i. We can compute the value of the right side for each candidate Y? But Y is an integer in [-1e9, 1e9], we can't iterate all. However, we can find the exact Y by analyzing the function.

    // Let's denote C = V + M - (d3 - d2). Then we need h(Y) = Y + C.
    // h(Y) = min_i (x_i + |y_i - Y|). We want to find Y such that this equals Y + C.
    // Consider the function f_i(Y) = x_i + |y_i - Y|. We want min_i f_i(Y) = Y + C.
    // This means that for the minimizing anchor point i, we have x_i + |y_i - Y| = Y + C.
    // Also, for all other j, x_j + |y_j - Y| >= Y + C.
    // We can solve for Y from the equality for each i, and then check if it satisfies the inequality for all j, and also that Y is in the correct range (i.e., the absolute value condition matches the sign we assumed).
    // For a given i, the equation x_i + |y_i - Y| = Y + C.
    // Case 1: Y <= y_i. Then |y_i - Y| = y_i - Y. Equation: x_i + y_i - Y = Y + C => 2Y = x_i + y_i - C => Y = (x_i + y_i - C) / 2.
    // This requires Y <= y_i and Y must be integer (so x_i + y_i - C must be even).
    // Case 2: Y >= y_i. Then |y_i - Y| = Y - y_i. Equation: x_i + Y - y_i = Y + C => x_i - y_i = C.
    // This doesn't determine Y; it just gives a condition on C. If this holds, then any Y >= y_i satisfies the equation? Wait: If x_i - y_i = C, then for Y >= y_i, f_i(Y) = x_i + Y - y_i = Y + (x_i - y_i) = Y + C. So the equality holds for all Y >= y_i. But we also need that this i is the minimizer, i.e., for all j, f_j(Y) >= Y + C. This could restrict Y to an interval.

    // So we can find Y by checking all anchor points i and solving the equation. Since n <= 100, we can just collect all possible Y candidates from Case 1 (for each i, if (x_i + y_i - C) is even, Y = (x_i + y_i - C)/2, check if Y <= y_i). Also from Case 2, if x_i - y_i == C, then any Y >= y_i is a candidate, but we need to find the specific Y that also satisfies the global minimum condition. However, we also have the constraint that Y is the actual initial coordinate, which is unique. We can test each candidate Y by computing h(Y) and checking if it equals Y + C, and also checking if the minimizer i indeed gives that value. But we also need to ensure that the robot's initial position is consistent with all queries. We can just find Y by solving the equation, and then compute X from Eq1. Then we can verify if this (X, Y) is consistent with d1, d2, d3, d4? Actually, if we find a candidate (X, Y) that satisfies all four distances, it must be the correct one. Since the interactor is not adaptive, the true (X, Y) is fixed and will satisfy all equations. There might be multiple candidates? The problem guarantees that the initial coordinates are uniquely determined by the queries? Not necessarily, but we can just output the one we find. We can also use an extra move to confirm if needed, but we have 6 moves left. We can just output the answer after finding a consistent (X, Y). To be safe, we can find all candidates and then maybe use another query to distinguish, but likely the equations yield a unique solution.

    // Let's formalize:
    // Given: n anchor points (x_i, y_i). V = 2e9.
    // We have d1, d2, d3, d4 from the 4 moves.
    // Compute M = min_i (x_i - y_i).
    // From d4: Y - X = d4 - M - 4V.  (Eq1)
    // From d3 and d2: Let delta = d3 - d2.
    // We know h(Y+V) = Y + V + M (since Y+V >= max(y_i) because V=2e9 and Y >= -1e9, max(y_i) <= 1e9, so Y+V >= 1e9 >= max(y_i)).
    // Also h(Y+V) = d3 + X - 2V? Wait: d3 = h(Y+V) - X + 2V => h(Y+V) = d3 + X - 2V.
    // But we also have h(Y+V) = Y + V + M.
    // So d3 + X - 2V = Y + V + M => X - Y = M + 3V - d3. This is another relation. Combine with Eq1: Y - X = d4 - M - 4V. Adding: 0 = d4 - d3 - V => d4 - d3 = V, which is consistent. So no new info.

    // We need to use d1 or d2 to get Y.
    // We have d2 = h(Y) - X + 2V => h(Y) = d2 + X - 2V.
    // Substitute X from Eq1: X = Y - (d4 - M - 4V) = Y - d4 + M + 4V.
    // So h(Y) = d2 + Y - d4 + M + 4V - 2V = Y + (d2 - d4 + M + 2V).
    // Let C = d2 - d4 + M + 2V. Then h(Y) = Y + C.
    // So we need to find Y such that min_i (x_i + |y_i - Y|) = Y + C.
    // This is exactly the equation we derived earlier with C = V + M - (d3 - d2)? Let's check: earlier we had C = V + M - (d3 - d2). Are these consistent? d3 - d2 = (M - X + Y + 3V) - (h(Y) - X + 2V) = M + Y + V - h(Y). So h(Y) = M + Y + V - (d3 - d2). Thus C = M + V - (d3 - d2). But we also have h(Y) = Y + (d2 - d4 + M + 2V). Equating: M + V - d3 + d2 = d2 - d4 + M + 2V => V - d3 = -d4 + 2V => d4 - d3 = V. So they are the same C. So we can compute C = d2 - d4 + M + 2V. (Or using d3, d2: C = M + V - (d3 - d2). Both should give the same, we can use either.)

    // So we need to solve h(Y) = Y + C.
    // h(Y) = min_i (x_i + |y_i - Y|).
    // We can find Y by considering each anchor point i as the potential minimizer.
    // For a fixed i, the condition that i is the minimizer and gives value Y + C means:
    // 1) x_i + |y_i - Y| = Y + C
    // 2) For all j, x_j + |y_j - Y| >= Y + C.
    // We can solve 1) for Y:
    // Case A: Y <= y_i. Then x_i + y_i - Y = Y + C => 2Y = x_i + y_i - C => Y = (x_i + y_i - C)/2. This requires Y <= y_i and the division exact.
    // Case B: Y >= y_i. Then x_i + Y - y_i = Y + C => x_i - y_i = C. If this holds, then any Y >= y_i satisfies the equation for this i. But we also need that for this Y, i is the minimizer. So we need to find Y >= y_i such that for all j, x_j + |y_j - Y| >= Y + C.
    // In Case B, if x_i - y_i = C, then the value is Y + C. For other j, we need x_j + |y_j - Y| >= Y + C.
    // This inequality can be rewritten as: for Y >= y_i, we need for all j: x_j + |y_j - Y| - Y >= C.
    // Let's analyze the function g_j(Y) = x_j + |y_j - Y| - Y.
    // If Y >= y_j, then g_j(Y) = x_j + Y - y_j - Y = x_j - y_j.
    // If Y <= y_j, then g_j(Y) = x_j + y_j - Y - Y = x_j + y_j - 2Y.
    // So for a given Y, the condition is min_j g_j(Y) >= C. But we know that for the true Y, min_j g_j(Y) = C (achieved by the minimizer i). So we need min_j g_j(Y) = C.
    // So we can find Y by looking at the function G(Y) = min_j g_j(Y). We need G(Y) = C.
    // G(Y) is a piecewise linear function. We can compute it for any Y in O(n). We can find all Y where G(Y) = C by considering the breakpoints and solving linear equations.
    // Since n is small, we can just collect all candidate Y values from Case A for all i, and also from the boundaries of intervals where G(Y) might equal C.
    // Alternatively, we can just iterate over all anchor points i, and for each, consider the two cases, generate candidate Y, and then check if that Y satisfies h(Y) = Y + C. Since the true Y must satisfy this, and there are at most 2n candidates from Case A, we can just test them all. What about Case B? If x_i - y_i = C, then any Y >= y_i could be a candidate. But we can find the exact Y by also using the fact that Y must be consistent with the other queries? Actually, if there is an interval of Y satisfying the condition, then the initial Y could be any of them? But the problem guarantees the initial coordinates are fixed and we need to output them. There must be a unique Y that also satisfies the other constraints? Wait, we haven't used d1 yet. d1 gives another equation: d1 = h(Y) - X + V. But we already used d2 and d3 and d4. d1 might provide additional constraint to pinpoint Y if Case B yields an interval.
    // Let's check if Case B can actually happen. x_i - y_i = C. C = d2 - d4 + M + 2V. M is min(x_i - y_i). So C >= M + 2V + (d2 - d4). d2 and d4 are distances, d2 - d4 could be negative. But V is 2e9, so C is around 2e9 + M. Since M is at most 2e9 (x_i up to 1e9, y_i down to -1e9, so x_i - y_i up to 2e9). So C is around 4e9. But x_i - y_i is at most 2e9. So x_i - y_i = C is impossible because C is much larger than any possible x_i - y_i. Let's verify: V = 2e9. M = min(x_i - y_i) >= -2e9. d2 and d4 are distances, they are non-negative. d2 - d4 could be as low as -d4. d4 is roughly M - X + Y + 4V. X, Y in [-1e9, 1e9], so d4 is around 4V = 8e9. So d2 - d4 is around -8e9. Then C = d2 - d4 + M + 2V could be around -8e9 + (-2e9) + 4e9 = -6e9. That's very negative. x_i - y_i is at least -2e9. So it's possible that x_i - y_i = C? C could be -6e9, which is less than -2e9, so no anchor point can have x_i - y_i that small. So Case B (x_i - y_i = C) is impossible because C is either very large positive or very negative, outside the range of x_i - y_i. Let's check the range: x_i, y_i in [-1e9, 1e9], so x_i - y_i in [-2e9, 2e9]. V = 2e9. M in [-2e9, 2e9]. d2 and d4 are distances from far away points. d4 = M - X + Y + 4V. X, Y in [-1e9, 1e9], so -X + Y in [-2e9, 2e9]. So d4 in [M - 2e9 + 4V, M + 2e9 + 4V] = [M + 6e9, M + 10e9] roughly. d2 = h(Y) - X + 2V. h(Y) = min_i (x_i + |y_i - Y|). x_i in [-1e9, 1e9], |y_i - Y| in [0, 2e9], so h(Y) in [-1e9, 3e9]. -
