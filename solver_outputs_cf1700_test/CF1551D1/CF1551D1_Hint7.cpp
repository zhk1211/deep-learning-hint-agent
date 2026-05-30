// Hint7
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, m, k;
        cin >> n >> m >> k;
        
        int total = n * m / 2;
        int vertical = total - k;
        
        bool possible = false;
        
        // Check if k horizontal dominoes are possible
        // Horizontal dominoes occupy 1x2 blocks, so they need at least k pairs of adjacent cells in rows
        // We can place horizontal dominoes in rows: each row can have at most m/2 horizontal dominoes
        // So max horizontal = n * (m/2)
        // Similarly, vertical dominoes occupy 2x1 blocks, max vertical = m * (n/2)
        
        // For a given k, we need to check if we can place exactly k horizontal and the rest vertical
        // Key insight: we can tile the board if and only if we can satisfy parity and capacity constraints
        
        // If n is odd, we must have at least m/2 horizontal dominoes in the last row
        // If m is odd, we must have at least n/2 vertical dominoes in the last column
        
        // More generally, we can think of it as: we can reduce the problem by filling full rows/columns
        // and then checking the remaining even-by-even subgrid
        
        if (n % 2 == 0 && m % 2 == 0) {
            // Both even: we can place any number of horizontal dominoes as long as it's even? 
            // Actually, we can place horizontal dominoes in pairs of rows. 
            // In an even-by-even board, we can achieve any k that is between 0 and n*m/2 
            // with the same parity as something? Let's analyze.
            // We can tile the board with all horizontal or all vertical.
            // Can we have exactly k horizontal? 
            // Consider coloring: horizontal domino covers one black and one white in chessboard coloring.
            // In even-even board, number of black = number of white.
            // Any tiling has equal number of black and white covered, but that doesn't restrict k.
            // Actually, we can achieve any k from 0 to n*m/2? Not exactly.
            // Let's think: we can place horizontal dominoes in pairs of rows. 
            // If we take two rows, we can place 0 to m horizontal dominoes in that pair, 
            // and the rest vertical? But vertical dominoes span two rows, so they must be placed in pairs of rows.
            // In an even-by-even board, we can partition into 2x2 blocks. 
            // In each 2x2, we can have 0, 1, or 2 horizontal dominoes? 
            // 0 horizontal: 2 vertical. 2 horizontal: 0 vertical. 1 horizontal: impossible because the other two cells would need to be covered by vertical dominoes, but they are in different rows? Actually, in a 2x2, if you place one horizontal, the remaining two cells are in different rows and columns, you can't cover them with a vertical domino because they are not adjacent vertically. So in a 2x2, you can only have 0 or 2 horizontal dominoes.
            // Therefore, in an even-by-even board, the number of horizontal dominoes must be even? 
            // Let's test: n=2, m=2. k can be 0 or 2. k=1 is impossible. So yes, k must be even.
            // For n=4, m=4, can we have k=2? Yes, sample says YES. k=2 is even.
            // Can we have k=4? Probably yes. k=6? Maybe. 
            // So condition: k must be even? But wait, what about n=2, m=4? n=2 (even), m=4 (even). 
            // Can we have k=1? Let's see: 2x4 board. Horizontal dominoes: 1. Total cells=8, so 4 dominoes. 
            // 1 horizontal, 3 vertical. Is it possible? 
            // Place one horizontal in top row, then we have 2 cells in top row left, and 4 in bottom row. 
            // The two top cells are separated? Actually, if we place a horizontal at left, top row has two cells left, but they are adjacent? They are a 1x2 block, so we can place another horizontal? But we only want 1 horizontal. 
            // If we place one horizontal, the rest must be vertical. 
            // Vertical dominoes cover one cell in top row and one in bottom row in the same column. 
            // So if we place a horizontal covering (1,1)-(1,2), then columns 1 and 2 have top cells occupied. 
            // We need to cover bottom cells in columns 1 and 2 with vertical dominoes? But vertical dominoes need both top and bottom cells in the same column. Top cells in columns 1 and 2 are already covered, so we can't place vertical dominoes there. So we must cover bottom cells in columns 1 and 2 with horizontal dominoes? That would add more horizontal. So k=1 seems impossible. 
            // So indeed, in even-by-even, k must be even.
            // Let's check sample: 4 4 2 -> YES (k=2 even). 2 4 2 -> YES (k=2 even). 2 3 0 -> n=2 even, m=3 odd -> different case.
            // So for even n and even m: possible if k is even and 0 <= k <= n*m/2? 
            // But wait, is there any upper bound? Max horizontal is n * (m/2). So k <= n * m/2 is automatically satisfied if k <= n*(m/2)? Actually n*m/2 = n*(m/2) because m even. So k <= n*m/2 is the same as k <= n*(m/2). So only parity condition.
            // But is every even k achievable? Let's test n=2, m=4: k=0 (YES), k=2 (YES), k=4 (YES). k=1,3 (NO). So yes.
            // For n=4, m=4: k=0,2,4,6,8? k=8 is all horizontal. k=6? Can we do 6 horizontal and 2 vertical? 
            // 6 horizontal = 12 cells, 2 vertical = 4 cells. Total 16. 
            // We can place 3 rows of all horizontal (6 horizontal) and one row of? Actually, if we place 3 rows of horizontal, that's 3 rows * 2 horizontal per row = 6 horizontal, covering 3 rows completely. The last row has 4 cells, we need to cover with vertical? But vertical dominoes need two rows. So we can't. 
            // Alternative: we can mix. For example, in a 4x4, we can have two 2x2 blocks with 2 horizontal each (total 4), and two 2x2 blocks with 0 horizontal (total 0), sum=4. To get 6, we need three 2x2 blocks with 2 horizontal and one with 0 horizontal? That gives 6 horizontal, 2 vertical. Is that possible? Let's try: place 2x2 blocks: top-left: 2 horizontal, top-right: 2 horizontal, bottom-left: 2 horizontal, bottom-right: 0 horizontal (2 vertical). But vertical dominoes in bottom-right 2x2: they are vertical, so they occupy columns 3 and 4, rows 3 and 4. That's fine. So yes, k=6 is possible. So all even k from 0 to 8 are possible.
            // So condition: if n even and m even: k % 2 == 0.
            possible = (k % 2 == 0);
        } else if (n % 2 == 1 && m % 2 == 0) {
            // n odd, m even.
            // We must fill the last row entirely with horizontal dominoes, because vertical dominoes need two rows.
            // So we need at least m/2 horizontal dominoes for the last row.
            // After placing those, we have an (n-1) x m board, which is even x even.
            // In the even x even part, we need k - m/2 horizontal dominoes.
            // So condition: k >= m/2, and (k - m/2) must be even (from even-even case).
            // Also, k <= n*m/2 (total dominoes) is automatically satisfied if we can place the rest? 
            // Max horizontal in even part: (n-1) * (m/2). So total max horizontal = m/2 + (n-1)*(m/2) = n*(m/2) = n*m/2. So k <= n*m/2 is the given constraint.
            // So condition: k >= m/2 and (k - m/2) % 2 == 0.
            if (k >= m/2 && (k - m/2) % 2 == 0) {
                possible = true;
            }
        } else if (n % 2 == 0 && m % 2 == 1) {
            // n even, m odd.
            // We must fill the last column entirely with vertical dominoes, because horizontal dominoes need two columns.
            // So we need at least n/2 vertical dominoes for the last column.
            // Total vertical = total - k. So vertical >= n/2 => (n*m/2 - k) >= n/2 => k <= n*m/2 - n/2 = n*(m-1)/2.
            // After placing those vertical dominoes, we have an n x (m-1) board, which is even x even.
            // In the even x even part, we need k horizontal dominoes (since no horizontal in the last column).
            // So condition: k must be even (from even-even case) and k <= n*(m-1)/2.
            // Also k >= 0.
            if (k % 2 == 0 && k <= n * (m - 1) / 2) {
                possible = true;
            }
        } else {
            // Both odd: n*m is odd, but problem says n*m is even, so this case never happens.
        }
        
        cout << (possible ? "YES" : "NO") << '\n';
    }
    
    return 0;
}
