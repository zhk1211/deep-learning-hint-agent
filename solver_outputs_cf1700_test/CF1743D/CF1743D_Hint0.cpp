// Hint0
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

    // s1 will be the whole suffix starting from first_one
    string s1 = s.substr(first_one);
    int m = s1.size();

    // Find the next zero after the first one
    int first_zero = -1;
    for (int i = 1; i < m; ++i) {
        if (s1[i] == '0') {
            first_zero = i;
            break;
        }
    }

    if (first_zero == -1) {
        // All ones, answer is just s1
        cout << s1 << "\n";
        return 0;
    }

    // We need to choose s2 to maximize OR with s1.
    // s2 must be a substring of s that starts at some position p in [first_one, first_one + first_zero]
    // and has length m.
    // We only need to consider candidates that start at positions where s[p] == '1'.
    // Among those, we want to maximize the OR.
    string best = s1;
    int max_start = first_one + first_zero;
    for (int start = first_one; start <= max_start; ++start) {
        if (s[start] == '0') continue;
        // s2 is s.substr(start, m)
        // We only need to compute OR for bits from first_zero to m-1
        string cur = s1;
        for (int i = first_zero; i < m; ++i) {
            if (s[start + i] == '1') {
                cur[i] = '1';
            }
        }
        if (cur > best) {
            best = cur;
        }
    }

    cout << best << "\n";
    return 0;
}
