// Hint1
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m, i1, j1, i2, j2;
        string d;
        cin >> n >> m >> i1 >> j1 >> i2 >> j2 >> d;

        int di = (d[0] == 'D' ? 1 : -1);
        int dj = (d[1] == 'R' ? 1 : -1);

        int ci = i1, cj = j1;
        int bounces = 0;
        const int MAX_STEPS = 4 * n * m + 5;
        int steps = 0;

        while (steps < MAX_STEPS) {
            if (ci == i2 && cj == j2) {
                cout << bounces << "\n";
                break;
            }

            bool bounced = false;
            if (ci + di < 1 || ci + di > n) {
                di = -di;
                bounced = true;
            }
            if (cj + dj < 1 || cj + dj > m) {
                dj = -dj;
                bounced = true;
            }
            if (bounced) bounces++;

            ci += di;
            cj += dj;
            steps++;
        }
        if (steps == MAX_STEPS) {
            cout << -1 << "\n";
        }
    }
    return 0;
}
