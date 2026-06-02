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

    // The string without leading zeros
    string t = s.substr(first_one);
    int m = t.size();

    // Find the first zero in t
    int first_zero = -1;
    for (int i = 0; i < m; ++i) {
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

    // We will try to choose a second substring that starts at some position
    // and has length m, to maximize the OR with t.
    // The second substring can start anywhere from 0 to first_zero.
    // We only need to consider starting positions up to first_zero.
    string best = t;
    int max_start = min(first_zero, m - 1);
    for (int start = 0; start <= max_start; ++start) {
        // We only care about the part that can affect bits from first_zero onwards
        // The length of the second substring should be m - start, but we only need to
        // consider up to m characters. Actually we can take the substring of length m
        // starting at start, but if it goes beyond n, we can't. However, we can always
        // take a substring of length m starting at start if start + m <= n.
        // Since we are in t (which starts at first_one), the original index is first_one + start.
        // We need first_one + start + m <= n => start <= n - first_one - m = 0? Wait m = n - first_one.
        // So start + m <= m => start <= 0. That means we cannot take a substring of length m
        // starting at start > 0 within t? Actually t is the suffix starting at first_one.
        // The original string length is n. t starts at first_one, so t's length m = n - first_one.
        // If we take a second substring starting at first_one + start, its maximum length is n - (first_one + start) = m - start.
        // So we cannot take length m if start > 0. But we can take the substring from first_one + start to the end,
        // which has length m - start. That's fine because we only care about bits up to m-1.
        // The OR with t will have the same higher bits as t, and we want to maximize the lower bits.
        // So we can just take the substring from start to the end of t, and pad with leading zeros conceptually.
        // Actually we can just take the substring s.substr(first_one + start) and compare with t.
        string cand = s.substr(first_one + start);
        // We want to OR cand with t. Since t is longer or equal, we align them to the right (LSB).
        // But t is the longer one? Actually t is length m, cand is length m - start.
        // To compute OR, we can just iterate from the end.
        string res = t;
        int offset = m - cand.size();
        for (int i = 0; i < (int)cand.size(); ++i) {
            if (cand[i] == '1') {
                res[offset + i] = '1';
            }
        }
        if (res > best) {
            best = res;
        }
    }

    cout << best << "\n";
    return 0;
}
