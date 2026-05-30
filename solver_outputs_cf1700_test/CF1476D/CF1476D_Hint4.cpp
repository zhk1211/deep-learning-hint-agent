// Hint4
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    string s;
    cin >> s;
    
    // We'll compute the answer for each starting city i (0..n)
    // The key insight: the graph of possible moves is undirected if we consider
    // that after each move, directions flip. Actually, the traveler can move
    // back and forth, effectively making certain edges traversable in both
    // directions over two steps. This leads to connected components in an
    // undirected graph where edges exist if the original road allows movement
    // in a specific alternating pattern.
    
    // For each road i (between i-1 and i), we can traverse it from left to right
    // if s[i-1] == 'R', and from right to left if s[i-1] == 'L'.
    // But after moving, all directions flip. So if we move from i-1 to i using
    // an 'R' road, then next step we can move back from i to i-1 because the road
    // becomes 'L'. So effectively, we can always go back and forth on any road,
    // but the initial direction determines which side we can start from.
    
    // Actually, the problem reduces to: from a starting city, we can move along
    // a road if its current direction matches our travel direction. After moving,
    // all directions flip. This is equivalent to: we can traverse a road if
    // the original direction is R and we are moving right, or L and moving left.
    // But after flipping, the road becomes opposite, so we can traverse it back.
    // So the reachable set is a contiguous segment around the start, determined
    // by alternating L/R patterns.
    
    // Let's compute for each city the maximum left and right reach.
    // We can precompute left reach and right reach using DP.
    
    vector<int> left(n + 1, 0), right(n + 1, 0);
    
    // left[i] = how many steps we can go left from city i
    // To go left from i to i-1, we need road i to be 'L' (since we are moving left).
    // After moving, directions flip, so next step left would require road i-1 to be 'R' originally? 
    // Let's think: start at i, move left to i-1 using road i (must be 'L').
    // Now all roads flip. To move further left from i-1 to i-2, we need road i-1 to be 'L' after flip,
    // which means originally it was 'R'. So pattern: L, R, L, R, ... for leftward movement.
    // So left reach depends on alternating pattern starting with 'L' at position i.
    
    // Similarly for right: start at i, move right to i+1 using road i+1 (must be 'R').
    // Then flip, next right move needs road i+2 to be 'R' after flip -> originally 'L'.
    // Pattern: R, L, R, L, ...
    
    // We can compute left reach from left to right:
    // left[0] = 0
    // For i from 1 to n:
    //   if s[i-1] == 'L':
    //       if i-2 >= 0 and s[i-2] == 'R':
    //           left[i] = left[i-2] + 2; // wait, careful.
    // Actually, we can define arrays that store the maximum steps we can go in one direction.
    
    // Let's compute consecutive alternating segments.
    // We can precompute for each position the length of alternating sequence starting with L or R.
    
    // For leftward: we need s[i-1] == 'L' to start. Then next needed is s[i-2] == 'R', then s[i-3] == 'L', etc.
    // So we can compute an array left_len[i] = max steps left from i.
    // left_len[0] = 0, left_len[1] = (s[0] == 'L' ? 1 : 0)
    // For i >= 2: if s[i-1] == 'L' and s[i-2] == 'R', then left_len[i] = left_len[i-2] + 2;
    // else if s[i-1] == 'L' then left_len[i] = 1; else 0.
    
    vector<int> left_len(n + 1, 0);
    left_len[0] = 0;
    if (n >= 1) left_len[1] = (s[0] == 'L' ? 1 : 0);
    for (int i = 2; i <= n; ++i) {
        if (s[i-1] == 'L') {
            if (s[i-2] == 'R') {
                left_len[i] = left_len[i-2] + 2;
            } else {
                left_len[i] = 1;
            }
        } else {
            left_len[i] = 0;
        }
    }
    
    // For rightward: we need s[i] == 'R' to start (road i+1). Then s[i+1] == 'L', etc.
    // Compute right_len[i] = max steps right from i.
    vector<int> right_len(n + 1, 0);
    right_len[n] = 0;
    if (n >= 1) right_len[n-1] = (s[n-1] == 'R' ? 1 : 0);
    for (int i = n-2; i >= 0; --i) {
        if (s[i] == 'R') {
            if (s[i+1] == 'L') {
                right_len[i] = right_len[i+2] + 2;
            } else {
                right_len[i] = 1;
            }
        } else {
            right_len[i] = 0;
        }
    }
    
    // Answer for city i: 1 (starting city) + left_len[i] + right_len[i]
    for (int i = 0; i <= n; ++i) {
        cout << 1 + left_len[i] + right_len[i] << (i == n ? '\n' : ' ');
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
