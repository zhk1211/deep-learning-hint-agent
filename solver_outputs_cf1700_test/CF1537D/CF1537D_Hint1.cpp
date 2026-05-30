// Hint1
#include <bits/stdc++.h>
using namespace std;

bool is_prime(int n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        
        if (n == 1) {
            cout << "Bob\n";
        } else if (n % 2 == 1) {
            cout << "Bob\n";
        } else {
            // n is even
            if ((n & (n - 1)) == 0) {
                // n is a power of two
                int exp = 0;
                int temp = n;
                while (temp > 1) {
                    temp /= 2;
                    exp++;
                }
                if (exp % 2 == 1) {
                    cout << "Bob\n";
                } else {
                    cout << "Alice\n";
                }
            } else {
                cout << "Alice\n";
            }
        }
    }
    return 0;
}
