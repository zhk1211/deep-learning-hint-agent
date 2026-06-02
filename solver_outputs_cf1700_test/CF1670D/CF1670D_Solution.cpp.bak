// Solution
#include <bits/stdc++.h>
using namespace std;

long long calc(long long n) {
    if (n == 0) return 0;
    n--;
    long long ans = (n / 3) * (n / 3) * 3;
    for (int i = 0; i <= n % 3; i++) {
        ans += (n / 3) * 2 + i;
    }
    return ans * 2;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        int low = 0, high = 1e9;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (calc(mid) < n) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        cout << low << '\n';
    }
    return 0;
}
