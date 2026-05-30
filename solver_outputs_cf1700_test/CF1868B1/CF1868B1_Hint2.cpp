// Hint2
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
        
        long long target = sum / n;
        vector<int> give(32, 0), receive(32, 0);
        bool possible = true;
        
        for (int i = 0; i < n; ++i) {
            long long diff = a[i] - target;
            if (diff == 0) continue;
            
            bool found = false;
            for (int x = 0; x <= 31; ++x) {
                long long give_val = 1LL << x;
                long long need = diff + give_val;
                if (need <= 0) continue;
                // check if need is a power of two
                if ((need & (need - 1)) == 0) {
                    int y = __builtin_ctzll(need);
                    if (y >= 0 && y <= 31) {
                        give[x]++;
                        receive[y]++;
                        found = true;
                        break;
                    }
                }
            }
            if (!found) {
                possible = false;
                break;
            }
        }
        
        if (possible) {
            for (int i = 0; i < 32; ++i) {
                if (give[i] != receive[i]) {
                    possible = false;
                    break;
                }
            }
        }
        
        cout << (possible ? "Yes" : "No") << '\n';
    }
    
    return 0;
}
