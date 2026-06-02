#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        long long d, k;
        cin >> d >> k;
        
        long long z = 0;
        while ((z + 1) * (z + 1) * k * k * 2 <= d * d) {
            z++;
        }
        
        if (z * z * k * k + (z + 1) * (z + 1) * k * k <= d * d) {
            cout << "Ashish\n";
        } else {
            cout << "Utkarsh\n";
        }
    }
    
    return 0;
}
