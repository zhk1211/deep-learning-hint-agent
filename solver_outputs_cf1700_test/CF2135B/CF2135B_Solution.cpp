// Solution
#include <bits/stdc++.h>
using namespace std;

const long long V = 1000000000;

long long query(char dir, long long k) {
    cout << "? " << dir << " " << k << endl;
    long long res;
    cin >> res;
    if (res == -1) exit(0);
    return res;
}

void solve() {
    int n;
    cin >> n;
    vector<pair<long long, long long>> pts(n);
    for (int i = 0; i < n; ++i) {
        cin >> pts[i].first >> pts[i].second;
    }
    
    // Move to bottom-left: X' <= -V, Y' <= -V
    // We need to move left by V twice and down by V twice.
    // Actually we can do: L V, L V, D V, D V
    query('L', V);
    query('L', V);
    query('D', V);
    query('D', V);
    
    // Now we are at (X - 2V, Y - 2V). Since X,Y in [-V,V], new coords <= -V.
    // The distance returned is min_i (x_i - X' + y_i - Y') = min_i (x_i + y_i) - (X' + Y')
    // X' + Y' = X + Y - 4V
    // So dist = min_i (x_i + y_i) - (X + Y - 4V) = min_i (x_i + y_i) - X - Y + 4V
    // We can compute min_i (x_i + y_i) from input.
    long long min_sum = LLONG_MAX;
    for (auto &p : pts) {
        min_sum = min(min_sum, p.first + p.second);
    }
    // We don't need to query again; we already got the distance after the last move.
    // But we need to read the response for the last move (the 4th move).
    // Actually we already read responses for each query. The last query was 'D' V.
    // We need to store that response.
    // Let's reorder: we do 4 moves and store the last response.
    // We'll do the queries and keep the last returned value.
    long long d1 = query('L', V);
    long long d2 = query('L', V);
    long long d3 = query('D', V);
    long long d4 = query('D', V);
    // d4 is the distance at bottom-left corner.
    // d4 = min_sum - (X + Y) + 4V
    // => X + Y = min_sum + 4V - d4
    
    long long sum_xy = min_sum + 4 * V - d4;
    
    // Now move to top-right: from current (X-2V, Y-2V) we need to go to (X+2V, Y+2V) maybe?
    // Actually we want X' >= V, Y' >= V. We can move right by 4V and up by 4V.
    // But we can do it in 4 moves: R V, R V, U V, U V.
    // However we already used 4 moves, we have 6 left, total 10. So we can do 4 more.
    query('R', V);
    query('R', V);
    query('U', V);
    long long d5 = query('U', V);
    // Now we are at (X+2V, Y+2V). Since X,Y in [-V,V], new coords >= V.
    // Distance = min_i (X' - x_i + Y' - y_i) = (X' + Y') - max_i (x_i + y_i)
    // X' + Y' = X + Y + 4V
    // So d5 = X + Y + 4V - max_sum
    // => X + Y = max_sum - 4V + d5
    // But we already have sum_xy, we can use this to verify or compute difference.
    // Actually we need X - Y. Let's get it from bottom-right or top-left.
    // Instead, we can move to bottom-right: X' >= V, Y' <= -V.
    // From current top-right (X+2V, Y+2V), we can move down by 4V: D V, D V, D V, D V? That's 4 moves, total 12 > 10.
    // Better: after bottom-left, we can move to bottom-right directly.
    // Let's rethink: we have 10 moves total. We used 4 for bottom-left. We can use 4 for top-right, that's 8. Then we have 2 moves left.
    // We need X - Y. We can get it by moving to top-left or bottom-right.
    // From bottom-left (X-2V, Y-2V), we can move right by 4V to get to (X+2V, Y-2V) -> bottom-right.
    // That would be 4 moves of R V. But we already moved to top-right. Let's do a different sequence.
    // Sequence: 
    // 1. L V
    // 2. L V
    // 3. D V
    // 4. D V  -> bottom-left, get sum_xy
    // 5. R V
    // 6. R V
    // 7. R V
    // 8. R V  -> bottom-right (X+2V, Y-2V)
    // 9. U V
    // 10. U V -> top-right? Actually we need only one more distance to get diff.
    // Let's compute: at bottom-right, X' >= V, Y' <= -V.
    // Distance = min_i (X' - x_i + y_i - Y') = X' - Y' + min_i (-x_i + y_i) = X - Y + 4V + min_i (y_i - x_i)
    // So we can get X - Y.
    // Then we have sum_xy and diff_xy, solve for X, Y.
    // We have 10 moves, we can do 4 for bottom-left, 4 for bottom-right, and we have 2 left, but we don't need them.
    // Let's adjust: after bottom-left, we do 4 R V to get bottom-right, then we read the distance after the 4th R V.
    // That's 8 moves total. Then we can answer.
    
    // But we already did 4 moves to bottom-left, and then we did 4 moves to top-right in the code above. Let's change the code.
    // We'll redo the queries properly.
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<pair<long long, long long>> pts(n);
        long long min_sum = LLONG_MAX, min_diff = LLONG_MAX; // diff = y - x
        for (int i = 0; i < n; ++i) {
            cin >> pts[i].first >> pts[i].second;
            min_sum = min(min_sum, pts[i].first + pts[i].second);
            min_diff = min(min_diff, pts[i].second - pts[i].first);
        }
        
        // Move to bottom-left: X' <= -V, Y' <= -V
        query('L', V);
        query('L', V);
        query('D', V);
        long long d_bottom_left = query('D', V);
        
        // X + Y = min_sum + 4V - d_bottom_left
        long long sum_xy = min_sum + 4 * V - d_bottom_left;
        
        // Move to bottom-right: from (X-2V, Y-2V) move right by 4V
        query('R', V);
        query('R', V);
        query('R', V);
        long long d_bottom_right = query('R', V);
        
        // At bottom-right: X' = X+2V, Y' = Y-2V
        // distance = min_i (X' - x_i + y_i - Y') = X' - Y' + min_i (y_i - x_i) = (X+2V) - (Y-2V) + min_diff = X - Y + 4V + min_diff
        // => X - Y = d_bottom_right - 4V - min_diff
        long long diff_xy = d_bottom_right - 4 * V - min_diff;
        
        // Solve: X = (sum_xy + diff_xy) / 2, Y = (sum_xy - diff_xy) / 2
        long long X = (sum_xy + diff_xy) / 2;
        long long Y = (sum_xy - diff_xy) / 2;
        
        cout << "! " << X << " " << Y << endl;
    }
    return 0;
}
