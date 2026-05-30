// Hint0
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

    // We will find X and Y separately.
    // First, find X.
    // Move left by a large number to ensure we are to the left of all points.
    // We'll use 2e9 as a safe large number.
    const ll INF = 2'000'000'000LL;
    ll d0 = ask('L', INF);
    // After moving left by INF, current X = X0 - INF.
    // d0 = min_i |xi - (X0 - INF)| + |yi - Y0|
    // Since INF is huge, X0 - INF is very negative, so |xi - (X0 - INF)| = (X0 - INF) - xi? Actually xi - (X0 - INF) = xi - X0 + INF.
    // For large enough INF, xi - (X0 - INF) > 0, so |xi - (X0 - INF)| = xi - X0 + INF.
    // So d0 = min_i (xi - X0 + INF + |yi - Y0|) = INF - X0 + min_i (xi + |yi - Y0|).
    // Let C = min_i (xi + |yi - Y0|). Then d0 = INF - X0 + C.
    // We don't know C yet.

    // Now move right by some amount to get another equation.
    // Move right by 1.
    ll d1 = ask('R', 1);
    // Now current X = X0 - INF + 1.
    // d1 = min_i |xi - (X0 - INF + 1)| + |yi - Y0|.
    // For large INF, xi - (X0 - INF + 1) = xi - X0 + INF - 1 > 0, so |...| = xi - X0 + INF - 1.
    // d1 = INF - X0 - 1 + min_i (xi + |yi - Y0|) = d0 - 1.
    // Wait, this doesn't give new info. We need to move enough so that the minimizing point might change or the absolute value changes sign.
    // Actually, we can find X by moving left/right and observing when the distance changes slope.
    // But we have only 10 moves. We can find X and Y by binary search? No, we don't get direct distance to a known point.

    // Alternative approach: The distance function f(x,y) = min_i |xi - x| + |yi - y| is convex.
    // We can find the minimum of f(x,y) over all (x,y). The initial position (X,Y) is somewhere.
    // We can move to the minimum point? But we need to find (X,Y), not the minimum.
    // Actually, we can find (X,Y) by moving in four directions and using the distances to triangulate.

    // Let's reset: We are at (X,Y). We can move by k in four directions.
    // Let f(x,y) = min_i |xi - x| + |yi - y|.
    // We query f(X + dx, Y + dy).
    // We want to recover X and Y.

    // Observe that f(x,y) is a lower envelope of cones. The distance changes slope when we cross lines x=xi or y=yi.
    // We can find X by moving left until the distance increases by exactly the amount we moved? Not exactly.

    // Better: We can find the closest anchor point? Not directly.

    // Let's think: We can move to a point where distance is 0? If we hit an anchor point, we know we are at that anchor point, and we know how much we moved, so we can recover (X,Y). But we might not hit an anchor point in 10 moves.

    // Actually, we can find X and Y independently by moving far left and far down, then moving right/up step by step and observing the change in distance.
    // Since we have only 10 moves, we can do a "binary search" on the coordinate by moving large amounts and seeing the difference.

    // Let's try to find X:
    // Move left by INF. d0 = f(X - INF, Y).
    // Move right by 2*INF. d1 = f(X + INF, Y).
    // For large INF, the closest point in x-direction will be the one with smallest xi + |yi - Y| for left, and smallest -xi + |yi - Y| for right.
    // But we don't know Y.

    // Maybe we can find Y first by moving up/down similarly? Still coupled.

    // Another idea: The function f(x,y) is piecewise linear with slopes in {-1,0,1} in each direction.
    // We can find the exact X by moving left until the distance decreases by 1 per unit moved, and then when we pass the X coordinate of the closest point, the slope changes.
    // But we can't move continuously.

    // Let's use the fact that we can move by any k up to 1e9. We can move to a far away point where the distance function simplifies.
    // Suppose we move left by INF. Then f(X - INF, Y) = INF - X + min_i (xi + |yi - Y|).
    // Let A = min_i (xi + |yi - Y|). Then dL = INF - X + A.
    // Move right by INF from original: f(X + INF, Y) = INF + X + min_i (-xi + |yi - Y|).
    // Let B = min_i (-xi + |yi - Y|). Then dR = INF + X + B.
    // So dL + dR = 2*INF + A + B.
    // dR - dL = 2*X + B - A.
    // We don't know A and B, but they depend on Y.

    // Similarly, move down by INF: f(X, Y - INF) = INF - Y + min_i (yi + |xi - X|) = INF - Y + C.
    // Move up by INF: f(X, Y + INF) = INF + Y + min_i (-yi + |xi - X|) = INF + Y + D.
    // We have four unknowns: X, Y, and the min expressions.
    // But we can also move to combinations? We have 10 moves, we can try to solve for X and Y.

    // Actually, we can find X and Y by moving to a point where the distance is 0? If we can find an anchor point, we can then move to it and know our offset.
    // How to find an anchor point? We can try to move to the point that minimizes f(x,y). The minimum of f is 0 (since anchor points exist). The set of minima is the intersection of the Manhattan Voronoi regions? Actually, the minimum is 0 at the anchor points themselves. So if we can find any anchor point, we can move there and then we know our initial position.
    // But we don't know where the anchor points are? They are given! We know all anchor points from the input!
    // Yes! The anchor points are given to us at the start. So we know all (xi, yi).
    // We are at an unknown (X,Y). We can make moves and get the minimum Manhattan distance to any anchor point.
    // We want to find (X,Y) in <= 10 moves.
    // Since we know the anchor points, we can compute the distance function for any (x,y) ourselves.
    // So we can query f(x,y) for some (x,y) that we choose by moving.
    // We need to determine (X,Y) from a few queries.

    // This is like: we have an unknown point (X,Y), and we can query the function f(x,y) = min_i |xi - x| + |yi - y| at points of our choice relative to (X,Y). That is, we can query f(X+dx, Y+dy) for any dx,dy we choose (by moving).
    // We want to find X and Y.

    // Since we know the anchor points, we can try to find (X,Y) by making queries that give us information about the distances to specific anchor points.
    // For example, if we move far away, the closest anchor point might be the one that minimizes some linear function. But we can compute which anchor point that is for a given direction if we knew (X,Y)? No, we don't know (X,Y).

    // Alternative: We can try to find the Voronoi region of (X,Y). The distance function tells us the distance to the closest anchor point. If we move a little, the closest anchor point might change. We can detect which anchor point is closest by moving in different directions and seeing how the distance changes.
    // Actually, if we are at (X,Y), the distance is d = |xi - X| + |yi - Y| for some i. If we move left by 1, the distance becomes |xi - (X-1)| + |yi - Y|. If X > xi, then |xi - (X-1)| = |xi - X + 1| = (X - xi - 1) if X-1 >= xi, i.e., X > xi. So distance decreases by 1. If X <= xi, distance increases by 1. So by moving left by 1, we can tell if X > xi or X <= xi for the closest anchor point.
    // But we don't know which anchor point is closest. However, we can make multiple moves to figure out the exact (X,Y).

    // Since we have up to 10 moves, we can do a ternary search? No, the function is convex, we can find the minimum? But we want the initial point, not the minimum.

    // Wait, we can find (X,Y) by moving to a known anchor point! How? We can try to move to each anchor point? But we don't know the path. We can move directly to an anchor point if we know the vector from (X,Y) to that anchor point. But we don't know (X,Y).

    // Idea: We can find the distance to a specific anchor point by moving far away so that only that anchor point matters? Not exactly.

    // Let's think about the function f(x,y). For a given anchor point i, the distance is |xi - x| + |yi - y|. The function f is the minimum of these. If we move far in some direction, the closest anchor point will be the one that minimizes some linear combination. For example, if we move far to the left (x -> -infty), the distance to anchor i is (xi - x) + |yi - y| = -x + xi + |yi - y|. The minimum over i will be achieved by the anchor point that minimizes xi + |yi - y|. This depends on y. But if we also move far down, we can simplify further.

    // Suppose we move left by INF and down by INF. Then x = X - INF, y = Y - INF.
    // Distance to anchor i: |xi - (X-INF)| + |yi - (Y-INF)| = (xi - X + INF) + (yi - Y + INF) = 2*INF - X - Y + xi + yi (assuming INF large enough so that xi > X-INF and yi > Y-INF, which is true if INF > max(xi) - X and INF > max(yi) - Y. Since coordinates are up to 1e9, we can choose INF = 2e9 to be safe).
    // So f(X-INF, Y-INF) = 2*INF - X - Y + min_i (xi + yi).
    // We know min_i (xi + yi) because we know all anchor points! Let S_min = min_i (xi + yi).
    // Then d_ld = 2*INF - X - Y + S_min.
    // Similarly, move right by INF and up by INF: f(X+INF, Y+INF) = 2*INF + X + Y + min_i (-xi - yi) = 2*INF + X + Y + T_min, where T_min = min_i (-xi - yi) = -max_i (xi + yi).
    // So we can get X+Y from these two queries.

    // Let's do that:
    // Query 1: move left by INF and down by INF? But we can only move in one direction per move. We can move left by INF, then move down by INF. That's two moves.
    // Actually, we can move left by INF, then move down by INF. The second move will give us the distance at (X-INF, Y-INF). So we can get d_ld.
    // Similarly, we can move right by INF and up by INF to get d_ru.
    // But we need to return to (X,Y) or keep track of our current position. We don't need to return; we just need to know our current position relative to initial. We can always compute our current position as (X + sum of moves). But we don't know X,Y. However, we can just make moves and remember the total displacement. At the end, we can compute X,Y from the queries and the known displacements.

    // Let's design a sequence of moves:
    // Start at (X,Y).
    // Move left by INF: current = (X-INF, Y). Query gives d1 = f(X-INF, Y).
    // Move down by INF: current = (X-INF, Y-INF). Query gives d2 = f(X-INF, Y-INF).
    // Now we have d2 = 2*INF - X - Y + S_min, where S_min = min_i (xi + yi).
    // So X+Y = 2*INF + S_min - d2.   (Equation 1)

    // Next, we want X-Y or something. We can move right by 2*INF and up by 2*INF? But we are at (X-INF, Y-INF). To get to (X+INF, Y+INF), we need to move right by 2*INF and up by 2*INF. That's two more moves.
    // Move right by 2*INF: current = (X+INF, Y-INF). Query d3 = f(X+INF, Y-INF).
    // Move up by 2*INF: current = (X+INF, Y+INF). Query d4 = f(X+INF, Y+INF).
    // d4 = 2*INF + X + Y + T_min, where T_min = min_i (-xi - yi) = -max_i (xi + yi).
    // So X+Y = d4 - 2*INF - T_min. This should match Equation 1, giving a consistency check.

    // But we still need another equation to separate X and Y. We can get X-Y by moving left and up, or right and down.
    // For example, from (X-INF, Y-INF), we could move right by 2*INF and down by 0? Actually, we can get f(X+INF, Y-INF) = d3.
    // Let's compute d3 = f(X+INF, Y-INF). For large INF, x = X+INF is large positive, y = Y-INF is large negative.
    // Distance to anchor i: |xi - (X+INF)| + |yi - (Y-INF)| = (X+INF - xi) + (yi - Y + INF) = 2*INF + X - Y + (-xi + yi).
    // So d3 = 2*INF + X - Y + min_i (-xi + yi).
    // Let U_min = min_i (-xi + yi). We can compute U_min from the anchor points.
    // Then X - Y = d3 - 2*INF - U_min.   (Equation 2)

    // Now we have X+Y and X-Y, so we can solve for X and Y.
    // We used 4 moves: L INF, D INF, R 2*INF, U 2*INF.
    // But wait: after L INF and D INF, we are at (X-INF, Y-INF). Then R 2*INF moves to (X+INF, Y-INF). Then U 2*INF moves to (X+INF, Y+INF). That's 4 moves.
    // We have up to 10 moves, so 4 is fine.

    // Let's verify the assumptions: We need INF large enough so that for all anchor points, the absolute values simplify as above.
    // For d2: we need X-INF < xi and Y-INF < yi for all i. So INF > max_i xi - X and INF > max_i yi - Y. Since X,Y are in [-1e9, 1e9] and xi,yi in [-1e9, 1e9], the maximum possible value of xi - X is 2e9, and similarly for yi - Y. So INF must be > 2e9. We can choose INF = 3e9? But k is limited to 1e9! Oh no! The problem says 0 <= k <= 1e9. So we cannot move by more than 1e9 in one move.
    // That changes everything. We cannot move by INF = 2e9. The maximum move is 1e9.
    // But coordinates are up to 1e9 in absolute value. So we cannot guarantee that we move far enough to make all xi - (X-INF) positive. For example, if X = 1e9 and we move left by 1e9, we get to 0. Some anchor points could have xi = -1e9, so xi - (X-INF) = -1e9 - 0 = -1e9, which is negative. So the absolute value doesn't simplify to a linear function uniformly.

    // We need a different approach that works with k <= 1e9.

    // Since k <= 1e9, we can move by up to 1e9. The total range of coordinates is [-1e9, 1e9]. So we can move from any point to any other point in at most 2 moves? Actually, the maximum distance in one coordinate is 2e9, so we need two moves of 1e9 to cross the entire range. But we can't move diagonally.

    // New idea: We can find the exact X and Y by binary search on the coordinate axes using the distance function. Since the function is convex and piecewise linear with slopes -1, 0, 1, we can find the point where the slope changes. But we only have 10 moves. Binary search on a range of 2e9 would take about 31 moves. Not enough.

    // But we don't need to find the minimum of f; we need to find the initial point. The initial point is just some point. We can try to find the distance to a specific anchor point by moving to a location where that anchor point is the unique closest? Not guaranteed.

    // Let's reconsider: We know all anchor points. We can compute the function f(x,y) for any (x,y). We are allowed to query f at points (X+dx, Y+dy) for any dx,dy with |dx|,|dy| <= 1e9 per move, but we can chain moves. So we can effectively query f at any point we can reach in a few moves. Since we can move by 1e9 each move, we can reach any point in the plane in at most 4 moves? Actually, from (X,Y), we can move left by up to 1e9, etc. The reachable points in m moves are those with Manhattan distance from (X,Y) <= m * 1e9. With 10 moves, we can reach any point in the square [-1e9, 1e9]^2? The maximum distance from (X,Y) to a point in that square is at most 4e9 (if X=1e9, Y=1e9 and we want to go to -1e9, -1e9, that's 4e9). With 10 moves of 1e9, we can travel 1e10 total Manhattan distance, so we can reach any point easily.

    // So we can query f at any point we want, as long as we can describe the moves. But we don't know (X,Y), so we don't know the absolute coordinates of the points we query. We only know the relative displacement from (X,Y). So we can query f(X+dx, Y+dy) for any dx,dy that we can achieve by a sequence of moves. But we don't know the absolute (X+dx, Y+dy). However, we know the anchor points in absolute coordinates. So if we can deduce the absolute coordinates of some queried point, we can find (X,Y).

    // How to deduce the absolute coordinates? We can try to find a point where f is 0, i.e., an anchor point. If we can reach an anchor point, we will know its absolute coordinates (since they are given), and we know the displacement from (X,Y) to that anchor point (by summing our moves). Then (X,Y) = (anchor - displacement). So the problem reduces to: find any anchor point in at most 10 moves.

    // Can we find an anchor point? We can try to move to the anchor point that minimizes some heuristic. But we don't know which one is closest. However, we can use the fact that we can query the distance. We can perform a kind of gradient descent? Since f is convex, we can move in the direction that decreases f. But we only get the value, not the gradient. We can approximate the gradient by making small moves? But we have limited moves.

    // Alternative: We can find the closest anchor point by moving in the four cardinal directions by 1 and observing the change in f. The change tells us the sign of (X - xi) and (Y - yi) for the closest anchor point. Specifically, if we move left by 1, f decreases by 1 if X > xi for the closest anchor point, increases by 1 if X < xi, and stays the same (or changes by 0?) Actually, if X = xi, moving left by 1 increases the distance by 1 (since |xi - (X-1)| = 1). So the change is +1 if X <= xi, and -1 if X > xi. Wait: |xi - X| - |xi - (X-1)|. If X > xi, then |xi - X| = X - xi, |xi - (X-1)| = X-1 - xi, difference = 1. So moving left decreases distance by 1? Let's recalc: new distance - old distance = (X-1 - xi) - (X - xi) = -1. So distance decreases by 1. If X <= xi, old = xi - X, new = xi - (X-1) = xi - X + 1, difference = +1. So moving left decreases distance if X > xi, increases if X <= xi. So by moving left by 1, we can tell if we are to the right of the closest anchor point's x-coordinate.

    // Similarly, moving right by 1: decreases if X < xi, increases if X >= xi.
    // Moving down by 1: decreases if Y > yi, increases if Y <= yi.
    // Moving up by 1: decreases if Y < yi, increases if Y >= yi.

    // So we can determine the relative position to the closest anchor point. But there might be multiple anchor points at the same distance? The problem says coordinates are pairwise distinct, so no two anchor points are the same. But there could be ties in distance. If there is a tie, the minimum distance is the same, but moving by 1 might change the distance according to the closest points? Actually, if there are multiple closest points, moving by 1 might decrease the distance if all of them are on one side, or increase if all on the other, or stay the same if mixed? The function f is the minimum of distances. If there are multiple points achieving the minimum, moving slightly will change the distance to each. The new minimum will be the minimum of the new distances. The change in f might not be simply ±1. It could be 0 if the new minimum is still determined by a point that didn't change distance? Actually, if there are two points at equal distance, one with xi > X and one with xi < X, moving left by 1 will decrease distance to the one with xi < X (if X > xi) and increase distance to the one with xi > X. The new minimum might be the same as the old minimum? Let's analyze: Suppose d = |x1 - X| + |y1 - Y| = |x2 - X| + |y2 - Y|. If we move left by 1, new distances are d1' and d2'. The new minimum is min(d1', d2'). The change could be -1, 0, or +1. It's not as straightforward.

    // However, we can avoid ties by moving to a point where the closest anchor point is unique. Since the set of points with ties is a set of measure zero (lines), we can perturb. But we don't know the current point.

    // Maybe we can find an anchor point by moving to the point that minimizes f? The minimum of f is 0, achieved at anchor points. The function f is convex. We can find the minimum by ternary search? But it's a 2D function. We can do ternary search on x and y separately? f(x,y) is convex in x for fixed y, and convex in y for fixed x. We can find the x-coordinate of the minimum for a given y by ternary search on x. But we only have 10 moves total. Ternary search on a range of 2e9 would take about 60 moves for 1D. Not feasible.

    // We need a smarter way. Notice that we can move by large amounts (up to 1e9). We can use the fact that the anchor points are given. We can compute the function f(x,y) for any (x,y). We are querying f at points (X+dx, Y+dy). We can choose dx, dy to be such that the queried point is exactly one of the anchor points? But we don't know X,Y, so we don't know what dx,dy to use to land on an anchor point.

    // However, we can try to guess an anchor point and move towards it? Suppose we guess that the closest anchor point is some specific anchor point i. We can move left by some amount to try to make X - dx = xi? But we don't know X.

    // Wait, we can use the distance queries to compute the exact distance to a specific anchor point if we can isolate it. How to isolate? If we move far in one direction, the closest anchor point will be the one that minimizes some linear function. But as we saw, we can't move far enough to guarantee that all xi - (X+dx) have the same sign, because max move is 1e9 and coordinates range is 2e9.

    // But we can move to the corners of the possible range. The initial (X,Y) is in [-1e9, 1e9]. The anchor points are also in that range. If we move left by 1e9, our x-coordinate becomes X - 1e9. Since X >= -1e9, X - 1e9 >= -2e9. The anchor points have xi >= -1e9. So it's possible that some anchor points have xi < X - 1e9? No, xi >= -1e9, and X - 1e9 >= -2e9. So xi could be greater or smaller. For example, X = -1e9, move left by 1e9 -> x = -2e9. Then all xi >= -1e9 > -2e9, so xi > x. So for that move, xi - x > 0 for all i. So |xi - x| = xi - x. That simplifies! If we can ensure that after a move, our x-coordinate is less than all xi, then the absolute value simplifies. Similarly, if we move right by 1e9, our x-coordinate could be greater than all xi if X + 1e9 > 1e9 >= max xi. But max xi is 1e9. So if X + 1e9 > 1e9, i.e., X > 0, then x > all xi. But if X <= 0, X + 1e9 <= 1e9, so there might be xi = 1e9 which is equal or greater. So we cannot guarantee that we can make x greater than all xi with one move, because X could be -1e9, then X+1e9 = 0, and max xi = 1e9 > 0.

    // But we can make two moves: move left by 1e9, then left by another 1e9? That would be total left 2e9, but k <= 1e9 per move. We can move left by 1e9 twice. That would bring x to X - 2e9. Since X >= -1e9, X - 2e9 <= -1e9 - 2e9 = -3e9? Wait, X >= -1e9, so X - 2e9 >= -3e9. But xi >= -1e9, so xi > X - 2e9 for all i. So after two left moves of 1e9 each, we are guaranteed to be to the left of all anchor points. Similarly, two right moves of 1e9 guarantee we are to the right of all anchor points. Two down moves guarantee below all, two up moves guarantee above all.

    // So we can reach the "far left" region in 2 moves, "far right" in 2 moves, etc. But we have only 10 moves. We can do:
    // Move left by 1e9, move left by 1e9: now x = X - 2e9, which is < all xi. y = Y.
    // Then we can query f at this point. Let's call this point A: (X - 2e9, Y).
    // At point A, for all i, xi > x, so |xi - x| = xi - x = xi - X + 2e9.
    // So f(A) = min_i (xi - X + 2e9 + |yi - Y|) = 2e9 - X + min_i (xi + |yi - Y|).
    // This still depends on Y through |yi - Y|. We need to also make y far down or up to simplify the y term.

    // If we then move down by 1e9 twice, we get to (X - 2e9, Y - 2e9). At this point, y < all yi? yi >= -1e9, Y - 2e9 >= -1e9 - 2e9 = -3e9, so yi > y for all i. So |yi - y| = yi - y = yi - Y + 2e9.
    // Then f(B) = min_i (xi - X + 2e9 + yi - Y + 2e9) = 4e9 - X - Y + min_i (xi + yi).
    // This is exactly what we had earlier, but with INF = 2e9. And we achieved it in 4 moves (L, L, D, D). But wait, we can move by 1e9 each time, so we need two left moves and two down moves. That's 4 moves. Then we have f(B) = 4e9 - X - Y + S_min, where S_min = min_i (xi + yi). We know S_min.
    // So we get X + Y = 4e9 + S_min - f(B).   (Eq1)

    // Now we need another equation. We can move to the far right and far up. From B, we are at (X - 2e9, Y - 2e9). We can move right by 4e9? But max move is 1e9, so we need four moves of 1e9 right to get to X + 2e9. Then up by 4e9 to get to Y + 2e9. That would be 8 more moves, total 12 > 10. Not allowed.

    // But we don't need to go to the opposite corner. We can get X - Y from a different combination. For example, from B, we can move right by 4e9 (4 moves) to get to (X + 2e9, Y - 2e9). At that point, x > all xi (since X + 2e9 >= -1e9 + 2e9 = 1e9, and max xi = 1e9, so x >= max xi; actually if X = -1e9, X+2e9 = 1e9, which is equal to max possible xi. If xi = 1e9, then x = xi, so |xi - x| = 0, not simplified to x - xi. To guarantee x > all xi, we need X + 2e9 > 1e9, i.e., X > -1e9. If X = -1e9, then X+2e9 = 1e9, which is not strictly greater than max xi (which could be 1e9). So we might have equality. To avoid equality, we can move right by 2e9 + something? But we only have 1e9 moves. We can move right by 1e9 three times? That would be X - 2e9 + 3e9 = X + 1e9. Max xi = 1e9. If X = -1e9, X+1e9 = 0 < 1e9. So not greater. We need to move right enough so that x > 1e9. Since max xi = 1e9, we need x > 1e9. x = X + total_right. We want X + total_right > 1e9 => total_right > 1e9 - X. Since X >= -1e9, 1e9 - X <= 2e9. So total_right = 2e9 + 1? But we can only move in multiples of 1e9? No, we can move by any k up to 1e9. We can move by 1e9, then by 1e9, then by 1? That would be 2e9+1. But we have limited moves. We can do: from B, move right by 1e9 (1 move), then right by 1e9 (2nd move), then right by 1 (3rd move). That's 3 moves to get to x = X - 2e9 + 2e9 + 1 = X + 1. Wait, that's not enough. Let's recalc: B is at x = X - 2e9. We want to get to x > 1e9. We need to add more than 1e9 - X + 2e9? Actually, we want x > max xi = 1e9. So X - 2e9 + R > 1e9 => R > 3e9 - X. Since X >= -1e9, 3e9 - X <= 4e9. So we need R > 4e9? That's too many moves.

    // This approach of moving to extremes to simplify absolute values seems to require many moves because the range is 2e9 and we can only move 1e9 at a time. To get to a point where all xi are on one side, we need to move by at least 2e9 in one direction, which takes at least 3 moves (since 1e9 * 2 = 2e9, but we need strictly greater/less, so maybe 2e9+1, requiring 3 moves). And to do that for both x and y, we need 6 moves just to get to one corner. Then to get another equation we need to go to another corner, requiring many more moves.

    // We need a different strategy that uses the fact that we can query at any point, and we know the anchor points. Maybe we can find X and Y by solving a system of equations using queries that are not
