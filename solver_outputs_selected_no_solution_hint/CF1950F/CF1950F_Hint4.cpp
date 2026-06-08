#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int a, b, c;
        cin >> a >> b >> c;

        // A tree with a+b+c vertices must have a+b+c-1 edges.
        // Also, sum of children = 2*a + 1*b + 0*c = 2a + b.
        // In any rooted tree, sum of children = number of edges = total vertices - 1.
        if (2 * a + b != a + b + c - 1) {
            cout << -1 << '\n';
            continue;
        }

        // If no vertices with children, only possible if c == 1 (single node)
        if (a == 0 && b == 0) {
            cout << (c == 1 ? 0 : -1) << '\n';
            continue;
        }

        // Greedy construction: build tree level by level.
        // Start with root (level 0). It can be either type a or b.
        // We want to minimize height, so we should place type-a vertices as high as possible
        // because they create more children to place remaining vertices.
        // We simulate BFS-like level filling.

        // At each level we have some number of "slots" (vertices that need children).
        // Initially we have 1 slot (the root).
        // We process levels until all a and b are placed, then remaining slots must be filled with c leaves.
        // The height is the number of levels processed.

        int slots = 1; // vertices in current level that need children
        int height = 0;
        bool possible = true;

        while (a > 0 || b > 0) {
            if (slots == 0) {
                // No slots but still have vertices to place -> impossible
                possible = false;
                break;
            }
            // We will fill the current level's slots with a's and b's.
            // To minimize height, we use as many a's as possible because they give 2 children each.
            int use_a = min(slots, a);
            a -= use_a;
            slots -= use_a;
            // remaining slots in this level will be filled with b's
            int use_b = min(slots, b);
            b -= use_b;
            slots -= use_b;
            // If there are still slots left, they must be filled with c (leaves), but we don't have c yet?
            // Actually we can fill them with c later, but we are placing a and b first.
            // If after using all a and b we still have slots, we can't place more a or b, so we break.
            // But we must ensure we don't leave slots unfilled if we still have a or b.
            // The loop condition ensures we only continue if a>0 or b>0.
            // If slots > 0 after using all possible a and b, we cannot place more a/b, so we must fill remaining slots with c.
            // But we haven't placed c yet. That's fine: those slots become leaves (c).
            // However, if we still have a or b left, we need more slots, but we have none -> impossible.
            if (slots > 0 && (a > 0 || b > 0)) {
                possible = false;
                break;
            }
            // Next level slots = 2 * use_a + 1 * use_b
            int next_slots = 2 * use_a + use_b;
            slots = next_slots;
            height++;
        }

        if (!possible) {
            cout << -1 << '\n';
            continue;
        }

        // Now all a and b are placed. We have some slots left that must be filled with c leaves.
        if (slots != c) {
            cout << -1 << '\n';
        } else {
            cout << height << '\n';
        }
    }

    return 0;
}
