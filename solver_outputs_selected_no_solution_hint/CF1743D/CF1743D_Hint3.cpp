#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    string s;
    cin >> n >> s;
    
    // Find first '1'
    int first_one = -1;
    for (int i = 0; i < n; i++) {
        if (s[i] == '1') {
            first_one = i;
            break;
        }
    }
    
    if (first_one == -1) {
        cout << "0\n";
        return 0;
    }
    
    // The string without leading zeros
    string t = s.substr(first_one);
    int m = t.size();
    
    // Find first zero in t
    int first_zero = -1;
    for (int i = 0; i < m; i++) {
        if (t[i] == '0') {
            first_zero = i;
            break;
        }
    }
    
    if (first_zero == -1) {
        // All ones
        cout << t << "\n";
        return 0;
    }
    
    // We will try to maximize the result by choosing s1 = t (the whole string from first 1)
    // and s2 = some substring of t of length m - first_zero, starting at some position <= first_zero.
    // Actually we can shift s2 to cover the first zero with a 1 if possible.
    
    string best = t;
    int len = m - first_zero; // length of s2
    
    // Try all possible starting positions for s2 within t
    // s2 must start at some index i such that i + len <= m and i <= first_zero
    // Actually we can start s2 anywhere from 0 to first_zero (inclusive)
    for (int i = 0; i <= first_zero; i++) {
        string cur = t;
        for (int j = 0; j < len; j++) {
            if (t[i + j] == '1') {
                cur[first_zero + j] = '1';
            }
        }
        if (cur > best) {
            best = cur;
        }
    }
    
    cout << best << "\n";
    return 0;
}
