#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<int> a(n), b(m);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int i = 0; i < m; ++i) cin >> b[i];

        // Check if there is any composite number in b
        // A number is composite if it is > 1 and not prime.
        // Since b_i <= n+m <= 2e6, we can just check primality per element.
        bool has_composite = false;
        for (int y : b) {
            if (y <= 1) continue;
            bool is_prime = true;
            if (y % 2 == 0) {
                is_prime = (y == 2);
            } else {
                for (int d = 3; d * d <= y; d += 2) {
                    if (y % d == 0) {
                        is_prime = false;
                        break;
                    }
                }
            }
            if (!is_prime) {
                has_composite = true;
                break;
            }
        }

        if (has_composite) {
            cout << "Alice\n";
        } else {
            // All numbers in b are prime (or 1, but 1 is not prime and not composite; 1 is not in b as per constraints? Actually b_i >=1, but 1 is neither prime nor composite. 
            // However, if 1 is present, it's not composite. The logic: if no composite, Bob wins? Let's check.
            // According to hints: if b has a composite number, Alice always wins. Otherwise Bob wins.
            cout << "Bob\n";
        }
    }
    return 0;
}
