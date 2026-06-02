#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    string s;
    cin >> n >> s;
    
    // Find first '1' to skip leading zeros
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
    
    // Find first zero after the leading one
    int first_zero = -1;
    for (int i = 1; i < m; i++) {
        if (t[i] == '0') {
            first_zero = i;
            break;
        }
    }
    
    if (first_zero == -1) {
        // All ones, answer is just t
        cout << t << "\n";
        return 0;
    }
    
    // We will try to maximize the result by choosing second substring
    // that starts at some position and has length m - first_zero
    int len = m - first_zero;
    string best = t;
    
    // Try all possible starting positions for the second substring
    // The second substring must be of length len, starting at some position <= first_zero
    for (int start = 0; start <= first_zero; start++) {
        string cur = t;
        for (int i = 0; i < len; i++) {
            if (t[start + i] == '1') {
                cur[first_zero + i] = '1';
            }
        }
        if (cur > best) {
            best = cur;
        }
    }
    
    cout << best << "\n";
    return 0;
}
