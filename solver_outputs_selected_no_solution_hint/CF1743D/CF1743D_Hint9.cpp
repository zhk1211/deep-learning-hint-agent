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
    
    // Find the first zero in t
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
    
    // We will try to maximize the OR by choosing a second substring
    // that starts somewhere between 0 and first_zero, and has length m - first_zero.
    // Actually we want to shift a copy of t to cover the first zero.
    int len = m - first_zero;
    string best = t;
    
    // Try all possible shifts from 1 to first_zero
    for (int shift = 1; shift <= first_zero; shift++) {
        string cur = t;
        for (int i = 0; i < len; i++) {
            if (t[first_zero + i] == '0' && t[shift + i] == '1') {
                cur[first_zero + i] = '1';
            }
        }
        if (cur > best) best = cur;
    }
    
    cout << best << "\n";
    return 0;
}
