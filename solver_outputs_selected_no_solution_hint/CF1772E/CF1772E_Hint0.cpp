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
        vector<int> p(n);
        for (int i = 0; i < n; ++i) {
            cin >> p[i];
        }
        
        // Check if already sorted ascending or descending (should not happen per problem)
        // but we handle anyway.
        bool asc = true, desc = true;
        for (int i = 0; i < n; ++i) {
            if (p[i] != i + 1) asc = false;
            if (p[i] != n - i) desc = false;
        }
        if (asc) {
            cout << "First\n";
            continue;
        }
        if (desc) {
            cout << "Second\n";
            continue;
        }
        
        // Count fixed points for ascending and descending
        int asc_fixed = 0, desc_fixed = 0;
        for (int i = 0; i < n; ++i) {
            if (p[i] == i + 1) asc_fixed++;
            if (p[i] == n - i) desc_fixed++;
        }
        
        // Elements that are in correct position for both (only possible if n is odd and middle element)
        int both = 0;
        if (n % 2 == 1) {
            int mid = n / 2;
            if (p[mid] == mid + 1) both = 1;
        }
        
        // Moves needed for each player to win by only recoloring
        int first_moves = n - asc_fixed;
        int second_moves = n - desc_fixed;
        
        // Adjust for the fact that if an element is fixed for both, it's already blue for both? 
        // Actually, the game: players can recolor red elements to blue. 
        // The winning condition is that the permutation becomes sorted in their favor.
        // They can also rearrange blue elements arbitrarily.
        // The key insight from known solution: 
        // Let a = number of positions where p_i != i (for first player)
        // Let b = number of positions where p_i != n - i + 1 (for second player)
        // Let c = number of positions where p_i != i AND p_i != n - i + 1 (both wrong)
        // Then first player needs to fix a positions, second needs b.
        // But they share c positions that are wrong for both.
        // The game reduces to: first wants to make a=0, second wants b=0.
        // On a turn, a player can either decrease their own count by 1 (by coloring a red element blue that is in wrong position for them),
        // or they can skip, or rearrange blue elements (which doesn't change counts).
        // Actually coloring an element blue doesn't immediately fix it; it just allows future rearrangement.
        // But the known solution: if a <= b - c, then First wins? Let's derive.
        
        // Let x = asc_fixed, y = desc_fixed.
        // a = n - x, b = n - y.
        // c = number of positions where p_i != i and p_i != n-i+1.
        // We can compute c = n - (x + y - both) because positions that are correct for at least one: x + y - both.
        // So c = n - x - y + both.
        
        int c = n - asc_fixed - desc_fixed + both;
        int a = n - asc_fixed;
        int b = n - desc_fixed;
        
        // Now apply the known condition:
        // First wins if a <= b - c? Actually from editorial of similar problem (CF 1730C? No, this is CF 1787C? Let's think)
        // The known result: 
        // If a <= b - c: First wins (because First can force win before Second)
        // If b < a - c: Second wins
        // Else Tie.
        // Let's test with examples.
        // Example 1: 1 2 4 3
        // asc_fixed: 1,2 are fixed -> 2. desc_fixed: none? 4 3 2 1 -> p1=1 (no), p2=2 (no), p3=4 (yes), p4=3 (no) -> 1.
        // both = 0. a = 2, b = 3, c = 4 - 2 - 1 + 0 = 1.
        // a <= b - c? 2 <= 3 - 1 = 2 -> true -> First. Correct.
        // Example 2: 2 3 1
        // asc_fixed: none -> 0. desc_fixed: 3 2 1 -> p1=2 (no), p2=3 (no), p3=1 (yes) -> 1.
        // both = 0. a = 3, b = 2, c = 3 - 0 - 1 + 0 = 2.
        // a <= b - c? 3 <= 2 - 2 = 0 -> false.
        // b < a - c? 2 < 3 - 2 = 1 -> false.
        // Tie. Correct.
        // Example 3: 3 4 5 2 1
        // asc_fixed: none -> 0. desc_fixed: 5 4 3 2 1 -> p1=3 (no), p2=4 (yes), p3=5 (no), p4=2 (yes), p5=1 (yes) -> 3.
        // both = 0. a = 5, b = 2, c = 5 - 0 - 3 + 0 = 2.
        // a <= b - c? 5 <= 2 - 2 = 0 -> false.
        // b < a - c? 2 < 5 - 2 = 3 -> true -> Second. Correct.
        // Example 4: 1 5 6 3 2 4
        // asc_fixed: p1=1 -> 1. desc_fixed: 6 5 4 3 2 1 -> p1=1 (no), p2=5 (yes), p3=6 (no), p4=3 (no), p5=2 (no), p6=4 (no) -> 1.
        // both = 0. a = 5, b = 5, c = 6 - 1 - 1 + 0 = 4.
        // a <= b - c? 5 <= 5 - 4 = 1 -> false.
        // b < a - c? 5 < 5 - 4 = 1 -> false.
        // Tie. Correct.
        
        if (a <= b - c) {
            cout << "First\n";
        } else if (b < a - c) {
            cout << "Second\n";
        } else {
            cout << "Tie\n";
        }
    }
    return 0;
}
