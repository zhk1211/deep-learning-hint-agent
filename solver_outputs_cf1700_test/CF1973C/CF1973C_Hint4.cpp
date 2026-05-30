// Hint4
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
        vector<int> p(n);
        for (int i = 0; i < n; i++) {
            cin >> p[i];
        }
        
        vector<int> pos(n + 1);
        for (int i = 0; i < n; i++) {
            pos[p[i]] = i;
        }
        
        vector<int> q(n);
        vector<bool> used(n + 1, false);
        
        // We want to make elements at odd indices (0-based) local maxima
        // For odd indices, assign large values; for even indices, assign small values
        // But we need to ensure a_i = p_i + q_i forms peaks at odd indices
        
        // Strategy: sort positions by p_i value, assign largest q to positions where we want peaks
        // We want peaks at indices 1,3,5,... (0-based)
        // So we assign largest q values to these positions, but in order of their p_i values
        // To make a_i large, we pair large p_i with large q_i
        
        vector<int> odd_pos, even_pos;
        for (int i = 0; i < n; i++) {
            if (i % 2 == 1) odd_pos.push_back(i);
            else even_pos.push_back(i);
        }
        
        // Sort odd positions by p_i descending (so largest p gets largest q)
        sort(odd_pos.begin(), odd_pos.end(), [&](int i, int j) {
            return p[i] > p[j];
        });
        
        // Sort even positions by p_i ascending (so smallest p gets smallest q)
        sort(even_pos.begin(), even_pos.end(), [&](int i, int j) {
            return p[i] < p[j];
        });
        
        int val = n;
        for (int idx : odd_pos) {
            q[idx] = val--;
        }
        for (int idx : even_pos) {
            q[idx] = val--;
        }
        
        for (int i = 0; i < n; i++) {
            cout << q[i] << " \n"[i == n - 1];
        }
    }
    
    return 0;
}
