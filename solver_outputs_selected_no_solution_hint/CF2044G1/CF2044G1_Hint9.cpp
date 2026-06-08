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
        vector<int> r(n);
        for (int i = 0; i < n; ++i) {
            cin >> r[i];
            --r[i];
        }
        
        vector<int> state(n, 1);
        vector<int> next_state(n);
        int year = 1;
        
        while (true) {
            fill(next_state.begin(), next_state.end(), 0);
            for (int i = 0; i < n; ++i) {
                if (state[i] > 0) {
                    next_state[r[i]]++;
                }
            }
            for (int i = 0; i < n; ++i) {
                if (next_state[i] > 1) next_state[i] = 1;
            }
            
            bool stable = true;
            for (int i = 0; i < n; ++i) {
                if (next_state[i] != state[i]) {
                    stable = false;
                    break;
                }
            }
            
            if (stable) {
                cout << year + 1 << '\n';
                break;
            }
            
            state.swap(next_state);
            ++year;
        }
    }
    
    return 0;
}
