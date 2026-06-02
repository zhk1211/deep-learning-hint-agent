#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<vector<long long>> h(n, vector<long long>(n));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cin >> h[i][j];
            }
        }
        vector<long long> a(n), b(n);
        for (int i = 0; i < n; i++) cin >> a[i];
        for (int j = 0; j < n; j++) cin >> b[j];
        
        // Check if already beautiful
        bool beautiful = true;
        for (int i = 0; i < n && beautiful; i++) {
            for (int j = 0; j < n - 1; j++) {
                if (h[i][j] == h[i][j + 1]) {
                    beautiful = false;
                    break;
                }
            }
        }
        for (int i = 0; i < n - 1 && beautiful; i++) {
            for (int j = 0; j < n; j++) {
                if (h[i][j] == h[i + 1][j]) {
                    beautiful = false;
                    break;
                }
            }
        }
        if (beautiful) {
            cout << "0\n";
            continue;
        }
        
        // We need to hire exactly n workers (one per row or one per column)
        // or it's impossible.
        // There are two cases: all n from A, or all n from B.
        // Actually, we can also mix? But hints say: if we can't with < n workers, we can't at all.
        // And if we hire n workers all from A, what happens?
        // Let's analyze: hiring worker i from A adds 1 to row i.
        // If we hire all rows, each row gets +1, so all heights increase by 1.
        // Then adjacent differences remain the same! So if it wasn't beautiful before, it won't be after.
        // Similarly for all columns.
        // So we need to hire exactly n workers, but not all from the same company.
        // Actually, we can hire some from A and some from B, total n.
        // Let's think: each row and each column can be incremented at most once.
        // We need to choose a set of rows R and a set of columns C such that |R| + |C| = n.
        // Then for each cell (i,j), new height = h[i][j] + (i in R) + (j in C).
        // We need no adjacent equal heights.
        // Consider two adjacent cells horizontally: (i,j) and (i,j+1).
        // Their new heights: H1 = h[i][j] + r_i + c_j, H2 = h[i][j+1] + r_i + c_{j+1}.
        // We need H1 != H2 => h[i][j] + c_j != h[i][j+1] + c_{j+1}.
        // So c_j - c_{j+1} != h[i][j+1] - h[i][j] for all i.
        // This must hold for all rows i. That means the difference c_j - c_{j+1} must avoid all values D_i = h[i][j+1] - h[i][j] for i=1..n.
        // Similarly for vertical: r_i - r_{i+1} != h[i+1][j] - h[i][j] for all j.
        // Since r_i, c_j are 0 or 1 (we either hire or not), the differences can be -1, 0, 1.
        // So we need to assign 0/1 to r_i and c_j such that |R|+|C|=n and these difference constraints hold.
        // This is like a 2-coloring problem on a bipartite graph? Let's see.
        // Let x_i = r_i, y_j = c_j. We need x_i - x_{i+1} != V_{i,j} for all j, where V_{i,j} = h[i+1][j] - h[i][j].
        // And y_j - y_{j+1} != H_{i,j} for all i, where H_{i,j} = h[i][j+1] - h[i][j].
        // Since x_i, y_j in {0,1}, the differences are in {-1,0,1}.
        // If for some adjacent pair, the required difference to avoid is not in {-1,0,1}, then the constraint is automatically satisfied (since we can't produce that difference).
        // But if it is in {-1,0,1}, we must ensure our assignment doesn't produce that exact difference.
        // This is similar to a system of constraints: we can think of it as we need to choose x_i, y_j to avoid certain forbidden differences.
        // Notice that if we flip all bits (0->1, 1->0), the differences remain the same. So the set of solutions is symmetric.
        // Also, the total number of ones must be n.
        // Let's try to model as: we need to assign each row and column a value 0 or 1.
        // The constraints are: for each adjacent row pair (i,i+1), for each column j, we cannot have x_i - x_{i+1} = V_{i,j} if V_{i,j} in {-1,0,1}.
        // This means if there exists a column j where V_{i,j} = d, then we cannot have x_i - x_{i+1} = d.
        // So for a fixed row pair, we might have multiple forbidden differences from different columns.
        // Since d can be -1,0,1, if all three are forbidden, then no assignment for x_i, x_{i+1} works -> impossible.
        // Similarly for columns.
        // This suggests we can treat rows and columns separately? But they are coupled through the total sum constraint.
        // Wait, the constraints for rows involve only x_i, and for columns involve only y_j. They are independent except for the total sum |R|+|C| = n.
        // So we can find all valid assignments for rows (x_1..x_n) that satisfy all vertical constraints, and similarly for columns (y_1..y_n) satisfying horizontal constraints.
        // Then we need to pick one row assignment and one column assignment such that sum(x) + sum(y) = n, minimizing cost sum(a_i x_i) + sum(b_j y_j).
        // Since n <= 1000, we can try to find all possible row assignments? But there could be many.
        // However, the constraints are local: x_i and x_{i+1} cannot have certain differences.
        // This is like a 2-state DP on a line. For rows, we can compute DP to find if there is any valid assignment, and also the minimum cost for each possible sum of x_i.
        // Similarly for columns.
        // Then we can combine.
        // Let's formalize:
        // For rows: we have variables x_1..x_n in {0,1}.
        // For each i from 1 to n-1, we have a set of forbidden differences F_i subset of {-1,0,1}.
        // F_i = { h[i+1][j] - h[i][j] for j=1..n } intersected with {-1,0,1}.
        // We need x_i - x_{i+1} not in F_i.
        // This is equivalent to: if 0 in F_i, then x_i != x_{i+1}. If 1 in F_i, then we cannot have (x_i=1, x_{i+1}=0). If -1 in F_i, then we cannot have (x_i=0, x_{i+1}=1).
        // So it's a set of allowed transitions between x_i and x_{i+1}.
        // We can do DP: dp[i][val] = minimum cost for first i rows ending with x_i = val, and we can also track the number of ones (sum) if we want to combine later.
        // But we need to know the possible sums for rows. Since n <= 1000, we can keep a bitset or boolean array of achievable sums with min cost? Actually we need min cost for each possible sum.
        // We can do DP with state (i, val, sum) -> min cost. That's O(n^2) which is 1e6, acceptable.
        // Similarly for columns.
        // Then we iterate over possible sum_r and sum_c such that sum_r + sum_c = n, and take min cost_r[sum_r] + cost_c[sum_c].
        // If no such pair, output -1.
        // But wait: is it always true that we must hire exactly n workers? The hints say: if we can't with less than n, we can't at all. And if we hire n all from A, it doesn't change differences. So we need exactly n workers, and not all from one company? Actually, if we hire n workers all from A, the differences remain same, so if original was not beautiful, it remains not. So we need at least one from the other company? But the total must be n. Could it be that we hire n-1 from A and 1 from B? That sums to n. So yes, total n.
        // But is it possible that we need more than n? No, because each row/column can be incremented at most once, and there are only n rows and n columns. If we hire more than n, by pigeonhole, some row or column gets hired twice? But we can't hire same worker twice. So max is 2n? Wait, we can hire at most one worker per row and one per column, so max total workers is 2n. But the hints say: "What is the maximum number of workers we need to hire?" and "How many workers do we need to hire at least?" and "If we can't make the city beautiful with less than n workers, we can't make it beautiful at all." This suggests that the answer is always either 0, or exactly n workers, or impossible. Let's check: Could we hire 2n workers? That would increment every row and every column by 1. Then each cell gets +2. Differences remain the same. So if original not beautiful, still not. So hiring all 2n doesn't help. What about n+1? Suppose we hire all rows and one column. Then each cell in that column gets +2, others +1. This changes differences. But the hints say if we can't with less than n, we can't at all. So the minimum number of workers needed is either 0 or exactly n. Let's test with a small example: n=2, grid: 1 1 / 1 1. Not beautiful. Can we make it beautiful with 2 workers? If we hire row1 and col1: grid becomes 2 2 / 2 1? Actually row1+1, col1+1: (1,1)->2, (1,2)->2? Wait: h[1][1]=1 +1(row1)+1(col1)=3? No: row1 adds to all columns, col1 adds to all rows. So h[1][1] gets +2, h[1][2] gets +1 (only row1), h[2][1] gets +1 (only col1), h[2][2] gets 0. So grid: 3 2 / 2 1. Adjacent: 3!=2, 2!=1, 3!=2, 2!=1. Beautiful! So 2 workers worked. What about 1 worker? Hire row1: grid becomes 2 2 / 1 1 -> adjacent 2=2 horizontally, not beautiful. Hire col1: 2 1 / 2 1 -> vertical 2=2. So 1 worker fails. So min is 2 = n. What about 3 workers? Hire both rows and col1: row1+1, row2+1, col1+1. Grid: 1 1 / 1 1 -> row1: 2 2, row2: 2 2, col1: +1 to both rows col1 -> 3 2 / 3 2. Not beautiful (vertical 3=3). So 3 workers fails. So indeed, only 0 or n workers can work? Let's check if there is a case where we need n+something? Suppose n=3, grid all 1. Not beautiful. Try 3 workers: e.g., hire row1, row2, col1. Then grid: row1+1, row2+1, col1+1. Original all 1. After: row1: 2 2 2? Actually row1 gets +1 from row1, +1 from col1 on col1 -> col1: 3, others: 2. row2: similar: col1: 3, others: 2. row3: only col1 hired -> col1: 2, others: 1. Grid:
        // 3 2 2
        // 3 2 2
        // 2 1 1
        // Check adjacent: row1: 3!=2, 2==2 -> fail. So 3 workers not enough. Try 4 workers? But hints say if can't with <n, can't at all. So maybe for all 1 grid, it's impossible? Let's test: n=3 all 1. Can we make it beautiful? We need no two adjacent equal. Since all start equal, we need to add different amounts to adjacent cells. Each row/col adds 1 to a whole line. This is like we have a matrix of increments: increment[i][j] = r_i + c_j. We need h[i][j]+r_i+c_j != h[i][j+1]+r_i+c_{j+1} => c_j - c_{j+1} != 0 (since h equal). So we need c_j != c_{j+1} for all j. That means columns must alternate 0,1,0,1,... So c must be either 0,1,0 or 1,0,1. Similarly, rows must alternate. So r must be alternating. Then total sum of r + sum of c = ? For n=3, alternating rows: 0,1,0 sum=1 or 1,0,1 sum=2. Alternating cols: sum=1 or 2. Total sum can be 2,3,4. So we can achieve total sum 3 (n=3) by picking row sum=1, col sum=2 or vice versa. Let's test: r = 0,1,0; c = 1,0,1. Then increments:
        // r0: 1 0 1
        // r1: 2 1 2
        // r0: 1 0 1
        // Add to all 1: grid:
        // 2 1 2
        // 3 2 3
        // 2 1 2
        // Check adjacent: row1: 2!=1, 1!=2 ok. row2: 3!=2, 2!=3 ok. row3: 2!=1, 1!=2 ok. vertical: col1: 2!=3, 3!=2 ok. col2: 1!=2, 2!=1 ok. col3: 2!=3, 3!=2 ok. Beautiful! And we used 3 workers (row2, col1, col3). So it is possible with exactly n workers. So the hint holds: we need exactly n workers.
        // So the approach is correct.
        
        // Now implement DP for rows:
        // dp_r[i][val][sum] = min cost, but we can optimize to 2D: dp[i][val] = vector of min cost for each sum? Actually we can keep an array of size n+1 for each state (i, val) representing min cost to achieve that sum.
        // Since n<=1000, total states 2*1000, each with array of size 1001 -> 2e6, acceptable.
        // We'll use vector<vector<long long>> dp_curr(2, vector<long long>(n+1, INF));
        // Initialize for i=0: dp_curr[0][0] = 0, dp_curr[1][1] = a[0].
        // Then for i from 1 to n-1, compute next.
        // Allowed transitions: for each prev_val, for each next_val, check if (prev_val - next_val) is allowed.
        // Allowed if difference d = prev_val - next_val is NOT in F_i.
        // F_i is the set of forbidden differences for transition from i to i+1 (0-indexed: between row i and i+1).
        // So we precompute F_row[i] for i=0..n-2.
        
        vector<set<int>> F_row(n-1);
        for (int i = 0; i < n-1; i++) {
            for (int j = 0; j < n; j++) {
                long long diff = h[i+1][j] - h[i][j];
                if (diff >= -1 && diff <= 1) {
                    F_row[i].insert((int)diff);
                }
            }
        }
        
        vector<vector<long long>> dp_r(2, vector<long long>(n+1, INF));
        dp_r[0][0] = 0;
        dp_r[1][1] = a[0];
        for (int i = 0; i < n-1; i++) {
            vector<vector<long long>> next_dp(2, vector<long long>(n+1, INF));
            for (int prev = 0; prev < 2; prev++) {
                for (int s = 0; s <= i+1; s++) {
                    if (dp_r[prev][s] == INF) continue;
                    for (int nxt = 0; nxt < 2; nxt++) {
                        int d = prev - nxt;
                        if (F_row[i].count(d)) continue;
                        int ns = s + nxt;
                        long long cost = dp_r[prev][s] + (nxt ? a[i+1] : 0);
                        if (cost < next_dp[nxt][ns]) {
                            next_dp[nxt][ns] = cost;
                        }
                    }
                }
            }
            dp_r = move(next_dp);
        }
        // Now dp_r[val][sum] is min cost for rows ending with val, with total sum.
        // We can combine to get min cost for each sum regardless of final value:
        vector<long long> min_cost_r(n+1, INF);
        for (int val = 0; val < 2; val++) {
            for (int s = 0; s <= n; s++) {
                if (dp_r[val][s] < min_cost_r[s]) {
                    min_cost_r[s] = dp_r[val][s];
                }
            }
        }
        
        // Similarly for columns:
        vector<set<int>> F_col(n-1);
        for (int j = 0; j < n-1; j++) {
            for (int i = 0; i < n; i++) {
                long long diff = h[i][j+1] - h[i][j];
                if (diff >= -1 && diff <= 1) {
                    F_col[j].insert((int)diff);
                }
            }
        }
        
        vector<vector<long long>> dp_c(2, vector<long long>(n+1, INF));
        dp_c[0][0] = 0;
        dp_c[1][1] = b[0];
        for (int j = 0; j < n-1; j++) {
            vector<vector<long long>> next_dp(2, vector<long long>(n+1, INF));
            for (int prev = 0; prev < 2; prev++) {
                for (int s = 0; s <= j+1; s++) {
                    if (dp_c[prev][s] == INF) continue;
                    for (int nxt = 0; nxt < 2; nxt++) {
                        int d = prev - nxt;
                        if (F_col[j].count(d)) continue;
                        int ns = s + nxt;
                        long long cost = dp_c[prev][s] + (nxt ? b[j+1] : 0);
                        if (cost < next_dp[nxt][ns]) {
                            next_dp[nxt][ns] = cost;
                        }
                    }
                }
            }
            dp_c = move(next_dp);
        }
        vector<long long> min_cost_c(n+1, INF);
        for (int val = 0; val < 2; val++) {
            for (int s = 0; s <= n; s++) {
                if (dp_c[val][s] < min_cost_c[s]) {
                    min_cost_c[s] = dp_c[val][s];
                }
            }
        }
        
        long long ans = INF;
        for (int sr = 0; sr <= n; sr++) {
            int sc = n - sr;
            if (sc < 0 || sc > n) continue;
            if (min_cost_r[sr] != INF && min_cost_c[sc] != INF) {
                ans = min(ans, min_cost_r[sr] + min_cost_c[sc]);
            }
        }
        if (ans == INF) cout << "-1\n";
        else cout << ans << "\n";
    }
    return 0;
}
