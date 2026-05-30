// Hint8
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
    
    // Find the end of the first block of ones
    int block_end = first_one;
    while (block_end + 1 < n && s[block_end + 1] == '1') {
        block_end++;
    }
    
    // The first substring is the whole string s
    // The second substring will be a prefix of s starting at some position
    // We only need to consider starting positions from first_one to block_end+1
    // because the first block of ones is small (expected O(1))
    
    string best = s;
    int len = n - first_one; // length of the suffix starting at first_one
    
    // Try all possible starting positions for the second substring
    for (int start = first_one; start <= block_end + 1 && start < n; start++) {
        string cur = s;
        // Apply OR with substring starting at 'start'
        for (int i = 0; i < len && start + i < n; i++) {
            if (s[start + i] == '1') {
                cur[first_one + i] = '1';
            }
        }
        if (cur > best) {
            best = cur;
        }
    }
    
    // Remove leading zeros
    int pos = 0;
    while (pos < n && best[pos] == '0') pos++;
    if (pos == n) {
        cout << "0\n";
    } else {
        cout << best.substr(pos) << '\n';
    }
    
    return 0;
}
