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
        vector<long long> b(n);
        long long sum_b = 0;
        for (int i = 0; i < n; ++i) {
            cin >> b[i];
            sum_b += b[i];
        }
        
        if (n == 1) {
            cout << "YES\n" << b[0] << "\n";
            continue;
        }
        
        long long total_a = sum_b / (1LL * n * (n + 1) / 2);
        if (sum_b % (1LL * n * (n + 1) / 2) != 0 || total_a <= 0) {
            cout << "NO\n";
            continue;
        }
        
        vector<long long> a(n);
        bool ok = true;
        for (int i = 0; i < n; ++i) {
            int prev = (i - 1 + n) % n;
            long long diff = b[prev] - b[i] + total_a;
            if (diff % n != 0 || diff <= 0) {
                ok = false;
                break;
            }
            a[i] = diff / n;
            if (a[i] > 1000000000 || a[i] < 1) {
                ok = false;
                break;
            }
        }
        
        if (!ok) {
            cout << "NO\n";
        } else {
            cout << "YES\n";
            for (int i = 0; i < n; ++i) {
                cout << a[i] << " \n"[i == n - 1];
            }
        }
    }
    return 0;
}
