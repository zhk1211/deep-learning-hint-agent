// Solution
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        long long n;
        int x, y;
        cin >> n >> x >> y;
        vector<int> a(x);
        for (int i = 0; i < x; ++i) {
            cin >> a[i];
        }
        sort(a.begin(), a.end());

        long long ans = x - 2;
        vector<int> gaps;
        for (int i = 0; i < x; ++i) {
            int nxt = (i + 1) % x;
            long long diff;
            if (nxt == 0) {
                diff = a[0] + n - a[i] - 1;
            } else {
                diff = a[nxt] - a[i] - 1;
            }
            if (diff == 1) {
                ans += 1;
            } else if (diff > 1) {
                gaps.push_back(diff);
            }
        }

        sort(gaps.begin(), gaps.end(), [](int a, int b) {
            if ((a % 2) != (b % 2)) return (a % 2) < (b % 2);
            return a < b;
        });

        for (int g : gaps) {
            if (y == 0) break;
            if (g % 2 == 1) {
                int take = min(y, g / 2);
                ans += take * 2;
                y -= take;
                if (take == g / 2) {
                    // all inner vertices used, gap becomes size 1
                    // but we already counted the +1 from diff==1? Actually no, we didn't.
                    // Wait, we need to check if we fully fill the odd gap.
                    // If we take all g/2 vertices, we get +1 extra triangle from the remaining 1-gap.
                    // But we already added +1 for diff==1? No, we didn't add for diff==1 here because diff>1.
                    // Actually, when we fill all possible vertices in an odd gap, the remaining structure gives one more triangle.
                    // Let's think: initial odd gap g=2k+1. We can place k vertices, each gives +2 triangles (one for the vertex itself, one for making a 2-apart).
                    // After placing k vertices, the gap becomes effectively size 1, which gives +1 triangle.
                    // So total triangles gained = 2k + 1 = g.
                    // But our loop adds take*2, and we need to add the final +1 if we took all k.
                    // However, we must be careful: if we don't take all k, we don't get the final +1.
                    if (take == g / 2) {
                        ans += 1;
                    }
                }
            } else {
                int take = min(y, g / 2);
                ans += take * 2;
                y -= take;
                // For even gaps, we never get an extra +1 at the end.
            }
        }

        // Any leftover y can be used to place vertices that just give +1 each, but they break good vertices.
        // Actually, according to editorial, after processing gaps, leftover y just give +1 each.
        // But we must be careful: placing a vertex in a gap that we didn't fully fill?
        // The editorial says: "if we have any leftover vertices, we can just ignore them. This is because since we have maximized the number of good vertices, even though any further vertex we place will increase the answer by 1, it will break two good vertices which will decrease the answer by 1."
        // Wait, that means leftover vertices don't increase answer? Let's re-read: "even though any further vertex we place will increase the answer by 1, it will break two good vertices which will decrease the answer by 1." Net change 0.
        // So we don't add anything for leftover y.
        // But wait, what if we have y left and we didn't fill a gap completely? The editorial says we process smaller odd gaps first, and we take as many as possible. If we have leftover y after all gaps are fully filled? Then placing anywhere gives net 0.
        // However, if we have an even gap and we didn't take all g/2, we might still have y left. Placing one more there gives +1 but breaks nothing? Actually, if we place one more in an even gap that we partially filled, we might create a situation. But the editorial says leftover y gives no net gain. So we just stop.

        cout << ans << "\n";
    }
    return 0;
}
