// Solution
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
        
        if (n % 2 == 1) {
            cout << "Bob\n";
        } else {
            // n is even
            // check if n is a power of 2
            if ((n & (n - 1)) == 0) {
                // power of 2
                int cnt = 0;
                while (n > 1) {
                    n >>= 1;
                    cnt++;
                }
                if (cnt % 2 == 0) {
                    cout << "Alice\n";
                } else {
                    cout << "Bob\n";
                }
            } else {
                cout << "Alice\n";
            }
        }
    }
    return 0;
}
