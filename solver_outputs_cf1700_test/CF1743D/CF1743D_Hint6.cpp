// Hint6
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
    for (int i = 0; i < n; ++i) {
        if (s[i] == '1') {
            first_one = i;
            break;
        }
    }

    if (first_one == -1) {
        cout << "0\n";
        return 0;
    }

    // s1 is the whole string starting from first_one
    string s1 = s.substr(first_one);
    int m = (int)s1.size();

    // Find the first zero after the leading ones
    int first_zero = -1;
    for (int i = 0; i < m; ++i) {
        if (s1[i] == '0') {
            first_zero = i;
            break;
        }
    }

    if (first_zero == -1) {
        // All ones, answer is s1
        cout << s1 << "\n";
        return 0;
    }

    // We need to choose a second substring s2 of length m starting at some position in s1
    // such that s1 OR s2 is maximized.
    // The optimal s2 will start somewhere in the first block of ones (positions 0..first_zero-1)
    // because we want to flip the first zero to 1 if possible.
    // We only need to check candidates that differ in the first few bits after first_zero.
    // Since n up to 1e6, we can check all possible starts in the first block.
    string best = s1;
    int max_start = first_zero; // inclusive
    for (int start = 0; start <= max_start; ++start) {
        string s2 = s1.substr(start, m);
        string cur(m, '0');
        for (int i = 0; i < m; ++i) {
            if (s1[i] == '1' || s2[i] == '1')
                cur[i] = '1';
            else
                cur[i] = '0';
        }
        if (cur > best)
            best = cur;
    }

    cout << best << "\n";
    return 0;
}
