#include <bits/stdc++.h>
using namespace std;

const long long V = 1000000000;

void solve() {
    int n;
    cin >> n;
    vector<long long> x(n), y(n);
    for (int i = 0; i < n; ++i) {
        cin >> x[i] >> y[i];
    }

    // Move to bottom-left corner: X' <= -V, Y' <= -V
    cout << "? L " << V << endl;
    long long s1;
    cin >> s1;
    if (s1 == -1) exit(0);
    cout << "? L " << V << endl;
    long long s2;
    cin >> s2;
    if (s2 == -1) exit(0);
    cout << "? D " << V << endl;
    long long s3;
    cin >> s3;
    if (s3 == -1) exit(0);
    cout << "? D " << V << endl;
    long long s4;
    cin >> s4;
    if (s4 == -1) exit(0);

    // After these moves, current coordinates are (X - 2V, Y - 2V)
    // The response s4 is min_i ( (X - 2V - x_i) + (Y - 2V - y_i) ) because X-2V <= -V <= x_i? Wait, we need to ensure X-2V <= x_i and Y-2V <= y_i.
    // Actually, after two L and two D, X' = X - 2V, Y' = Y - 2V.
    // Since X, Y >= -V, X' <= -V, Y' <= -V. And all anchor points have coordinates >= -V? No, anchor points can be as low as -V.
    // But the editorial says: after moving to bottom-left, we have X', Y' <= -V, so for any anchor point (x_i, y_i), we have X' <= x_i and Y' <= y_i? Not necessarily, anchor points can be less than -V? No, anchor points are in [-V, V].
    // So X' <= -V <= x_i? Actually x_i can be -V, so X' <= -V <= x_i is true because X' <= -V and x_i >= -V. So X' <= x_i holds. Similarly Y' <= y_i.
    // Thus |x_i - X'| + |y_i - Y'| = (x_i - X') + (y_i - Y') = (x_i + y_i) - (X' + Y').
    // So s4 = min_i (x_i + y_i) - (X' + Y').
    // X' + Y' = X + Y - 4V.
    // So s4 = min_i (x_i + y_i) - (X + Y - 4V) = min_i (x_i + y_i) - X - Y + 4V.
    // Thus X + Y = min_i (x_i + y_i) + 4V - s4.

    long long min_sum = LLONG_MAX;
    for (int i = 0; i < n; ++i) {
        min_sum = min(min_sum, x[i] + y[i]);
    }
    long long X_plus_Y = min_sum + 4 * V - s4;

    // Now move to top-right corner: X' >= V, Y' >= V
    // We are currently at (X - 2V, Y - 2V). We need to go to (X + 2V, Y + 2V) or similar.
    // Move right 4V and up 4V? But we have only 4 moves left (total 8). We can do R 4V and U 4V.
    // But k <= 1e9, 4V = 4e9 > 1e9. So we cannot move 4V in one step.
    // Instead, we can move right V multiple times? We have 4 moves left. We can do R V, R V, U V, U V.
    // That would bring us to (X - 2V + 2V, Y - 2V + 2V) = (X, Y). But we need to be in top-right corner (X' >= V, Y' >= V).
    // Actually, from (X - 2V, Y - 2V), if we move R V, R V, U V, U V, we end up at (X, Y). That's not top-right corner.
    // The editorial says: "Similarly, if we move to the top-right corner (or the bottom-left one), we can know the value of X - Y. It would take 4 extra steps of moving down or moving right (instead of 4 + 4)."
    // Wait, we already used 4 moves to go to bottom-left. Now we need to go to top-right. But we only have 4 moves left. How?
    // The editorial says: "Let's first use four operations (L, V), (L, V), (U, V), and (U, V)." That's 4 moves to go to bottom-left? No, (L, V), (L, V) moves left 2V, (U, V), (U, V) moves up 2V? Wait, (U, V) is up, not down. The editorial says (L, V), (L, V), (U, V), (U, V). That would move left 2V and up 2V, ending at (X - 2V, Y + 2V). That's top-left corner, not bottom-left.
    // Let's re-read: "Let's first use four operations (L, V), (L, V), (U, V), and (U, V). Since initially -V <= X, Y <= V, after the four operations, it's guaranteed that X', Y' <= -V." Wait, if we move up, Y increases. So Y' = Y + 2V. Since Y >= -V, Y' >= V. That's not <= -V. There's a contradiction.
    // Maybe the editorial has a typo? It says "X', Y' <= -V" but moving up makes Y' larger. Perhaps they meant (L, V), (L, V), (D, V), (D, V) for bottom-left. But then they say "Similarly, if we move to the top-right corner (or the bottom-left one)". So they use bottom-left and top-right. The example in the note uses D and L. So bottom-left is correct.
    // Let's assume the editorial meant: first 4 moves: L V, L V, D V, D V to go to bottom-left. Then we have 4 moves left. To go to top-right, we need to move right 4V and up 4V. But we can't do that in one move. We can do R V, R V, U V, U V? That only moves 2V right and 2V up, ending at (X, Y). Not top-right.
    // Wait, maybe we don't need to physically go to top-right. We can use the responses from the moves to deduce X - Y. How?
    // After the first 4 moves, we are at (X - 2V, Y - 2V). We have s4 = min_i (x_i + y_i) - (X + Y) + 4V.
    // Now we make 4 more moves. Suppose we move R V, R V, U V, U V. Then we end at (X, Y). The response after the last move would be min_i |x_i - X| + |y_i - Y|. But we don't know X and Y. That doesn't directly give X - Y.
    // Alternatively, we can move to top-right by doing R 4V and U 4V, but k is limited to 1e9. 4V = 4e9 > 1e9. So we can't.
    // Maybe we can move to top-right in 4 moves by doing R V, R V, U V, U V? That only gets us to (X, Y). To get to top-right (X + 2V, Y + 2V), we need 4V right and 4V up from bottom-left. That's 8 moves total if we do V each. But we only have 4 moves left.
    // Perhaps the initial 4 moves are not all V. Maybe we can move by 2V in one move? k <= 1e9, V = 1e9, so 2V = 2e9 > 1e9. Not allowed.
    // Let's re-read the editorial carefully: "Let's first use four operations (L, V), (L, V), (U, V), and (U, V). Since initially -V <= X, Y <= V, after the four operations, it's guaranteed that X', Y' <= -V." This is clearly wrong if U is up. Maybe U means down? No, U is up.
    // Perhaps the coordinate system is different? Or maybe they meant (L, V), (L, V), (D, V), (D, V) but wrote U by mistake. The note in the problem uses D and L. So bottom-left is correct.
    // Let's think: We want to get to a state where X' <= -V and Y' <= -V. That requires moving left at least V + X and down at least V + Y. Since X, Y >= -V, V+X >= 0, V+Y >= 0. The maximum needed is 2V. So two moves of V each for left and down suffices. So 4 moves: L V, L V, D V, D V.
    // Now we are at (X - 2V, Y - 2V). We have X' <= -V, Y' <= -V.
    // Now we want to get to a state where X' >= V and Y' >= V. That requires moving right 4V and up 4V from current position. That's 8 moves of V each. We only have 4 moves left. So we can't.
    // But the editorial says: "It would take 4 extra steps of moving down or moving right (instead of 4 + 4)." This suggests we don't need to go all the way to top-right. Maybe we can go to a different corner using the remaining 4 moves.
    // Wait: "Similarly, if we move to the top-right corner (or the bottom-left one), we can know the value of X - Y." They mention bottom-left again? That's confusing.
    // Let's parse: "Let's first use four operations (L, V), (L, V), (U, V), and (U, V)." Maybe it's (L, V), (L, V), (D, V), (D, V) for bottom-left, and then "Similarly, if we move to the top-right corner (or the bottom-left one)" might mean we can also get X - Y by moving to top-right or bottom-left? No.
    // Another interpretation: The first 4 moves are to go to a corner where both coordinates are <= -V. That gives X+Y. Then we can use the next 4 moves to go to a corner where one coordinate is <= -V and the other >= V, which gives X - Y. How? If we go to (X - 2V, Y + 2V) (top-left), then X' <= -V, Y' >= V. Then |x_i - X'| + |y_i - Y'| = (x_i - X') + (Y' - y_i) = (x_i - y_i) - (X' - Y'). So we get X' - Y' = X - Y - 4V. Then we can solve for X - Y.
    // To get to top-left from bottom-left (X - 2V, Y - 2V), we need to move up 4V. That's 4 moves of U V. So total moves: 4 (to bottom-left) + 4 (to top-left) = 8 moves. That works!
    // Let's check: Start at (X, Y). Move L V, L V, D V, D V -> (X - 2V, Y - 2V). Get s4 = min_sum - (X+Y) + 4V.
    // Then move U V, U V, U V, U V -> (X - 2V, Y + 2V). Get s8 = min_i ( |x_i - (X - 2V)| + |y_i - (Y + 2V)| ).
    // Since X - 2V <= -V <= x_i, and Y + 2V >= V >= y_i, we have |x_i - X'| = x_i - X', |y_i - Y'| = Y' - y_i.
    // So s8 = min_i (x_i - X' + Y' - y_i) = min_i (x_i - y_i) - (X' - Y') = min_i (x_i - y_i) - (X - 2V - Y - 2V) = min_i (x_i - y_i) - (X - Y) + 4V.
    // Thus X - Y = min_i (x_i - y_i) + 4V - s8.
    // Then we have X+Y and X-Y, solve for X and Y.
    // This uses exactly 8 moves. Perfect!

    // So the plan:
    // Moves 1-4: L V, L V, D V, D V. After 4th move, we get s4.
    // Moves 5-8: U V, U V, U V, U V. After 8th move, we get s8.
    // Then compute and output answer.

    // But wait: The problem says we can make up to 10 moves. We use 8. The sample uses 3 moves in second test case, but that's just an example. Our 8 moves are within limit.

    // Let's implement exactly that.

    // We already did L V, L V, D V, D V and got s4.
    // Now do U V four times.
    cout << "? U " << V << endl;
    long long s5;
    cin >> s5;
    if (s5 == -1) exit(0);
    cout << "? U " << V << endl;
    long long s6;
    cin >> s6;
    if (s6 == -1) exit(0);
    cout << "? U " << V << endl;
    long long s7;
    cin >> s7;
    if (s7 == -1) exit(0);
    cout << "? U " << V << endl;
    long long s8;
    cin >> s8;
    if (s8 == -1) exit(0);

    long long min_diff = LLONG_MAX;
    for (int i = 0; i < n; ++i) {
        min_diff = min(min_diff, x[i] - y[i]);
    }
    long long X_minus_Y = min_diff + 4 * V - s8;

    long long X = (X_plus_Y + X_minus_Y) / 2;
    long long Y = (X_plus_Y - X_minus_Y) / 2;

    cout << "! " << X << " " << Y << endl;
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
