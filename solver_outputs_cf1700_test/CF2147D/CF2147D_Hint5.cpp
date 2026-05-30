// Hint5
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
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        
        // Count frequencies of each value
        map<int, int> freq;
        for (int x : a) {
            freq[x]++;
        }
        
        // Collect frequencies of odd numbers
        vector<int> odd_freqs;
        for (auto& p : freq) {
            if (p.first % 2 == 1) {
                odd_freqs.push_back(p.second);
            }
        }
        
        // Sort descending
        sort(odd_freqs.begin(), odd_freqs.end(), greater<int>());
        
        long long diff = 0;
        for (size_t i = 0; i < odd_freqs.size(); ++i) {
            if (i % 2 == 0) {
                diff += odd_freqs[i];
            } else {
                diff -= odd_freqs[i];
            }
        }
        
        // Total points = sum of all a_i
        long long total = accumulate(a.begin(), a.end(), 0LL);
        
        // Alice = (total + diff) / 2, Bob = (total - diff) / 2
        long long alice = (total + diff) / 2;
        long long bob = (total - diff) / 2;
        
        cout << alice << " " << bob << "\n";
    }
    
    return 0;
}
