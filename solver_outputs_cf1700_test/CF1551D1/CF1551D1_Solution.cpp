// Solution
#include <iostream>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m, k;
        cin >> n >> m >> k;

        if (n % 2 == 0) {
            // n even
            if (k % 2 == 0 && k <= n * (m / 2)) {
                cout << "YES\n";
            } else {
                cout << "NO\n";
            }
        } else {
            // n odd -> m must be even (since n*m is even)
            if (k >= m / 2 && (k - m / 2) % 2 == 0 && k <= n * m / 2) {
                cout << "YES\n";
            } else {
                cout << "NO\n";
            }
        }
    }
    return 0;
}
