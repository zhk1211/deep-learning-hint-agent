// Hint3
#include <bits/stdc++.h>
using namespace std;

const int MAX_C = 10000000;
const int MAX_N = MAX_C; // n <= c, and c <= 10^7

int sum_div[MAX_N + 1];
int ans[MAX_C + 1];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Initialize sum_div array
    for (int i = 1; i <= MAX_N; ++i) {
        for (int j = i; j <= MAX_N; j += i) {
            sum_div[j] += i;
        }
    }

    // Initialize ans array with -1
    memset(ans, -1, sizeof(ans));

    // For each n, if sum_div[n] <= MAX_C and ans[sum_div[n]] is not set, set it
    for (int n = 1; n <= MAX_N; ++n) {
        int c = sum_div[n];
        if (c <= MAX_C && ans[c] == -1) {
            ans[c] = n;
        }
    }

    int t;
    cin >> t;
    while (t--) {
        int c;
        cin >> c;
        cout << ans[c] << '\n';
    }

    return 0;
}
