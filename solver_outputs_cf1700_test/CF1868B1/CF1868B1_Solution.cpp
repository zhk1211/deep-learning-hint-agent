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
        vector<long long> a(n);
        long long sum = 0;
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
            sum += a[i];
        }
        if (sum % n != 0) {
            cout << "No\n";
            continue;
        }
        long long s = sum / n;
        
        multiset<int> S, T;
        bool ok = true;
        
        for (int i = 0; i < n; ++i) {
            if (a[i] == s) continue;
            long long diff = a[i] - s;
            bool found = false;
            // Try to find x, y such that 2^x - 2^y = diff
            // Since diff != 0, x != y
            // We can iterate over possible x (0..30) and check if 2^x - diff is a power of two
            for (int x = 0; x <= 30; ++x) {
                long long p2x = 1LL << x;
                long long need = p2x - diff;
                if (need <= 0) continue;
                // check if need is a power of two
                if ((need & (need - 1)) == 0) {
                    int y = __builtin_ctzll(need);
                    if (y <= 30) {
                        S.insert(x);
                        T.insert(y);
                        found = true;
                        break;
                    }
                }
            }
            if (!found) {
                ok = false;
                break;
            }
        }
        if (ok && S == T) {
            cout << "Yes\n";
        } else {
            cout << "No\n";
        }
    }
    return 0;
}
